#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <time.h>

void resend_message(int ch, int c_sock) {
    char buff[60];
    snprintf(buff, sizeof(buff), "reserver message :%d", ch);
    printf("Resending Message to client: %s \n", buff);
    write(c_sock, buff, strlen(buff) + 1);
    usleep(1000);
}

int main() {
    int s_sock, c_sock;
    struct sockaddr_in server, other;
    socklen_t add;
    char buff[50];
    fd_set set1;
    struct timeval timeout1;

    s_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (s_sock == -1) {
        perror("Socket creation failed");
        return -1;
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(9009);
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(s_sock, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("Binding failed");
        close(s_sock);
        return -1;
    }

    printf("\tServer Up\nSelective repeat scheme\n\n");
    listen(s_sock, 10);
    add = sizeof(other);
    c_sock = accept(s_sock, (struct sockaddr *)&other, &add);

    int tot = 0;
    while (tot < 9) {
        for (int j = tot; j < tot + 3; j++) {
            snprintf(buff, sizeof(buff), "server message :%d", j);
            printf("Message sent to client: %s\n", buff);
            write(c_sock, buff, strlen(buff) + 1);
            usleep(1000);
        }

        for (int k = tot; k < tot + 3; k++) {
            FD_ZERO(&set1);
            FD_SET(c_sock, &set1);
            timeout1.tv_sec = 2;
            timeout1.tv_usec = 0;

            if (select(c_sock + 1, &set1, NULL, NULL, &timeout1) <= 0) {
                printf("Timeout for message: %d\n", k);
                resend_message(k, c_sock);
                continue;
            }

            read(c_sock, buff, sizeof(buff));
            printf("Message from Client: %s\n", buff);
            if (buff[0] == 'n') {
                printf("Corrupt message detected (msg %d)\n", buff[strlen(buff) - 1] - '0');
                resend_message(buff[strlen(buff) - 1] - '0', c_sock);
                continue;
            }

            tot++;
        }
    }

    close(c_sock);
    close(s_sock);
    return 0;
}

