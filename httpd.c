#include "httpd.h"
//wo
static void usage(const char *proc)
{
	printf("usage : %s [PORT]\n", proc);
}

void print_log(const char *fun, int line, int err_no,\
	  const char *err_str)
{
	printf("[%s: %d] [%d] [%s]\n", fun, line, err_no, err_str);
}

void print_debug(const char *msg)
{
#ifdef _DEBUG_
	printf("%s\n", msg);
#endif
}


void bad_request(int client)
{
	char buf[1024];

	sprintf(buf, "HTTP/1.1 400 BAD REQUEST\r\n");
	send(client, buf, sizeof(buf), 0);
	sprintf(buf, "Content-type:text/html!\r\n");
	send(client, buf, sizeof(buf), 0);
	sprintf(buf, "\r\n");
	send(client, buf, sizeof(buf), 0);
	sprintf(buf, "<P>Your browser sent a bad request,");;
	send(client, buf, sizeof(buf), 0);
	sprintf(buf, "such as a POST without a Content-Length.\r\n");
	send(client, buf, sizeof(buf), 0);
}

void cannot_execute(int client)
{
	char buf[1024];

	sprintf(buf, "HTTP/1.1 500 Internal Server Error\r\n");
	send(client, buf, sizeof(buf), 0);
	sprintf(buf, "Content-type:text/html!\r\n");
	send(client, buf, sizeof(buf), 0);
	sprintf(buf, "\r\n");
	send(client, buf, sizeof(buf), 0);
	sprintf(buf, "<P>Erroe prohibited CGI execution.\r\n");
	send(client, buf, sizeof(buf), 0);
}


void not_found(int client)  
{  
   	char buf[1024];  
		  
	/* 404 页面 */  
    sprintf(buf, "HTTP/1.1 404 NOT FOUND\r\n");  
	send(client, buf, strlen(buf), 0);  
    /*服务器信息*/   
	sprintf(buf, "Content-Type: text/html\r\n"); 
	send(client, buf, strlen(buf), 0);  
	sprintf(buf, "\r\n");  
	send(client, buf, strlen(buf), 0);  
	sprintf(buf, "<HTML><TITLE>Not Found</TITLE>\r\n");  
	send(client, buf, strlen(buf), 0);  
	sprintf(buf, "<BODY><P>The server could not fulfill\r\n");  
	send(client, buf, strlen(buf), 0);  
	sprintf(buf, "your request because the resource specified\r\n");  
	send(client, buf, strlen(buf), 0);  
    sprintf(buf, "is unavailable or nonexistent.\r\n");
	send(client, buf, strlen(buf), 0);
    sprintf(buf, "</BODY></HTML>\r\n"); 
	send(client, buf, strlen(buf), 0);  
}  

void unimplemented(int client)  
{  
	char buf[1024];  
		  
	/* HTTP method 不被支持*/  
	sprintf(buf, "HTTP/1.1 501 Method Not Implemented\r\n");  
	send(client, buf, strlen(buf), 0);  
	/*服务器信息*/  
 
	sprintf(buf, "Content-Type: text/html\r\n");  
	send(client, buf, strlen(buf), 0);  
	sprintf(buf, "\r\n");  
	send(client, buf, strlen(buf), 0);  
	sprintf(buf, "<HTML><HEAD><TITLE>Method Not Implemented\r\n");  
	send(client, buf, strlen(buf), 0);  
	sprintf(buf, "</TITLE></HEAD>\r\n");  
	send(client, buf, strlen(buf), 0);  
	sprintf(buf, "<BODY><P>HTTP request method not supported.\r\n");  
	send(client, buf, strlen(buf), 0);  
	sprintf(buf, "</BODY></HTML>\r\n");  
	send(client, buf, strlen(buf), 0);  
}  


void echo_error_to_client(int client, int error_code)
{
	switch(error_code)
	{
		case 400:
			bad_request(client);
			break;
		case 404:
			not_found(client);
			break;
		case 500:
			cannot_execute(client);
			break;
		case 501:
			unimplemented(client);
			break;
		default:
			perror("error_code");
			break;
	}
}

static int start(short port)
{
	int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(listen_sock == -1){
    	print_log(__FUNCTION__, __LINE__, errno, strerror(errno));
		//wenjian hang cuowumai cuowuxiaoxi
		exit(1);
		//zhong zhi jin cheng
	}

	//reuse port
	//problem
	int flag = 1;
	setsockopt(listen_sock, SOL_SOCKET, SO_REUSEPORT, &flag, sizeof(flag));

	struct sockaddr_in local;
	//chuan ru ben di xin xi
	local.sin_family = AF_INET;  //xieyi
	local.sin_port = htons(port);//zhujixulie-->wangluoxulie
    local.sin_addr.s_addr = htonl(INADDR_ANY);
	//#define INADDR_ANY ((unsigned) 0x00000000)
	//bang ding de IP di zhi shi zhu ji shang ren yi you xiao di zhi
	//bu xu yao wo men zhuan hua
	
	socklen_t len = sizeof(local);
	if(bind(listen_sock, (struct sockaddr*)&local, len) == -1)
	{
		print_log(__FUNCTION__, __LINE__, errno, strerror(errno));
		//wenjian hang cuowumai cuowuxiaoxi
		exit(2);
		//zhong zhi jin cheng
	}

	if(listen(listen_sock, _BACK_LOG_) == -1)
	{
	    print_log(__FUNCTION__, __LINE__, errno, strerror(errno));
		//wenjian hang cuowumai cuowuxiaoxi
		exit(3);
		//zhong zhi jin cheng
	}

	return listen_sock;
}

//return num success
//return <=0 failed
int get_line(int sock, char *buf, int len)
{
	if(!buf || len < 0)
	{
		return 0;
	}
	int i = 0;
	int n = 0;
	char c = '\0';

	while(i < len-1 && c != '\n')
	{
		n = recv(sock, &c, 1, 0);
		if(n > 0)
		{
			//Windows \r->\n  Linux \r\n->\n
			if(c == '\r')
			{
				n = recv(sock, &c, 1, MSG_PEEK);
				//MSG_PEEK(xiu tan)
				//kui tan nei he huan chong qv shu ju
				if(n > 0 && c == '\n')//Windows
				{
					recv(sock, &c, 1, 0);//Delete (cong nei he huan chong qv)
				}
				else
				{
					c = '\n';
				}
			}
			buf[i++] = c;
		}
		else//failed
		{
			c = '\n';
		}
	}

	buf[i] = '\0';
	return i;
}

void clear_header(int client)
{
	char buf[1024];
	memset(buf, '\0', sizeof(buf));
	int ret = 0;

	do
	{
		ret = get_line(client, buf, sizeof(buf));
	}while(ret > 0 && strcmp(buf, "\n") != 0);
}

void echo_html(int client, const char *path, unsigned int file_size)
{
	if(!path)
	{
		return;
	}

	int in_fd = open(path,O_RDONLY);
	if(in_fd < 0){
	print_debug("open index html error");
	echo_error_to_client(client, 404);
	return;
	}
	print_debug("open index html success");
	char echo_line[1024];
	strncpy(echo_line, HTTP_VERSION, strlen(HTTP_VERSION)+1);
	strcat(echo_line, " 200 OK");
	strcat(echo_line, "\r\n\r\n");

	send(client, echo_line, strlen(echo_line), 0);
	//fa song gei dui duan wang luo
	print_debug("send echo head success");
	if(sendfile(client, in_fd, NULL, file_size) < 0)
	{
		print_debug("send_file error");
		echo_error_to_client(client, 404);
		close(in_fd);
		return;
	}

	print_debug("send_file success");

	close(in_fd);
}

void exe_cgi(int sock_client,const char* path,const char* method,const char*query_string)
{
	char buf[_COMM_SIZE_];
	int numchars = 0;
	int content_length = -1;

	//pipe
	int cgi_input[2] = {0,0};
	int cgi_output[2] = {0,0};
	pid_t id;

	if(strcasecmp(method,"GET") == 0){
		clear_header(sock_client);
	}else{
		//POST
		do{
			memset(buf,'\0',sizeof(buf));
			numchars = get_line(sock_client,buf,sizeof(buf));
			if(strncasecmp(buf,"Content-Length:",strlen("Content-Length:"))==0){
				content_length = atoi(&buf[16]);
			}
		}while(numchars>0&&strcmp(buf,"\n")!=0);
	    if(content_length == -1){
	    	echo_error_to_client(sock_client,400);
	    	return;
	    }
	}

	memset(buf,'\0',sizeof(buf));
	strcpy(buf,HTTP_VERSION);
	strcat(buf," 200 OK\r\n\r\n");
	send(sock_client,buf,strlen(buf),0);

	if(pipe(cgi_input) == -1){
		echo_error_to_client(sock_client,404);
	}
	if(pipe(cgi_output) == -1){
		close(cgi_input[0]);
		close(cgi_input[1]);
		echo_error_to_client(sock_client,404);
		return;
	}

	if((id = fork()) < 0){
		close(cgi_input[0]);
		close(cgi_input[1]);
		close(cgi_output[0]);
		close(cgi_output[1]);
		echo_error_to_client(sock_client,404);
		return;
	}else if(id == 0){//child
		char method_env[_COMM_SIZE_];
		char query_env[_COMM_SIZE_/10];
		char content_len_env[_COMM_SIZE_];
		memset(method_env,'\0',sizeof(method_env));
		memset(query_env,'\0',sizeof(query_env));
		memset(content_len_env,'\0',sizeof(content_len_env));

		close(cgi_input[1]);
		close(cgi_output[0]);

		dup2(cgi_input[0],0);
		dup2(cgi_output[1],1);

		sprintf(method_env,"REQUEST_METHOD=%s",method);
		putenv(method_env);
		if(strcasecmp("GET",method) == 0){//GET
			sprintf(query_env,"QUERY_STRING=%s",query_string);
			putenv(query_env);
		}else{//POST
			sprintf(content_len_env,"CONTENT_LENGTH=%d",content_length);
			putenv(content_len_env);
		}

		execl(path,path,NULL);
		exit(1);

	}else{//father
		close(cgi_input[0]);
		close(cgi_output[1]);

		int i = 0;
		char c = '\0';
		if(strcasecmp("POST",method) == 0){
			for(;i<content_length;i++){
				recv(sock_client,&c,1, 0);
				write(cgi_input[1],&c,1);
			}
		}
		while(read(cgi_output[0],&c,1) > 0){
			send(sock_client,&c,1, 0);
		}

		close(cgi_input[1]);
		close(cgi_output[0]);

		waitpid(id,NULL,0);
	}
}




void *accept_request(void *arg)
{
	print_debug("get a new connect...\n");
	pthread_detach(pthread_self());//detach
	//bu xu yao qv deng dai qi ta jin cheng
	//fen li zhuang tai
	int sock_client = (int)arg;

	char buffer[_COMM_SIZE_];
	char *query_string = NULL;
	char url[_COMM_SIZE_];
	char method[_COMM_SIZE_/10];
	char path[_COMM_SIZE_];
	int cgi = 0;
	//cgi == 1 zhi xing
	//database interface
	//interface

	memset(path, '\0', sizeof(path));
	memset(url, '\0', sizeof(url));
	memset(method, '\0', sizeof(method));
	memset(buffer, '\0', sizeof(buffer));

	if(get_line(sock_client, buffer, sizeof(buffer)) < 0)//failed
	{
		echo_error_to_client(sock_client, 400);
		return (void*)-1;
	}
	int i = 0;
	int j = 0;//buffer line index
	//get method
	while(!isspace(buffer[j]) && i < sizeof(method)-1\
		   	&& j < sizeof(buffer))//bu shi kong ge
	{
		method[i] = buffer[j];
		i++;
		j++;	   
	}
	method[i] = '\0';

	
	//clear space point useful url start
	while(isspace(buffer[j]) && j < sizeof(buffer))
	{
		j++;
	}

	//get url
	i = 0;
	while(!isspace(buffer[j]) && i < sizeof(url)-1
			&& j < sizeof(buffer))
	{
		url[i] = buffer[j];
		i++;
		j++;
	}
	url[i] = '\0';

	print_debug(method);
	print_debug(url);

	if(strcasecmp(method, "GET") && strcasecmp(method, "POST"))
	{
		echo_error_to_client(sock_client, 501);
		return (void*)-1;
	}

	if(strcasecmp(method, "POST") == 0)
	{
		cgi = 1;
	}

	if(strcasecmp(method, "GET") == 0)
	{//huo qv url
		query_string = url;
		//query_string-->
		//zi yuan ding wei
		while(*query_string != '?' && *query_string != '\0')
		{
			query_string++;
		}
		if(*query_string == '?')//url = /XXX/XXX+?+arg
		{
			*query_string = '\0';
			query_string++;
			cgi = 1;
			//ji yu URL chuan di de can shu
			//zhi xiang can shu
			//
			//she ji dao chu li xu yao diao yong CGI mo kuai
		}
	}

	sprintf(path, "htdocs%s", url);
	if(path[strlen(path)-1] == '/')
	{   //DIR 
		//cha zhao CGI zi yuan
		strcat(path, MAIN_PAGE);
	}

	
	print_debug(path);

	struct stat st;
	//huo qv dang qian zi yuan lu jing
	if(stat(path, &st) == -1)
	{
		//int stat(const char *file_name, struct stat *buf)
		//tong gup wen jian ming filename huo qv wen jian xin xi
		//bing bao cun zai buf zhi xiang de jie gou ti zhong
		//error -1
		print_debug("missing cgi");
		perror("stat");
		//failed, does not exit
		clear_header(sock_client);
		echo_error_to_client(sock_client, 404);
	}
	else
	{
		//file exist
		if(S_ISDIR(st.st_mode))
		{
			strcat(path, "/");
			strcat(path, MAIN_PAGE);
		}
		else if(st.st_mode & S_IXUSR ||\
				st.st_mode & S_IXGRP ||\
				st.st_mode & S_IXOTH)
		{
			//DIR
		    cgi = 1;
		}
		else
		{}

		if(cgi)
		{
			exe_cgi(sock_client, path, method, query_string);
		}
		else
		{
			//pu tong zi yuan
			clear_header(sock_client);
			print_debug("begin enter our echo_html");
			echo_html(sock_client, path, st.st_mode);
			//fan hui yi ge wang ye
		}
	}

	close(sock_client);
	//zhu dong guan bi
	//HTTP wu lian jie
	return NULL;
}

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		//yi zhi ding zi dong huo qv IP
		usage(argv[0]);
		exit(1);
	}
	int port = atoi(argv[1]);
	int sock = start(port);

	//listen_sock
	struct sockaddr_in client;
	socklen_t len = 0;

	while(1)
	{
		int new_sock = accept(sock, (struct sockaddr*)&client, &len);
			//jiang cong jin ting tao jie zi na hui lai de fang dao
			// client zhong
		if(new_sock == -1)//accept error
		{
			print_log(__FUNCTION__, __LINE__, errno, strerror(errno));
		    //wenjian hang cuowumai cuowuxiaoxi
		    
			continue;
			//ji xu xun huan
		}
	    pthread_t new_thread;
		pthread_create(&new_thread, NULL, accept_request, (void*)new_sock);
			//chuang jian xian cheng

	}

	return 0;
}

