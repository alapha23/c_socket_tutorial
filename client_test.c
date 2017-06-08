/*
** client.c -- a stream socket client demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define PORT 3490 // the port client will be connecting to 

#define MAXDATASIZE 100 // max number of bytes we can get at once 
#define BACKLOG 5

/*
void write_to_server(){
	write_all_to_socket();
}
void read_from_server(){
	read_ll_from_socket()
}
*//*
void get_input(){

}
*/
int main(int argc, char *argv[])
{
    int sockfd, new_fd, numbytes;
    char buf[MAXDATASIZE];
    struct hostent *he;
    struct sockaddr_in their_addr; // connector's address information 

    if (argc != 2) {
        fprintf(stderr,"usage: client hostname\n");
        exit(1);
    }

    if ((he=gethostbyname(argv[1])) == NULL) {  // get the host info 
        perror("gethostbyname error");
        exit(1);
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket create error");
        exit(1);
    }

    their_addr.sin_family = AF_INET;    // host byte order 
    their_addr.sin_port = htons(PORT);  // short, network byte order 
    their_addr.sin_addr = *((struct in_addr *)he->h_addr);
    memset(&(their_addr.sin_zero), '\0', 8);  // zero the rest of the struct 

    if (connect(sockfd, (struct sockaddr *)&their_addr,
                                          sizeof(struct sockaddr)) == -1) {
        perror("connect");
        exit(1);
    }

    int listenfd, yes=1;
    struct sockaddr_in my_addr;
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);    }
    if (setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) { perror("setsockopt");
        exit(1);    }
 my_addr.sin_family = AF_INET;         // host byte order
 my_addr.sin_port = htons(PORT+1);     // short, network by
 my_addr.sin_addr.s_addr = INADDR_ANY; // automatically fil
 memset(&(my_addr.sin_zero), '\0', 8); // zero the rest of
    if ( bind(listenfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) {     perror("bind");        exit(1);    }
    if (listen(listenfd, BACKLOG) == -1) {        perror("listen");       exit(1);    }



    while(1){
	if (( numbytes = read(sockfd, buf, MAXDATASIZE -1)) < 0){	perror("recv");	exit(1);}
	buf[numbytes] = '\0';
	if (*buf != '\0')
		printf("Received: %s\n",buf);
	if(!strcmp(buf, "exit"))
		break;
	memset(&buf, '\0', MAXDATASIZE - 1);


	int n = sizeof(struct sockaddr_in); 
	if (new_fd = accept(listenfd, (struct sockaddr *)&their_addr, &n )== -1){
		perror("accept error");exit(1);}
	if(read(new_fd, buf, 255)<0){perror("read error\n");exit(1);}
	if(!fork()){
		close(listenfd);
		strcpy(buf,"hey");
		if (send(new_fd, buf, 14, 0) == -1)
	                perror("send");
                close(new_fd);
		exit(0);
	}
/*	read(new_fd, buf, 255);
	if (!fock()){
		close(sockfd);
		if( send(new_fd, "try", 255, 0) < 0){
			perror("error writing");}	
		close(new_fd);
		exit(0);
	}
	close(new_fd);*/
/*	if (write( sockfd, "Hi", strlen("Hi"))<0){
		perror("error writing to the socket");
		exit(1);}*/
    }
/*   if ((numbytes=recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
        perror("recv");
        exit(1);
    }
    buf[numbytes] = '\0';
    printf("Received: %s",buf);
*/
   close(sockfd);
    return 0;
} 
