#include "MysqlConn.h"

MysqlConn::MysqlConn()
{
	m_conn = mysql_init(nullptr);
	mysql_set_character_set(m_conn,"utf8");
}

MysqlConn::~MysqlConn() {
	if (m_conn!=nullptr) {
		mysql_close(m_conn);
	}
	freeResult();
}

//连接数据库
bool MysqlConn::connect(std::string user, std::string password, std::string dbname,std::string ip, unsigned short port)
{
	MYSQL* ptr=mysql_real_connect(m_conn,ip.c_str(),user.c_str(),password.c_str(),dbname.c_str(),port,nullptr,0);
	return ptr!=nullptr;
}

//更新数据库:update,insert,delete
bool MysqlConn::update(std::string sql)
{
	if (mysql_query(m_conn, sql.c_str())) {
		return false;
	}
	return true;

}

//查询数据库
bool MysqlConn::query(std::string sql)
{
	freeResult();
	if (mysql_query(m_conn, sql.c_str())) {
		return false;
	}
	m_result=mysql_store_result(m_conn);
	return true;
}


//遍历查询得到的结果集
bool MysqlConn::next()
{
	if (m_result!=nullptr) {
		m_row=mysql_fetch_row(m_result);
		if (m_row!=nullptr) {
			return true;
		}
	}
	return false;
}


//得到结果集中的字段值
std::string MysqlConn::value(int index)
{
	int count = mysql_num_fields(m_result);
	if (index >= count || index < 0) {
		return std::string();
	}
	char* val = m_row[index];

	//假如得到的字段值是0101二进制
	unsigned long length=mysql_fetch_lengths(m_result)[index];
	return std::string(val,length);
}


//事务操作
bool MysqlConn::transaction()
{
	return mysql_autocommit(m_conn,false);
}


//事务成功
bool MysqlConn::commit()
{
	return mysql_commit(m_conn);
}


//事务失败
bool MysqlConn::rollback()
{
	return mysql_rollback(m_conn);
}

//刷新起始的空闲时间点
void MysqlConn::refreshAliveTime()
{
	m_alivetime = std::chrono::steady_clock::now();
}

//计算连接存活的总时长
long long MysqlConn::getAliveTime()
{
	std::chrono::nanoseconds res = std::chrono::steady_clock::now() - m_alivetime;
	std::chrono::milliseconds millsec = std::chrono::duration_cast<std::chrono::milliseconds>(res);
	return millsec.count();
}

//施法结果集资源
void MysqlConn::freeResult()
{
	if (m_result!=nullptr) {
		mysql_free_result(m_result);
		m_result = nullptr;
	}
}

