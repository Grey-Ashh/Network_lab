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
	int sock_fd,newsockfd;
	struct sockaddr_in server_addr, client_addr;
	socklen_t client_len;
	char buffer[256];

	sock_fd=socket(AF_INET,SOCK_STREAM,0);
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

	if(listen(sock_fd,5)<0)
	{
		printf("Error listening.\n");
		close(sock_fd);
		exit(1);
	}	
	else
		printf("Listening ok\n");	

	client_len = sizeof(client_addr);
	newsockfd = accept(sock_fd, (struct sockaddr *)&client_addr, &client_len);
	if (newsockfd < 0) 
	{
        	printf("Error accepting connection");
        	close(sock_fd);
        	exit(1);
    	}
	else
		printf("Connected\n");

	memset(buffer, 0, sizeof(buffer));
	
	read(newsockfd,buffer,sizeof(buffer));

	printf("Client: %s\n", buffer);

   	char *response = "Message received!";
	write(newsockfd, response, sizeof(response));
	//if (write(newsockfd, response, strlen(response)) < 0) 
	//{
	//        perror("Error sending response to client");
	//        close(newsockfd);
	//        close(sock_fd);
	//        exit(1);
	//}
    	close(newsockfd);  
   	close(sock_fd);    
    	printf("Connection closed.\n");


}
