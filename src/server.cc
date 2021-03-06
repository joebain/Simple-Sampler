#include "server.h"

#include <list>
#include <iostream>
#include <math.h>

#include "jack.h"
#include "sample.h"

Server::Server() {
	jack = new Jack();
	bit_effect = new BitEffect(1);
	add_bit_effect(*bit_effect);
    samples_to_pads_lock = 0;
}

Server::~Server() {
	
}

bool Server::start() {
	jack->connect();
	outbound_frames = new float[jack->buffer_size()];
    
	running = true;
	
	while (running) {
		usleep(10);
        update();
	}
	
	return true;
}

void Server::update() {
    bool refresh_sample_pad_links = false;
	for (std::list<Pad>::iterator pad = pads.begin() ;
        pad != pads.end() ; ++pad)
    {
        if (pad->has_sample_changed()) {
            std::cout << "sample has changed: " << pad->get_sample()->get_name() << std::endl;
            refresh_sample_pad_links = true;
        }
    }
    if (refresh_sample_pad_links) {
        link_samples_to_pads();
    }
}

bool Server::stop() {
	running = false;
	
	jack->close();
	
	delete outbound_frames;

	return true;
}

bool Server::add_sample(Sample sample) {
	samples.push_back(sample);
	
	std::cout << "added a sample, name: " << sample.get_filename() << std::endl;
	
	samples.back().set_effect(bit_effect);
	
	return add_sound_maker(&samples.back());
}

bool Server::add_sound_maker(SoundMaker* sound_maker) {
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

int Server::get_new_sample_id() {
    int id = 0;
    for (std::list<Sample>::iterator sample = samples.begin(); sample != samples.end() ; ++sample) {
        if (sample->id >= id) {
            id = sample->id + 1;
        }
    }
    return id;
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
	
	if (outbound_frames == NULL) return;
	
	for (int i = 0; i < length ; i++) {
		outbound_frames[i] = 0.0f;
	}
		
	for (std::list<SoundMaker*>::iterator sound_maker = sound_makers.begin() ;
			sound_maker != sound_makers.end() ; ++sound_maker) {
		if ((*sound_maker)->is_playing()) {
			(*sound_maker)->next_frames(outbound_frames, length);
			for (int a = 0; a < length ; a++) {
				frames[a] += outbound_frames[a];
			}	
		}
	}
}

void Server::take_frames(float frames[], int length) {
    for (std::list<Sample>::iterator sample = samples.begin() ;
			sample != samples.end() ; ++sample) {
        if (sample->is_recording()) {
            sample->record_frames(frames, length);
        }
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
	pad->hit(velocity);
	last_played_or_playing = pad->get_sample();
	/*
	PadEvent e;
	e.pad_id = pad->get_id();
	e.velocity = velocity;
	e.on = true;
	
	std::map<int,Sample*>::iterator si
		= pads_to_samples.find(pad->get_id());
	
	if (si == pads_to_samples.end())
		return;
	
	si->second->give_event(e);
	last_played_or_playing = si->second;
	*/
	
}

void Server::midi_off(int event_number) {
	std::map<int,Pad*>::iterator pi
		= events_to_pads.find(event_number);
	
	if (pi == events_to_pads.end())
		return;
	
	Pad* pad = pi->second;
	
	pad->release();
	/*
	Event e;
	e.pad_id = pad->get_id();
	e.on = false;
	
	std::map<int,Sample*>::iterator si
		= pads_to_samples.find(pad->get_id());
	
	if (si == pads_to_samples.end())
		return;
		
	si->second->give_event(e);
	*/
}

void Server::pitch_bend(int on, int value) {
	float exp = (float) (value-63) / 64.0f; //-1 to 1
	float pitch = pow(2, exp);
	if (last_played_or_playing) {
		if (on)
			last_played_or_playing->set_pitch(pitch);
		else
			last_played_or_playing->reset_pitch();
	}
	//std::cout << "pitch val: " << std::dec << value << " or " << pitch << std::endl;
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
	pads.clear();
	for (std::list<Pad>::iterator pi = new_pads.begin() ;
			pi != new_pads.end() ; ++pi) {
		add_pad(*pi);
	}
}

void Server::add_pad(Pad pad) {
	pads.push_back(pad);
	events_to_pads[pad.get_event_number()] = &(pads.back());
    std::cout << "added a pad: " << pad.get_event_number() << std::endl;
}

bool Server::add_bit_effect(BitEffect effect) {
	bit_effects.push_back(effect);
	
	return add_effect(&bit_effects.back());
}

bool Server::add_bit_effects(std::list<BitEffect> new_effects) {
	for (std::list<BitEffect>::iterator i = new_effects.begin() ;
		i != new_effects.end(); ++i)
	{
		if (!add_bit_effect(*i)) return false;
	}
	
	return true;
}

bool Server::remove_bit_effect(BitEffect effect) {
	uint size = bit_effects.size();
	
	for (std::list<BitEffect>::iterator e = bit_effects.begin() ;
		e != bit_effects.end() ; ++e)
	{
		if (e->equal(&effect)) {
			bit_effects.erase(e);
			break;
		}
	}

	return size > bit_effects.size() && remove_effect(&effect);
}

bool Server::remove_effect(Effect* effect) {
	uint size = effects.size();
	
	for (std::list<Effect*>::iterator e = effects.begin() ;
		e != effects.end() ; ++e)
	{
		if ((*e)->equal(effect)) {
			effects.erase(e);
			break;
		}
	}

	return size > effects.size();
}

bool Server::add_effect(Effect* effect) {
	effects.push_back(effect);
	return true;
}

void Server::link_samples_to_pads() {
    
    ++samples_to_pads_lock;
    std::cout << "lock: " << samples_to_pads_lock << std::endl;
    samples_to_pads.clear();
    for (std::list<Sample>::iterator sample = samples.begin() ;
        sample != samples.end() ; ++sample)
    {
        samples_to_pads[sample->id] = *(new std::list<Pad*>());
        for (std::list<Pad>::iterator pad = pads.begin() ;
            pad != pads.end() ; ++pad)
        {
            if (pad->sample_is_set()) {
                if (pad->get_sample()->id == sample->id) {
                    samples_to_pads[sample->id].push_back(&(*pad));
                }
            }
        }
    }
    --samples_to_pads_lock;
}

std::list<Pad*> Server::get_pads_for_sample(Sample* sample) {
    
    ++samples_to_pads_lock;
    //std::cout << "lock: " << samples_to_pads_lock << std::endl;
    return samples_to_pads[sample->id];
    --samples_to_pads_lock;
}
