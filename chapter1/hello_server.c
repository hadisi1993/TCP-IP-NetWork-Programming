#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
void error_handling(char*message);

int main(int argc,char*argv[]){
	//首先设置两个socket,客户端的和服务器端的
	int serv_sock;
	int clnt_sock;

	//设置客户端和服务器端的地址
	struct sockaddr_in serv_addr; 
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;

	char message[] = "Hello World";

	if(argc!=2){
		printf("Usage : %s <port>\n",argv[0]);
		exit(1);
	}
	//使用socket函数创建socket
	serv_sock = socket(PF_INET,SOCK_STREAM,0);
	if(serv_sock==-1)
		error_handling("socket() error");

	memset(&serv_addr,0,sizeof(serv_addr));

	//配置地址信息
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	//为socket绑定地址和端口
	if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
		error_handling("bind() error");

	//监听serv_sock
	if(listen(serv_sock,5)==-1)
		error_handling("listen() error");

	clnt_addr_size = sizeof(clnt_addr);

	//接受来自客户端的socket
	clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size);
	if(clnt_sock ==-1)
		error_handling("accept() error");

	// 向客户端传输信息
	write(clnt_sock,message,sizeof(message));
	close(clnt_sock);
	close(serv_sock);
	return 0;
}
void error_handling(char*message){
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}
