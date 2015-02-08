#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

int parseTheCommand(char command[], char *parsedCommand[])
{
  char *p;
  char *saveptr1, *token;
  int i;
  int background = 0;

  for (i = 0, p = command; ; i++, p = NULL) {
    token = strtok_r(p, " ", &saveptr1);
    if (token == NULL) break;
    if (strcmp(token, "&") == 0) {
      background = 1;
      parsedCommand[i] = NULL;
    }
    else parsedCommand[i] = strdup(token);
  } 
  for (i; i < 20; i++) {
    parsedCommand[i] = NULL;
  }
  return background;
}

void getCommandFromUser(char command[])
{
  size_t size = 256;
  scanf("%79[^\n]%*c", command);
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
//      int i;for(i=0;i<20;i++){printf("%d: %s\n",i,parsedCommand[i]);}
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
