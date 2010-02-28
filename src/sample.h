#ifndef _SAMPLE_H_
#define _SAMPLE_H_

#include <sndfile.h>
#include <string>
#include <list>
#include <map>
#include <rubberband/RubberBandStretcher.h>

#include "sound_maker.h"
#include "pad_event.h"

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
		
		std::list<PadEvent> events;
		
		std::string filename;
		std::string name;
		float stopping_at;
		float last_play_from;
		
		float base_speed;
		float playing_speed;
		float base_pitch;
		float playing_pitch;
		
		bool try_add_event(PadEvent e);
		void blank(const float *const * frames, int length);
		void print_events();
		
		bool play();
		bool play(float position);
		bool stop();
		
		bool reset_required;
	public:
		Sample();
		~Sample();
		
		bool load(std::string filename);
		
		bool give_event(PadEvent e);
		
		void set_looping(bool looping) {this->looping = looping; }
		bool is_looping() {return looping;}
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
};

#endif // _SAMPLE_H_
