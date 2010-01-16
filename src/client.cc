#include "client.h"

#include <iostream>

#include "server.h"
#include "loader.h"

Client::Client(Server* server) {

	this->server = server;
}

void Client::start() {
	/*
	
		
	Synth synth1("sine synth");
	server->add_synth(synth1);
	
	
	*/
	while (!server->get_jack()->is_ready()) {
		sleep(0.2);
	}
	server->get_jack()->start();
	
	//SoundMaker* sample = server->get_sound_maker(0);
	//SoundMaker* synth = server->get_sound_maker(1);
	//sample->play();
	//synth->play();
	
	//~ while(true) {
		//~ sleep(1);
	//~ }
}

void Client:: stop() {
	
}

void Client::update() {
	
}

void Client::load_pads(std::string filename) {
	std::list<Pad> pads;
	Loader::load_controller_config(filename,pads);
	server->set_pads(pads);
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
