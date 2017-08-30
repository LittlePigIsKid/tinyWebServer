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
#define C_SIZE 24 // cache size
#define T_SIZE 1024 // token size
#define A_SIZE 1024 //user agent size
#define AC_SIZE 1024 //accept 
#define H_SIZE 64 //host size
#define EN_SIZE 64 //accept-encoding size
#define CO_SIZE 64 //content-length size
#define CONN_SIZE 64 //connection size

#define RECV_LINE_BUF 1024 // recv function buf size

/*
* this struct is used to storge the http request infomation
*/
struct http_request {
	char method[M_SIZE];
	char url[U_SIZE];
	char edition[E_SIZE];
	char status[S_SIZE];
	char cache_control[C_SIZE];
	char token[T_SIZE];
	int context_length; // http post request context-length
};

/*
*this function is used to send the head of response
*/
void send_headers(int client, const char *filename);
/*
*if succeed, return 1 else return -1
*/
int process_http_request(int client, struct http_request *request);
/*
*recv one line from the sock, not process the buf overflow
*/
int recv_one_line(int sock, char *buf, int size);
/*
*recv n bytes from the sock, not process the buf overflow
*/
int recv_n_bytes(int sock, char *buf, int size, int n);
/*
* process get request
*/
int process_http_get(int client, struct http_request *request);
/*
*process post request
*/
int process_http_post(int client, struct http_request *request);
#endif
