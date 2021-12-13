#ifndef IRINLITHIUM_TCP_H
#define IRINLITHIUM_TCP_H
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

typedef struct tcp_server {
	int max_connections, socket, client_socket;
	unsigned short port;
	socklen_t client_length;
	struct sockaddr_in client;
} tcp_server;
int connect_TCPClient(int *hSocket, const char *address, const unsigned short *serverPort);
int send_TCPClient(int *hSocket, const char *request, const int request_length);
int receive_TCPClient(int *hSocket, char *receive, short receive_size);
int bind_TCPServer(int *hSocket, const int max_connections, const unsigned short serverPort);
int wait_TCPServer(int *hSocket, int *sock, struct sockaddr_in *client, socklen_t *client_length);

#endif
