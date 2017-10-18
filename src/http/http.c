#include "../core/sha.h"
#include "http.h"

void send_headers(int client, const char *filename)
{
    char buf[1024];
    (void)filename;  /* could use filename to determine file type */

    strcpy(buf, "HTTP/1.0 200 OK\r\n");
    send(client, buf, strlen(buf), 0);
    strcpy(buf, SERVER_STRING);
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "Content-Type: text/html\r\n");
    send(client, buf, strlen(buf), 0);
    strcpy(buf, "\r\n");
    send(client, buf, strlen(buf), 0);
}

int process_http_request(int client, struct http_request *request)
{
	char buf[RECV_LINE_BUF];
	memset(buf, '\0', sizeof(buf));
	recv_one_line(client, buf, sizeof(buf));
	sscanf(buf, "%s %s %s", (request->method), (request->url), (request->edition));
	printf("first line is: %s", buf);
	printf("end_reading first line\n");
	if (0 == strcmp(request->method, "GET")) {
		printf("get method\n");
		//process get method
	}
	else if (0 == strcmp(request->method, "POST")) {
		printf("post method \n");
		process_http_post(client, request);
		
	}
	else {
		printf("method not supportted");
		return ERROR_STATUS;
	}
	return SUCCESS_STATUS;
}

/*
* return the size that received
*/
int recv_one_line(int sock, char *buf, int size)
{
	int index = 0;
	int recv_size = 0;
	char recv_buf[1];
	char recv_next_buf[1];
	while (index < size && ((recv_size = recv(sock, recv_buf, 1, 0)) == 1)) {
		if (recv_buf[0] == '\r') {
			if (((recv_size = recv(sock, recv_next_buf, 1, 0)) == 1) && recv_next_buf[0] == '\n') {
				buf[index++] = '\n';
				break; 
			} else if (recv_size != 1) {
				if (recv_size == 0) {
					printf("the recv return nothing\n");
				} else {
					printf("recv error, errno = %d\n", errno);
				}
			} else {
				buf[index++] = recv_buf[0];
			}
		} else {
			buf[index++] = recv_buf[0];
		}

	}
	return index;
}

int process_http_get(int client, struct http_request *request)
{
	
}

int process_http_post(int client, struct http_request *request)
{
	char buf[RECV_LINE_BUF];
	char key[1024];
	char value[1024];
	//process head
	do {
		memset(buf, '\0', sizeof(buf));
		memset(key, '\0', sizeof(key));
		memset(value, '\0', sizeof(value));
		recv_one_line(client, buf, sizeof(buf));
		sscanf(buf, "%[^:]%*s%s", key, value);
		printf("%s", buf);
		printf("key: %s\n", key);
		printf("value: %s\n", value);
		if (0 == strcasecmp("content-length", key))
			request->context_length = atoi(value);
	} while(0 != strcmp(buf, "\n"));
	printf("out of process http post\n");
	memset(buf, '\0', sizeof(buf));
	if (SUCCESS_STATUS == recv_n_bytes(client, buf, sizeof(buf), request->context_length))
		printf("%s\n", buf);
	else 
		printf("process post body error\n");
}

int recv_n_bytes(int sock, char*buf, int size, int n)
{
	int result = read(sock, buf, n);
	if (result != n) {
		return ERROR_STATUS;
	} else {
		return SUCCESS_STATUS;
	}
}
