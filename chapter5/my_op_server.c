/*
 * 自己动手写一个应用程序
 * */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>

#define BUF_SIZE 1024
//错误处理函数
void error_handling(char*message);  
int calculate(int operand1,int operand2,char operator);
int main(int argc,char*argv[])
{


	if(argc!=2){
		printf("Usage : %s <port>\n",argv[0]);
		exit(1);
	}
	// 0. 定义
	int serv_sock,clnt_sock;
	struct sockaddr_in serv_adr,clnt_adr;
	char message[BUF_SIZE],result_buf[BUF_SIZE];
	socklen_t clnt_adr_size;
	int str_len,result_len;
	// 定义最终结果和两个操作数,一个操作符
	int result,operand;
     	char operator;	

	// 1. 创建socket
	serv_sock = socket(PF_INET,SOCK_STREAM,0);
	
	if(serv_sock==-1)
		error_handling("socket() error!");
	//2. 为socket绑定地址信息
	memset(&serv_adr,0,sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port  = htons(atoi(argv[1]));
	
	if(bind(serv_sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1)
		error_handling("bind() error!");


	//3. 监听连接
	if(listen(serv_sock,5)==-1)
		error_handling("listen error!");
	//4. 接收来自客户端的连接
	clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_adr,&clnt_adr_size);
	if(clnt_sock==-1)
		error_handling("accept() error!");


	//5. 读取数据并进行处理
	str_len = read(clnt_sock,message,BUF_SIZE);
	if(str_len==-1)
		error_handling("read() error!");


	//6. 进行计算
	operator = message[str_len-1];
	for(int i=0;i<str_len;i++){
		operand = atoi(&message[i]);
		if(i==0) result = operand;
		else result = calculate(result,operand,operator);
		while(isdigit(message[i]))i++;
	}	
	//7. 写回数据
	sprintf(result_buf,"Operation result: %d",result);
	result_len = strlen(result_buf);
	write(clnt_sock,result_buf,result_len);

	
	//8. 关闭socket
	close(clnt_sock);
	close(serv_sock);
	return 0;
}

int calculate(int operand1,int operand2,char operator){
	switch(operator){
		case '+': return operand1+operand2;break;
		case '-': return operand1-operand2;break;
		case '*': return operand1*operand2;break;
		case '/': return operand1/operand2;break;
		default:return -1;break;
	}
}
void error_handling(char*message)
{
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}
