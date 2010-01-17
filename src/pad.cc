#include "pad.h"

#include <iostream>

Pad::Pad(int event_number, int id) {
	this->event_number = event_number;
	this->id = id;
	position = 0.0f;
	has_a_sample = false;
}

void Pad::hit(float velocity) {
	std::cout << "pad hit, id: " << id << std::endl;
	
	if (!has_a_sample) return;
	
	PadEvent e;
	e.position = position;
	e.on = true;
	e.velocity = velocity;
	e.pad_id = id;
	
	sample->give_event(e);
}

void Pad::release() {
	if (!has_a_sample) return;
	
	PadEvent e;
	e.position = position;
	e.on = false;
	e.pad_id = id;
	
	sample->give_event(e);
}
