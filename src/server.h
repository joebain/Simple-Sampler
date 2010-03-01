#ifndef _SERVER_H_
#define _SERVER_H_

#include <list>
#include <map>

#include "jack.h"
#include "sound_maker.h"
#include "sample.h"
#include "synth.h"
#include "loader.h"
#include "pad_event.h"
#include "effect.h"
#include "bit_effect.h"

class Server {
	private:
		bool running;
	
		Jack* jack;

		float* working_frames;

		std::list<SoundMaker*> sound_makers;
		std::list<Synth> synths;
		std::list<Sample> samples;
		std::list<Effect*> effects;
		std::list<BitEffect> bit_effects;
		BitEffect* bit_effect;
		Sample* last_played_or_playing;
		
		std::list<Pad> pads;
		std::map<int, Pad*> events_to_pads;
		
		bool remove_sound_maker(SoundMaker* sound_maker);
		bool add_sound_maker(SoundMaker* sound_maker);
		
		bool remove_effect(Effect* effect);
		bool add_effect(Effect* effect);
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
		std::list<Sample> & get_samples() { return samples; }
		bool remove_sample(Sample sample);
		
		bool add_bit_effect(BitEffect effect);
		bool add_bit_effects(std::list<BitEffect> effect);
		std::list<BitEffect> & get_bit_effects() { return bit_effects; }
		BitEffect* get_bit_effect() {return bit_effect;}
		bool remove_bit_effect(BitEffect effect);
		
		std::list<Effect*> get_effects() { return effects; }
		
		SoundMaker* get_sound_maker(int id);
		
		bool add_synth(Synth synth);
		bool remove_synth(Synth synth);
		
		void set_pads(std::list<Pad> pads);
		void add_pad(Pad pad);
		std::list<Pad> & get_pads() { return pads; }
};

#endif
