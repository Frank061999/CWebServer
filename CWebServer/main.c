#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include "DataStructures/HashMap.h"
#include "http.h"
#include <string.h>
#include "DataStructures/StaticHashMaps.h"
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h> // for mkdir
#include <sys/types.h> // for modes
#include <errno.h>

char SITE_DIRECTORY[PATH_MAX];

void* handle_client(void* socket){
    pthread_t thread_id = pthread_self();
    printf("Handling Client %lu\n", (unsigned long)thread_id);
    int client_socket_fd = (int)(uintptr_t) socket;
    size_t bufferSize = 2048;
    
    char* buffer = (char*) calloc(1, bufferSize);
    int errorCode;
    char* nextRequestPTR = buffer;
    HttpRequest hr;
    do {
        memset(&hr, 0, sizeof(hr));
        errorCode = ReceiveRequest(client_socket_fd, buffer, bufferSize, &nextRequestPTR, &hr);
        if(errorCode == -1) {
            FreeHashMap(hr.headers);
            break;
        }
        
        printf("Method: %s\tPath: %s\tVersion: %s\n",hr.method, hr.url, hr.version);
//        PrintHashMap(hr.headers);
        
        SendResponse(client_socket_fd, &hr);
//        pthread_t thread_id = pthread_self();
//        printf("Sent message! Thread id: %lu\n", (unsigned long)thread_id);
        FreeHashMap(hr.headers);
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

int initDirectory(void){
    if(mkdir("site", 0000700) == -1){ // rwx for owner
        if (errno != EEXIST) {
            perror("Error creating directory");
            exit(1);
        }
    }
    if (getcwd(SITE_DIRECTORY, PATH_MAX) == NULL) {
        perror("Error getting current working directory");
        exit(1);
    }
    {
        char* temp = "/site";
        if(PATH_MAX * (2/3) - strlen(temp) - strlen(SITE_DIRECTORY) < 0) {
            printf("Error: directory PATH_MAX overflow\n Terminating program.\n");
            exit(1);
        }
        strcat(SITE_DIRECTORY, temp);
    }
    chdir(SITE_DIRECTORY);
    printf("Directory initialized: %s\n", SITE_DIRECTORY);
    return 0;
}

int main(int argc, char** argv) {

    initDirectory();
    
    
    int server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket_fd < 0){
        return -1;
    }
    
    int opt = 1;
    setsockopt(server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(80);
    
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

