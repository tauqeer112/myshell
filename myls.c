/***
        -a            do not ignore entries starting with .(hidden)
        -l            use a long listing format
        -n            like -l, but list numeric user and group IDs
      
**/

        
#include "myheaders.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <sys/stat.h>

#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define	DIR_MODE	(FILE_MODE | S_IXUSR | S_IXGRP | S_IXOTH)
void sort(struct dirent **dir, struct stat **buf, int length);


#define	min(a,b)	((a) < (b) ? (a) : (b))
#define	max(a,b)	((a) > (b) ? (a) : (b))




typedef int Oper(char*, const struct stat *, int);
bool bit_a, bit_A, bit_c, bit_d, bit_i, bit_l, bit_n, bit_R, bit_s, bit_t;
char temp;



//determine the arguments passed by the user

void parse_args(int argc, char **argv)
{

        char c;
        while( (c=getopt(argc,argv,"aln")) != -1 )
        {
	       switch(c)
               {
                        case 'a':
                        	        bit_a = true;
                                	break;

               

                        case 'l':
	                                bit_l = true;
		                        break;

                        case 'n':
	                                bit_n = true;
	                                break;

                         default:
                                	printfile("mylshelp.txt");
									exit(0);
							
                }
        }

}





// Username from file descriptor

void user_name(unsigned int id)
{

	struct passwd *pd;

	pd = getpwuid(id);
	printf(" %10s", pd->pw_name);

}






//Group name from file descriptor
void group_name(unsigned int id)
{

	struct group *pd;

	pd = getgrgid(id);
	printf(" %10s", pd->gr_name);

}

// print the permission of the file.
void printpermission(const struct stat *statbuf ){

        printf( (S_ISDIR(statbuf->st_mode)) ? "d" : "-" );
		printf( (statbuf->st_mode & S_IRUSR) ? "r" : "-" );
		printf( (statbuf->st_mode & S_IWUSR) ? "w" : "-" );
		printf( (statbuf->st_mode & S_IXUSR) ? "x" : "-" );
		printf( (statbuf->st_mode & S_IRGRP) ? "r" : "-" );
		printf( (statbuf->st_mode & S_IWGRP) ? "w" : "-" );
		printf( (statbuf->st_mode & S_IXGRP) ? "x" : "-" );
		printf( (statbuf->st_mode & S_IROTH) ? "r" : "-" );
		printf( (statbuf->st_mode & S_IWOTH) ? "w" : "-" );
		printf( (statbuf->st_mode & S_IXOTH) ? "x" : "-" );
}

//prints the data to stdout according to the bit set from argument.
int print_data(char* name, const struct stat *statbuf, int count)
{
	int i;
	char time[100];
	struct passwd *pwd;
	struct group *grp;

	if(!bit_l)
		for(i=0; i<count; i++)
			printf("\t");
	if(bit_l)
	{	//remember USR GRP OTH
         printpermission(statbuf);

		printf("%3lu",statbuf->st_nlink);

		if(bit_s)
			printf("%4lu",statbuf->st_blocks);

		if(bit_n)
		{
			printf(" %u ",statbuf->st_uid);
			printf(" %u ",statbuf->st_gid);
		}

		else
		{
			pwd = getpwuid(statbuf->st_uid);
			grp = getgrgid(statbuf->st_gid);
			printf(" %10s", pwd->pw_name);
			printf(" %10s ",grp->gr_name);
		}

		printf("%7ld",statbuf->st_size);

			strftime(time,100,"%b %d %H:%M",localtime(&statbuf->st_ctime));

		printf(" %s ",time);
	}

	printf("%s\n",name);

}






//traversing the current directory
int dir_traverse(char *par, Oper *op)
{
	static int count = -1;
	count++;
	struct stat statbuf;
	struct stat** buf;
	DIR *dp;
	struct dirent** entry;
	struct dirent* tempEntry;
	int i,count1 = 0;
	char *filename;

	if( (filename = (char*)malloc(sizeof(char) * (strlen(par) + pathconf(par,_PC_PATH_MAX) + 1))) == NULL )
	{
		printf("\nNo memory\n");
		goto end;
	}

	if( lstat(par,&statbuf)<0 )
	{
		printfile("mylshelp.txt");
		goto end;
	}

	if(S_ISDIR(statbuf.st_mode) == 0)
	{
		op(par, &statbuf, 0);
		goto end;
	}

	if(bit_d)
	{
		op(par, &statbuf, 0);
		goto end;
	}

	count1 = scandir(par, &entry, NULL, NULL);
	buf = (struct stat**)calloc(count1, sizeof(struct statbuf*));

	for(i=0; i<count1; i++)
	{
		strcpy(filename,par);
		if(filename[ strlen(filename)-1 ] != '/')
			strcat(filename,"/");
		strcat(filename,entry[i]->d_name);
		buf[i] = (struct stat*)calloc(1,sizeof(struct stat));
		lstat(filename,buf[i]);
	}

	for(i=0; i<count1; i++)
	{
		bool a, b;

		a = (entry[i]->d_name[0]!='.' || bit_a);    //|| bit_A);
		b = ((strcmp(entry[i]->d_name,".") && strcmp(entry[i]->d_name,".."))) ; //|| !bit_A);

		if(a && b)
			op(entry[i]->d_name, buf[i], count);

		if(bit_l)
			continue;

	}

	end:
		count--;
		free(filename);

	return 0;

}




int main(int argc, char **argv)
{

	parse_args(argc, argv);

	if(optind >= argc )
		dir_traverse(".", print_data);
	else
		dir_traverse(argv[optind], print_data);

	return 0;
}