#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>

/*
*this method is used to send headers to the client
*/
#ifndef __HTTP_H
#define __HTTP_H

#define METHOD_SIZE 24 // method size
#define STATUS_SIZE 24 // status size
#define URl_SIZE 128 // url size
#define EDITION_SIZE 24 // edtion of http
#define CACHE_SIZE 24 // cache size
#define TOKEN_SIZE 1024 // token size
#define AGENT_SIZE 1024 //user agent size
#define ACCEPT_SIZE 1024 //accept 
#define HOST_SIZE 64 //host size
#define ENCODING_SIZE 64 //accept-encoding size
#define CONTENT_LENGTH_SIZE 64 //content-length size
#define CONNECTION_SIZE 64 //connection size
#define CONTENT_TYPE_SIZE 32 //content-type size
#define RECV_LINE_BUF 1024 // recv function buf size
#define CONTENT_LENGTH_SIZE 32 //content-length size
#define CONTENT_SIZE 1024 //content size of response
/*
* this struct is used to storge the http request infomation
*/
struct http_request {
	char method[METHOD_SIZE];
	char url[URL_SIZE];
	char edition[EDITION_SIZE];
	char status[STATUS_SIZE];
	char cache_control[CACHE_SIZE];
	char token[TOKEN_SIZE];
	int context_length; // http post request context-length
};

struct http_response {
	char status[STATUS_SIZE];
	char http_edition[EDITION_SIZE];
	char server[SERVER_SIZE];
	char connection[CONNECTION_SIZE]; 
	char content_type[CONTENT_TYPE_SIZE];
	char content_length[CONTENT_LENGTH_SIZE];
	char buf[CONTENT_SIZE];
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
