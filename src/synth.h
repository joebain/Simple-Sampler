#ifndef _SYNTH_H_
#define _SYNTH_H_

#include <string>

#include "sound_maker.h"

class Synth : public SoundMaker {
	public:
		Synth(std::string name);
		bool play();
		bool stop();
		
		bool is_playing() { return playing; }
		void next_frames(float frames[], int length);
		float next_frame();
		
		std::string get_name() { return name; }
	private:
		bool playing;
		float period;
		std::string name;
		float freq;
};

#endif
