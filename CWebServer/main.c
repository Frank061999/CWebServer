#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include "DataStructures/HashMap.h"
#include "http.h"
#include <sys/ioctl.h>

#include <string.h>

void* handle_client(void* socket){
    int client_socket_fd = (int)(uintptr_t) socket;
    size_t bufferSize = 256;
    
    char* buffer = (char*) calloc(1, bufferSize);
    int errorCode;
    char* nextRequestPTR = buffer;
    HttpRequest hr;
    do {
        memset(&hr, 0, sizeof(hr));
        errorCode = ReceiveRequestHeaders(client_socket_fd, buffer, bufferSize, &nextRequestPTR, &hr);
        if(errorCode == -1) break;
        
        printf("Method: %s\nPath: %s\nVersion: %s\n",hr.method, hr.url, hr.version);
//        PrintHashMap(hr.headers);
        
        const char *response = "Hello, from MAC!";
        long err = send(client_socket_fd, response, strlen(response), 0);
        if(err == -1) {
            printf("Failed to send\n");
        }
        printf("Sent message!\n");
    } while (errorCode != -1);
    
    
    
    
    
    close(client_socket_fd);
    free(buffer);
    return NULL;
}


void print_with_control_characters(const char *str) {
    while (*str) {
        if (*str == '\r') {
            printf("[CR]");
        } else if (*str == '\n') {
            printf("[LF]\n");
        } else {
            putchar(*str);
        }
        str++;
    }
}


int main(int argc, char** argv) {
    
    int server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket_fd < 0){
        return -1;
    }
    
    int opt = 1;
    setsockopt(server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(3000);
    
    if (bind(server_socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        return 1;
    }
    if(listen(server_socket_fd, 10) == 0) printf("Now Listening\n");
    
    while(1){
        struct sockaddr_in client_addr;
        socklen_t addrlen = sizeof(client_addr);
        int client_socket_fd = accept(server_socket_fd, (struct sockaddr*) &client_addr, &addrlen);
        pthread_t thread;
        pthread_create(&thread, NULL, handle_client, (void*)(uintptr_t) client_socket_fd);
        pthread_detach(thread);
    }
    
    return 0;
    
}

