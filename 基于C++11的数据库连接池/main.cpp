#include <iostream>
#include <memory>
#include "MysqlConn.h"
int query() {
	MysqlConn conn;
	conn.connect("root","tanzhiming","tanzhiming","192.168.10.1");
	std::string sql = "INSERT INTO student VALUES(4,'Ãú',24,'ÄÐ')";
	bool flag=conn.update(sql);
	std::cout << "flag value = " << flag << std::endl;

	sql = "SELECT * FROM student";
	conn.query(sql);

	while (conn.next()) {
		std::cout << conn.value(0) << ","
			<< conn.value(1) << ","
			<< conn.value(2) << ","
			<< conn.value(3) << std::endl; 
	}
	return 0;
}
int main() {
	query();
	return 0;
}