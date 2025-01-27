#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
int main()
{
	int port;
	int sock_fd;
	char msg[200];
	struct sockaddr_in server_addr;
	printf("Enter Port: ");
	scanf("%d",&port);
	
	
	sock_fd=socket(AF_INET,SOCK_DGRAM,0);
	if(sock_fd<0)
	{
		printf("Error creating socket.\n");
		exit(1);
	}
	else
		printf("Socket Created.\n");	
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(port);
	server_addr.sin_addr.s_addr=inet_addr("192.168.0.130");

	int connect_flag;
	connect_flag=connect(sock_fd,(struct sockaddr *)&server_addr, sizeof(server_addr));
	if(connect_flag<0)
	{
		printf("Error connecting.\n");
		close(sock_fd);
		exit(1);
	}	
	else
		printf("Connected\n\n");
	int flag=1;
	while(flag==1)
	{
	   	char message[20];
		memset(message, 0, sizeof(message));
		printf("client: ");
		scanf("%s",message);
		if(strcmp(message,"quit")==0)
		{
			flag=0;
			sendto(sock_fd, message, strlen(message), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
			break;
		}	
		if (sendto(sock_fd, message, strlen(message), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) 
		{
			perror("Send failed");
			close(sock_fd);
			exit(1);
		}
		recvfrom(sock_fd, msg, sizeof(msg), 0, NULL, NULL);
		printf("Server: %s\n",msg);

	}
	close(sock_fd);
    	printf("\nConnection closed.\n");
}
