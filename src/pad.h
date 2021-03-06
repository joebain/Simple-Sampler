#ifndef _PAD_H_
#define _PAD_H_

#include "sample.h"

class Pad {
	private:
		Sample* sample;
		
		int event_number;
		int id;
		
		int x,y;
		
		bool has_a_sample;
        bool sample_has_changed;
	public:
		Pad() { Pad(0,0); }
		Pad(int event_number, int id);
		
		float start_position;
		float end_position;
		
		int get_event_number() { return event_number; }
		const int get_id() const { return id; }
		void set_event_number(int event_number) { this->event_number = event_number; }
		void set_id(int id) { this->id = id; }
		
		void set_pos(int x, int y) { this->x = x; this->y = y; }
		int get_x() { return x; }
		int get_y() { return y; }
		
		bool is_playing() const;
		
		void set_sample(Sample* sample);
		Sample* get_sample() { return sample; }
        bool sample_is_set() { return has_a_sample; }
        bool has_sample_changed();
		
		void hit(float velocity);
		void release();
		
		bool operator== (const Pad & other) const;
};

#endif
