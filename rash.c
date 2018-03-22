#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include<unistd.h>

#define TOK_BUFSIZE 64
#define TOK_DELIM " \t\r\n\a"

char *readline(void) {
	char *buffer=NULL;
	ssize_t x=0;
	getline(&buffer,&x,stdin);
	return buffer; }
	
char **split(char *buffer) {
  	int bufsize = TOK_BUFSIZE, position = 0;
  	char **tokens = malloc(bufsize * sizeof(char*));
  	char *token, **tokens_backup;

  	if (!tokens) {
    	fprintf(stderr, "allocation error\n");
   		exit(EXIT_FAILURE); }

  	token = strtok(buffer, TOK_DELIM);
  	while (token != NULL) {
   		tokens[position] = token;
    	//printf("%d %s\t",position,token);
    	position++;
		if (position >= bufsize) {
	    	bufsize += TOK_BUFSIZE;
      		tokens_backup = tokens;
      		tokens = realloc(tokens, bufsize * sizeof(char*));
      		if (!tokens) {
				free(tokens_backup);
        		fprintf(stderr, "allocation error\n");
        		exit(EXIT_FAILURE); } }

    	token = strtok(NULL, TOK_DELIM); }
  	tokens[position] = NULL;
  	return tokens; }
  	
int routine(char **cmd_parts) {
	int status;
	pid_t pid = fork();
	if(pid == 0) {
		if (execvp(cmd_parts[0], cmd_parts) == -1) {
			perror("error"); }
		exit(EXIT_FAILURE); } 
	do {
		waitpid(pid, &status, WUNTRACED); } 
	while (!WIFEXITED(status) && !WIFSIGNALED(status));
	return 1; }

int main(void) {
	char *cmd,*backup;
	char **cmd_parts;
	char *e="exit\n";
  	pid_t pid;
  	int stat,ext=1,pipe=0;
	do {
		printf(">> ");
		cmd=readline();
		strcpy(backup,cmd);
		cmd_parts=split(cmd);
		if(strcmp(backup,e)!=0) {
	  		stat=routine(cmd_parts);
			free(cmd);
			free(cmd_parts); }
		else ext=0;
	} while(ext); }
