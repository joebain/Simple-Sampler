#include "sample.h"

#include <iostream>
#include <math.h>

#include <sndfile.h>
#include <rubberband/RubberBandStretcher.h>

Sample::Sample() {
	file = 0;
	filename = "";
	playing = false;
	looping = false;
	effect_on = false;
	timestretch_on = false;
	sticky_loops = false;
	
	next_end_point = 1.0;
	next_start_point = 0.0;
	end_point = 1.0;
	start_point = 0.0;
	pad_down = false;
	base_speed = 1.0;
	playing_speed = 1.0;
	base_pitch = 1.0;
	playing_pitch = 1.0;
}

bool Sample::load(std::string filename)
{
	if (file) sf_close(file);

	this->filename = filename;
	
	if (! (file = sf_open(filename.c_str(), SFM_READ, &sfinfo))) {
    	std::cerr << "Unable to open sample file " << filename << " (" <<  sf_strerror(file) << ")" << std::endl;
		return false;
    };
	
	total_frames = sfinfo.frames;
	std::cout << "total frames " << total_frames << std::endl;
	sample_rate = sfinfo.samplerate;
	
	rubber_band = new RubberBand::RubberBandStretcher(sample_rate, 1/*channels*/,
			RubberBand::RubberBandStretcher::OptionProcessRealTime |
			RubberBand::RubberBandStretcher::OptionWindowLong );
	//rubber_band->setDebugLevel(3);
	
	return true;
}

void Sample::control() {
	if (pad_down) looping = true;
	else looping = false;
	
	float pos = get_position();
	if (pos >= end_point) {
		if (start_point == end_point) {
			stop();
		} else if (looping) {
			start_point = next_start_point;
			//end_point = next_end_point;
			
			set_position(start_point);
			
			std::cout << "looping" << std::endl;
		} else {
			stop();
			
		}
	}
}

bool Sample::play() {
	return play(0.0f);
}

void Sample::set_position(float position) {
	if (position > 1 || position < 0) return;
	int pos_i = position * total_frames;
	sf_seek(file, pos_i, SEEK_SET); //current position in the file
	
	reset_required = true; //for rubberband
}

void Sample::set_end_point(float position) {
	//if (sticky_loops) next_end_point = position;
	end_point = position;
}

void Sample::set_start_point(float position) {
	if (!looping || !playing) start_point = position;
	next_start_point = position;
}

bool Sample::request_play() {
	std::cout << "play request" << std::endl;
	//if (!playing) {
	if (next_start_point == start_point) {
		play(start_point);
		return true;
	}
	return false;
}

bool Sample::play(float position)
{	
	std::cout << "playing" << std::endl;
	set_position(position);
	playing = true;
	
	return true;
}

//~ bool Sample::force_play(float position) {
	//~ int pos = (int) (total_frames * position + 0.5);
	//~ sf_seek(file, pos, SEEK_SET);
	//~ 
	//~ reset_required = true;
	//~ 
	//~ playing = true;
	//~ looping = true;
	//~ last_play_from = position;
	//~ 
	//~ return true;
//~ }

bool Sample::give_event(PadEvent e)
{
	//an 'on' event
	if (e.on) {
		on_event(e);	
	}
	
	//an 'off' event
	else {
		if (sticky_loops) return true; //no off events with sticky loops
		off_event(e);
	}
	
	return false;
}

void Sample::on_event(PadEvent e) {
	bool no_old_events = events.size() == 0;
	
	PadEvent old_front = events.front();
	PadEvent old_back = events.back();
	
	if (!try_add_event(e)) {
		if (sticky_loops) { //treat down as an up if we havent got a down already
			off_event(e);
		} else {
			return;
		}
	}
	set_start_point(events.front().start_position);
	set_end_point(events.back().end_position);

	//here we have a new early pad so we want to try and play from there
	if (no_old_events || events.front().start_position != old_front.start_position) {
		request_play();
	}
	
	pad_down = true;
	
	print_info();
}

void Sample::off_event(PadEvent e) {
	
	try_remove_event(e);
	
	if (events.size() != 0) {
		set_start_point(events.front().start_position);
		set_end_point(events.back().end_position);
	}
	else {
		pad_down = false;
		//stop();
	}
	
	print_info();
}

bool Sample::try_add_event(PadEvent e) {
	for (std::list<PadEvent>::iterator ei = events.begin() ;
			ei != events.end() ; ++ei) {
		if (ei->start_position > e.start_position) {
			events.insert(ei,e);
			return true;
		} else if (ei->pad_id == e.pad_id) {
			return false;
		}
	}
	events.push_back(e);
	return true;
}

bool Sample::try_remove_event(PadEvent e) {
	for (std::list<PadEvent>::iterator ei = events.begin() ;
		ei != events.end() ; ++ei)
	{
		if (ei->pad_id == e.pad_id) {
			events.erase(ei);
			print_events();
			return true;
		}		
	}
	return false;
}

bool Sample::stop()
{
	print_info();

	playing = false;
	set_position(1.0);
	
	start_point = 0;
	next_start_point = 0;
	end_point = 1;
	
	std::cout << "stopping" << std::endl;

	//~ if (looping || sticky_loops) {
		//~ force_play(next_start);
		//~ stopping_at = next_stop;
		//~ std::cout << "looped sample " << filename << std::endl;
	//~ } else {
		//~ playing = false;
		//~ stopping_at = 1.0;
		//~ std::cout << "stopped playing sample " << filename << std::endl;
	//~ }
	
	return true;
}

Sample::~Sample() {
	//stop();
	
	//if (file) sf_close(file);
	//delete rubber_band;
}

Sample & Sample::operator=(const Sample &other) {
	if (this == &other)
		return *this;
	
	this->filename = other.filename;
	this->file = other.file;
	
	return *this; 
}

void Sample::set_speed(float new_speed) {
	playing_speed = new_speed;
	rubber_band->setTimeRatio(new_speed);
}

void Sample::reset_speed() {
	set_speed(base_speed);
}

void Sample::set_pitch(float new_pitch) {
	playing_pitch = new_pitch;
	rubber_band->setPitchScale(new_pitch);
}

void Sample::reset_pitch() {
	set_pitch(base_pitch);
}

float Sample::get_position() {
	int pos_i = sf_seek(file, 0, SEEK_CUR); //current position in the file
	if (pos_i != -1) {
		return (float)pos_i / (float)total_frames;
	}
	return 1.0f;
}

void Sample::next_frames(float frames[], int length) {
	
	control();
	
	if (playing) {
		
		//rubberband
		if (timestretch_on) {
			if (reset_required) {
				rubber_band->reset();
				reset_required = false;
			}
			while(rubber_band->available() < length) {
				int frames_needed_by_rb = rubber_band->getSamplesRequired();
				int frames_to_give_to_rb = length < frames_needed_by_rb ? length : frames_needed_by_rb;
				
				int frames_got_from_sf = sf_readf_float(file, frames, frames_to_give_to_rb);
				if (frames_got_from_sf != length) { //run out of file
					check_sf_errors();
					stop();
					rubber_band->retrieve(&frames, rubber_band->available());
					return;
				}
				rubber_band->process(&frames, frames_to_give_to_rb, false);
			}
			
			rubber_band->retrieve(&frames, length);
		} else {
			if (sf_readf_float(file, frames, length) != length) { //run out of file
				//~ check_sf_errors();
				//~ stop();
				//~ return;
			}
		}
		
		check_sf_errors();
		
		//other effect
		if (effect && effect_on)
			effect->process(frames, length);
	}
}

void Sample::check_sf_errors() {
	if (sf_error(file) != 0) {
		std::cerr << "there was an error getting sound data (" << sf_strerror(file) << ")" << std::endl;
	}
}

bool Sample::is_playing() {
	return playing;
}
/*
bool Sample::add_pad(int pad_id, float position) {
	pad_ids.push_back(pad_id);
	pads_to_positions[pad_id] = position;
	
	return true;
}

bool Sample::remove_pad(int pad_id) {
	std::list<int>::iterator pad_id_i = pad_ids.begin();
	while (pad_id_i != pad_ids.end()) {
		if (*pad_id_i == pad_id) {
			pad_ids.erase(pad_id_i);
			return pads_to_positions.erase(pad_id) > 0;
		}
		++pad_id_i;
	}
	
	return false;
}
*/
void Sample::print_events() {
	std::cout << "events:" << std::endl;
	for (std::list<PadEvent>::iterator ei = events.begin() ;
			ei != events.end() ; ++ei) {
		std::cout << "		pos " << ei->start_position << " @ " << name << std::endl;
	}
}

void Sample::print_info() {
	std::cout << "start " << start_point << ", end " << end_point
			<< ", next start " << next_start_point //<< ", next end " << next_end_point
			<< ", pad down " << pad_down
			<< ", looping " << looping << ", sticky loops " << sticky_loops
			<< std::endl;
}
