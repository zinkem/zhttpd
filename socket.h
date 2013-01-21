#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
 * Encapsulates error checking when creating a Berkly Socket
*/

typedef struct zSocket {
	int channelfd;
} zSocket;

int newTcpSocket();
struct sockaddr_in * bindAndListen(int sfd, int port, int qsize);
