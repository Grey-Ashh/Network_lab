#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
int main()
{
	int sock_fd;
	struct sockaddr_in server_addr, client_addr;
	socklen_t client_len;
	char buffer[256];

	sock_fd=socket(AF_INET,SOCK_DGRAM,0);
	if(sock_fd<0)
	{
		printf("Error creating socket.\n");
		exit(1);
	}
	else
		printf("Socket created\n");	
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(8080);
	server_addr.sin_addr.s_addr=INADDR_ANY;

	if(bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr))<0)
	{
		printf("Error binding.\n");
		close(sock_fd);
		exit(1);
	}	
	else
		printf("Binded\n");	
	printf("Listening ok\n");	
	client_len = sizeof(client_addr);
	int flag=1;
	while(flag==1)
	{
		memset(buffer, 0, sizeof(buffer));
		
		recvfrom(sock_fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &client_len);
		if(strcmp(buffer,"quit")==0)
		{
			break;
		}
		printf("Client: %s\n", buffer);

	   	char response[20];
		printf("server: ");
		scanf("%s",response);	
		 sendto(sock_fd, response, strlen(response), 0, (struct sockaddr *)&client_addr, client_len);

	}	 
   	close(sock_fd);    
    	printf("\nConnection closed.\n");


}
