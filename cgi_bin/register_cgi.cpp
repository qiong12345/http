#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sql_connect.h" //<sql_connect.h>

const string _remote_ip = "127.0.0.1";
const string _remote_user = "root";
const string _remote_passwd = "";
const string _remote_db   = "remote_db";


int main()
{
	int content_length = -1;
	char method[1024];
	char query_string[1024];
	char post_data[4096];
	memset(method, '\0', sizeof(method));
	memset(query_string, '\0', sizeof(query_string));
	memset(post_data, '\0', sizeof(post_data));

    const string _host = _remote_ip;
	const string _user = _remote_user;
	const string _passwd = _remote_passwd;
	const string _db   = _remote_db;
	cout<<"<html>"<<endl;
	cout<<"<head>register student</head>"<<endl;
	cout<<"<body>"<<endl;

	strcpy(method, getenv("REQUEST_METHOD"));
	printf("<p>method : %s</p>", method);
	if(strcasecmp("GET", method) == 0)
	{
		strcpy(query_string, getenv("QUERY_STRING"));
	}
	else if(strcasecmp("POST", method) == 0)
	{
		content_length = atoi(getenv("CONTENT_LENGTH"));
		int i = 0;
		for(; i < content_length; ++i)
		{
			read(0, &post_data[i], 1);
		}
		post_data[i] = '\0';
		printf("<p>post_data : %s</p>",post_data);
	}
	else
	{
  	//DO NOTHING
		return 1;
	}
	char name[64];
	char age[64];
	char school[64];
	char hobby[64];

	int i = 0;
	while(post_data[i] != '\0')
	{
		if(post_data[i] == '=' || post_data[i] == '&')
		{
			post_data[i] = ' ';
		}
		i++;
	}
	memset(name,'\0', sizeof(name));
	memset(age,'\0', sizeof(age));
	memset(school,'\0', sizeof(school));
	memset(hobby,'\0', sizeof(hobby));

	sscanf(post_data, "%*s %s %*s %s %*s %s %*s %s",name, age, school, hobby);

	string _name = name;
	string _age = age;
	string _school = school;
	string _hobby = hobby;
	sql_connecter conn(_host, _user, _passwd, _db);
	conn.begin_connect();
    conn.insert_sql_(_name, _age, _school, _hobby);	

	cout<<"</body>"<<endl;
	cout<<"</html>"<<endl;
}

