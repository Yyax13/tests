#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        return 1;

    }

    printf("Starting listening at %s\n", argv[1]);

    int sockFd;
    sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFd < 0) {
        perror("Can't open socket");
        return 1;

    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons((short)atoi(argv[1]));
    
    int bindSock = bind(sockFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (bindSock < 0) {
        perror("Can't bind");
        return 1;

    }

    int listenAtSock = listen(sockFd, 20);
    if (listenAtSock < 0) {
        perror("Can't start listening");
        return 1;

    }

    int clientFd;
    struct sockaddr_in clientAddr;
    socklen_t clientLen;
    clientLen = sizeof(clientAddr);

    clientFd = accept(sockFd, (struct sockaddr*)&clientAddr, &clientLen);
    if (clientFd < 0) {
        perror("Can't accept connection");
        return 1;

    }

    char clientIP[INET6_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, sizeof(clientIP)) == NULL) {
        perror("Can't get IP Address");

    } else {
        printf("Connected with %s\n", clientIP);

    }
    
    while (1) {
        char recvBuff[2048];
        memset(recvBuff, 0, sizeof(recvBuff));

        int recvMsg = recv(clientFd, recvBuff, sizeof(recvBuff), 0);
        if (recvMsg == 0) {
            printf("Connection closed.\nGoodbye...\n\n");
            return 0;

        } else if (recvMsg < 0) {
            perror("Can't recv");
            return 1;

        }

        printf("unknown_guy said \"%s\"\n", recvBuff);
        
        char msg[256];
        memset(msg, 0, sizeof(msg));

        printf("-> ");
        fflush(stdout);

        fgets(msg, sizeof(msg), stdin);
        msg[strcspn(msg, "\n")] = '\0';

        int sendMsg = send(clientFd, msg, strlen(msg), 0);
        if (sendMsg < 0) {
            perror("Can't send");
            return 1;

        }

    }

}
