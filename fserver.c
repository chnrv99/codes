#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>

void error(const char* msg){
    // inbuilt error function
    perror(msg);
    exit(1);
}

int main(int argc, char* argv[]){
    // argc will be 2, argv can have the port no
    if(argc<2){
        fprintf(stderr, "port number not provided\n");
        exit(1);
    }
    int sockfd, newsockfd, portno, n;
    char buffer[255];

    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd<0){
        error("Socket failure");
    }

    // clears any data in the var given
    bzero((char*)&serv_addr, sizeof(serv_addr));

    // atoi converts the char to int
    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if(bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr))<0){
        error("binding fail");
    }

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);

    if(newsockfd < 0){
        error("Error on accept");
    }
    
    FILE *fp;

    int ch=0;
    fp = fopen("receieved_msg.txt","w");
    int words;

    read(newsockfd, &words, sizeof(int));

    while(ch!=words){
        read(newsockfd, buffer, 255);
        fprintf(fp, "%s ", buffer);
        ch++;
    }

    printf("File receieved");

    close(newsockfd);
    close(sockfd);
    return 0;
}