#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n; //socket file desciptor, port number, message
    struct sockaddr_in serv_addr;//server address
    struct hostent *server;//server/host information

    char buffer[256];//buffer for code
    if (argc < 3) {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }
    portno = atoi(argv[2]);//portnumber string into int
    sockfd = socket(AF_INET, SOCK_STREAM, 0);//init socket
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);//gethost by the name
    if (server == NULL) {//gethostbyname() returns null if it doesnt work
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));//turn serv_addr into zeros
    serv_addr.sin_family = AF_INET;//message over internet type
    bcopy((char *)server->h_addr, 
        (char *)&serv_addr.sin_addr.s_addr,
        server->h_length);//bzero for h_addr, its a string
    serv_addr.sin_port = htons(portno);//network addr portnumber
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) //
        error("ERROR connecting");
    printf("Please enter the message: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);//get message
    n = write(sockfd,buffer,strlen(buffer));//write message to the socket
    if (n < 0) 
        error("ERROR writing to socket");
    bzero(buffer,256);
    n = read(sockfd,buffer,255);//read from socket
    if (n < 0) 
        error("ERROR reading from socket");
    printf("%s\n",buffer);
    close(sockfd);//close socket
    return 0;
}
