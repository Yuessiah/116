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

	char buf[maxc];
	pid_t pid = fork();
	if(!pid) while(1) {
		fgets(buf, 64, stdin);
		buf[strlen(buf)-1] = '\0';
		send(sockfd, buf, sizeof(buf), 0);
	} else while(1) {
		if(recv(sockfd, buf, sizeof(buf), 0) <= 0) continue;
		printf("%s\n", buf);
	}

	close(sockfd);
	return 0;
}
