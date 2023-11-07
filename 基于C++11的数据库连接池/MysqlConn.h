#pragma once
#include <iostream>
#include <mysql.h>
#include <chrono>
class MysqlConn
{
public:
	//初始化数据库连接
	MysqlConn();
	//释放数据库连接
	~MysqlConn();
	//连接数据库
	bool connect(std::string user,std::string password,std::string dbname,std::string ip,unsigned short port=3306);
	//更新数据库:update,insert,delete
	bool update(std::string sql);
	//查询数据库
	bool query(std::string sql);
	//遍历查询得到的结果集
	bool next();
	//得到结果集中的字段值
	std::string value(int index);
	//事务操作
	bool transaction();
	//事务成功
	bool commit();
	//事务失败
	bool rollback();
	//刷新起始的空闲时间点
	void refreshAliveTime();
	//计算连接存货的总时长
	long long getAliveTime();
private:
	void freeResult();
	MYSQL* m_conn = nullptr;
	MYSQL_RES* m_result = nullptr;
	MYSQL_ROW m_row = nullptr;

	//计算时长
	std::chrono::steady_clock::time_point m_alivetime;
};

