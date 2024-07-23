//
//  http.c
//  CWebServer
//
//  Created by Ardalan Owrangi on 8/3/24.
//

#include "http.h"
#include "DataStructures/HashMap.h"
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define HEADER_BUCKET_SIZE 29


size_t bytesRemaining(int sock){
    int size;
    ioctl(sock, FIONREAD, &size);
    return size;
}

int ValidateRequest(HttpRequest* rq){
    // TODO: Validate Method
    // TODO: Validate URL
    // TODO: Validate Version
    // TODO: Validate Headers
    // TODO: Validate Body
    return 1;
}

HttpRequest InitHttpRequest(char* request, size_t requestLength){
    HttpRequest hr;
    memset(&hr,0,sizeof(hr));
    char* requestCopy = request;

    for (int i = 0; i < 3; i++) {
        char* token = strsep(&requestCopy, " \r\n");
        if(token == NULL){
            hr.isValid = 0;
            return hr;
        }
        switch (i) {
            case 0:
                strcpy(hr.method, token);
                break;
            case 1:
                strcpy(hr.url, token);
                break;
            case 2:
                strcpy(hr.version, token);
                break;
        }
    }
    hr.headers = CreateHashMap(HEADER_BUCKET_SIZE);
    int c = 0;
    for(char* line = strsep(&requestCopy, "\r\n"); line != NULL; line = strsep(&requestCopy, "\r\n")){
        if(*line == '\0') {
            if(++c == 2) {
                break;
            }
            continue;
        }
        char* key = strsep(&line, ":");
        while(*line == ' ') line++; // Skip white space
        char* value = strsep(&line, "\r");
        HashMapAppend(hr.headers, key, strlen(key)+1, value, strlen(value)+1);
        c--;
    }
    hr.isValid = ValidateRequest(&hr);
    return hr;
}

    
void RecieveHeaders(int clientSocket, char* buffer, size_t bufferSize, char** readFrom){
    //TODO: Implement
    
}
