#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void math(char data_string[])
{
	if(!data_string)
	{
		return;
	}
	char *data1 = NULL;
	char *data2 = NULL;
	char *start = data_string;
	while(*start != '\0')
	{
		if(*start == '=' && data1 == NULL)
		{
			data1 = start+1;
			start++;
			continue;
		}
		if(*start == '&')
		{
			*start = '\0';
			start++;
			continue;
		}
		if(*start == '=' && data1 != NULL)
		{
			data2 = start+1;
			break;
		}
		start++;
	}
	int int_data1 = atoi(data1);
	int int_data2 = atoi(data2);
	int add_res = int_data1+int_data2;
	int sub_res = int_data1-int_data2;
	int mul_res = int_data1*int_data2;
	double div_res = (double)int_data1/(double)int_data2;

	printf("<p>math [add] result :%d + %d = %d </p>\n", int_data1, int_data2,add_res);
	printf("<p>math [sub] result :%d - %d = %d </p>\n", int_data1, int_data2,sub_res);
	printf("<p>math [mul] result :%d * %d = %d </p>\n", int_data1, int_data2,mul_res);
	printf("<p>math [div] result :%d / %d = %0.2f </p>\n", int_data1, int_data2,div_res);
}

int main()
{
	int content_length = -1;
	char method[1024];
	char query_string[1024];
	char post_data[4096];
	memset(post_data, '\0', sizeof(post_data));
	memset(method ,'\0', sizeof(method));
	memset(query_string, '\0', sizeof(query_string));

    printf("<html>\n");
    printf("\t<head>MATH</br></head>");
	printf("\t<body>\n");
	strcpy(method, getenv("REQUEST_METHOD"));
	if(strcasecmp("GET", method) == 0)
	{
		strcpy(query_string, getenv("QUERY_STRING"));
		math(query_string);
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
		printf("<p>post data: %s</p>", post_data);
		math(post_data);
	}
	else
	{
  	//DO NOTHING
		return 1;
	}

	printf("\t</body>\n");
    printf("</html>\n");
    return 0;
}
