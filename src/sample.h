#ifndef _SAMPLE_H_
#define _SAMPLE_H_

#include <sndfile.h>
#include <string>
#include <list>
#include <map>
#include <rubberband/RubberBandStretcher.h>

#include "sound_maker.h"
#include "pad_event.h"
#include "effect.h"

class Sample : public SoundMaker
{
	private:
		SNDFILE* file;
		SF_INFO sfinfo;
		
		RubberBand::RubberBandStretcher* rubber_band;
		
		bool playing;
		bool looping;
		
		int total_frames;
		int sample_rate;
		
		//std::list<int> pad_ids;
		//std::map<int,float> pads_to_positions;
		
		Effect* effect;
		
		std::list<PadEvent> events;
		
		std::string filename;
		std::string name;
		float stopping_at;
		float next_stop;
		float next_start;
		float last_play_from;
		
		float base_speed;
		float playing_speed;
		float base_pitch;
		float playing_pitch;
		
		bool effect_on;
		
		void on_event(PadEvent e);
		void off_event(PadEvent e);
		bool try_add_event(PadEvent e);
		bool try_remove_event(PadEvent e);
		void blank(const float *const * frames, int length);
		void print_events();
		void print_info();
		
		bool play();
		bool play(float position);
		bool force_play(float position);
		bool stop();
		
		bool reset_required;
	public:
		Sample();
		~Sample();
		
		bool load(std::string filename);
		
		bool give_event(PadEvent e);
		
		bool sticky_loops;
						
		bool is_playing();
		void next_frames(float frames[], int length);
		float next_frame();

		Sample & operator=(const Sample &other);
		
		std::string get_filename() { return filename; }
		std::string get_name() {
			if (!name.empty()) return name;
			int start_pos = filename.find_last_of("/\\");
			int end_pos = filename.rfind('.');
			name = filename.substr(start_pos+1,end_pos - start_pos - 1);
			return name;
		}
		
		//std::list<int> get_pad_ids() { return pad_ids; }
		
		//bool add_pad(int pad_id, float position);
		//bool remove_pad(int pad_id);
		
		void set_speed(float new_speed);
		void reset_speed();
		void set_pitch(float new_pitch);
		void reset_pitch();
		
		void set_effect(Effect* effect) { this->effect = effect; }
		bool effect_enabled() { return effect_on; }
		void set_effect_on(bool value) { effect_on = value; }
};

#endif // _SAMPLE_H_
