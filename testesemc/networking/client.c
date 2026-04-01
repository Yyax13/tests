#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <ip> <port>\n", argv[0]);
        return 1;

    }

    printf("Starting connection with %s\n", argv[1]);

    int sockFd;
    sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFd < 0) {
        perror("Can't open socket");
        return 1;

    }

    struct sockaddr_in targetAddr;
    targetAddr.sin_family = AF_INET;
    targetAddr.sin_addr.s_addr = inet_addr(argv[1]);
    targetAddr.sin_port = htons((short)atoi(argv[2]));

    int connTarget = connect(sockFd, (struct sockaddr *)&targetAddr, sizeof targetAddr);
    if (connTarget < 0) {
        perror("Can't connect");
        return 1;

    }

    while (1) {
        char cmd[256];
        memset(cmd, 0, sizeof(cmd));
        
        printf("-> ");
        fflush(stdout);

        fgets(cmd, sizeof(cmd), stdin);
        cmd[strcspn(cmd, "\n")] = '\0';

        int sendMsg = send(sockFd, cmd, strlen(cmd), 0);
        if (sendMsg < 0) {
            perror("Can't send");
            return 1;

        }

        char recvBuff[2048];
        memset(recvBuff, 0, sizeof(recvBuff));

        int recvMsg = recv(sockFd, recvBuff, sizeof(recvBuff), 0);
        if (recvMsg == 0) {
            printf("Connection closed.\nGoodbye...\n\n");
            return 0;

        } else if (recvMsg < 0) {
            perror("Can't recv");
            return 1;

        }           
        
        printf("unknown_server said \"%s\"\n", recvBuff);

    }
    
    close(sockFd);
    return 0;

}
