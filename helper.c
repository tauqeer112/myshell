#include "csapp.h"
#include "myheaders.h"
#include <limits.h>


const char* LookLikeShell(){
    char hostname[HOST_NAME_MAX];
    char username[LOGIN_NAME_MAX];
    gethostname(hostname, HOST_NAME_MAX);
    getlogin_r(username, LOGIN_NAME_MAX);
    long size;
    char *directory;
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
       // printf("Current working dir: %s\n", cwd);
   } else {
       perror("getcwd() error");
       return "error";
   }
    size = pathconf(".",_PC_PATH_MAX);
    if ((directory = (char *)malloc((size_t)size)) != NULL)
        getcwd(directory, (size_t)size);
    // snprintf(looklike,"%s%s@%s%s:~%s%s%s $ ",GREEN,username,BLUE,hostname,RED,cwd,NORMAL);
    strcat(looklike,GREEN);
    strcat(looklike,username);
    strcat(looklike,"@");
    strcat(looklike,BLUE);
    strcat(looklike,hostname);
    strcat(looklike,":~");
    strcat(looklike,RED);
    strcat(looklike,cwd);
    strcat(looklike,NORMAL);
    strcat(looklike," $ ");
    return looklike;

}


int builtin_cd(char **args)
{
	if (args[1] == NULL) 
	{
		printf("myShell: expected argument to \"cd\"\n");
	} 
	else 
	{
		if (chdir(args[1]) != 0) 
		{
			perror("myShell: ");
		}
	}
	return 1;
}

char **splitLine(char *line)
{
	char **tokens = (char **)malloc(sizeof(char *) * 64);
	char *token;
	char delim[10] = " \t\n\r\a";
	int pos = 0, bufsize = 64;
	if (!tokens)
	{
		printf("\nBuffer Allocation Error.");
		exit(EXIT_FAILURE);
	}
	token = strtok(line, delim);
	while (token != NULL)
	{
		tokens[pos] = token;
		pos ++;
		if (pos >= bufsize)
		{
			bufsize += 64;
			line = realloc(line, bufsize * sizeof(char *));
			if (!line) // Buffer Allocation Failed
			{
			printf("\nBuffer Allocation Error.");
			exit(EXIT_FAILURE);
			}
		}
		token = strtok(NULL, delim);
	}
	tokens[pos] = NULL;
	return tokens;
}


void printhelpfile(){

    FILE *fptr;
  
    char filename[100], c;
  
    // Open file
    fptr = fopen("help.txt", "r");
    if (fptr == NULL)
    {
        printf("Cannot open file \n");
        exit(0);
    }
  
    // Read contents from file
    c = fgetc(fptr);
    while (c != EOF)
    {
        printf ("%c", c);
        c = fgetc(fptr);
    }
    printf("\n");
    fclose(fptr);
}