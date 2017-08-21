#include"sha.h"
#include"http.h"
#include"connect.h"

#include<errno.h>
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

int process_http_request(int client, struct http_request *request);
int recv_one_line(int sock, char *buf, int size);

int
main(int argc, char *argv[])
{
	assert(argc >= 2);

	char *ip = "127.0.0.1";
	int port = atoi(argv[1]);
	
	//build the address 
	struct sockaddr_in address;
	bzero(&address, sizeof(address));
	address.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &address.sin_addr);
	address.sin_port = htons(port);

	//build the listener
	int listenfd = socket(PF_INET, SOCK_STREAM, 0);
	assert(listenfd >= 0);

	// make the socket non_block
	//setnoblocking(listenfd);

	//bind the address to listener
	assert(bind(listenfd, (struct sockaddr*) &address, sizeof(address)) != -1);

	assert(listen(listenfd, 5) != -1);
	
	while(1) {
		struct sockaddr_in client_address;
		socklen_t client_addr_length = sizeof(client_address);
		int connfd = accept(listenfd, (struct sockaddr*) &client_address, &client_addr_length);
		assert(connfd >= 0);

		//read from the connfd and print out 
		char buf_read[READ_BUFFER];	
		int read_size;
		//while ((read_size = read(connfd, buf_read, READ_BUFFER)) > 0) { 
			//assert(write(1, buf_read, read_size) == read_size);
			//printf("the read_size = %d\n", read_size);
		//}
//		read_size = read(connfd, buf_read, READ_BUFFER);
//		assert(write(1, buf_read, read_size) == read_size);
//		printf("the read_size = %d\n", read_size);

		struct http_request request;
		//memset(request, 
		process_http_request(connfd, &request);

		//write to the client
		int back_fd = open("./index.html", O_RDONLY);
		assert(back_fd != -1);
		char buf_write[WRITE_BUFFER];
		int write_size;
		
		printf("out put the response\n");
		
		send_headers(connfd, NULL);	

		//send(connfd, head_of_http, sizeof(head_of_http), 0); 
		while ((write_size = read(back_fd, buf_write, WRITE_BUFFER)) > 0) {
			assert(write(connfd, buf_write, write_size) == write_size);
			printf("%s\n", buf_write);
		}

		printf("finish write back\n");

		assert(write_size == 0);
		close(back_fd);

		close(connfd);
		printf("close connfd\n");
	}
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


