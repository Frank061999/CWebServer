//
//  http.h
//  CWebServer
//
//  Created by Ardalan Owrangi on 8/3/24.
//

#ifndef http_h
#define http_h

#include <stdio.h>
#include "HashMap.h"

typedef struct HttpRequest {
    int isValid;
    char method[10];  // e.g. "GET", "POST", etc.
    char url[1024];   // the requested URL
    char version[10]; // e.g. "HTTP/1.1"
    HashMap* headers;  // a HashMap of request headers
    char* body;  // the request body (if any)
} HttpRequest;
HttpRequest InitHttpRequest(char* request, size_t requestLength);
#endif /* http_h */
