#ifndef _HTTPD_H_
#define _HTTPD_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/stat.h>

#define _BACK_LOG_ 5
#define _COMM_SIZE_ 1024
#define MAIN_PAGE "index.html"
#define HTTP_VERSION "HTTP/1.1"

#endif
