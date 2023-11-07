#pragma once
#include <iostream>
#include <mysql.h>
#include <chrono>
class MysqlConn
{
public:
	//��ʼ�����ݿ�����
	MysqlConn();
	//�ͷ����ݿ�����
	~MysqlConn();
	//�������ݿ�
	bool connect(std::string user,std::string password,std::string dbname,std::string ip,unsigned short port=3306);
	//�������ݿ�:update,insert,delete
	bool update(std::string sql);
	//��ѯ���ݿ�
	bool query(std::string sql);
	//������ѯ�õ��Ľ����
	bool next();
	//�õ�������е��ֶ�ֵ
	std::string value(int index);
	//�������
	bool transaction();
	//����ɹ�
	bool commit();
	//����ʧ��
	bool rollback();
	//ˢ����ʼ�Ŀ���ʱ���
	void refreshAliveTime();
	//�������Ӵ������ʱ��
	long long getAliveTime();
private:
	void freeResult();
	MYSQL* m_conn = nullptr;
	MYSQL_RES* m_result = nullptr;
	MYSQL_ROW m_row = nullptr;

	//����ʱ��
	std::chrono::steady_clock::time_point m_alivetime;
};

