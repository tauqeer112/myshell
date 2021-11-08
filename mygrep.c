#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>

// #include <getopt.h>
#include <unistd.h>
#define GREEN  "\x1B[32m"
#define NORMAL  "\x1B[0m"
#define BLUE  "\x1B[34m"
#define RED  "\x1B[31m"



// #define FILENAME "file.txt"
char *usage = " -a pattern for all \n  -n filename pattern to search in filename\n -f pattern to search only file \n -d pattern to search directory name \n  ";


void grepfunc(char *filename,char *pattern){
       /* Open the file for reading */
  char *line_buf = NULL;
  size_t line_buf_size = 0;
  int line_count = -1;
  ssize_t line_size;
  char **lines;
  char *match;
//   char pattern[1024];
//   strcpy(pattern,"haj");
  lines = malloc(sizeof(char*)*5000);
  FILE *fp = fopen(filename, "r");
  if (!fp)
  {
    fprintf(stderr, "Error opening file '%s'\n", filename);
    exit(0);
  }


  line_size = getline(&line_buf, &line_buf_size, fp);

  /* Loop through until we are done with the file. */
  while (line_size >= 0)
  {
    /* Increment our line count */
    line_count++;
    lines[line_count] = malloc(sizeof(char)*(line_size+1));
    strcpy(lines[line_count],line_buf);
    line_size = getline(&line_buf, &line_buf_size, fp);
  }
  for (size_t i = 0; i <= line_count; i++)
  { 
      
      match = strstr(lines[i],pattern);
      if(match==NULL){
          continue;
      }else{
        int len = strlen(lines[i]);
       lines[i][len-1] = '\0';
       printf("%s , %s%s, line number = %zu ,filename = %s, %s\n",lines[i],RED,pattern,i,filename,NORMAL);
      }
  }
  /* Free the allocated line buffer */
  free(line_buf);
  line_buf = NULL;

  /* Close the file now that we are done with it */
  fclose(fp);

}


void grepdir(char *dirname,char *pattern){
   char *match;
    match = strstr(dirname,pattern);
      if(match!=NULL){
       printf(" Directory =  %s  , pattern = %s%s%s\n",dirname,RED,pattern,NORMAL);
      }
    
}

int main(int argc, char **argv )
{    char ch;
     char filename[1024];
     char pattern[1024];
     
    int fflag =0, aflag=0 ,dflag=0,nflag=0,normalflag=0;
    // printf("%d",argc);
    if(argc<2){
        printf("%s",usage);
        exit(0);
    }
    if (argc==2){
         normalflag =1;
         strcpy(pattern,argv[1]);
     }
     else{
             while ((ch= getopt(argc, argv, "f:a:d:n:")) != -1) {
     switch (ch) {
      case 'n':
      case  'N':
         if(argc<4){
             printf("%s \nsometing missing",usage);
             exit(0);
         }
        strcpy(filename,optarg);
        nflag =1;
        break;
      case 'a':
      case 'A':
       if(argc<3){
             printf("%s \nsometing missing",usage);
             exit(0);
         }
        //  printf("%d",argc);
        strcpy(pattern,optarg);
         aflag=1;
         break;
      case 'f':
      case 'F':
         if(argc<3){
             printf("%s \nsometing missing",usage);
             exit(0);
         }
        //  printf("%d",argc);
          strcpy(pattern,optarg);
          fflag =1;
          break;
      case 'd':
      case 'D':
           if(argc<3){
             printf("%s \nsometing missing",usage);
             exit(0);
         }
        //   printf("%d",argc);
          strcpy(pattern,optarg);
          dflag =1;
          break;
      case '?':
        fprintf (stderr,"%s",usage);
        exit(EXIT_FAILURE);
    }
  }
     }
     if(nflag == 1){
       while(optind < argc){
       strcpy(pattern,argv[optind]);
       optind++;
   }
     }
    // printf("%d  %d %d %d  %d\n\n\n\n",nflag , aflag , dflag, fflag ,normalflag);
    // char *pattern = "this";    
   DIR *d;
   struct dirent *dir;
   d = opendir(".");
   if (d) {
    while ((dir = readdir(d)) != NULL) {
          
          if(fflag == 1){
                if(dir->d_type==DT_REG){
                grepfunc(dir->d_name,pattern);
                grepdir(dir->d_name,pattern);
                }
          }else if (aflag==1 || normalflag==1){
            if(dir->d_type==DT_REG){
                grepfunc(dir->d_name,pattern);
                grepdir(dir->d_name,pattern);
            }else if(dir->d_type == DT_DIR){
                    grepdir(dir->d_name,pattern);
            }else{

            }
          }else if (dflag ==1)
          {
              if(dir->d_type == DT_DIR){
                    grepdir(dir->d_name,pattern);
              }
          }else if (nflag ==1){
            grepfunc(filename,pattern);
            exit(0);
          }
      
         



            // if(dir->d_type==DT_REG){
            //     grepfunc(dir->d_name,pattern);
            //     grepdir(dir->d_name,pattern);
            // }else if(dir->d_type == DT_DIR){
            //         grepdir(dir->d_name,pattern);
            // }else{

            // }
    }
    closedir(d);
  }
    

}
