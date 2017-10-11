#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cctype>
#include<unistd.h>
#include<pthread.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<map>
#include<string>
using std::map;
using std::string;

#define PORT "8787"
#define BACKLOG 20
const int maxc = 64;

pthread_t* pid;
pthread_mutex_t lock;
int domainCount = 0;
map<string, string> record;

void* request(void*);
void SET(char*, char*, char*);
void GET(char*, char*);
void INFO(char*);
bool domain_format(char*);
bool ip_format(char*);

int main(void)
{
	struct sockaddr_storage remoteaddr;
	socklen_t addrlen = sizeof remoteaddr;
	struct addrinfo *ai, hints;
	bzero(&hints, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	getaddrinfo(NULL, PORT, &hints, &ai);

	int listener = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
	bind(listener, ai->ai_addr, ai->ai_addrlen);
	listen(listener, BACKLOG);

	pthread_mutex_init(&lock, NULL);

	while(1) {
		int newfd = accept(listener, (struct sockaddr*)&remoteaddr, &addrlen);
		pid = (pthread_t*)malloc(sizeof(pthread_t));
		pthread_create(pid, NULL, request, (void*)&newfd);
	}

	freeaddrinfo(ai);
	pthread_mutex_destroy(&lock);
	return 0;
}

void* request(void* arg)
{
	char rbuf[maxc*3], sbuf[maxc*2], method[maxc], domain[maxc], ipv4[maxc];
	int nbytes, fd = *((int*)arg);

	while(1) {
		if(recv(fd, rbuf, sizeof(rbuf), 0) <= 0) {
			close(fd); break;
		} else {
			sscanf(rbuf, "%s%s%s", method, domain, ipv4);
			for(int i = 0; domain[i] != '\0'; i++) domain[i] = tolower(domain[i]);

			if(!strcmp(method, "INFO")) INFO(sbuf);
			else if(!strcmp(method, "GET")) GET(domain, sbuf);
			else if(!strcmp(method, "SET")) SET(domain, ipv4, sbuf);
			else sprintf(sbuf, "405 \"Method Not Allowed\"");

			puts(sbuf);
			send(fd, sbuf, sizeof(sbuf), 0);
		}
	}
}

void SET(char* domain, char* ipv4, char* sbuf)
{
	if((!ip_format(ipv4)) || (!domain_format(domain))) {
		sprintf(sbuf, "400 \"Bad Request\"");
		return;
	}

	pthread_mutex_lock(&lock);

	string a(domain), b(ipv4);
	if(!record.count(a)) domainCount++;
	else record.erase(a);
	record[a] = b;
	sprintf(sbuf, "200 \"OK\"");

	pthread_mutex_unlock(&lock);
}
void GET(char* domain, char* sbuf)
{
	if(!domain_format(domain)) {
		sprintf(sbuf, "400 \"Bad Request\"");
		return;
	}

	string a(domain);
	if(record.count(a) > 0) sprintf(sbuf, "200 \"OK\" %s", record[a].c_str());
	else sprintf(sbuf, "404 \"Not Found\"");
}
void INFO(char* sbuf) { sprintf(sbuf, "200 \"OK\" %d", domainCount); }

bool domain_format(char* domain)
{
	int i = 0;
	bool dot = true, once = false;
	while(domain[i]) {
		if(domain[i] == '.') {
			if(dot) return false;
			else dot = true;
			once = true;
		} else if(isalpha(domain[i]) || isdigit(domain[i])) dot = false;
		else return false;
		i++;
	}

	return once;
}
bool ip_format(char* arg)
{
	char ip[maxc];
	strcpy(ip, arg);
	for(int i = 0; ip[i] != '\0'; i++) {
		if(ip[i] == '.') { ip[i] = ' '; continue; }
		if(isdigit(ip[i])) continue;
		return false;
	}

	int d[4];
	sscanf(ip, "%d%d%d%d", d, d+1, d+2, d+3);
	for(int i = 0; i < 4; i++) if(d[i] > 255 || d[i] < 0) return false;

	return true;
}
