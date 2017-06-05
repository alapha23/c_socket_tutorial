/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
//     socklen_t clilen; //clilen stores the size of the address of the client
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr; //internet addresses stored
//cli_addr contains the address of the client which connects to the server
     int n;		//n is the return value for read() and write() calls
     if (argc < 2) error("ERROR, no port provided\n");

     sockfd = socket(AF_INET, SOCK_STREAM, 0);//it creates a new sokcets
     if (sockfd < 0) error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));//set serv_addr to zero
     portno = atoi(argv[1]);			//port number, converting from string to an integer

     serv_addr.sin_family = AF_INET;		
     serv_addr.sin_addr.s_addr = htons(INADDR_ANY);
     serv_addr.sin_port = htons(portno);

//Start binding the socket to an address
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");

     listen(sockfd,5);				//Allowing process to listen on the socket for connections
//Backlog queue is 5, number of conections that can be waiting
//     clilen = sizeof(cli_addr);

//sockfd is listening so we must use newsockfd to accept
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 sizeof(cli_addr));
     if (newsockfd < 0) error("ERROR on accept");

     bzero(buffer,256);
     n = read(newsockfd,buffer,255);
     if (n < 0) error("ERROR reading from socket");
     printf("Here is the message: %s\n",buffer);
     n = write(newsockfd,"I got your message",18);
     if (n < 0) error("ERROR writing to socket");
     close(newsockfd);
     close(sockfd);
     return 0; 
}
