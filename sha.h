#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<errno.h>
#include<string.h>
#include<assert.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<arpa/inet.h>

#define READ_BUFFER 1024
#define WRITE_BUFFER 1024
#define SERVER_STRING "Server: jdbhttpd/0.1.0\r\n"