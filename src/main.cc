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

int main (int argc, char *argv[]) {
	server = new Server();
	
	//client = new Client(server);
	
	//for some reason it doesn't work calling init_gtk ??
	//GuiClient::init_gtk(argc, argv);
	Gtk::RC::add_default_file("data/themes/Elegant Brit/gtk-2.0/gtkrc");
	Gtk::Main kit(argc, argv);
	
	client = new GuiClient(server);
	
	pthread_t thread1, thread2;

	pthread_create( &thread1, NULL, start_server, NULL);
	pthread_create( &thread2, NULL, start_client, NULL);

	pthread_join( thread1, NULL);
	pthread_join( thread2, NULL);
	
	return 0;
}
