#include"mailbox_interface.h"
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include<sys/types.h>
const int SIZE = sizeof(mail_t) + 10;

void mail_move_in(mailbox_t, mail_t*);
void mail_move_out(mailbox_t, mail_t*);

mailbox_t mailbox_open(int id)
{
	char name[20];
	sprintf(name, "__mailbox_%d", id);
	int shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	ftruncate(shm_fd, SIZE);
	return mmap((void*)(id*SIZE), SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
}

int mailbox_unlink(int id)
{
	char name[20];
	sprintf(name, "__mailbox_%d", id);
	return shm_unlink(name);
}

int mailbox_close(mailbox_t box) { return munmap(box, SIZE); }

int mailbox_send(mailbox_t box, mail_t *mail)
{
	if(mailbox_check_full(box)) return -1;
	*((char*)box) = 1; //set box be full
	mail_move_in(box, mail);
	return 0;
}

int mailbox_recv(mailbox_t box, mail_t *mail)
{
	if(mailbox_check_empty(box)) return -1;
	*((char*)box) = 0; //set box be empty
	mail_move_out(box, mail);
	return 0;
}

int mailbox_check_empty(mailbox_t box) { return !(*((char*)box)); }
int mailbox_check_full(mailbox_t box) { return *((char*)box); }

void mail_move_in(mailbox_t box, mail_t *mail)
{
	char* ptr = (char*)(box+1);
	ptr += sprintf(ptr, "%d%c", mail->from, ' ');
	ptr += sprintf(ptr, "%d%c", mail->to,   ' ');
	ptr += sprintf(ptr, "%d%c", mail->type, ' ');
	ptr += sprintf(ptr, "%s%c", mail->sstr, ' ');
	ptr += sprintf(ptr, "%c%s%c", 2, mail->lstr, '\0');
}

void mail_move_out(mailbox_t box, mail_t *mail)
{
	char* ptr = (char*)(box+1);
	sscanf(ptr, "%d%d%d%s",
			&mail->from,
			&mail->to,
			&mail->type,
			mail->sstr
			);
	int i; for(i = 0; ptr[i] != 2; i++);
	ptr+=i+1; sprintf(mail->lstr, "%s", ptr);
}
