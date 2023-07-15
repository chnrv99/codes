#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<string.h>

void error(const char* msg){
    // inbuilt error function
    perror(msg);
    exit(1);
}

int main(int argc, char* argv[]){
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[255];
    if(argc<3){
        fprintf(stderr, "No port given");
        exit(1);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd<0){
        error("Error opening socket");
    }

    // inbuilt function, gets data from the given IP address
    server = gethostbyname(argv[1]);

    // if IP given is wrong
    if(server==NULL)
        fprintf(stderr,"Error no such host");
    
    // clearing the serv_addr
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

    // bcopy copies *server to serv_addr
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    // connect to server
    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0){
        error("Connection failed");
    }

    while(1){
        bzero(buffer, 255);
        fgets(buffer,255,stdin);
        n=write(sockfd, buffer, strlen(buffer));
        if(n<0)
            error("Error on writing");
        bzero(buffer,255);
        n=read(sockfd,buffer,255);
        if(n<0){
            error("Error in rreading");
        }
        printf("Server: %s",buffer);

        int i = strcmp("Bye", buffer);
        if(i==0)
            break;
    }
    close(sockfd);
    return 0;

}