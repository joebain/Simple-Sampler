//#include "main.h"

#include <pthread.h>
#include <iostream>
#include <stdlib.h>
#include <string>

#include <gtkmm.h>

#include "server.h"
#include "client.h"
#include "gui_client.h"

Client* client;
Server* server;

void* start_server(void* args) {
	server->start();
	std::cout << "done server" << std::endl;
	return NULL;
}

void* start_client(void* args) {
	client->start();
	std::cout << "done client" << std::endl;
	return NULL;
}

void* run_client(void* args) {
	client->running = true;
	while (client->running) {
		client->update();
		sleep(0.01); //just be reasonable now
	}
	std::cout << "stopped client" << std::endl;
	return NULL;
}

int main (int argc, char *argv[]) {
	server = new Server();
	
	//client = new Client(server);
	
	//for some reason it doesn't work calling init_gtk ??
	//GuiClient::init_gtk(argc, argv);
	Gtk::RC::add_default_file("data/themes/Elegant Brit/gtk-2.0/gtkrc");
	Gtk::Main kit(argc, argv);
	
	
	client = new GuiClient(server);
	
	pthread_t thread1, thread2;//, thread3;

	pthread_create( &thread1, NULL, start_server, NULL);
	pthread_create( &thread2, NULL, start_client, NULL);
	//pthread_create( &thread3, NULL, run_client, NULL);

	pthread_join( thread1, NULL);
	pthread_join( thread2, NULL);
	//pthread_join( thread3, NULL);
	
	return 0;
}
