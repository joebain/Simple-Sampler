#ifndef _LOADER_H_
#define _LOADER_H_

#include <string>
#include <list>

#include <libxml++/libxml++.h>

#include "sample.h"
#include "pad.h"

class Loader
{
	private:
		static std::string get_attribute(xmlpp::Node* node, std::string name);
		static Pad parse_pad_node(xmlpp::Node* pad_node);
		static float parse_float(std::string string_value);
		static int parse_int(std::string string_value);
		static std::string to_string(int i);
		static std::string to_string(float i);
	public:
		static bool load_sample(std::string, Sample& sample);
		static int load_samples_from_file(std::string filename, std::list<Sample> & samples);
		static bool save_sample_file(std::string filename, std::list<Sample> & samples);
		static int load_controller_config(std::string filename, std::list<Pad> & pads);
		static bool save_controller_config(std::string filename, std::list<Pad> & pads);
		static bool link_pads_to_samples(std::string filename, std::list<Pad> & pads, std::list<Sample> & samples);
};

#endif // _LOADER_H_
