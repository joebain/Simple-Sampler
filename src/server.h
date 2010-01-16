#ifndef _SERVER_H_
#define _SERVER_H_

#include <list>
#include <map>

#include "jack.h"
#include "sound_maker.h"
#include "sample.h"
#include "synth.h"
#include "loader.h"
#include "event.h"

class Server {
	private:
		bool running;
	
		Jack* jack;

		std::list<SoundMaker*> sound_makers;
		std::list<Synth> synths;
		std::list<Sample> samples;
		
		Sample* last_played_or_playing;
		
		std::list<Pad> pads;
		
		std::map<int, Pad*> events_to_pads;
		std::map<int, Sample*> pads_to_samples;
		
		bool remove_sound_maker(SoundMaker* sound_maker);
		bool add_sound_maker(SoundMaker* sound_maker);
	public:
		Server();
		~Server();
		bool start();
		bool stop();
		
		void midi_on(int event_number, int velocity);
		void midi_off(int event_number);
		void pitch_bend(int on, int value);
		void controller_change(int controller_number, int value);
		
		Jack* get_jack() {return jack;}
		
		void get_frames(float frames[], int length);

		bool add_sample(Sample sample);
		bool add_samples(std::list<Sample> sample);
		bool remove_sample(Sample sample);
		
		SoundMaker* get_sound_maker(int id);
		
		bool add_synth(Synth synth);
		bool remove_synth(Synth synth);
		
		void set_pads(std::list<Pad> pads);
		std::list<Pad> & get_pads() { return pads; }
};

#endif
