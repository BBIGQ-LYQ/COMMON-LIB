
//通过struct sockaddr_in结构体配置服务器地址
/* struct sockaddr_in {
       sa_family_t    sin_family; // address family: AF_INET 
       in_port_t      sin_port;   // port in network byte order 
       struct in_addr sin_addr;   //internet address 
   };*/
/* Internet address. 
   struct in_addr {
       uint32_t       s_addr;     // address in network byte order 
   };*/

#include "BBIGQ_TCP.h"

/**
 * TCP下，设置IPv4服务端配置信息，并绑定和监听
 * @Author   BBIGQ
 * @DateTime 2019-11-25T10:49:48+0800
 * @param    HTONS                    端口号
 * @param    IP_ADDR                  IP地址
 * @param    LISTEN_SIZE              最大的链接数量
 * @return                            返回socket文件描述符
 */
int server_socket_IPv4_set(int HTONS, const char *IP_ADDR, int LISTEN_SIZE)
{
	//创建socket网络通信对象
	int ser_socket =  socket(AF_INET, SOCK_STREAM, 0);
	if(ser_socket < 0)
	{
		perror("");
		return -1;
	}
	else
		printf("create socket success socket id :%d\n", ser_socket);

	//设置绑定服务端信息
	struct sockaddr_in myset;
	memset(&myset,0,sizeof(struct sockaddr_in));
	myset.sin_family = AF_INET;
	myset.sin_port	= htons(HTONS);
	myset.sin_addr.s_addr =((IP_ADDR == NULL)?(INADDR_ANY):(inet_addr(IP_ADDR)));
	
	//int bind(int socket, const struct sockaddr *address, socklen_t address_len);
	//设置重用(复用)本地地址
	#ifdef REUSER
	int arg = 1;
	int msg = sizeof(int );
	setsockopt(ser_socket, SOL_SOCKET, SO_REUSEADDR, &arg, msg);
	#endif

	int ret = bind(ser_socket, (struct sockaddr *)&myset,sizeof(myset));
	if(ret < 0)
	{
		perror("connect error");
		return -1;
	}
	else
		printf("connect success\n");

	//监听
	ret = listen(ser_socket,LISTEN_SIZE);
	if(ret < 0)
	{
		perror("listen error");
		return -1;
	}
	else
		printf("listen success\n");

	return ser_socket;
}

/**
 * 设置客户端的配置信息，并connect上
 * @Author   BBIGQ
 * @DateTime 2019-11-25T10:51:33+0800
 * @param    HTONS                    端口号
 * @param    IP_ADDR                  服务端的IP地址
 * @return                            返回socket文件描述符
 */
int client_socket_IPv4_set(int HTONS, const char *IP_ADDR)
{
	//创建socket网络通信对象
	int cli_socket =  socket(AF_INET, SOCK_STREAM, 0);
	if(cli_socket < 0)
	{
		perror("");
		return -1;
	}
	else
		printf("create socket success\n");

	//设置连接信息
	struct sockaddr_in myset;
	memset(&myset,0,sizeof(struct sockaddr_in));
	myset.sin_family = AF_INET;
	myset.sin_port	= htons(HTONS);
	myset.sin_addr.s_addr 	= inet_addr(IP_ADDR);
	
	//int connect(int socket, const struct sockaddr *address,socklen_t address_len);
	//连接
	int ret = connect(cli_socket, (struct sockaddr *)&myset, sizeof(myset));
	if(ret < 0)
	{
		perror("connect error");
		return -1;
	}
	else
		printf("connect success\n");

	return cli_socket;
}

/**
 * 向指定的socket(客户端)发送文件
 * @Author   BBIGQ
 * @DateTime 2019-11-25T10:53:04+0800
 * @param    SOCKETFD                 接收方的socket文件描述符
 * @param    FILE_PATH                源文件(要发送的文件)路径
 */
void send_file_by_tcp(int SOCKETFD, char *FILE_PATH)
{
	char buff[MAX_BUFF];
	int fd = open(FILE_PATH, O_RDONLY);
	if(fd < 0)
	{
		perror("open file error");
		return;
	}
	while(1)
	{
		memset(buff, 0, MAX_BUFF);
		//从文件中读数据
		int ret = read(fd, buff, MAX_BUFF);
		if(ret < 0)
		{
			if(errno == EINTR)
			{
				perror("An interrupt occurred in send-reading");
				break;
			}
			else if(errno == ECONNRESET)
			{
				perror("A disconnect occurred in send-reading");
				break;
			}
			perror("");
			break;
		}
		//写入socket
		ret = write(SOCKETFD, buff, ret);
		if(ret < 0)
		{
			if(errno == EINTR)
			{
				perror("An interrupt occurred in send-writing");
				break;
			}
			else if(errno == EPIPE)
			{
				perror("A disconnect occurred in send-writing");
				break;
			}
			perror("");
			break;
		}
		if(ret == 0)
		{
			printf("send over!\n");
			break;
		}

	}

	close(SOCKETFD);
	close(fd);
	return;
}

/**
 * 向指定的socket文件描述符接收文件
 * @Author   BBIGQ
 * @DateTime 2019-11-25T10:55:18+0800
 * @param    SOCKETFD                 (服务端接收时：发送方的id)(客户端接收时：自己的socket)
 * @param    FILE_PATH                要保存的路劲
 */
void receive_file_by_tcp(int SOCKETFD, char *FILE_PATH)
{
	char buff[MAX_BUFF];
	int fd = open(FILE_PATH, O_CREAT|O_RDWR, 0777);
	if(fd < 0)
	{
		perror("open file error");
		return;
	}
	while(1)
	{
		memset(buff, 0, MAX_BUFF);
		int ret = read(SOCKETFD, buff, MAX_BUFF);
		if(ret < 0)
		{
			if(errno == EINTR)
			{
				perror("An interrupt occurred in receive-reading");
				break;
			}
			else if(errno == ECONNRESET)
			{
				perror("A disconnect occurred in receive-reading");
				break;
			}
			perror("");
			break;
		}
		ret = write(fd, buff, ret);
		if(ret < 0)
		{
			if(errno == EINTR)
			{
				perror("An interrupt occurred in receive-writing");
				break;
			}
			else if(errno == EPIPE)
			{
				perror("A disconnect occurred in receive-writing");
				break;
			}
			perror("");
			break;
		}
		if(ret == 0)
		{
			printf("recv over!\n");
			break;
		}
		
	}

	close(SOCKETFD);
	close(fd);
	return;

}
/*
int server_socket_IPv6_set()
{

}


int client_socket_IPv6_set()
{

}*/

