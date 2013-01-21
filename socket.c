#include "socket.h"


int newTcpSocket(){

	int domain = PF_INET;
	int type = SOCK_STREAM;
	int protocol = IPPROTO_TCP;
	int socketfd = socket(domain, type, protocol);
	if(socketfd == -1){
		perror("cannot create socket");
		exit(EXIT_FAILURE);
	}

	return socketfd;
}

struct sockaddr_in* bindAndListen(int sfd, int port, int qsize){

	struct sockaddr_in* addr;
	addr = (struct sockaddr_in *)malloc(sizeof(struct sockaddr));

	memset(addr, 0, sizeof(*addr));
	addr->sin_family = AF_INET;
	addr->sin_port = htons(port);
	addr->sin_addr.s_addr = INADDR_ANY;

	int b = bind(sfd, (struct sockaddr *)addr, sizeof(*addr));
	if(b == -1){
		perror("Error bind failed");
		close(sfd);
		exit(EXIT_FAILURE);
	}

	int l = listen(sfd, qsize);
	if(l == -1){
		perror("error listen failed");
		close(sfd);
		exit(EXIT_FAILURE);
	}
	printf("Listening on port %d with a queue size of %d\n", port, qsize);
}