#include "http.h"

#ifndef __CGI_H
#define __CGI_H

/*
*function to call the cgi
*i try to abstract the way to call cgi cause of the motivation to extend the program to support other languages such as java/python/...
*/

int cgi(struct http_response*, struct http_request*);
int cgi_cpp(struct http_response*, struct http_request*);
#endif
