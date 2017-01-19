/**********************************/
/*Warning Some error are existing.*/
/**********************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/wait.h>

#define MAX_ARGC 20
#define MAX_ARGV 100
#define MAX_PIPE 15


void exec_cmd_line(char***);
char*** cmd_parser(char*);


int main()
{
	char input[MAX_PIPE*MAX_ARGC*MAX_ARGV];
	int in  = dup(STDIN_FILENO);
	int out = dup(STDOUT_FILENO);

	while(1) {
		if(STDIN_FILENO  != in)  dup2(in,  STDIN_FILENO);
		if(STDOUT_FILENO != out) dup2(out, STDOUT_FILENO);

		printf("0$ ");
		fgets(input, sizeof(input), stdin);
		input[strlen(input)-1] = '\0';

		if(!strcmp(input, "exit")) break;
		exec_cmd_line(cmd_parser(input));
	}

	return EXIT_SUCCESS;
}


void creat_proc(int in_fd, int out_fd, char** argv, int pipecnt, int pipe_fd[][2])
{
	pid_t p = fork();

	if(p < 0) {
		fprintf(stderr, "Error: Unable to create child.\n");
		exit(EXIT_FAILURE);
	} else if(p == 0) {
		if(in_fd  != STDIN_FILENO)  dup2(in_fd, STDIN_FILENO);
		if(out_fd != STDOUT_FILENO) dup2(out_fd, STDOUT_FILENO);

		for(int i = 0; i < pipecnt; i++) {
			close(pipe_fd[i][0]);
			close(pipe_fd[i][1]);
		}

		execvp(argv[0], argv);
	}
}

void exec_cmd_line(char*** argvs)
{
	int cmdcnt = 0;
	while(argvs[cmdcnt]) cmdcnt++;

	int pipecnt = cmdcnt-1;
	int pipe_fd[MAX_PIPE][2];

	for(int i = 0; i < pipecnt; i++) pipe(pipe_fd[i]);

	for(int C = 0; C < cmdcnt; C++) {
		int in_fd = C? pipe_fd[C-1][0] : STDIN_FILENO;
		int out_fd = (C < cmdcnt-1)? pipe_fd[C][1] : STDOUT_FILENO;
		creat_proc(in_fd, out_fd, argvs[C], pipecnt, pipe_fd);
	}

	for(int i = 0; i < pipecnt; i++) {
		close(pipe_fd[i][0]);
		close(pipe_fd[i][1]);
	}

	for(int i = 0; i < cmdcnt; i++) {
		int status;
		wait(&status);
	}
}

void io_redirection(char*, char);

char*** cmd_parser(char* seq)
{
	io_redirection(seq, '>');
	io_redirection(seq, '<');

	static char* cmd[MAX_PIPE+1];

	cmd[0] = strtok(seq, "|");
	for(int i = 1; i <= MAX_PIPE; i++) {
		cmd[i] = strtok(NULL, "|");
		if(cmd[i] == NULL) break;
	}

	static char*  argvs_arr[MAX_PIPE+1][MAX_ARGC];
	static char** argvs[MAX_PIPE+1];

	for(int i = 0; cmd[i]; i++) {
		argvs[i] = argvs_arr[i];

		argvs[i][0] = strtok(cmd[i], " ");
		for(int j = 1; j < MAX_ARGC; j++) {
			argvs[i][j] = strtok(NULL, " ");
			if(argvs[i][j] == NULL) break;
		}
	}

	return argvs;
}

void io_redirection(char* seq, char redir)
{
	int i = 0;
	while(1) {
		if(seq[i] == redir) break;
		if(seq[i] == '\0') return;
		i++;
	}
	seq[i] = ' ';

	int j = 0;
	char fname[20];
	for(i = i+2; seq[i] != ' ' && seq[i] != '\0'; i++) {
		fname[j++] = seq[i];
		seq[i] = ' ';
	}
	fname[j] = '\0';

	int fd;
	if(redir == '>') {
		fd = open(fname, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		dup2(fd, STDOUT_FILENO);
	}
	if(redir == '<') {
		fd = open(fname, O_RDONLY);
		dup2(fd, STDIN_FILENO);
	}
	close(fd);
}
