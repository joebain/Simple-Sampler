//#include "main.h"

#include <pthread.h>
#include <iostream>
#include <stdlib.h>
#include <string>

#include <gtkmm.h>

#include <cluttermm.h>
#include <clutter-gtkmm.h>

#include "server.h"
#include "client.h"
#include "gui_client.h"

Client* client;
Server* server;

void* start_server(void* args) {
	server->start();
	return NULL;
}

int main (int argc, char *argv[]) {
	server = new Server();
	
	//client = new Client(server);
	
	//init clutter
	Clutter::Gtk::init(&argc, &argv);
	
	//for some reason it doesn't work calling init_gtk ??
	//GuiClient::init_gtk(argc, argv);
	Gtk::RC::add_default_file("data/themes/Elegant Brit/gtk-2.0/gtkrc");
	Gtk::Main kit(argc, argv);
	
	
	client = new GuiClient(server);
	
	pthread_t thread1;

	pthread_create( &thread1, NULL, start_server, NULL);
		
	client->start();
	
	pthread_join( thread1, NULL);
			
	return 0;
}
