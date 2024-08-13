//
//  http.c
//  CWebServer
//
//  Created by Ardalan Owrangi on 8/3/24.
//

#include "http.h"
#include "DataStructures/HashMap.h"
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define HEADER_BUCKET_SIZE 29


int ValidateHttpHeaders(HttpRequest* hr){
    // TODO: Implement
    return 1;
}


HttpRequest ParseHttpHeaders(char* request, size_t requestLength){
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
    int timesEndofLineEncountered = 0;
    for(char* line = strsep(&requestCopy, "\r\n"); line != NULL; line = strsep(&requestCopy, "\r\n")){
        if(*line == '\0') {
            if(++timesEndofLineEncountered == 2) break;
            continue;
        }
        char* key = strsep(&line, ":");
        while(*line == ' ') line++; // Skip white space
        char* value = strsep(&line, "\r");
        HashMapAppend(hr.headers, key, strlen(key)+1, value, strlen(value)+1);
        timesEndofLineEncountered--;
    }
    
    
    return hr;
}

int isValidHttpMethod(char* method) { // TODO: Replace with HashSet lookup
    return strcmp(method, "GET") == 0 || strcmp(method, "POST") == 0 ||
    strcmp(method, "PUT") == 0 || strcmp(method, "DELETE") == 0 ||
    strcmp(method, "HEAD") == 0 || strcmp(method, "OPTIONS") == 0 ||
    strcmp(method, "PATCH") == 0 || strcmp(method, "TRACE") == 0 ||
    strcmp(method, "CONNECT") == 0;
}

int isValidURL(char* url){
    return 1; // TODO: Implement
}
int isValidHttpVersion(char* version){
    return 1; // TODO: Implement
}

int ReceiveRequestHeaders(int client_socket, char* buffer, size_t bufferSize, char** nextRequestPTR, HttpRequest* hr){
    const char* lastElement = buffer + bufferSize - 1;
    char* iterator = buffer;
    char* readFrom = iterator;
    char* currentRequestPointer = NULL;
    
    
    if(*nextRequestPTR == buffer) goto Receive; // No request in buffer - Skip leftover request handling
    
    
    // Leftover request handling:
    char* temp;
    int FullRequestAvailable = (temp = strnstr(*nextRequestPTR, "\r\n\r\n", lastElement - *nextRequestPTR + 1)) != NULL;
    if(FullRequestAvailable){
        // IS THIS NEEDED??? Adjust pointer to point to next request or the start of buffer
        currentRequestPointer = *nextRequestPTR;
        *nextRequestPTR = temp;
        while(**nextRequestPTR == '\r' || **nextRequestPTR == '\n') (*nextRequestPTR)++;
        if(*nextRequestPTR >= lastElement) *nextRequestPTR = buffer;
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
        ssize_t bytesReceived = recv(client_socket, iterator, bytesToReceive, 0);
        if(bytesReceived <= 0) return -1;
        iterator += bytesReceived;
        if((temp = strnstr(readFrom, "\r\n\r\n", bufferSize)) != NULL) break;
        else readFrom = iterator - buffer < 2 ? buffer : iterator - 2;
    }
    currentRequestPointer = buffer;
    *nextRequestPTR = temp;
    while(**nextRequestPTR == '\r' || **nextRequestPTR == '\n') (*nextRequestPTR)++;
    if(*nextRequestPTR >= lastElement) *nextRequestPTR = buffer;
    
    
ParseRequest:
    *hr = ParseHttpHeaders(currentRequestPointer, bufferSize); // TODO: Check size
    
    // TODO: Handle body
    return 0;
}
