#include "BBIGQ_UDP.h"

/**
 * 创建UDP通信socket
 * @Author   BBIGQ
 * @DateTime 2019-11-26T09:25:05+0800
 * @return                            返回udp_socket
 */
int UDP_IPv4_set(void)
{
	int udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if(udp_socket < 0)
	{
		perror("socket error");
		return -1;
	}
	else
		printf("create socket success socket id :%d\n", udp_socket);

	return udp_socket;
}

/**
 * 配置信息的设置
 * @Author   BBIGQ
 * @DateTime 2019-11-26T09:59:33+0800
 * @param    HTONS                    端口号
 * @param    IP_ADDR                  IP地址
 * @param    udp_addr                struct sockaddr_in*结构体
 * @return                            [description]
 */
int UDP_ADDR_SET(int HTONS, char const *IP_ADDR, struct sockaddr_in *udp_addr)
{
	if(IP_ADDR == NULL)
		return -1;
	//配置信息
	udp_addr->sin_family = AF_INET;
	udp_addr->sin_port = htons(HTONS);
	udp_addr->sin_addr.s_addr = inet_addr(IP_ADDR);

	return 0;
}

/**
 * 设置和绑定接收者的信息
 * @Author   BBIGQ
 * @DateTime 2019-11-26T09:31:13+0800
 * @param    udp_socket               udp_socket
 * @param    HTONS                    端口号
 * @param    IP_ADDR                  IP地址
 * @return                            [description]
 */
int UDP_set_and_bind(int udp_socket, int HTONS, char const *IP_ADDR)
{
	//设置绑定服务端信息
	struct sockaddr_in udp_addr;
	memset(&udp_addr,0,sizeof(struct sockaddr_in));

	//绑定接收者个人信息
	udp_addr.sin_family = AF_INET;
	udp_addr.sin_port = htons(HTONS);
	udp_addr.sin_addr.s_addr = inet_addr(IP_ADDR);
	// 绑定接收者的IP地址信息
	// int bind(int socket, const struct sockaddr *address,socklen_t address_len);
	int ret = bind(udp_socket, (struct sockaddr *)&udp_addr, sizeof(udp_addr));
	if(ret < 0)
	{
		perror("bind error");
		return -1;
	}

	return 0;
}

/**
 * 广播设置
 * @Author   BBIGQ
 * @DateTime 2019-11-26T17:37:46+0800
 * @param    udp_socket               udp_socket
 * @return                            [description]
 */
int UDP_Broadcast_set(int udp_socket)
{
	//开启广播属性
	int on = 1;
	int ret = setsockopt(udp_socket, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));
	if(ret < 0)
	{
		perror("UDP_Broadcast_set error");
		return -1;
	}
	else 
		printf("Open broadcast success!\n");
	
	return 0;
}

/**
 * 组播配置
 * @Author   BBIGQ
 * @DateTime 2019-11-26T17:36:41+0800
 * @param    udp_socket               udp_socket
 * @param    MEMB_ADDR                组播地址
 * @return                            [description]
 */
int UDP_Membership_set(int udp_socket, char const *MEMB_ADDR)
{

	struct ip_mreq array_addr;
	array_addr.imr_multiaddr.s_addr = inet_addr(MEMB_ADDR); //选择组播地址 224.0.0.1-239.255.255.255
	array_addr.imr_interface.s_addr = inet_addr("0.0.0.0");//自动选择本地网卡加入组播
	int ret = setsockopt(udp_socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, &array_addr, sizeof(array_addr));
	if(ret < 0)
	{
		perror("");
		return -1;
	}
	else
		printf("加入组播成功\n");

	return 0;
}