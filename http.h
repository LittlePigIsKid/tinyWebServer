#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>

/*
*this method is used to send headers to the client
*/
#ifndef __HTTP_H
#define __HTTP_H

void send_headers(int client, const char *filename);

#endif
