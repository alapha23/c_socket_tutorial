#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MYPORT 5000

/* Socket: a way to speak to other programs using standard Unix file descriptors
 * 
 * Stream sockets are reliable two-way connected communication streams.
 * Datagrams sockets also use IP for routing, but they use UDP rather than TCP
*//*
struct sockaddr{
	unsigned short	sa_family;	//address family, AF_INET
	char		sa_data[14];	//14 bytes of protocol address, contains a destination address and port number for the socket
};*/
// Internet address
/*
struct in_addr {
	unsigned long s_addr;	//4 bytes
};*/
/* 
struct sockaddr_in{
	short int	sin_family;	//Address family
	unsigned short int sin_port;	//Port number
	struct in_addr	sin_addr;	//Internet address
	unsigned char	sin_zero[8];	//Same size as struct sockaddr
};*/
//sockaddr_in can be cast to a pointer to a struct sockaddr and vice-versa

/*
 * htons() -- Host to Network Short
 * htonl() -- Host to Network Long
 * ntohs() -- Network to Host Short
 * ntohl() -- Network to Host Long
 * Remember to put your bytes in Network Byte Order before put on the network
*/


int main(){
/* A socket descriptor is of the type int
 * 
 * 
 *
*/
//	struct sockaddr_in ina;
//	ina.sin_addr.s_addr = inet_addr("10.12.110.57");
//inet_addr() returns address in Network Byte Order	

//For a cleaner interface, we can use inet_aton()
//include <sys/socket.h>
//include <netinet/in.h>
//include <arpa/inet.h>
//int inet_aton(const char *cp, struct in_addr *inp);

	struct sockaddr_in my_addr;	
	my_addr.sin_family = AF_INET;	//host byte order
	my_addr.sin_port = htons(MYPORT);//short, network byte order
	inet_aton("10.12.110.57", &(my_addr.sin_addr));
	memset(&(my_addr.sin_zero), '\0', 8);//zero the rest of the struct

//the above is about converting IP to binary representations
	printf("%s\n", inet_ntoa(my_addr.sin_addr));
//ntoa means "network to ascii", it returns a pointer to a char

//include <sys/types.h>
//include <sys/socket.h>
//int socket(int domain, int type, int protocol);
/*domain should be set to "AF_INET"
 *type tells kernel what kind of socket this is: SOCK_STREAM or SOCK_DGRAM
 *set protocol to "0" to have socket() choose the correct protocol based on the type
 *socket() returns a socket descriptor
*/
	int my_sock;
	my_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (my_sock < 0)
		perror("Error opening socket");

/* Once done opening a socket, we might have to associate the socket with a port on a local machine
 * Port number is used by the kernel to match an incoming packet to a certain process's socket descriptor
 * int bind(int sockfd, struct sockaddr *my_addr, int addrlen);
 * 
 * 
*/
	if( bind(my_sock, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) < 0)
	perror("Error on binding.");
/*	Some of process of getting your own IP address and port
 *	my_addr.sin_port = 0;	//choose an unused port at random
 *	my_addr.sin_addr.s_addr = INADDR_ANY;//use my IP address, INADDR_ANY is 
0
 * 	All ports below 1024 are RESERVED
*/ 	

/*
 * int connect (int sockfd, struct sockaddr *serv_addr, int addrlen);
 * sockfd: socket file descriptor, serv_addr is the struct sockaddr containing destination port and IP address; addrlen is often set to sizeof(struct sockaddr)
 *
*/
	connect(my_sock, (struct sockaddr *)&my_addr, sizeof(struct sockaddr));
//NEED to check if connected successfully

/*First we listen(), then we accept()
 *int listen(int sockfd, int backlog);
 *backlog is the number of connections allowed on the incoming queue
 * 
*/
	listen(my_sock, 10);

/*accept() will return a brand new socket file descriptor	
 *the original one is still listening on the port while the new one is ready to send() and recv()
 *int accept(int sockfd, void *addr, int *addrlen);
 *addr will be a pointer to a local struct sockaddr_in where the incoming connection will go
 *addrlen should be set to sizeof(struct sockaddr_in) before its address is passed to accept()
*/
	int new_fd = accept(my_sock, (struct sockaddr *)&my_addr, sizeof(struct sockaddr_in));
// new_fd will be used for all send() and recv() calls

/*
 *	int send(int sockfd, const void *msg, int len, int flags);
 *Just set flags to 0
 *send() returns the number of bytes actually sent out
 *sockfd is the socket descriptor you want to send data to
*/	
	char *msg = "hi";
	int len, bytes_sent;
	len = strlen(msg);
	bytes_sent = send(new_fd, msg, len, 0);

/*
 *	int recv(int sockfd, void *buf, int len, unsigned int flags);
 *sockfd is the socket descriptor to read from
 *buf is the buffer to read information into
 *len is the maximum length of the buffer
 *flags can be set to 0
 *If recv() returns 0, it means the remote side has closed the connection
*/

	close(my_sock);

	return 0;
}



