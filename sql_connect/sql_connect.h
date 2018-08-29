#ifndef _SQL_CONNECT_
#define _SAL_CONNECT_

#include <iostream>
#include "mysql.h"
#include <string>
using namespace std;

#pragma comment(lib,"mysqlclient.lib")

class sql_connecter
{
public:
	sql_connecter(const string &host, const string &user, const string &passwd, const string &db);
	bool begin_connect();
	bool close_connect();
	bool insert_sql(const string &data);
	bool select_sql(string field_name[],string _out_data[][5],\
		   	int &_out_row);
//	bool updata_sql();
//	bool delete_table();
//	bool delete_sql();
//	bool creat_table();
	~sql_connecter();
	void show_info();
private:
	MYSQL_RES *res;
	//这个结构代表返回行的一个查询的(SELECT, SHOW, DESCRIBE, EXPLAIN)的
	//结果。从查询返回的信息在本章下文称为结果集合。
	MYSQL *mysql_base;
	string host;
	string user;
	string passwd;
	string db;
};

#endif

