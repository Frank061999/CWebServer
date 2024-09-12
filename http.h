//
//  http.h
//  CWebServer
//
//  Created by Ardalan Owrangi on 8/3/24.
//

#ifndef http_h
#define http_h

#include <stdio.h>
#include "DataStructures/HashMap.h"

typedef struct HttpRequest {
    int isValid;
    char method[10];
    char url[1024];
    char version[10];
    HashMap* headers;
    size_t bodyLength;
    char* body;
} HttpRequest;

HttpRequest ParseHttpHeaders(char* request, size_t requestLength);
int ReceiveRequest(int client_socket, char* buffer, size_t bufferSize, char** endOfRequest, HttpRequest* hr);
int SendResponse(int client_socket, HttpRequest* hr);
#endif /* http_h */
