#include "client.h"

#include <iostream>

#include "server.h"
#include "loader.h"

Client::Client(Server* server) {

	this->server = server;
}

void Client::start() {
	
	while (!server->get_jack()->is_ready()) {
		sleep(0.2);
	}
	server->get_jack()->start();
	
}

void Client:: stop() {
	running = false;
}

void Client::update() {
	//nothing yet
}

void Client::load_pads(std::string filename) {
	std::list<Pad> pads;
	Loader::load_controller_config(filename,pads);
	server->set_pads(pads);
}

bool Client::save_pads(std::string filename) {
	std::list<Pad> & pads = server->get_pads();
	return Loader::save_controller_config(filename, pads);
}

bool Client::load_samples(std::string filename) {
	std::list<Sample> samples;
	if (Loader::load_samples_from_file(filename,samples) <= 0) return false;
	return server->add_samples(samples);
}

bool Client::load_sample(std::string filename) {
	Sample sample;
	if (!Loader::load_sample(filename, sample)) return false;
	return server->add_sample(sample);
}

bool Client::link_pads_to_samples(std::string filename) {
	std::list<Pad> & pads = server->get_pads();
	std::list<Sample> & samples = server->get_samples();
	return Loader::link_pads_to_samples(filename, pads, samples);
}

bool Client::save_samples(std::string filename) {
	std::list<Sample> & samples = server->get_samples();
	return Loader::save_sample_file(filename, samples);
}
