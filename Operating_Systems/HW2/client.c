#include"mailbox_interface.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{
	int id;
	char name[SIZE_OF_SHORT_STRING];
	char msg[SIZE_OF_LONG_STRING];

	printf("----Identity Settings----\n");
	printf("Enter your Mailbox-ID: ");
	scanf("%d", &id); getchar();
	printf("Enter your Name: ");
	scanf("%s", name); getchar();
	printf("-------------------------\n");
	printf("\nPlease enter \"JOIN\" to join the chatroom.\n");
	//printf("\nPlease enter \"JOIN number\" to join the number of Chatroom.\n");

	mailbox_t sbox = mailbox_open(0);
	mailbox_t cbox = mailbox_open(id);
	mail_t* smail = (mail_t*)malloc(sizeof(mail_t));
	mail_t* rmail = (mail_t*)malloc(sizeof(mail_t));
	memset(cbox, 0, sizeof(mailbox_t)+10);
	memset(smail, 0, sizeof(mail_t));
	memset(rmail, 0, sizeof(mail_t));
	smail->from = id;
	strcpy(smail->sstr, name);

	pid_t pid = fork();
	if(!pid) {
		while(1) {
			fflush(stdin);
			fgets(msg, SIZE_OF_LONG_STRING, stdin);
			msg[strlen(msg)-1] = '\0'; //remove the '\n' charactor

			smail->type = BROADCAST;
			if(msg[0] == 'J') smail->type = JOIN;
			if(msg[0] == 'L') smail->type = LEAVE;
			if(msg[0] == 'W') {
				smail->type = WHISPER;
				int i = 0;
				for(; msg[i] != ' '; i++);
				sscanf(msg+(++i), "%d", &smail->to);
				for(; msg[i] != ' '; i++);
				strcpy(smail->lstr, msg+(++i));
			}
			if(smail->type == BROADCAST) strcpy(smail->lstr, msg);

			while(mailbox_send(sbox, smail));
		}
	} else {
		while(1) {
			while(mailbox_recv(cbox, rmail));
			if(rmail->type == JOIN)      printf("%s@mailbox%d is joined!\n", rmail->sstr, rmail->from);
			if(rmail->type == LEAVE)     printf("%s@mailbox%d is leaved.\n", rmail->sstr, rmail->from);
			if(rmail->type == WHISPER)   printf("%s@mailbox%d whisper: %s\n", rmail->sstr, rmail->from, rmail->lstr);
			if(rmail->type == BROADCAST) printf("%s@mailbox%d: %s\n", rmail->sstr, rmail->from, rmail->lstr);
		}
	}

	mailbox_unlink(id);
	return 0;
}
