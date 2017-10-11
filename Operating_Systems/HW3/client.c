#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#define PORT 8787

const int maxc = 64;

int main(int argc, char* argv[])
{
	struct sockaddr_in dest;
	bzero(&dest, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_addr.s_addr = inet_addr(argv[1]);
	dest.sin_port = htons(PORT);
	int sockfd = socket(PF_INET, SOCK_STREAM, 0);

	connect(sockfd, (struct sockaddr*)&dest, sizeof(dest));

	char sbuf[maxc*3], rbuf[maxc*2];
	pid_t pid = fork();
	if(!pid) while(1) {
		fgets(sbuf, maxc*3, stdin);
		sbuf[strlen(sbuf)-1] = '\0';
		send(sockfd, sbuf, sizeof(sbuf), 0);
	} else while(1) {
		if(recv(sockfd, rbuf, sizeof(rbuf), 0) > 0) printf("%s\n", rbuf);
	}

	close(sockfd);
	return 0;
}
