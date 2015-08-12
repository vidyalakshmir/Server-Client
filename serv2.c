/*
 ============================================================================
 Name        : serv2.c
 Description : An one-process-per-request TCP server program that runs on the local machine with port 8089. The server maintains 
			   a counter: each time a client connects to the server, the server increments it counter, and returns its value 
			   to the client.
 ============================================================================
 */


#include<sys/types.h>
#include<string.h>
#include<sys/socket.h>
#include<stdio.h>
#include<stdlib.h>
#include<netdb.h>
#include<arpa/inet.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h> 
#include <signal.h>

union semun
{
  int val;
  struct semid_ds *buf;
  unsigned short *array;
  struct seminfo *__buf;
};

int main()
{
        int sockdesc,status;
        struct addrinfo hints;
        struct addrinfo *servinfo;
	/********** Initialization and creation of semaphores for synchronization ********/
	  int shmid,my_sem;
	  union semun arg;
	  struct semid_ds sem_buf;
	  static ushort sem_array[1] = { 1 };
	  struct sembuf up = { 0, 1, 0 };	//Defining the up operation on the semaphore
	  struct sembuf down = { 0, -1, 0 };	//Defining the down operation on the semaphore
	  my_sem = semget (IPC_PRIVATE, 1, 0600);	//Creating the semaphore
	  arg.buf = &sem_buf;
	  arg.array = sem_array;
	  semctl (my_sem, 0, SETALL, arg);	//Setting the value of the semaphore

	shmid = shmget(IPC_PRIVATE, sizeof(int), 0600); //Creating 
	int *counter = (int *) shmat(shmid, 0, 0);
	*counter = 0;

          
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
        freeaddrinfo(servinfo);
        /********** Listen call **************/
        int listendesc = listen(sockdesc, 10);
        if(listendesc == -1)
        {
                printf("\nError while trying to listen\n");
                exit(1);
        }
        
        /********** Communicating phase **********/
        while(1)
        {
                /*********** Accepting the connections **********/
                struct sockaddr_storage client_addr;
                socklen_t addr_size = sizeof(client_addr);
                int client_sockdesc;
                client_sockdesc = accept(sockdesc, (struct sockaddr*) & client_addr, &addr_size);
		if(!fork())		//child process for handling the requests
		{
			int count,temp;			
			semop (my_sem, &down, 1);                	
	        	*counter = *counter + 1;
			count = *counter;
			semop (my_sem, &up, 1);
			temp=htonl(count);
			int sendflag = send(client_sockdesc, &temp, sizeof(temp), 0);
	                if(sendflag == -1)
        	        {
        	                printf("\nError in sending data\n");
		                exit(1);
        	        }
			exit(0);
		}
                close(client_sockdesc);                     //parent process does not require the connection socket anymore
         }
        close(sockdesc);
}
