#include"mailbox_interface.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define TOTAL 30

int joined[TOTAL];

int main()
{
	memset(joined, 0, TOTAL);
	mail_t* mail = (mail_t*)malloc(sizeof(mail_t));
	mailbox_t sbox = mailbox_open(0);
	mailbox_t cbox;

	mail_t welcome_mail = { .from = 0, .type = WHISPER };
	strcpy(welcome_mail.sstr, "Admin");
	mail_t thanks_mail  = { .from = 0, .type = WHISPER };
	strcpy(thanks_mail.sstr,  "Admin");

	while(1) {
		while(mailbox_recv(sbox, mail));
		cbox = mailbox_open(mail->from);
		switch (mail->type) {
			case JOIN:
				joined[mail->from] = 1;
				sprintf(welcome_mail.lstr, "Hello, %s. Welcome to the Chatroom!!\n------\n\nBROADCAST\n  Direct to enter your message that will broadcast your message to everyone in chatroom.\nWHISPER\n  Enter: WHISPER mailbox-id message\nLEAVE\n  Enter: LEAVE\n\n------", mail->sstr);
				mailbox_send(cbox, &welcome_mail);
				break;
			case LEAVE:
				joined[mail->from] = 0;
				sprintf(thanks_mail.lstr, "Good bye %s! See you next time~\n------", mail->sstr);
				mailbox_send(cbox, &thanks_mail);
				break;
		}
		mailbox_close(cbox);

		if(mail->type == WHISPER) {
			cbox = mailbox_open(mail->to);
			mailbox_send(cbox, mail);
			mailbox_close(cbox);
		}
		if(mail->type == JOIN || mail->type == LEAVE || mail->type == BROADCAST) {
			for(int i = 0; i < TOTAL; i++) if(joined[i] && i != mail->from) {
				cbox = mailbox_open(i);
				mailbox_send(cbox, mail);
				mailbox_close(cbox);
			}
		}
	}

	mailbox_unlink(0);
	return 0;
}
