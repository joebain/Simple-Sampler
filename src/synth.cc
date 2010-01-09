#include "synth.h"

#include <iostream>
#include <math.h>

Synth::Synth(std::string name) {
	playing = false;
	period = 0;
	this->name = name;
	
	//freq = 0.00272526639f;
	  freq = 0.00172526639f;
}

bool Synth::play() {
	playing = true;
	period = 0;
	
	std::cout << "playing synth " << name << std::endl;
	
	return true;
}

bool Synth::stop() {
	playing = false;
	
	return true;
}

void Synth::next_frames(float frames[], int length) {
	if (playing) {
		for (int i = 0; i < length; i++) {
			frames[i] = sin(period * M_PI * 2);
			period += freq;
		}
	}
}
