#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <stdlib.h>


void error (char* msg) {
	perror(msg);
	exit(1);
}



int main () {

	int client_desc;
	struct sockaddr_in server;

	client_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (client_desc < 0)
		error("Could not create client socket\n");

	memset(&server, 0, sizeof(server));
	server.sin_port = htons(2222);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (connect(client_desc, (struct sockaddr *) &server, sizeof(server)) < 0)
		error("Could not connect to the server\n");

	int size = 100;

	// Citeste stringu

	char string[100];
	printf("string: ");
	fgets (string, size, stdin);

	// Trimit marimea stringului

	size = strlen (string);
	size = htons(size);
	send(client_desc, &size, sizeof(int), 0);

	// Trimite stringul
	size = ntohs(size);
	printf("%d\n", size);
	send (client_desc, &string, sizeof(char) * size, 0);


	// Trimite characterul ce trebuie cautat

	char c;
	printf("c:");
	scanf("%c",&c);
	send (client_desc, &c, sizeof(char), 0);

	// Primeste rezultaul de la server

	int s;
	recv (client_desc, &s, sizeof(int), 0);
	s = ntohs(s);
	
	int i;
	for (i = 0; i < s; i++)
	{
		int nr;
		recv(client_desc, &nr, sizeof(int), 0);
		nr = ntohs(nr);
		printf("%d ", nr);
	}

	printf("\n");

	close(client_desc);

	return 0;
}