#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/stat.h>

int fsize(char *name) {
	struct stat st;
	stat(name, &st);
	return st.st_size;
}

char* file_get_contents(char* filename) {
	FILE *file=fopen(filename, "r");
	if(!file) {
		printf("Can't open file %s\n", filename);
		return NULL;
	}
	
	int size=fsize(filename);
	
	char* result=(char*)malloc(size+1);
	fread(result, size, 1, file);
	fclose(file);
	result[size]='\0';
	
	return result;
}

void reparse_code(char* code) {
	// replace newlines to spaces
	size_t size=strlen(code);
	int i;

	for (i=0; i<size; i++) if (code[i] == '\n') code[i]=' ';
}

int main(int argc, char** argv) {
	if (argc < 2) {
		printf("Usage: gimp-exec filename [IP adress] [port]\n");
		return 1;
	}

	char* filename=argv[1];

	int sockfd, port, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	char* host=(argc > 2) ? argv[2] : "127.0.0.1";
	if (argc > 3) {
		sscanf(argv[3], "%d", &port);
	} else {
		port=10008;
	}

	char* request=file_get_contents(filename);

	if (request == NULL) {
		return 1;
	}

	reparse_code(request);
	size_t length=strlen(request);

	char magic='G';
	char first=length/256;
	char second=length % 256;
	char buffer[5];

	sockfd=socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		printf("Error opening socket\n");
		return 1;
	}

	bzero(&serv_addr, sizeof(serv_addr));
	bzero(&buffer[0], 5);

	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr=inet_addr(host);

	int connected=connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	if (connected < 0) {
		printf("Error connecting\n");
		return 1;
	}

	write(sockfd, &magic, 1);
	write(sockfd, &first, 1);
	write(sockfd, &second, 1);
	write(sockfd, request, length);
	recv(sockfd, &buffer, 4, 0);

	int answer_length=buffer[2]*256+buffer[3];
	char* answer=(char*)malloc(answer_length+1);

	recv(sockfd, answer, answer_length, 0);
	close(sockfd);

	answer[answer_length]='\0';
	printf("%s\n", answer);
	free(answer);

	if (buffer[0] == magic && buffer[1]==0) {
		printf("Success\n");
		return 0;
	} else return 1;
}
