	
myshell: mainshell.c helper.c csapp.c myls mymv mygrep mycp myps     
	gcc -g -o myshell mainshell.c helper.c csapp.c -lreadline
# echorequirement: mainshell.c
# 	echo "Needs GNU readline library for history if compilation fails ++++++> Get from sudo apt-get install git build-essential libreadline-dev"

myls : myls.c helper.c
	gcc -g -o myls myls.c helper.c
mymv : mymv.c
	gcc -g -o mymv mymv.c
mygrep : mygrep.c
	gcc -g -o mygrep mygrep.c
mycp : mycp.c
	gcc -g -o mycp mycp.c

myps : myps.c 
	gcc -g -o myps myps.c
clean:
	rm -f myls mycp mymv mygrep myps myshell



# sudo apt-get install git build-essential libreadline-dev