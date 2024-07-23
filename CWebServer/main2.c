#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include "DataStructures/HashMap.h"
#include "DataStructures/KeyValue.h"



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
    server_addr.sin_port = htons(1111);
    
    if (bind(server_socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        return 1;
    }


    if(listen(server_socket_fd, 5) == 0){
        printf("Now Listening\n");
    }
    
    struct sockaddr_in client_addr;
    socklen_t addrlen = sizeof(client_addr);
    
    int client_socket_fd = accept(server_socket_fd, (struct sockaddr*) &client_addr, &addrlen);
    
    char buffer[2049];
    memset(buffer, 0, sizeof(buffer));
    
    recv(client_socket_fd, buffer, 2045, 0);
    buffer[2048]='\0';
    printf("%s\n",buffer);
    const char *response = "Hello, from MAC!";
    
    long err = send(client_socket_fd, response, strlen(response), 0);
    if(err == -1) {
        printf("failed to send\n");
        goto close;
    }
    printf("Sent message!\n");
close:
    close(client_socket_fd);
    close(server_socket_fd);
    return 0;
}

