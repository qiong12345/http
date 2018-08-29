#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sql_connect.h" //sql_connect.h

const string _remote_ip = "127.0.0.1";
const string _remote_user = "root";
const string _remote_passwd = "";
const string _remote_db   = "remote_db";

int main()
{
	int content_length = -1;
	int i, j;
	char method[1024];
	char query_string[1024];
	char post_data[4096];
	memset(method, '\0', sizeof(method));
	memset(query_string, '\0', sizeof(query_string));
	memset(post_data, '\0', sizeof(post_data));
	cout<<"<html>"<<endl;
	cout<<"<head>show student</head>\n"<<endl;
	cout<<"<body>"<<endl;
	const  string _host = _remote_ip;
	const  string _user = _remote_user;
	const  string _passwd = _remote_passwd;
	const  string _db   = _remote_db;

	string _sql_data[1024][5];
	string header[5];
	int curr_row = -1;
	sql_connecter conn(_host, _user, _passwd, _db);
	conn.begin_connect();
	conn.select_sql(header,_sql_data, curr_row);
	//cha xun
	cout<<"<table border=\"1\">"<< endl;
	cout<<"<tr>"<< endl;
	for(i = 0; i<5; i++){
		 cout<<"<th>"<<header[i]<<"</th>"<< endl;
	}
	cout<<"</tr>"<< endl;
	for(i = 0; i<curr_row; i++){
		cout<<"<tr>"<< endl;
		for(j=0; j<5; j++){
			cout<<"<td>"<<_sql_data[i][j]<<"</td>"<< endl;;
		}
		cout<<"</tr>"<< endl;
	}
	cout<<"</table>"<<endl;

	cout<<"</body>"<<endl;
	cout<<"</html>"<<endl;
}
