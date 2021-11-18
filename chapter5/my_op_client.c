#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#define BUF_SIZE 1024
void error_handling(char*message);
int main(int argc,char*argv[])
{
	//0. 定义
	int sock;
	char message[BUF_SIZE]="",operand[BUF_SIZE];
	int str_len,recv_len,send_len;	
	struct sockaddr_in serv_adr;
	
	if(argc!=3){
		printf("Usage : %s <IP> <port>\n",argv[0]);
		exit(1);
	}
	//1.创建socket
	sock = socket(PF_INET,SOCK_STREAM,0);
	if(sock==-1)
		error_handling("socket() error");

	//2.为socket绑定地址信息
	memset(&serv_adr,0,sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	//3.连接服务器
	if(connect(sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))==1)
		error_handling("connect() error!");
	else 
		puts("Connected.........");

	//4.输入消息
	int operCnt,oper;
	char operator;
	str_len = 0;
	printf("Operand count: ");
	scanf("%d",&operCnt);
	for(int i=1;i<=operCnt;i++){
		printf("Operand %d: ",i);
		scanf("%d",&oper);
		sprintf(operand,"%d ",oper);
		strcat(message,operand);
		// printf("%s\n",message);
		str_len = str_len+strlen(operand);
		
	}
	printf("Operator: ");
	getchar(); // 吃掉空格
	scanf("%c",&operator);
	// printf("%c\n",operator);
	message[str_len] = operator;
	message[++str_len] = '\0';

	//printf("message: %s\n",message);

	//printf("str_len:%d\n",str_len);
	//5.向服务器端发送消息
	send_len = write(sock,message,strlen(message));
	if(send_len==-1)
		error_handling("write error()!");

	//6.接受服务器端的计算结果
	
	recv_len = read(sock,&message[0],BUF_SIZE-1);
	if(recv_len==-1)
		error_handling("read() error!");

	message[recv_len] = '\0';

	//7.输出结果
	printf("%s\n",message);

	//8. 关系socket
	close(sock);
	return 0;
}
void error_handling(char*message){
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}
