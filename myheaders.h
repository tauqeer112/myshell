unsigned char *cmd;
int pid_status;
char looklike[500];
#define GREEN  "\x1B[32m"
#define NORMAL  "\x1B[0m"
#define BLUE  "\x1B[34m"
#define RED  "\x1B[31m"
const char* LookLikeShell();
void Execute(char *args[]);
int builtin_cd(char **args);
char **splitLine(char *line);
void printhelpfile();