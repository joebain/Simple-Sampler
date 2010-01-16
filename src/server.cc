#include "server.h"

#include <list>
#include <iostream>

#include "jack.h"
#include "sample.h"

Server::Server() {
	jack = new Jack();
}

Server::~Server() {
	
}

bool Server::start() {
	jack->connect();
	
	running = true;
	
	while (running) {
		sleep(1);
	}
	
	return true;
}

bool Server::stop() {
	running = false;
	
	jack->close();

	return true;
}

bool Server::add_sample(Sample sample) {
	samples.push_back(sample);
	std::list<int> pad_ids = sample.get_pad_ids();
	std::list<int>::iterator pad_id = pad_ids.begin();
	while (pad_id != pad_ids.end()) {
		pads_to_samples[*pad_id] = &(samples.back());
		++pad_id;
	}
	std::cout << "added a sample, name: " << sample.get_filename() << std::endl;
		
	return add_sound_maker(&samples.back());
}

bool Server::add_sound_maker(SoundMaker* sound_maker) {
	sound_maker->id = sound_makers.size();
	sound_makers.push_back(sound_maker);
	
	std::cout << "added a sound maker, id: " << sound_maker->id << std::endl;
	
	return true;
}

bool Server::add_samples(std::list<Sample> new_samples) {
	std::list<Sample>::iterator i = new_samples.begin();
	
	while (i != new_samples.end()) {
		if (!add_sample(*i)) return false;
		++i;
	}
	
	return true;
}

bool Server::remove_sound_maker(SoundMaker* sound_maker) {
	uint size = sound_makers.size();

	std::list<SoundMaker*>::iterator i = sound_makers.begin();

	while (i != sound_makers.end()) {
		if ((*i)->equal(sound_maker)) {
			sound_makers.erase(i);
			break;
		}
		++i;
	}

	if (size > sound_makers.size())
		return true;
	else
		return false;
}

bool Server::remove_sample(Sample sample) {
	uint size = sound_makers.size();

	//remove from pads to samples map
	std::map<int,Sample*>::iterator p2s = pads_to_samples.begin();
	while (p2s != pads_to_samples.end()) {
		if (p2s->second->equal(&sample)) {
			pads_to_samples.erase(p2s);
			break;
		}
		++p2s;
	}

	//remove from samples list
	std::list<Sample>::iterator s = samples.begin();
	while (s != samples.end()) {
		if (s->equal(&sample)) {
			samples.erase(s);
			break;
		}
		++s;
	}

	if (size > samples.size() && remove_sound_maker(&sample))
		return true;
	else
		return false;
}

bool Server::add_synth(Synth synth) {
	synths.push_back(synth);
	std::cout << "added a synth, name: " << synth.get_name() << std::endl;
	
	return add_sound_maker(&synths.back());
}

bool Server::remove_synth(Synth synth) {
	uint size = synths.size();

	std::list<Synth>::iterator i = synths.begin();

	while (i != synths.end()) {
		if (i->equal(&synth)) {
			synths.erase(i);
			break;
		}
		++i;
	}

	if (size > synths.size() && remove_sound_maker(&synth))
		return true;
	else
		return false;
}

void Server::get_frames(float frames[], int length) {
	
	std::list<SoundMaker*>::iterator sound_maker = sound_makers.begin();
	float gotten_frames[length];
	while (sound_maker != sound_makers.end()) {
		if ((*sound_maker)->is_playing()) {
			(*sound_maker)->next_frames(gotten_frames, length);
			for (int a = 0; a < length ; a++) {
				frames[a] += gotten_frames[a];
			}	
		}
		++sound_maker;
	}
}

SoundMaker* Server::get_sound_maker(int id) {
	std::list<SoundMaker*>::iterator sound_maker = sound_makers.begin();
	while (sound_maker != sound_makers.end()) {
		if ((*sound_maker)->id == id)
			return *sound_maker;
		++sound_maker;
	}
	return NULL;
}

void Server::midi_on(int event_number, int velocity) {
	std::map<int,Pad*>::iterator pi
		= events_to_pads.find(event_number);
	
	if (pi == events_to_pads.end())
		return;
	
	Pad* pad = pi->second;
	
	Event e;
	e.pad_id = pad->get_id();
	e.velocity = velocity;
	e.on = true;
	
	std::map<int,Sample*>::iterator si
		= pads_to_samples.find(pad->get_id());
	
	if (si == pads_to_samples.end())
		return;
		
	si->second->give_event(e);
	last_played_or_playing = si->second;
}

void Server::midi_off(int event_number) {
	std::map<int,Pad*>::iterator pi
		= events_to_pads.find(event_number);
	
	if (pi == events_to_pads.end())
		return;
	
	Pad* pad = pi->second;
	
	Event e;
	e.pad_id = pad->get_id();
	e.on = false;
	
	std::map<int,Sample*>::iterator si
		= pads_to_samples.find(pad->get_id());
	
	if (si == pads_to_samples.end())
		return;
		
	si->second->give_event(e);
}

void Server::pitch_bend(int on, int value) {
	float pitch = (float) (value+1) / 64.0f; //we can't set the pitch to 0 anyway
	std::cout << "pitch bend, val " << std::dec << value << " or " << pitch << std::endl;
	if (last_played_or_playing) {
		if (on)
			last_played_or_playing->set_pitch(pitch);
		else
			last_played_or_playing->reset_pitch();
	}
}

void Server::controller_change(int controller_number, int value) {
	float speed = (float) (value+1) / 64.0f;
	//std::cout << "speed bend, val " << std::dec << value << " or " << speed << std::endl;
	/*
	if (last_played_or_playing)
		last_played_or_playing->set_speed(speed);
	*/
}

void Server::set_pads(std::list<Pad> new_pads) {
	std::list<Pad>::iterator pad = new_pads.begin();
	pads.clear();
	while(pad != new_pads.end()) {
		pads.push_back(*pad);
		events_to_pads[pad->get_event_number()] = &(pads.back());
		++pad;
	}
}
