#include"sha.h"
#include"http.h"

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
	printf("line size is %d\n", recv_one_line(client, buf, sizeof(buf)));
	printf("the first line is:\n%s", buf);
	int index = 0;
	char * ptr;
	char * split = " ";
	ptr = strtok(buf, split);
	while (ptr  != NULL) {
		printf("%s\n", ptr);	
		ptr = strtok(NULL, split);
	}
	printf("end_reading ...\n");
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


