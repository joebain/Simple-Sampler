#include "pad.h"

#include <iostream>

Pad::Pad(int event_number, int id) {
	this->event_number = event_number;
	this->id = id;
	start_position = 0.0f;
	end_position = 1.0f;
	has_a_sample = false;
    sample_has_changed = false;
}

void Pad::hit(float velocity) {
	if (!has_a_sample) return;
	
	PadEvent e;
	e.start_position = start_position;
	e.end_position = end_position;
	e.on = true;
	e.velocity = velocity;
	e.pad_id = id;
	
	sample->give_event(e);
}

bool Pad::is_playing() const {
	if (!has_a_sample) return false;
	if (!sample->is_playing()) return false;
	float pos = sample->get_position();
	return pos > start_position && pos <= end_position && pos != 0 && pos != 1;
}

void Pad::set_sample(Sample* sample) {
    this->sample = sample;
    has_a_sample = true;
    sample_has_changed = true;
}

bool Pad::has_sample_changed() {
    bool tmp = sample_has_changed;
    sample_has_changed = false;
    return tmp;
}

void Pad::release() {
	if (!has_a_sample) return;
	
	PadEvent e;
	e.start_position = start_position;
	e.end_position = end_position;
	e.on = false;
	e.pad_id = id;
	
	sample->give_event(e);
}

bool Pad::operator== (const Pad & other) const {
	return id == other.get_id();
}
