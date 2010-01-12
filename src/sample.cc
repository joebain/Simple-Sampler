#include "sample.h"

#include <iostream>
#include <math.h>

#include <sndfile.h>
#include <rubberband/RubberBandStretcher.h>

Sample::Sample() {
	file = 0;
	filename = "";
	playing = false;
	stopping_at = 1.0;
	speed = 1.0;
	pitch = 1.0;
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
			RubberBand::RubberBandStretcher::OptionProcessRealTime);
	
	return true;
}

bool Sample::play() {
	return play(0.0f);
}

bool Sample::play(float position)
{	
	playing = true;
	
	std::cout << "playing sample " << filename << std::endl;
	
	int pos = (int) (total_frames * position + 0.5);
	
	sf_seek(file, pos, SEEK_SET);
	
	return true;
}

bool Sample::give_event(Event e)
{
	std::cout << "got event " << e.pad_id << std::endl;
	
	e.position = pads_to_positions[e.pad_id];
	
	if (e.on) {
		if (events.size() >= 1) {
			Event old_front = events.front();
			if (try_add_event(e)) {
				stopping_at = events.back().position;
				if (events.front().pad_id != old_front.pad_id)
					return play(events.front().position);	
			} else {
				stopping_at = 1.0f;
				return play(e.position);
			}
		} else {
			events.push_back(e);
			stopping_at = 1.0f;
			return play(e.position);
		}
	} else {
		std::list<Event>::iterator ei = events.begin();
		while (ei != events.end()) {
			if (ei->pad_id == e.pad_id) {
				events.erase(ei);
				print_events();
				return true;
			}
			++ei;
		}
	}
	
	return false;
}

bool Sample::try_add_event(Event e) {
	for (std::list<Event>::iterator ei = events.begin() ;
			ei != events.end() ; ++ei) {
		if (ei->position > e.position) {
			events.insert(ei,e);
			return true;
		} else if (ei->pad_id == e.pad_id) {
			return false;
		}
	}
	events.push_back(e);
	return true;
}

bool Sample::stop()
{
	playing = false;
	
	std::cout << "stopped playing sample " << filename << std::endl;
	
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
	speed = new_speed;
	rubber_band->setTimeRatio(new_speed);
}

void Sample::set_pitch(float new_pitch) {
	pitch = new_pitch;
	rubber_band->setPitchScale(new_pitch);
}

void Sample::next_frames(float frames[], int length) {
	
	if (playing) {
		
		int pos_i = sf_seek(file, 0, SEEK_CUR); //current position in the file
		if (pos_i != -1) {
			float pos_f = (float)pos_i / (float)total_frames;
			if (pos_f > stopping_at) {
				stop();
				stopping_at = 1.0;
				return;
			}
		}
		while(rubber_band->available() < length) {
			int frames_needed_by_rb = rubber_band->getSamplesRequired();
			int frames_to_give_to_rb = length < frames_needed_by_rb ? length : frames_needed_by_rb;
			//std::cout << "needed " << frames_to_give_to_rb << std::endl;
			int frames_got_from_sf = sf_readf_float(file, frames, frames_to_give_to_rb);
			if (frames_got_from_sf == 0) { //run out of file
				if (sf_error(file) != 0) {
					std::cerr << "there was an error getting sound data (" << sf_strerror(file) << ")" << std::endl;
				}
				stop();
				rubber_band->retrieve(&frames, rubber_band->available());
				return;
			}
			rubber_band->process(&frames, frames_to_give_to_rb, false);
		}
		
		rubber_band->retrieve(&frames, length);
	}
}

bool Sample::is_playing() {
	return playing;
}

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

void Sample::print_events() {
	std::cout << "events:" << std::endl;
	for (std::list<Event>::iterator ei = events.begin() ;
			ei != events.end() ; ++ei) {
		std::cout << "		pad no " << ei->pad_id << std::endl;
	}
}
