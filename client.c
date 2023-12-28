#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>  


void error(const char *msg) {
    perror(msg);
    exit(0);
}

int main(int argc, char* argv[]) {
    int sockfd, portno, n;
    char buffer[255]; 
    struct hostent *server;
    struct sockaddr_in serv_addr; 

    if (argc < 3) {
        fprintf(stderr, "usage %s hostname port \n", argv[0]);
        exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");

    server = gethostbyname(argv[1]);
    if (server == NULL) fprintf(stderr, "finding host error");   
    
    bzero((char*) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr_list[0], (char*) &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("Connection failed");

    while (1) {
        // write
        bzero(buffer, 255);
        fgets(buffer, 255, stdin);
        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0) error("write error");
        // read
        bzero(buffer, 255); 
        n = read(sockfd, buffer, 255);
        if (n < 0) error("read error");
        printf("server: %s", buffer);

        if (!strncmp("Bye", buffer, 3)) break; 
    }

    close(sockfd);

    return 0; 
}
