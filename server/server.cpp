/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

//error message
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) //get port number
{
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    int n;
    /*
        sockfd - file descriptor on server socket
        newsockfd - file descriptor on client socket
        portno - server port number
        clilen - size client address
        n - messages in and out
    */
    char buffer[256]; //reads from socket connection into buffer
    struct sockaddr_in serv_addr, cli_addr; 
    /*
    struct definition:
        struct sockaddr_in{
            short   sin_family; // must be AF_INET 
            u_short sin_port;
            struct  in_addr sin_addr;
            char    sin_zero[8]; // Not used, must be zero 
        };
    */

    if (argc < 2) {//see if port is there
        error("ERROR no port provided");
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    //starts a socket, family is AF_INET, sock_stream is datatype, 0 makes it a protocol based on past arg, tcp for SOCK_STREAM, UDP for DATAGRAM
    if (sockfd < 0) { //socket returns 0 if it works
    error("ERROR opening socket");
    }
    bzero((char *) &serv_addr, sizeof(serv_addr)); 
    //all values in serv_addr are set to 0

    portno = atoi(argv[1]); //port is string of integers now its integer

    serv_addr.sin_family = AF_INET; //internet instead of AF_UNIX for sharing files
    serv_addr.sin_port = htons(portno);
    //htons converts host byte order network byte order
    serv_addr.sin_addr.s_addr = INADDR_ANY;//INADDR_ANY means ip addr in this 
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){//binds socket to ip
        error("ERROR on binding");
    }

    listen(sockfd, 5); //listen for conections
    
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    //accepts client connection
    if (newsockfd < 0) {//checks for acception
        error("ERROR on accept");
    }

    bzero(buffer, 256); //buffer values set to zero
    n = read(newsockfd, buffer, 255); //n set to messages
    if (n < 0){
        error("ERROR reading from socket");
    }
    cout << "the message is" << buffer;

    

    return 0;
}