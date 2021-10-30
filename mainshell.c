
#include "csapp.h"
#include "limits.h"
#define LINE_SIZE 255
#define ARG_SIZE 20
#define HISTORY_SIZE 50
#include "limits.h"
#define ARG_MAX 2097152
#include <readline/readline.h>
#include <readline/history.h>
#include "myheaders.h"





int main(int argc, char const *argv[]){
    char look[500];
    char **args;
    
    while(1){

  strcpy(look,LookLikeShell());

  cmd = readline(look);
  args=splitLine(cmd);
  memset(looklike,0,sizeof(looklike));
  if(strlen(cmd)){
    add_history(cmd);
  }
  if(!strcmp(cmd,"")){
      continue;
  }
  else if(!strcmp(args[0],"cd")){builtin_cd(args);}
  else if(!strcmp(args[0],"exit")){exit(0);}
  else if(!strcmp(args[0],"help")){printhelpfile();}
  else if(!strcmp(args[0],"man")){
      if(fork()==0){
       execvp(args[0],args);
      }
      else{
          wait(NULL);
      }
      }
  else if(!strcmp(cmd,"q")){
      printf("\n");
      break;}
   else{
       if(system(cmd)>0){
           printf("Invalid command , Use man or help for usage info \"exit\" to exit\n");
       }
   }
}
    return 0;
}
