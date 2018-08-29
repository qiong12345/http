#include "demo_client.h"

void usage(const char *msg)
{
	printf("Usage : %s [IP] [PORT] \n", msg);
}

int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		usage(argv[0]);
		exit(1);
	}
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == -1)
	{
		perror("socket");
		exit(1);
	}

	int port = atoi(argv[2]);
	//lianjieyuanduanfuwuqi
	struct sockaddr_in remote;
	remote.sin_family = AF_INET;
	remote.sin_port = htons(port);
	remote.sin_addr.s_addr = inet_addr(argv[1]);

	if(connect(sock, (struct sockaddr*)&remote, sizeof(remote)) == -1)
	{
		perror("connect");
		exit(2);
	}

	close(sock);
	return 0;
}
