#include "client.h"

#include <iostream>

#include "server.h"
#include "loader.h"

Client::Client(Server* server) {

	this->server = server;
}

void Client::start() {
	/*
	std::list<Sample> samples;
	Loader::load_samples_from_file("data/samples.xml",samples);
	server->add_samples(samples);
		
	Synth synth1("sine synth");
	server->add_synth(synth1);
	
	std::list<Pad> pads;
	Loader::load_controller_config("data/controller.xml",pads);
	server->set_pads(pads);
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
