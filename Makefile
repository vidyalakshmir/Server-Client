all: serv1 serv2 serv3 client

serv1: serv1.c 
	gcc serv1.c -o serv1

serv2: serv2.c 
	gcc serv2.c -o serv2

serv3: serv3.c 
	gcc serv3.c -o serv3

client: client.c
	gcc client.c -o client

