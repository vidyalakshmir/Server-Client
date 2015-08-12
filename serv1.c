/*
 ============================================================================
 Name        : serv1.c
 Description : An iterative TCP server program that runs on the local machine with port 8089. The server maintains a 		
			   counter: each time a client connects to the server, the server increments it counter, and returns its value to the 	
			   client.
 ============================================================================
 */

#include<sys/types.h>
#include<string.h>
#include<sys/socket.h>
#include<stdio.h>
#include<stdlib.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<unistd.h>
int main()
{
        int sockdesc,status;
        struct addrinfo hints;
        struct addrinfo *servinfo;
          
        /********** Getting server IP address into structure *************/
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;
        status = getaddrinfo(NULL,"8089",&hints,&servinfo);
        if(status!=0)
        {
                printf("\nError getting address : %s\n",gai_strerror(status));
                exit(1);
        }
           
        /************* Creating a socket ****************/
        sockdesc = socket(servinfo->ai_family,servinfo->ai_socktype,servinfo->ai_protocol);
        if(sockdesc == -1)
        {
                printf("\nError in creating sockets\n");
                exit(1);
        }
        
        /******* Binding the socket to a port **********/
        int binddesc = bind(sockdesc, servinfo->ai_addr, servinfo->ai_addrlen);
        if(binddesc == -1)
        {
                printf("\nError in binding the socket to the port\n");
                exit(1);
        }
        
        /********** Listen call **************/
        int listendesc = listen(sockdesc, 10);
        if(listendesc == -1)
        {
                printf("\nError while trying to listen\n");
                exit(1);
        }
        
        /********** Communicating phase **********/
        int counter = 1;
        while(1)
        {
                /*********** Accepting the connections **********/
                struct sockaddr_storage client_addr;
                socklen_t addr_size = sizeof(client_addr);
                int client_sockdesc;
                client_sockdesc = accept(sockdesc, (struct sockaddr*) & client_addr, &addr_size);
                int sendflag = send(client_sockdesc, &counter, sizeof(counter), 0);
                if(sendflag == -1)
                {
                        printf("\nError in sending data\n");
                        exit(1);
                }
                counter++;
                close(client_sockdesc);                     
         }
        freeaddrinfo(servinfo);
        close(sockdesc);
}
