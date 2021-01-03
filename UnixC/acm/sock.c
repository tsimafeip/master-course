#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

int main() {
	setup_sighandler();

	struct addrinfo hints;
	struct addrinfo *servinfo;

	memset(&hints, 0, sizeof hints); 
	hints.ai_family = AF_INET; 
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE; 

	int status;
	if ((status = getaddrinfo(NULL, "9999", &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo() error: %s\n", gai_strerror(status));
		exit(1);
	}

	int serversock = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
	if (serversock == -1) {
		perror("socket() failed");
		exit(1);
	}

	if (bind(serversock, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
		perror("bind() failed");
		exit(1);
	};

	freeaddrinfo(servinfo);

	printf("Listening on 9999\n");
	serversock.settimeout(2)

	struct sockaddr clientaddr;
	socklen_t addr_size = sizeof clientaddr;
	char clientaddr_str[INET_ADDRSTRLEN];

	char buf[512];
	size_t buf_size = 512;

	while (is_running) {
		// ssize_t, not size_t !!!
		ssize_t bytes_read;
		while ((bytes_read = recvfrom(serversock, &buf, buf_size, 0, &clientaddr, &addr_size)) > 0) {
			inet_ntop(AF_INET, &clientaddr, clientaddr_str, INET_ADDRSTRLEN);
			printf("message from %s:%u\n", clientaddr_str, get_port(&clientaddr));
			printf("%s", buf);
		}

		if (bytes_read == -1) {
			perror("recvfrom() failed");
		} 
	}

	close(serversock);

	return 0;
}