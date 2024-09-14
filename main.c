#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include "http.h"
#include <string.h>
#include <limits.h>
#include <sys/stat.h>
#include <errno.h>
#include "DataStructures/CircularQueue.h"
#include <curses.h>

#define CONNECTION_QUEUE_LEN 500
#define THREAD_POOL_SIZE 6
#define BUFFER_SIZE 2048
#define SOCKET_TIMEOUT_SECONDS 2

char SITE_DIRECTORY[PATH_MAX];


void* threadWorker(void* connectionQueue){
	char buffer[BUFFER_SIZE];
	while(1)
	{
		memset(buffer, 0, BUFFER_SIZE);
		int client_socket_fd = QueueDequeue((CircularQueue*) connectionQueue); // Will sleep on this if queue is empty
		int errorCode;
		char* nextRequestPTR = buffer;
		HttpRequest hr;
		int connectionStatus = 1;
		while(connectionStatus) {
			memset(&hr, 0, sizeof(hr));
			errorCode = ReceiveRequest(client_socket_fd, buffer, BUFFER_SIZE, &nextRequestPTR, &hr);
			if(errorCode == -1) {
				connectionStatus = 0;
				goto CleanUp;
			}
			printf("Method: %s\tPath: %s\tVersion: %s\n",hr.method, hr.url, hr.version);
			// PrintHashMap(hr.headers);
			if(SendResponse(client_socket_fd, &hr) == -1 ||
				HashMapContains(hr.headers, "Connection", "close") ||
				strcasecmp(hr.version, "HTTP/1.0") == 0)
					connectionStatus = 0;

		CleanUp:
			if(hr.body != NULL) free(hr.body);
			FreeHashMap(hr.headers);
		}
		close(client_socket_fd);
	}
}


int initDirectory(int argc, char** argv){
	if(argc == 2) {
		if(realpath(argv[1], SITE_DIRECTORY) == NULL){
			printf("Invalid directory.\nUSAGE: %s [DIR]\nExiting Program\n", argv[0]);
			exit(1);
		}
		else goto Finish;
	}
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
Finish:
	chdir(SITE_DIRECTORY);
	printf("Directory initialized: %s\n", SITE_DIRECTORY);
	return 0;
}

int main(int argc, char** argv) {
	initDirectory(argc, argv);


	CircularQueue* connectionQueue = QueueInit(THREAD_POOL_SIZE);

	// Initalize threads
	pthread_t threadPool[CONNECTION_QUEUE_LEN];
	for (int i = 0; i < THREAD_POOL_SIZE; i++) {
		pthread_create(&threadPool[i], NULL, threadWorker, connectionQueue);
		pthread_detach(threadPool[i]);
	}

	int server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(server_socket_fd < 0){
		return -1;
	}

	int opt = 1;
	setsockopt(server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	struct timeval timeout = {.tv_sec = SOCKET_TIMEOUT_SECONDS, .tv_usec = 0};
	if(setsockopt(server_socket_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0 ||
		setsockopt(server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		printf("setsockopt() failed: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(80);

	if (bind(server_socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
		printf("bind() failed: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	if(listen(server_socket_fd, 100) == 0) printf("Now Listening\n");
	else {
		printf("listen() failed: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	while(1){
		struct sockaddr_in client_addr;
		socklen_t addrlen = sizeof(client_addr);
		int client_socket_fd = accept(server_socket_fd, (struct sockaddr*) &client_addr, &addrlen);
		char clientIP[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &client_addr.sin_addr, clientIP, INET_ADDRSTRLEN);
		printf("Accepting connection from %s:\n", clientIP);
		QueueEnqueue(connectionQueue, client_socket_fd);
	}

	return 0;

}

