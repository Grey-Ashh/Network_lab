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
	char *msg[20];
	struct sockaddr_in server_addr;
	printf("Enter Port: ");
	scanf("%d",&port);
	
	
	sock_fd=socket(AF_INET,SOCK_STREAM,0);
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
		printf("Connected\n");
	

	char *message = "Hello, server!";
	if (send(sock_fd, message, strlen(message), 0) < 0) 
	{
        	perror("Send failed");
	        close(sock_fd);
        	exit(1);
	}
	else
		printf("\n");
	recv(sock_fd,msg,sizeof(msg),0);
	printf("Server: %s\n",msg);
	
	close(sock_fd);
    	printf("Connection closed.\n");
}
