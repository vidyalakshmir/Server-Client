DESCRIPTIONS OF PROGRAMS
Description serv1.c
	- An iterative server which listens on the local machine at port 8089.
	- It accepts a client request, sends the value of counter, increments the counter, closes the connection and then again waits to accept a client request.
	- Only one client request can be handled at a time.
	
Description serv2.c
	- A one-process-per-request server which listens on the local machine at port 8089.
	- It accepts client request, forks a process to handle that request.
	- The parent process immediately closes the new socket descriptor created for this connection and waits to accept next client request.
	- The child process would send the value of counter, increments the counter and closes the connection and exit.
	- Shared variable counter and semaphore is used to get the desired output and prevent race conditions.

Description serv3.c
	- A preforked server which listens on the local machine at port 8089.
	- It forks off 10 child processes.
	- The parent process waits till all the child processes exits.
	- Each of the child process waits to accept a client request and when a client request arrives, it sends the counter value, increments the counter. closes the connection and again waits to accept client requests.
	- Shared variable counter and semaphore is used to get the desired output and prevent race conditions.

Description client.c
	- Client which connects to a server with ipaddress/hostname provided at runtime as argument and port 8089.
	- After successfully connecting to the server, the client received an integer value, prints it, closes the connection and exits.

USAGE
	serv1.c
	Run as ./serv1

	serv2.c
	Run as ./serv2

	serv3.c
	Type ./serv3

	client.c
	Run ./client server_ip
	where server_ip is the ipaddress or name of the server.



