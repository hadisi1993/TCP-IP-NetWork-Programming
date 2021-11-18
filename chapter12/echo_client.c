#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#define BUF_SIZE 1024
void error_handling(char*message);

int main(int argc,char*argv[]){
	//首先设置一个服务器端的scoket
	int sock;
	//设置客户端和服务器端的地址
	struct sockaddr_in serv_addr; 
	char message[30];
	int str_len=0;
	int idx = 0,read_len = 0;
	if(argc!=3){
		printf("Usage : %s <port>\n",argv[0]);
		exit(1);
	}
	//使用socket函数创建socket
	sock = socket(PF_INET,SOCK_STREAM,0);
	if(sock==-1)
		error_handling("socket() error");

	memset(&serv_addr,0,sizeof(serv_addr));

	//配置地址信息
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]); 
	serv_addr.sin_port = htons(atoi(argv[2]));
	
	//连接服务器
	if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
		error_handling("connect() error!");

	else 
		puts("Connected............");
	//循环向服务器发送消息,直到输入Q为止
	while(1)
	{
		fputs("Input message(Q to quit): ", stdout);
		fgets(message,BUF_SIZE,stdin);

		if(!strcmp(message,"q\n")||!strcmp(message,"Q\n"))
			break;

		write(sock,message,strlen(message));
		str_len = read(sock,message,BUF_SIZE-1);
		message[str_len]=0;
		printf("Message from server: %s",message);
	}
	close(sock);
	return 0;
}
void error_handling(char*message){
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}
