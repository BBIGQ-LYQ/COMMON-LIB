#ifndef _TCP_ANY_H_
#define _TCP_ANY_H_

#include <sys/socket.h>
#include <netinet/in.h>
//#include <netinet/ip.h> 
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <fcntl.h>

#define MAX_BUFF 1024

/* TCP - 服务端socket ipv4 初始化设置并返回socket文件描述符 */
int server_socket_IPv4_set(int HTONS, const char *IP_ADDR, int LISTEN_SIZE);

/* TCP - 客户端socket ipv4 初始化设置并返回socket文件描述符 */
int client_socket_IPv4_set(int HTONS, const char *IP_ADDR);

/* TCP - 通过socket发送文件 */
void send_file_by_tcp(int SOCKETFD, char *FILE_PATH);

/* TCP - 通过socket接收文件 */
void receive_file_by_tcp(int SOCKETFD, char *FILE_PATH);

/* */
//int server_socket_IPv6_set();

/* */
//int client_socket_IPv6_set();

#endif

