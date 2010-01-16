#ifndef _SAMPLE_H_
#define _SAMPLE_H_

#include <sndfile.h>
#include <string>
#include <list>
#include <map>
#include <rubberband/RubberBandStretcher.h>

#include "sound_maker.h"
#include "pad.h"
#include "event.h"

class Sample : public SoundMaker
{
	private:
		SNDFILE* file;
		SF_INFO sfinfo;
		
		RubberBand::RubberBandStretcher* rubber_band;
		
		bool playing;
		
		int total_frames;
		int sample_rate;
		
		std::list<int> pad_ids;
		std::map<int,float> pads_to_positions;
		
		std::list<Event> events;
		
		std::string filename;
		float stopping_at;
		
		float base_speed;
		float playing_speed;
		float base_pitch;
		float playing_pitch;
		
		bool try_add_event(Event e);
		void blank(const float *const * frames, int length);
		void print_events();
	public:
		Sample();
		~Sample();
		
		bool load(std::string filename);
		bool play();
		bool play(float position);
		bool give_event(Event e);
		bool stop();
		
		bool is_playing();
		void next_frames(float frames[], int length);
		float next_frame();

		Sample & operator=(const Sample &other);
		
		std::string get_filename() { return filename; }
		
		std::list<int> get_pad_ids() { return pad_ids; }
		
		bool add_pad(int pad_id, float position);
		bool remove_pad(int pad_id);
		
		void set_speed(float new_speed);
		void reset_speed();
		void set_pitch(float new_pitch);
		void reset_pitch();
};

#endif // _SAMPLE_H_
