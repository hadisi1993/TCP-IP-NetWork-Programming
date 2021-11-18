#include<stdio.h>
#include<string.h>
#include<unistd.h>
#define BUF_SIZE 30

int main(int argc,char*argv[]){
	
	int fds[2];
	char str[] = "Who are you?";
	char buf[BUF_SIZE];

	pid_t pid;

	pipe(fds);
	pid = fork();
	//实现父进程和子进程之间的通信
	if(pid==0){
		write(fds[1],str,sizeof(str));
	}
	else{
		read(fds[0],buf,BUF_SIZE);
		puts(buf);
	}
	return 0;
}
