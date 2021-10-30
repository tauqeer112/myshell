
#include "csapp.h"
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
  int argssize = count2D(args);
  memset(looklike,0,sizeof(looklike));
  if(strlen(cmd)){
    add_history(cmd);
  }
  if(!strcmp(cmd,"")){
      continue;
  }
  else if(!strcmp(args[0],"myls")){
      char lsargs[50]={0};
      strcat(lsargs,"./myls");
      strcat(lsargs," ");
      for (int i = 1; i < argssize; i++)
      {   
          strcat(lsargs,args[i]);
          strcat(lsargs," ");
      }
      
      system(lsargs);
  }else if(!strcmp(args[0],"mycp")){
      char cpargs[50]={0};
      strcat(cpargs,"./mycp");
      strcat(cpargs," ");
      for (int i = 1; i < argssize; i++)
      {   
          strcat(cpargs,args[i]);
          strcat(cpargs," ");
      }
      
      system(cpargs);
  }
  else if(!strcmp(args[0],"cd")){builtin_cd(args);}
  else if(!strcmp(args[0],"exit")){exit(0);}
  else if(!strcmp(args[0],"help")){printfile("help.txt");}
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
