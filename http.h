#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>

/*
*this method is used to send headers to the client
*/
#ifndef __HTTP_H
#define __HTTP_H

#define M_SIZE 24 // method size
#define S_SIZE 24 // status size
#define U_SIZE 128 // url size
#define E_SIZE 24 // edtion of http

#define RECV_LINE_BUF 1024 // recv function buf size

struct http_request {
	char method[M_SIZE];
	char url[U_SIZE];
	char edition[E_SIZE];
	char status[S_SIZE];
};


void send_headers(int client, const char *filename);
int process_http_request(int client, struct http_request *request);
int recv_one_line(int sock, char *buf, int size);
#endif
