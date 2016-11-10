#ifndef _MAILBOX_
#define _MAILBOX_

#define JOIN 9
#define BROADCAST 8
#define LEAVE 7
#define WHISPER 6

#define SIZE_OF_SHORT_STRING 64
#define SIZE_OF_LONG_STRING 512

typedef void* mailbox_t;
typedef struct __MAIL {
	int from;
	int to; // want to sent whisper mail
	int type;
	char sstr[SIZE_OF_SHORT_STRING];
	char lstr[SIZE_OF_LONG_STRING];
} mail_t;

mailbox_t mailbox_open(int id);
int mailbox_unlink(int id);
int mailbox_close(mailbox_t box);
int mailbox_send(mailbox_t box, mail_t *mail);
int mailbox_recv(mailbox_t box, mail_t *mail);
int mailbox_check_empty(mailbox_t box);
int mailbox_check_full(mailbox_t box);

#endif
