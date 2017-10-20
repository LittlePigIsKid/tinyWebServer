#include "sha.h"
#include "http.h"
#include "connect.h"
#include "dbg.h"

#define MAX_EVENTS 10

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
	setnoblocking(listenfd);

	//bind the address to listener
	assert(bind(listenfd, (struct sockaddr*) &address, sizeof(address)) != -1);

	assert(listen(listenfd, 5) != -1);
	
	int epollfd = epoll_create1(0);
	if (epollfd == -1) {
		perror("epoll_create1");
		exit(EXIT_FAILURE);
	}
	struct epoll_event event, events[MAX_EVENTS]; 
	event.events = EPOLLIN;
	event.data.fd = listenfd;
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &event) == -1) {
		perror("epoll_ctl: listen_sock");
		exit(EXIT_FAILURE);
	}

	for (;;) {
		int nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
		if (-1 == nfds) {
			log_info("%s\n", "epoll_wait");
			perror("epoll_wait");
			exit(EXIT_FAILURE);
		}
		for (int n = 0; n < nfds; n++) {
			if (listenfd == events[n].data.fd) {
				struct sockaddr_in client_address;
				socklen_t client_addr_length = sizeof(client_address);
				int connfd = accept(listenfd, (struct sockaddr*) &client_address, &client_addr_length);
				assert(connfd >= 0); 

				setnoblocking(connfd);
				event.events = EPOLLIN |EPOLLET;
				event.data.fd = connfd;
				if (epoll_ctl(epollfd, EPOLL_CTL_ADD, connfd, &event) == -1) {
					perror("epoll_ctl: connfd");
					exit(EXIT_FAILURE);
				} 
			} else {
				struct http_request request;
				int connfd = events[n].data.fd;
				process_http_request(connfd, &request);
				log_info("method: %s\n", request.method);
				log_info("edtion: %s\n", request.edition);

				//write to the client
				int back_fd = open("./index.html", O_RDONLY);
				assert(back_fd != -1);
				char buf_write[WRITE_BUFFER];
				int write_size;
		
				log_info("out put the response\n");
			
				send_headers(connfd, NULL);	

				//send(connfd, head_of_http, sizeof(head_of_http), 0); 
				while ((write_size = read(back_fd, buf_write, WRITE_BUFFER)) > 0) {
					assert(write(connfd, buf_write, write_size) == write_size);
					printf("%s\n", buf_write);
				}

				log_info("finish write back\n");

				assert(write_size == 0);
				close(back_fd);

				close(connfd);
				printf("close connfd\n");
			}

		}
	}
//	while(1) {
//		struct sockaddr_in client_address;
//		socklen_t client_addr_length = sizeof(client_address);
//		int connfd = accept(listenfd, (struct sockaddr*) &client_address, &client_addr_length);
//		assert(connfd >= 0);
//
//		struct http_request request;
//		//memset(request, 
//		process_http_request(connfd, &request);
//		log_info("method: %s\n", request.method);
//		log_info("edtion: %s\n", request.edition);
//
//		//write to the client
//		int back_fd = open("./index.html", O_RDONLY);
//		assert(back_fd != -1);
//		char buf_write[WRITE_BUFFER];
//		int write_size;
//		
//		log_info("out put the response\n");
//		
//		send_headers(connfd, NULL);	
//
//		//send(connfd, head_of_http, sizeof(head_of_http), 0); 
//		while ((write_size = read(back_fd, buf_write, WRITE_BUFFER)) > 0) {
//			assert(write(connfd, buf_write, write_size) == write_size);
//			printf("%s\n", buf_write);
//		}
//
//		log_info("finish write back\n");
//
//		assert(write_size == 0);
//		close(back_fd);
//
//		//close(connfd);
//		//printf("close connfd\n");
//	}
}



