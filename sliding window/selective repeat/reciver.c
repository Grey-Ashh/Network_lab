#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

int is_faulty() {
    return rand() % 4 > 2;  // Simulate message corruption
}

int main() {
    srand(time(0));

    int c_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (c_sock == -1) {
        perror("Socket creation failed");
        return -1;
    }

    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(9009);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(c_sock, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("Connection failed");
        close(c_sock);
        return -1;
    }

    printf("\n\tClient - Individual Acknowledgement Scheme\n\n");

    char msg1[50] = "acknowledgement of-";
    char msg2[50];
    char msg3[50] = "negative ack-";
    char buff[100];
    int count = -1;
    int flag = 1;

    while (count < 8) {
        bzero(buff, sizeof(buff));
        bzero(msg2, sizeof(msg2));

        if (count == 7 && flag == 1) {
            flag = 0;
            read(c_sock, buff, sizeof(buff));
            continue;
        }

        int n = read(c_sock, buff, sizeof(buff));
        if (n <= 0) break;

        char i = buff[strlen(buff) - 1];
        printf("Message received from server: %s\n", buff);

        int is_fault = is_faulty();
        printf("Corruption status: %d\n", is_fault);

        if (is_fault) {
            strcpy(msg2, msg3);
        } else {
            strcpy(msg2, msg1);
            count++;
        }

        msg2[strlen(msg2)] = i;
        write(c_sock, msg2, strlen(msg2) + 1);
    }

    close(c_sock);
    return 0;
}

