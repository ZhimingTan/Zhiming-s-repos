#pragma once
#include <queue>
#include "MysqlConn.h"
#include <mutex>
#include <condition_variable>
class ConnectionPool
{
public:
	static ConnectionPool* getConnectPool();
	ConnectionPool(const ConnectionPool& obj) = delete;
	ConnectionPool operator=(const ConnectionPool& obj) = delete;
	std::shared_ptr<MysqlConn> getConnection();
	~ConnectionPool();
private:
	bool parseJsonFile();
	void productConnection();
	void recycleConnection();
	void addConnection();
	ConnectionPool();
	std::string m_ip;
	std::string m_user;
	std::string m_passwd;
	std::string m_dbname;
	unsigned short m_port;
	int max_size;
	int min_size;
	int timeout;
	int idletime;
	std::queue<MysqlConn*>m_connectionQ;
	std::mutex m_mutexQ;
	std::condition_variable m_cond;
};

