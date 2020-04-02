#ifndef _UDP_ANY_H_
#define _UDP_ANY_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <fcntl.h>

#define MAX_BUFF 1024

int UDP_IPv4_set(void);

int UDP_ADDR_SET(int HTONS, char const *IP_ADDR, struct sockaddr_in *udp_addr);

int UDP_set_and_bind(int udp_socket, int HTONS, char const *IP_ADDR);

int UDP_Broadcast_set(int udp_socket);

int UDP_Membership_set(int udp_socket, char const *MEMB_ADDR);


#endif
