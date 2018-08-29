#include "sql_connect.h"

sql_connecter::sql_connecter(
		const string &_host, 
		const string &_user, 
		const string &_passwd, 
		const string &_db)
{
	this->mysql_base = mysql_init(NULL);
	this->host = _host;
	this->user = _user;
	this->passwd = _passwd;
	this->db = _db;
	this->res = NULL;
}

bool sql_connecter::begin_connect()
{
	if(mysql_real_connect(
			mysql_base, 
			host.c_str(), 
			user.c_str(), 
			passwd.c_str(),
			db.c_str(), 
			3306, NULL, 0)== NULL)
	{
		//连接一个MySQL服务器
		cerr<<"connect error"<<endl;
		return false;
	}else{
		cout<<"connect done"<<endl;
	}
	return true;
}

bool sql_connecter::close_connect()
{
	//关闭一个服务器连接
	mysql_close(mysql_base);
	cout << "connect close" << endl;
}

bool sql_connecter::select_sql(string field_name[],string _out_data[][5],\
	   	int &_out_row)
{
	string sql = "SELECT * FROM student_five_class";
	if(mysql_query(mysql_base, sql.c_str()) == 0)
	{
		//执行指定为一个空结尾的字符串的SQL查询
		cout<< "query success" << endl;
	}else{
		cerr<< "query failed" <<endl;
		return false;
	}
	res = mysql_store_result(mysql_base);
	//通过检查mysql_store_result()是否返回0，可检测查询是否没有结果集
	int row_num = mysql_num_rows(res);
	//返回一个结果集合中的行的数量
	int field_num = mysql_num_fields(res);
	//返回结果集中字段的数
	_out_row = row_num;

	MYSQL_FIELD *fd = NULL;
	//这个结构包含字段信息，例如字段名、类型和大小。其成员在下面更详细地描述
	//。你可以通过重复调用mysql_fetch_field()对每一列获得MYSQL_FIELD结构。
	//字段值不是这个结构的部分；他们被包含在一个MYSQL_ROW结构中
	int i = 0;
	for(; fd = mysql_fetch_field(res); )
	{
		//从结果集中取得列信息并作为对象返回
		//cout << fd->name << " \t";
		field_name[i++] = fd->name;
	}
	//cout << endl;

	//这是一个行数据的类型安全(type-safe)的表示。当前它实现为一个计数字节的字符串数组
	//。（如果字段值可能包含二进制数据，你不能将这些视为空终止串，因为这样的值可以在
	//内部包含空字节) 行通过调用mysql_fetch_row()获得。 
	for(int index= 0; index < row_num; index++)
	{
	    MYSQL_ROW _row = mysql_fetch_row(res);
		if(_row)
		{
			int start = 0;
			for(; start < field_num; ++start)
			{
			_out_data[index][start] = _row[start];	
			//	cout << _row[start] <<"\t";
			}
		//	cout<< endl;
		}
	}
	return true;
}


bool sql_connecter::insert_sql(const string &data)
{
	std::string sql = "INSERT INTO student_five_class(name,age,school, hobby) values";
	sql += "(";
	sql += data;
	sql += ");";
	if(mysql_query(mysql_base, sql.c_str()) == 0)
	{
		//执行指定为一个空结尾的字符串的SQL查询
		cout<< "query success" << endl;
		return true;
	}else{
		cerr<< "query failed" <<endl;
		return false;
	}

}

sql_connecter::~sql_connecter()
{
	close_connect();
	if(res)
	{
		free(res);
	}
}

void sql_connecter::show_info()
{
	cout << mysql_get_client_info() << endl;
	//取得 MySQL 客户端信息
}
#ifdef _DEBUG_

int main()
{
	string _sql_data[1024][5];
	string header[5];
	int curr_row = -1;
	const string _host = "127.0.0.1";
	const string _user = "root";
	const string _passwd = "";
	const string _db = "remote_db";
	const string _data = "\"Jimmy\", 19, \"xigongda\", \"sleeping\"";
	sql_connecter conn(_host, _user, _passwd, _db);
	conn.begin_connect();
	conn.select_sql(header,_sql_data, curr_row);
	sleep(1);
	for(int i = 0; i<5; i++){
		cout<<header[i]<<"\t";
	}
	std::cout<<std::endl;
	for(int i = 0; i<curr_row; i++){
		for(int j=0; j<5; j++){
			cout<<_sql_data[i][j]<<"\t";
		}
		cout<<endl;
	}
	conn.show_info();
	return 0;
}

#endif
