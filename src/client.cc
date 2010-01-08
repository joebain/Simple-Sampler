#include "client.h"

#include <iostream>

#include "server.h"
#include "loader.h"

Client::Client(Server* server) {

	this->server = server;
}

void Client::start() {
	std::vector<Sample> samples;
	Loader::load_from_file("data/test.samples",samples);
	server->add_samples(samples);
}

void Client:: stop() {
	
}

void Client::update() {
	
}
