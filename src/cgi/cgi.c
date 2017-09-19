#include"cgi.h"

int cgi(struct http_response* response, struct http_request* request)
{
	cgi_cpp(response, request);
}
int cgi_cpp(struct http_response *response, struct http_request *request)
{
}
