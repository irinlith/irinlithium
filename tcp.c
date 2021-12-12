#include "tcp.h"

static inline int bind_TCPSocket(int *hSocket, const int *clientPort) {
	struct sockaddr_in remote = {0};
	remote.sin_family = AF_INET;
	remote.sin_addr.s_addr = htonl(INADDR_ANY);
	remote.sin_port = htons(*clientPort);
	return bind(*hSocket, (struct sockaddr *) &remote, sizeof(remote));
}

int connect_TCPClient(int *hSocket, const char *address, const int *serverPort) {
	struct sockaddr_in remote = {0};
	remote.sin_addr.s_addr = inet_addr(address);
	remote.sin_family = AF_INET;
	remote.sin_port = htons(*serverPort);
	return connect(*hSocket, (struct sockaddr *) &remote, sizeof(struct sockaddr));
}

int send_TCPClient(int *hSocket, const char *request, const int request_length) {
	struct timeval tv;
	tv.tv_sec = 20;
	tv.tv_usec = 0;
	if (setsockopt(*hSocket, SOL_SOCKET, SO_SNDTIMEO, (char *) &tv, sizeof(tv)) < 0)
		return -1;
	return send(*hSocket, request, request_length, 0);
}

int receive_TCPClient(int *hSocket, char *receive, short receive_size) {
	struct timeval tv;
	tv.tv_sec = 20;
	tv.tv_usec = 0;
	if (setsockopt(*hSocket, SOL_SOCKET, SO_RCVTIMEO, (char *) &tv, sizeof(tv)) < 0)
		return -1;
	return recv(*hSocket, receive, receive_size, 0);
}

int bind_TCPServer(int *hSocket, const int max_connections, const int serverPort) {
	if ((*hSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		return -1;
	if (bind_TCPSocket(hSocket, &serverPort) < 0)
		return -1;
	listen(*hSocket, max_connections);
	return 0;
}

int wait_TCPServer(int *hSocket, int *sock, struct sockaddr_in *client, socklen_t *client_length) {
	*client_length = sizeof(struct sockaddr_in);
	if ((*sock = accept(*hSocket, (struct sockaddr *) client, (socklen_t *) client_length)) < 0)
		return -1;	
	return 0;
}
