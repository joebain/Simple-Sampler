#include <pthread.h>
#include <iostream>
#include <stdlib.h>

#include "server.h"
#include "client.h"

Server* server;
Client* client;

void* start_server(void* args) {
	server->start();
	
	return NULL;
}

void* start_client(void* args) {
	client->start();
	
	return NULL;
}

int main (int argc, char *argv[]) {
	system("pwd");
	
	server = new Server();
	client = new Client(server);

	pthread_t thread1, thread2;

	pthread_create( &thread1, NULL, start_server, NULL);
	pthread_create( &thread2, NULL, start_client, NULL);

	pthread_join( thread1, NULL);
    pthread_join( thread2, NULL);
	
	return 0;
}
