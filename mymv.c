#include<stdio.h>
#include<fcntl.h> 
#include<stdlib.h>
#include <getopt.h>
#include <unistd.h>
#define GREEN  "\x1B[32m"
#define NORMAL  "\x1B[0m"
#define BLUE  "\x1B[34m"
#define RED  "\x1B[31m"

char usage[250] = "mymv -flag filename1 filename2\nfF - force create filename1 and rename to filename2\ntt -rename filename1 to filename2 and truncate\niI - ignore if filename2 exists\ncC - check if operation is safe\nhH for help\n";
// fF - force create filename1 and rename to filename2
// tt -rename filename1 to filename2 and truncate
// iI - ignore if filename2 exists
// cC - check if filename2 already exists

int main(int argc,char *argv[]){
  int src, dest, count;
  int fflag=0, cflag=0,tflag=0,iflag=0;
  char buf[512];
  int ch;
  
  while ((ch= getopt(argc, argv, "FftTiIcChH")) != -1) {
    switch (ch) {
      case 'h':
      case 'H':
        printf("%s\n",usage);
      case 'f':
      case 'F':
        fflag = 1;
        break;
      case 'c':
      case 'C':
        cflag = 1;
        break;
      case 't':
      case 'T':
        tflag =1 ;
        break;
      case 'i':
      case 'I':
         iflag = 1;
      case '?':
        fprintf (stderr,"%s",usage);
        exit(EXIT_FAILURE);
    }
  }
  src = open(argv[1], O_RDONLY);
  dest = open(argv[2], O_RDONLY);
  if(fflag==1){
      if(src<0){
          dest = open(argv[2],O_CREAT | O_TRUNC | O_APPEND | O_RDWR, 0766);
          count = unlink(argv[1]);
      }else{
          dest = open(argv[2], O_WRONLY);
          while((count = read(src, buf, 100)) > 0){
             write(dest, buf, count); 
    }    
           count = unlink(argv[1]);
      }
  }else if(tflag==1){
       dest = open(argv[2],O_CREAT | O_TRUNC | O_APPEND | O_RDWR, 0766);
       count = unlink(argv[1]);
  }else if(iflag==1){
      if(dest<0){
          dest = open(argv[2],O_CREAT | O_TRUNC | O_APPEND | O_RDWR, 0766);
          while((count = read(src, buf, 100)) > 0){
             write(dest, buf, count);     
      }
            count = unlink(argv[1]);
      }
  }else if(cflag==1){
      if(dest>0){
          printf("%s","Destination file doesn't exist, it is safe to proceed\n");
      }else{
          printf("%s%s%s",RED,"Destination file does exist, Caution data may be lost\n",NORMAL);
      }
  }else{
      if(src>0 && dest<0){
          dest = open(argv[2],O_CREAT | O_TRUNC | O_APPEND | O_RDWR, 0766);
          while((count = read(src, buf, 100)) > 0){
             write(dest, buf, count);     
      }
      count = unlink(argv[1]);
  }
  
  }
  close(src);
  close(dest);
  //Delete the source file
//   count = unlink(argv[1]); 
  return 0;
}