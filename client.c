/*
 ============================================================================
 Name        : client.c
 Description : A client program which would connect to a TCP server running on port 8089 at machine whose address is provided 
			   during runtime. The client prints the information send from the server on the screen.
 ============================================================================
 */

#include<sys/types.h>
#include<string.h>
#include<sys/socket.h>
#include<stdio.h>
#include<stdlib.h>
#include<netdb.h>
#include<arpa/inet.h>

int main(int argc, char *argv[])
{
        int status;
        struct addrinfo hints;
        struct addrinfo *servinfo, *p;
        char ipstr[INET6_ADDRSTRLEN];
        
        memset(&hints,0,sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        
        status = getaddrinfo(argv[1],"8089",&hints, &servinfo);        
        if(status!=0)
        {
                printf("\n%s\n",gai_strerror(status));
        }
    /************ Creating the socket *************/
    int sockdesc = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    
    /*********** Connecting to the server socket *********/
    int conndesc = connect(sockdesc, servinfo->ai_addr, servinfo->ai_addrlen);
    if(conndesc == -1)
    {
        printf("\nError while connecting to the server");
        exit(1);
    }
    int counter;
    int recvflag = recv(sockdesc,&counter, sizeof(int),0);
    if(recvflag == -1)
    {
        printf("\nError in receiving");
        exit(1);
    }
    printf("I received: %d\n",ntohl(counter));
    close(sockdesc);
    freeaddrinfo(servinfo); 
    return 0;
}
