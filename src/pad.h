#ifndef _PAD_H_
#define _PAD_H_

#include "sample.h"

class Pad {
	private:
		Sample* sample;
		
		int event_number;
		int id;
		
		int x,y;
		
		bool has_a_sample; //for some reason the sample is not == NULL when we haven't set it
	public:
		Pad() { Pad(0,0); }
		Pad(int event_number, int id);
		
		float start_position;
		float end_position;
		
		int get_event_number() { return event_number; }
		int get_id() { return id; }
		void set_event_number(int event_number) { this->event_number = event_number; }
		void set_id(int id) { this->id = id; }
		
		void set_pos(int x, int y) { this->x = x; this->y = y; }
		int get_x() { return x; }
		int get_y() { return y; }
		
		void set_sample(Sample* sample) {
			this->sample = sample;
			has_a_sample = true;
		}
		Sample* get_sample() { return sample; }
		
		void hit(float velocity);
		void release();
};

#endif
