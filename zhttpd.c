#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "dictionary.h"
#include "socket.h"
#include "zstring.h"

#define SOCKET_BUFFER_SIZE 8124


typedef struct zHttpRequest {
	Dictionary * header;
	char * body;
	int outfd;
} zHttpRequest;

void freeReq(zHttpRequest * req){
	freeDictionary(req->header);
	free(req->body);
	free(req);
}


zHttpRequest * parseRequest(int connectfd){
	int br;
	char socket_buffer[SOCKET_BUFFER_SIZE];

	zHttpRequest *newRequest = (zHttpRequest *)malloc(sizeof(struct zHttpRequest));

	//grab contents of stream, store fd
	newRequest->outfd = connectfd;

  br = read(connectfd, socket_buffer, SOCKET_BUFFER_SIZE);

	socket_buffer[br] = '\0';
	printf("%s\n[%d bytes read]\n", (char*)socket_buffer, br);

	//build header dict, fill with header fields
	newRequest->header = newDictionary();

	int index = 0; //necessary for newNextLine to keep parsing socket_buffer
	//get first line of HTTP Request, store method & location
	char * line = newNextLine(socket_buffer, &index);	
	char * method_type = newIndexToChar(line, 0, ' ');
	addValueForKey(newRequest->header, newString("method"), method_type);
	int start = strlen(method_type)+1;
	char * location_req = newIndexToChar(line, start, ' ');
	addValueForKey(newRequest->header, newString("location"), location_req);
	free(line);

	//parse out rest of header fields, add to dict
	for(;;){
		line = newNextLine(socket_buffer, &index);
		int len = strlen(line);
		if(len <= 1)
			break;
		char * key_string = mutateToLower(newIndexToChar(line, 0, ':'));
		start = strlen(key_string)+2;
		char * val_string = newIndexToChar(line, start, '\r');
		//printf("key: '%s', val: [%s]\n", key_string, val_string);
		addValueForKey(newRequest->header, key_string, val_string);
		free(line);
	}

	//parse and store body
	newRequest->body = newIndexToChar(socket_buffer, index, '\0');
	//printDict(newRequest->header);
	//printf("Body::\n%s\n::end Body\n", newRequest->body);
	return newRequest;
}


void writeResponse(int channelfd, char * message){
	char *res;
	asprintf(&res, "%s", message);
	write(channelfd, res, strlen(res));
	free(res);
}

void writeFileToResponse(int channelfd, char * fname){

	char buffer[128];
  memset(buffer, 0, 128);
	getcwd(buffer, 128);
	strcat(buffer, fname);

	printf("opening: %s\n", buffer);
	FILE *data = fopen(buffer, "r");

	if(data == NULL){
		perror("Error, 404");
		writeResponse(channelfd, "HTTP/1.1 404 NOT FOUND\n\n");
		getcwd(buffer, 128);
		strcat(buffer, "/fserv/404.html");
		printf("404: %s\n", buffer);
		data = fopen(buffer, "r");
    if(data == NULL){
      perror("Error, 404, you're really fucked now");
      exit(-1);
    }
	} else {
		writeResponse(channelfd, "HTTP/1.1 200 OK\n\n");
	}
	int i = 0;
	char c = fgetc(data);
	while( c != EOF){
		buffer[i] = c;
		i++;
		if( i == 127){
			buffer[i] = '\0';
			i = 0;
			writeResponse(channelfd, buffer);
		}
		c = fgetc(data);
	}

	buffer[i] = '\0';
	writeResponse(channelfd, buffer);
  fclose(data);
}

void sendResponse(zHttpRequest * req){
	//default behavior fetches a file in ./fserv/
	char buffer[128];
  memset(buffer, 0, 128);
	strcpy(buffer, "/fserv");
  char * loc = getValueForKey(req->header, "location");
  strcat(buffer, loc);
  if( strcmp(loc, "/") == 0 ){
    strcat(buffer, "index.html");
  }

	writeFileToResponse(req->outfd, buffer);
}

void handleIncomingRequests(int socketfd){

	for(;;){
		printf("Waiting for connection...\n");
		int connectfd = accept(socketfd, NULL, NULL);
		if(0 > connectfd){
			perror("Error accept failed");
			close(socketfd);
			exit(EXIT_FAILURE);
		}

		zHttpRequest *zreq = parseRequest(connectfd);
		sendResponse(zreq);

		freeReq(zreq);

		if( -1 == shutdown(connectfd, SHUT_RDWR) ) {
			perror("cannot shutdown socket\n");
			close(connectfd);
			exit(EXIT_FAILURE);
		}

		close(connectfd);
		printf("Terminated Connection Following Response\n");

	}
}


int main(int argc, char* argv[]){
  // 1. create socket
  // 2. bind/listen
  // 3. accept new connections

	int socketfd = newTcpSocket();
	bindAndListen(socketfd, 3000, 1000);
	handleIncomingRequests(socketfd);

	return 0;
}
