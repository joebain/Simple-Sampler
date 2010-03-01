#include "pad.h"

#include <iostream>

Pad::Pad(int event_number, int id) {
	this->event_number = event_number;
	this->id = id;
	start_position = 0.0f;
	end_position = 1.0f;
	has_a_sample = false;
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

void Pad::release() {
	if (!has_a_sample) return;
	
	PadEvent e;
	e.start_position = start_position;
	e.end_position = end_position;
	e.on = false;
	e.pad_id = id;
	
	sample->give_event(e);
}
