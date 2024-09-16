//
//  http.c
//  CWebServer
//
//  Created by Ardalan Owrangi on 8/3/24.
//

#include "http.h"
#include <arpa/inet.h>
#include <string.h>

#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "DataStructures/StaticHashMaps.h"
#include <errno.h>
#ifdef __linux__
#include <linux/limits.h>
#include "DataStructures/strl.h"
#include <sys/sendfile.h>
#elif __APPLE__
#include <limits.h>
#endif

#define HEADER_BUCKET_SIZE 29


int HasValidHeaders(HttpRequest* hr){
    // TODO: Implement
    return 1;
}


HttpRequest ParseHttpHeaders(char* request, size_t requestLength){
    HttpRequest hr;
    memset(&hr,0,sizeof(hr));
    char* requestCopy = request;
    
    for (int i = 0; i < 3; i++) {
        char* token = strsep(&requestCopy, " \r\n");
        if(token == NULL) goto InvalidRequest;
        switch (i) {
            case 0:
                strlcpy(hr.method, token, sizeof(hr.method));
                break;
            case 1:
                strlcpy(hr.url, token, sizeof(hr.url));
                break;
            case 2:
                strlcpy(hr.version, token, sizeof(hr.version));
                break;
        }
    }
    hr.headers = CreateHashMap(HEADER_BUCKET_SIZE);
    int timesEndofLineEncountered = 0;
    for(char* line = strsep(&requestCopy, "\r\n"); line != NULL; line = strsep(&requestCopy, "\r\n")){
        if(*line == '\0') {
            if(++timesEndofLineEncountered == 2) break;
            continue;
        }
        char* key = strsep(&line, ":");
        while(*line == ' ') line++; // Skip white space
        char* value = strsep(&line, "\r");
        HashMapAppend(hr.headers, key, value);
        timesEndofLineEncountered--;
    }
    if(HasValidHeaders(&hr)) hr.isValid = 1;
    else
        InvalidRequest:
        hr.isValid = 0;
    return hr;
}




int isValidHttpVersion(char* version){
    return 1; // TODO: Implement
}

// This function supports pipelining but apprently pipelining is disabled in most browsers :(
int ReceiveRequest(int client_socket, char* buffer, size_t bufferSize, char** nextRequestPTR, HttpRequest* hr){
    bufferSize -= 1; // leave space for null terminator
    const char* lastElement = buffer + bufferSize - 1;
    char* iterator = buffer;
    char* readFrom = iterator;
    char* currentRequestPointer = NULL;
    
    
    if(*nextRequestPTR == buffer) goto Receive; // No request in buffer - Skip leftover request handling
    
    
    // Leftover request handling:
    char* temp;
    int FullRequestAvailable = (temp = strnstr(*nextRequestPTR, "\r\n\r\n", lastElement - *nextRequestPTR + 1)) != NULL;
    if(FullRequestAvailable){
        currentRequestPointer = *nextRequestPTR;
        *nextRequestPTR = temp;
        while(**nextRequestPTR == '\r' || **nextRequestPTR == '\n') (*nextRequestPTR)++;
        if(*nextRequestPTR > lastElement) *nextRequestPTR = buffer;
        goto ParseRequest;
    } else {
        // Partial request
        // Move partial request to start of buffer
        size_t partialSize = strnlen(*nextRequestPTR, lastElement - *nextRequestPTR + 1);
        memcpy(buffer, *nextRequestPTR, partialSize);
        memset(buffer + partialSize , 0, bufferSize - partialSize);
        iterator = buffer + partialSize;
    }
    
Receive:
    
    while(1) {
        size_t bytesToReceive = lastElement - iterator + 1;
        if(bytesToReceive == 0) return -1;
        ssize_t bytesReceived = recv(client_socket, iterator, bytesToReceive, 0); // TODO: Add timeout
        if(bytesReceived <= 0) return -1;
        iterator += bytesReceived;
        if((temp = strnstr(readFrom, "\r\n\r\n", bufferSize)) != NULL) break;
        else readFrom = iterator - buffer < 2 ? buffer : iterator - 2;
    }
    currentRequestPointer = buffer;
    *nextRequestPTR = temp;
    while(**nextRequestPTR == '\r' || **nextRequestPTR == '\n') (*nextRequestPTR)++; // TODO: Maybe just add 4 to the pointer (or 3?)
    if(*nextRequestPTR > lastElement) *nextRequestPTR = buffer;
    
    
ParseRequest:
    *hr = ParseHttpHeaders(currentRequestPointer, bufferSize); // TODO: Check size
    if(hr->isValid == 0) return -1;
    // TODO: Handle body
    return 0;
}
void Send500(int client_socket) {
    const char *response_header =
    "HTTP/1.1 500 Internal Server Error\r\n"
    "Content-Type: text/html\r\n"
    "Content-Length: %zu\r\n"
    "Connection: close\r\n"
    "\r\n";
    
    const char *response_body =
    "<html><head><title>500 Internal Server Error</title></head>"
    "<body><h1>500 Internal Server Error</h1>"
    "<p>There was an error processing your request.</p></body></html>";
    
    size_t body_length = strlen(response_body);
    
    char response[1024];
    snprintf(response, sizeof(response), response_header, body_length);
    strncat(response, response_body, sizeof(response) - strlen(response) - 1);
    
    // Send the response to the client
    send(client_socket, response, strlen(response), 0);
}
void Send404(int client_socket) {
    const char *response_header =
    "HTTP/1.1 404 Not Found\r\n"
    "Content-Type: text/html\r\n"
    "Content-Length: %zu\r\n"
    "Connection: close\r\n"
    "\r\n";
    
    const char *response_body =
    "<html><head><title>404 Not Found</title></head>"
    "<body><h1>404 Not Found</h1>"
    "<p>The requested URL was not found on this server.</p></body></html>";
    
    size_t body_length = strlen(response_body);
    
    
    char response[strlen(response_header) + body_length + 1];
    snprintf(response, sizeof(response), response_header, body_length);
    strlcat(response, response_body, sizeof(response));
    
    send(client_socket, response, strlen(response), 0);
}
int SendResponse(int client_socket, HttpRequest* hr){
    
    if(!hr->isValid) {
        char* response = "HTTP/1.1 400 Bad Request\r\n\r\n";
        send(client_socket, response, strlen(response), 0);
        return 1;
    }
    extern char SITE_DIRECTORY[PATH_MAX];
    char path[PATH_MAX];
    
    char *requestedPath = hr->url;
    if(*requestedPath == '/') requestedPath++;
    if(*requestedPath == '\0'){
        requestedPath = alloca(strlen(hr->url) + 4);
        requestedPath[0] = '.';
        requestedPath[1] = '/';
    }
    char* result = realpath(requestedPath, path);
    if(result == NULL || strncmp(SITE_DIRECTORY, path, strlen(SITE_DIRECTORY)) != 0){
        Send404(client_socket);
        return 1;
    }
    
    // check to see if it is file or directory:
    struct stat statbuf;
    if (stat(path, &statbuf) != 0) {
        Send500(client_socket);
        return 1;
    }
    if(S_ISDIR(statbuf.st_mode)){
        char* temp = "/index.html";
        strlcat(path, temp, PATH_MAX);
        if (stat(path, &statbuf) != 0) {
            if(errno == ENOENT) Send404(client_socket);
            else Send500(client_socket);
            return 1;
        }
    }
    int fd;
    if((fd = open(path, O_RDONLY)) == -1){
        Send500(client_socket);
        return 1;
    }
    char headers[128];
    
    // check if file has extension
    const char* mime;
    char* lastDot = strrchr(path,'.');
    char* lastSlash = strrchr(path, '/');
    if(lastDot > lastSlash && lastDot != path + strlen(path) - 1){
        mime = GetMime(lastDot + 1);
    }
    else mime = defaultMime; // no extension
    
    
    
    snprintf(headers, sizeof(headers),
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %lld\r\n\r\n",
             mime,
             statbuf.st_size
             );
    #ifdef __linux__
    int code = send(client_socket, headers, strnlen(headers, sizeof(headers)), 0);
    if(code > 0) {
        code = sendfile(client_socket, fd, NULL, statbuf.st_size);
        printf("bytes sent: %d\n", code);
    }
    #elif __APPLE__
    off_t len = 0;
    struct iovec headerStruct = {.iov_base = headers, .iov_len = strlen(headers)};
    struct sf_hdtr var = {.headers = &headerStruct, .hdr_cnt = 1, .trailers = NULL, .trl_cnt = 0};
    int code = sendfile(fd, client_socket, 0, &len, &var, 0);
    printf("bytes sent: %lld\n", len);
    #endif

    
    // In linux code will be number of bytes sent or -1 for fail, in osx 0 success -1 fail 
    return code;
    
    
}
