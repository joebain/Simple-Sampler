#include "pad.h"

#include <iostream>

Pad::Pad(int event_number, int id) {
	this->event_number = event_number;
	this->id = id;
}

void Pad::hit(float velocity) {
	std::cout << "pad hit, id: " << id << std::endl;
	
	if (sample == NULL) return;
	
	PadEvent e;
	e.position = position;
	e.on = true;
	e.velocity = velocity;
	
	sample->give_event(e);
}

void Pad::release() {
	if (sample == NULL) return;
	
	PadEvent e;
	e.position = position;
	e.on = false;
	
	sample->give_event(e);
}
