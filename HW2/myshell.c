/* myshell.c
 * Ian Gibson
 * 2/9/2015
 * CS470
 * This is a basic command interpreter. The program reads user input,
 * and creates a process to run the command.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

int parseTheCommand(char command[], char *parsedCommand[])
{
  /* Split command into an array
     return 1 if the command should run in the background
     return 0 by default */
  char *p;
  char *saveptr1, *token;
  int i;
  int background = 0;

  /* Read tokens from the input until the end of
     command is found. */
  for (i = 0, p = command; ; i++, p = NULL) {
    token = strtok_r(p, " ", &saveptr1);
    if (token == NULL) break;
    if (strcmp(token, "&") == 0) {
      background = 1;
      parsedCommand[i] = NULL;
    }
    else parsedCommand[i] = strdup(token);
  }
  /* Fill the rest of parsedCommand with NULL
     to overwrite previous command fully. */
  for (i; i < 20; i++) {
    parsedCommand[i] = NULL;
  }
  return background;
}

void getCommandFromUser(char command[])
{
  /* Read in user input from stdin */
  size_t size = 256;
  scanf("%79[^\n]%*c", command); /* ignores '\n' */
}

int main()
{
  char commandline[256];
  char  *parsedCommand[20];	
  int   background = 0;
  pid_t pid;
  printf("\nmyshell> "); 
  getCommandFromUser(commandline);
  background = parseTheCommand(commandline, parsedCommand);
  while((strcmp(commandline,"exit") != 0)
     && (strcmp(commandline,"quit") != 0))
  { 
    if ( (pid = fork ())== 0)		
    {
      execvp(parsedCommand[0], parsedCommand);	
      perror("Execvp failed");
      exit(EXIT_FAILURE); 
    }
    else					
    {  
      if(!background)				
        waitpid(pid, NULL, 0);			     
      printf("myshell> ");			
      strcpy(commandline, "\0");			
      getCommandFromUser(commandline);
      background = parseTheCommand(commandline, parsedCommand);
    }
  }
}
