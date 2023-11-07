#include "ConnectionPool.h"
#include <json/json.h>
#include <fstream>
#include <thread>
using namespace Json;
ConnectionPool* ConnectionPool::getConnectPool()
{
    static ConnectionPool pool;
    return &pool;
}

//连接的API接口
std::shared_ptr<MysqlConn> ConnectionPool::getConnection()
{
    std::unique_lock<std::mutex>locker(m_mutexQ);
    while (m_connectionQ.empty()) {
        if (std::cv_status::timeout == m_cond.wait_for(locker, std::chrono::milliseconds(timeout))) {
            if (m_connectionQ.empty()) {
                continue;
        }
      }   
    }

    std::shared_ptr<MysqlConn> conn(m_connectionQ.front(), [this](MysqlConn* conn) {
        std::lock_guard<std::mutex>locker(m_mutexQ);
        //m_mutexQ.lock();
        conn->refreshAliveTime();
        m_connectionQ.push(conn);
        //m_mutexQ.unlock();
        });
    m_connectionQ.pop();
    m_cond.notify_all();
    return conn;
}

ConnectionPool::~ConnectionPool()
{
    while (!m_connectionQ.empty()) {
        MysqlConn* tmp = m_connectionQ.front();
        m_connectionQ.pop();
        delete tmp;
    }
}

bool ConnectionPool::parseJsonFile()
{
    std::ifstream ifs("dbconfig.json");
    Reader rd;
    Value root;
    rd.parse(ifs,root);
    if (root.isObject()) {
        m_ip = root["ip"].asString();
        m_port = root["port"].asInt();
        m_user = root["username"].asString();
        m_passwd = root["passwd"].asString();
        m_dbname = root["dbname"].asString();
        min_size = root["min_size"].asInt();
        max_size = root["max_size"].asInt();
        idletime = root["max_idletime"].asInt();
        timeout = root["timeout"].asInt();
        return true;
    }
    return false;
}

//生产者生产连接
void ConnectionPool::productConnection()
{
    while (1) {
        std::unique_lock<std::mutex>locker(m_mutexQ);
        //此循环是如果连接数够用线程就暂时不要生产连接数
        while (m_connectionQ.size() >= min_size) {
            m_cond.wait(locker);
        }
        addConnection();
        m_cond.notify_all();
    }
}

//销毁连接
void ConnectionPool::recycleConnection()
{
    while (1) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::lock_guard<std::mutex>locker(m_mutexQ);
        while (m_connectionQ.size() > min_size) {
            MysqlConn* tmp = m_connectionQ.front();
            if (tmp->getAliveTime() >= idletime) {
                m_connectionQ.pop();
                delete tmp;
            }
            else {
                break;
            }
        }
    }
}

//添加连接数
void ConnectionPool::addConnection()
{
    MysqlConn* conn = new MysqlConn;
    conn->connect(m_user, m_passwd, m_dbname, m_ip, m_port);
    conn->refreshAliveTime();
    m_connectionQ.push(conn);
}

ConnectionPool::ConnectionPool()
{
    //加载配置文件
    if (!parseJsonFile()) {
        return;
    }
    for (int i = 0; i < min_size;i++) {
        addConnection();
    }
    std::thread producter(&ConnectionPool::productConnection,this);
    std::thread recycler(&ConnectionPool::recycleConnection,this);
    producter.detach();
    recycler.detach();
}
