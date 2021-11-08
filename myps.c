#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<dirent.h>
#include<sys/times.h>
#include<unistd.h>
#include<time.h>
#include<linux/limits.h>
#include <ctype.h>
#include <stdlib.h>
#include <pwd.h>
#include <math.h>

FILE *input;
char usage[1024]="simulates the following\n.myps->ps\nmyps -a ->ps -a\nmyps -F->ps -aF\nmyps -l->ps aux\n\n";

void getmemanduptiem(int rss , int utime , int stime, int cutime,int starttime,int cstime){
char path[1024];
double Hertz = sysconf(_SC_CLK_TCK);
FILE *fp;
char uptime_str[100];
size_t len=0;
char *line;
strcpy(path,"/proc/");
strcat(path,"uptime");
  fp=fopen(path,"r");
    if(fp!=NULL)
    {
        getline(&line,&len,fp);
        sscanf(line,"%s ",uptime_str);
    }
   fclose(fp);
    long uptime=atof(uptime_str);
    // long Hertz=sysconf(_SC_CLK_TCK);
    strcpy(path,"/proc/");
    strcat(path,"meminfo");

    fp=fopen(path,"r");
    unsigned long long total_memory;
    if(fp!=NULL)
    {
        getline(&line,&len,fp);
        sscanf(line,"MemTotal:        %llu kB",&total_memory);
    }
    fclose(fp);
     float memory_usage=100*rss/total_memory;
      unsigned long total_time=utime+stime;
            total_time=total_time+(unsigned long)cutime+(unsigned long)cstime;
            float seconds=uptime-(starttime/Hertz);
            float cpu_usage=100*((total_time/Hertz)/seconds);
            if(isnan(cpu_usage))//if entry is missing in proc
            {
                cpu_usage=0.0;
            }
            if(isnan(memory_usage))//if entry is missing in proc
            {
                memory_usage=0.0;
            }
    printf("%.2f\t",memory_usage);
    printf("%.2d\t",abs(cpu_usage));

}

void printcmd(int pid){
    FILE *fp;
    char filename[1024];
    char cmd[1024];
    int i;
    char *line;
    size_t len =0;
    sprintf(filename,"/proc/%d/cmdline",pid);
    fp=fopen(filename,"r");
    if(fp!=NULL)
    {
        getline(&line,&len,fp);
        sscanf(line,"%s ",cmd);
        fflush(stdout);
    }
   fclose(fp);
   printf("%s",cmd);
  
    
}

void readstr(char *x) { 
  int pid;
  fscanf(input, "%d ", &pid);
  fscanf(input, "%s ", x);
}

void readchar(char *c){
  fscanf(input, "%c", c);
}

void readnum(int *d){
  fscanf(input, "%d", d);
}

void printtime(int x) {  
  printf("%.2f", (((double)x) / (sysconf(_SC_CLK_TCK))));
}

void readllu(long long unsigned int *l){
    fscanf(input,"%llu",l);
}

void printtitle(){
    printf("%s  ","PID");
    printf("%s\t","TTY");
    printf("%s ","TIME");
    printf("%s","CMD");
    printf("%s","\n");

}
void printtitle_af(){
    printf("%s\t","USERID");
    printf("%s\t","PID");
    printf("%s\t","PPID");
    printf("%s\t","SZ");
    printf("%s\t","RSS");
    printf("%s\t","TTY");
    printf("%s\t","UPTIME");
    printf("%s\t","STARTIME");
    printf("%s\t","CMD");
    printf("%s","\n");
}

int basicps(int argc){

     FILE *fp;
     int stime;
     char mycmd[1000];
     struct stat buf;
     struct tm dt;
     char file[200];
     int pts =43;
  char c, fname[1000], pname[PATH_MAX], state, fullpath[PATH_MAX], st[10];                              
  int pid, ppid, pgrp, session, tty_nr, tpgid, flags, minflt, cminflt,
    majflt, cmajflt, utime, cutime,cstime ,priority,niceid,num_threads,itrealvalue,vsize,rss;
  long long unsigned int  starttime;
   printtitle();
  DIR *d;
  struct dirent *dir;
  
  unsigned int mask = 65280;
  
  input = NULL;
    
     if(argc < 1){
    printf("Invalid argument list\n");
    return 0;
  }
  else {
    //Go to /proc directory and search for directories having names as numbers; thus represent the processes runnning
    d = opendir("/proc");
    chdir("/proc");
    if(d){
      while ((dir = readdir(d)) != NULL){
	sprintf(fname, "%s", dir->d_name);
	if(isdigit(fname[0])){
	  sscanf(fname, "%d", &pid);
	  if(chdir(fname) == 0){
        //   strcpy(file,"/proc/");
          sprintf(file,"/proc/%d",pid);
        //   printf("%s",file);
          if(stat(file, &buf) == 0){
          dt = *(gmtime(&buf.st_ctime));
          memset(file,0,sizeof(file));
   }
   else
   {
    // perror(file);    //if stat fails, print a diagnostic.
   }

        //    printf("%s\n","----------------------------------------------------------------------------");
	    input = fopen("stat", "r");
	  }
	  if(!input){
	    perror("open");
	    return 1;
	  }
    //   strcpy(mycmd,cmdline(pid));
    //   printf("%s\n",mycmd);
	  readstr(pname);
	  readchar(&state);
	  readnum(&ppid);
	  readnum(&pgrp);
	  readnum(&session);
	  readnum(&tty_nr);
	  readnum(&tpgid);
	  readnum(&flags);
	  readnum(&minflt);
	  readnum(&cminflt);
	  readnum(&majflt);
	  readnum(&cmajflt);
	  readnum(&utime);
      readnum(&stime);
      readnum(&cutime);
      readnum(&cstime);
      readnum(&priority);
      readnum(&niceid);
      readnum(&num_threads);
      readnum(&itrealvalue);
      readllu(&starttime);
      readnum(&vsize);
      readnum(&rss);
    
	  if((tty_nr & mask) >> 8 == 136){   
        pts = (((tty_nr >> 20) << 8) | (tty_nr & 255));
        if(pts == 1 || pid == getpid()){
        printf("%d ",pid);
	    printf("pts/%d\t",pts);
        printtime(utime);
        printf("%s"," ");
        // void getmemanduptiem(int rss , int utime , int stime, int cutime,int starttime)
        printf("%s",pname);
       
        // printf("%d:%d:%d",dt.tm_hour, dt.tm_min, dt.tm_sec);
 
        printf("%s\n","");
        }
        
	  }
	  else if((tty_nr & mask) >> 8 == 4){
        // printf("%d = ",pid);
        // printf("%s ", pname);
	    // printf("tty%d \t ",((tty_nr >> 20) << 8) | (tty_nr & 255));
        // printtime(utime);
        // printf("  %d",vsize);
        // printf("%s\n","");
	  }
	  else{ 
	    // printf("? ");
	  }
	
	  chdir("..");
	
      }
	
      }
      closedir(d);
    }
  }
}

int ps_a(int argc){

     FILE *fp;
     char mycmd[1000];
     struct stat buf;
     struct tm dt;
     char file[200];
     int pts =43;
  char c, fname[1000], pname[PATH_MAX], state, fullpath[PATH_MAX], st[10];                              
  int pid, ppid, pgrp, session, tty_nr, tpgid, flags, minflt, cminflt,
    majflt, cmajflt, utime, cutime,cstime ,priority,niceid,num_threads,itrealvalue,vsize,rss;
  long long unsigned int  starttime;
   printtitle();
  DIR *d;
  struct dirent *dir;
  
  unsigned int mask = 65280;
  
  input = NULL;
    
     if(argc < 1){
    printf("Invalid argument list\n");
    return 0;
  }
  else {
    //Go to /proc directory and search for directories having names as numbers; thus represent the processes runnning
    d = opendir("/proc");
    chdir("/proc");
    if(d){
      while ((dir = readdir(d)) != NULL){
	sprintf(fname, "%s", dir->d_name);
	if(isdigit(fname[0])){
	  sscanf(fname, "%d", &pid);
	  if(chdir(fname) == 0){
        //   strcpy(file,"/proc/");
          sprintf(file,"/proc/%d",pid);
        //   printf("%s",file);
          if(stat(file, &buf) == 0){
          dt = *(gmtime(&buf.st_ctime));
          memset(file,0,sizeof(file));
   }
   else
   {
    // perror(file);    //if stat fails, print a diagnostic.
   }

        //    printf("%s\n","----------------------------------------------------------------------------");
	    input = fopen("stat", "r");
	  }
	  if(!input){
	    perror("open");
	    return 1;
	  }
    //   strcpy(mycmd,cmdline(pid));
    //   printf("%s\n",mycmd);
	  readstr(pname);
	  readchar(&state);
	  readnum(&ppid);
	  readnum(&pgrp);
	  readnum(&session);
	  readnum(&tty_nr);
	  readnum(&tpgid);
	  readnum(&flags);
	  readnum(&minflt);
	  readnum(&cminflt);
	  readnum(&majflt);
	  readnum(&cmajflt);
	  readnum(&utime);
      readnum(&cutime);
      readnum(&cstime);
      readnum(&priority);
      readnum(&niceid);
      readnum(&num_threads);
      readnum(&itrealvalue);
      readllu(&starttime);
      readnum(&vsize);
      readnum(&rss);
    
	  if((tty_nr & mask) >> 8 == 136){   
        pts = (((tty_nr >> 20) << 8) | (tty_nr & 255));
        if(pts>=0){
        printf("%d ",pid);
	    printf("pts/%d\t",((tty_nr >> 20) << 8) | (tty_nr & 255));
        printtime(utime);
        printf("%s"," ");
        printf("%s",pname);
        // printcmd(pid);
        // printf("%d:%d:%d",dt.tm_hour, dt.tm_min, dt.tm_sec);
 
        printf("%s\n","");
        }
        
	  }
	  else if((tty_nr & mask) >> 8 == 4){
        // printf("%d = ",pid);
        // printf("%s ", pname);
	    // printf("tty%d \t ",((tty_nr >> 20) << 8) | (tty_nr & 255));
        // printtime(utime);
        // printf("  %d",vsize);
        // printf("%s\n","");
	  }
	  else{ 
	    // printf("? ");
	  }
	
	  chdir("..");
	
      }
	
      }
      closedir(d);
    }
  }
}

int ps_af(int argc){
    //  getmemanduptiem(rss,utime,stime,cutime,starttime,cstime);
     FILE *fp;
     struct passwd *pws;
     uid_t userid;
     char mycmd[1000];
     struct stat buf;
     struct tm dt;
     char file[200];
     int pts =43;
  char c, fname[1000], pname[PATH_MAX], state, fullpath[PATH_MAX], st[10];                              
  int pid, ppid, pgrp, session, tty_nr, tpgid, flags, minflt, cminflt,
    majflt, cmajflt, utime, cutime,cstime ,priority,niceid,num_threads,itrealvalue,vsize,rss,stime,starttime;
   printtitle_af();
  DIR *d;
  struct dirent *dir;
  
  unsigned int mask = 65280;
  
  input = NULL;
    
     if(argc < 1){
    printf("Invalid argument list\n");
    printf("%s",usage);
    return 0;
  }
  else {
    //Go to /proc directory and search for directories having names as numbers; thus represent the processes runnning
    d = opendir("/proc");
    chdir("/proc");
    if(d){
      while ((dir = readdir(d)) != NULL){
	sprintf(fname, "%s", dir->d_name);
	if(isdigit(fname[0])){
	  sscanf(fname, "%d", &pid);
	  if(chdir(fname) == 0){
        //   strcpy(file,"/proc/");
          sprintf(file,"/proc/%d",pid);
        //   printf("%s",file);
          if(stat(file, &buf) == 0){
          dt = *(gmtime(&buf.st_ctime));
          userid = buf.st_uid;
           pws = getpwuid(userid);
        //    printf("%s",pws->pw_name);
          memset(file,0,sizeof(file));
   }
   else
   {
    // perror(file);    //if stat fails, print a diagnostic.
   }

        //    printf("%s\n","----------------------------------------------------------------------------");
	    input = fopen("stat", "r");
	  }
	  if(!input){
	    perror("open");
	    return 1;
	  }
    //   strcpy(mycmd,cmdline(pid));
    //   printf("%s\n",mycmd);
	  readstr(pname);
	  readchar(&state);
	  readnum(&ppid);
	  readnum(&pgrp);
	  readnum(&session);
	  readnum(&tty_nr);
	  readnum(&tpgid);
	  readnum(&flags);
	  readnum(&minflt);
	  readnum(&cminflt);
	  readnum(&majflt);
	  readnum(&cmajflt);
	  readnum(&utime);
      readnum(&stime);
      readnum(&cutime);
      readnum(&cstime);
      readnum(&priority);
      readnum(&niceid);
      readnum(&num_threads);
      readnum(&itrealvalue);
      readnum(&starttime);
      readnum(&vsize);
      readnum(&rss);
    
	  if((tty_nr & mask) >> 8 == 136){   
        pts = (((tty_nr >> 20) << 8) | (tty_nr & 255));
        if(pts==0 || pid ==getpid()){

        printf("%s\t",pws->pw_name);
        printf("%d\t",pid);
        printf("%d\t",ppid);
        printf("%d MB\t",vsize/1000000);
        printf("%d\t",rss);
        printf("pts/%d\t",pts);
        printtime(utime);
        printf("\t");
        printf("%d:%d:%d \t",dt.tm_hour, dt.tm_min, dt.tm_sec);
        // printcmd(pid);
         printf("%s",pname);
 
        printf("%s\n","");
        }
        
	  }
	  else if((tty_nr & mask) >> 8 == 4){
        // printf("%d = ",pid);
        // printf("%s ", pname);
	    // printf("tty%d \t ",((tty_nr >> 20) << 8) | (tty_nr & 255));
        // printtime(utime);
        // printf("  %d",vsize);
        // printf("%s\n","");
	  }
	  else{ 
	    // printf("? ");
	  }
	
	  chdir("..");
	
      }
	
      }
      closedir(d);
    }
  }
}

int ps_l(int argc){
    //  getmemanduptiem(rss,utime,stime,cutime,starttime,cstime);
     FILE *fp;
     struct passwd *pws;
     uid_t userid;
     char mycmd[1000];
     struct stat buf;
     struct tm dt;
     char file[200];
     int pts =43;
  char c, fname[1000], pname[PATH_MAX], state, fullpath[PATH_MAX], st[10];                              
  int pid, ppid, pgrp, session, tty_nr, tpgid, flags, minflt, cminflt,
    majflt, cmajflt, utime, cutime,cstime ,priority,niceid,num_threads,itrealvalue,vsize,rss,stime,starttime;
   printtitle_af();
  DIR *d;
  struct dirent *dir;
  
  unsigned int mask = 65280;
  
  input = NULL;
    
     if(argc < 1){
    printf("Invalid argument list\n");
    printf("%s",usage);
    return 0;
  }
  else {
    //Go to /proc directory and search for directories having names as numbers; thus represent the processes runnning
    d = opendir("/proc");
    chdir("/proc");
    if(d){
      while ((dir = readdir(d)) != NULL){
	sprintf(fname, "%s", dir->d_name);
	if(isdigit(fname[0])){
	  sscanf(fname, "%d", &pid);
	  if(chdir(fname) == 0){
          strcpy(file,"");
          sprintf(file,"/proc/%d",pid);
        //   printf("%s",file);
          if(stat(file, &buf) == 0){
          dt = *(gmtime(&buf.st_ctime));
          userid = buf.st_uid;
           pws = getpwuid(userid);
        //    printf("%s",pws->pw_name);
        //   memset(file,0,sizeof(file));
   }
   else
   {
    // perror(file);    //if stat fails, print a diagnostic.
   }

        //    printf("%s\n","----------------------------------------------------------------------------");
	    input = fopen("stat", "r");
	  }
	  if(!input){
	    perror("open");
	    return 1;
	  }
    //   strcpy(mycmd,cmdline(pid));
    //   printf("%s\n",mycmd);
	  readstr(pname);
	  readchar(&state);
	  readnum(&ppid);
	  readnum(&pgrp);
	  readnum(&session);
	  readnum(&tty_nr);
	  readnum(&tpgid);
	  readnum(&flags);
	  readnum(&minflt);
	  readnum(&cminflt);
	  readnum(&majflt);
	  readnum(&cmajflt);
	  readnum(&utime);
      readnum(&stime);
      readnum(&cutime);
      readnum(&cstime);
      readnum(&priority);
      readnum(&niceid);
      readnum(&num_threads);
      readnum(&itrealvalue);
      readnum(&starttime);
      readnum(&vsize);
      readnum(&rss);

      printf("%s\t",pws->pw_name);
      printf("%d\t",pid);
      printf("%d\t",ppid);
    //   getmemanduptiem(rss*4096,utime,stime,cutime,starttime,cstime);
      printf("%d MB\t",vsize/1000000);
      printf("%d\t",rss);
    
	  if((tty_nr & mask) >> 8 == 136){   
        pts = (((tty_nr >> 20) << 8) | (tty_nr & 255));
        printf("pts/%d\t",pts);
	  }
	  else if((tty_nr & mask) >> 8 == 4){
	    printf("tty%d \t ",((tty_nr >> 20) << 8) | (tty_nr & 255));
	  }
	  else{ 
	    printf("? \t");
	  }
        printtime(utime);
        printf("\t");
        printf("%d:%d:%d \t",dt.tm_hour, dt.tm_min, dt.tm_sec);
        // printcmd(pid);
        printf("%s",pname);
 
        printf("%s\n","");
	
	  chdir("..");
	
      }
	
      }
      closedir(d);
    }
  }
}
int getallprocessinfo(int argc){
     FILE *fp;
  char c, fname[1000], pname[PATH_MAX], state, fullpath[PATH_MAX], st[10];                              
  int pid, ppid, pgrp, session, tty_nr, tpgid, flags, minflt, cminflt,
    majflt, cmajflt, utime, cutime,cstime ,priority,niceid,num_threads,itrealvalue,vsize,rss;
  long long unsigned int  starttime;
      printf("%s\t", "pid");
      printf("%s\t", "ppid");
      printf("%s\t","session");
      printf("%s\t","flags");
      printf("\t%s\t","pgrp");
      printf("%s\t","tty/pts");
      printf("%s\t", "state");
	  printf("%s\t","utime");
	  printf("%s\n", "pname");
  DIR *d;
  struct dirent *dir;
  
  unsigned int mask = 65280;
  
  input = NULL;
  if(argc < 1){
    printf("Invalid argument list\n");
    printf("%s",usage);
    return 0;
  }
  else {
    //Go to /proc directory and search for directories having names as numbers; thus represent the processes runnning
    d = opendir("/proc");
    chdir("/proc");
    if(d){
      while ((dir = readdir(d)) != NULL){
	sprintf(fname, "%s", dir->d_name);
	if(isdigit(fname[0])){
	  sscanf(fname, "%d", &pid);
	  if(chdir(fname) == 0){
        //    printf("%s\n","----------------------------------------------------------------------------");
	    input = fopen("stat", "r");
	  }
	  if(!input){
	    perror("open");
	    return 1;
	  }
	  readstr(pname);
	  readchar(&state);
	  readnum(&ppid);
	  readnum(&pgrp);
	  readnum(&session);
	  readnum(&tty_nr);
	  readnum(&tpgid);
	  readnum(&flags);
	  readnum(&minflt);
	  readnum(&cminflt);
	  readnum(&majflt);
	  readnum(&cmajflt);
	  readnum(&utime);
      readnum(&cutime);
      readnum(&cstime);
      readnum(&priority);
      readnum(&niceid);
      readnum(&num_threads);
      readnum(&itrealvalue);
      readllu(&starttime);
      readnum(&vsize);
      readnum(&rss);
    
	  if((tty_nr & mask) >> 8 == 136){
        printf("%d = ",pid);
        printf("%s ", pname);
	    printf("pts/%d\t ",((tty_nr >> 20) << 8) | (tty_nr & 255));
        printtime(utime);
        printf("  %d",vsize);
        printf("%s\n","");
        
	  }
	  else if((tty_nr & mask) >> 8 == 4){
        printf("%d = ",pid);
        printf("%s ", pname);
	    printf("tty%d \t ",((tty_nr >> 20) << 8) | (tty_nr & 255));
        printtime(utime);
        printf("  %d",vsize);
        printf("%s\n","");
	  }
	  else{ 
	    // printf("? ");
	  }
	
	  chdir("..");
	
      }
	
      }
      closedir(d);
    }
  }
  return 0;
}

int main(int argc, char *argv[]){
    int x =1;
  int aflag =0, fflag=0,lflag=0,noflag=0;
  int ch;
   if (argc==1){
       noflag =1;
   }else{
      while ((ch= getopt(argc, argv, "aAfFlL")) != -1) {
    switch (ch) {
      case 'a':
      case 'A':
        aflag = 1;
        break;
      case 'f':
      case 'F':
        fflag = 1;
        break;
      case 'l':
      case 'L':
        lflag = 1;
        break;
      case '?':
        fprintf (stderr,"%s",usage);
        exit(EXIT_FAILURE);
    }
   }
   
  }
  if(noflag==1){
      basicps(5);
  }else if(aflag==1){
      ps_a(5);
  }else if(fflag==1){
      ps_af(5);
  }else if(lflag==1){
      ps_l(5);
  }else{
      printf("%s",usage);
  }

 return 0;
}