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
	public:
		static int load_samples_from_file(std::string filename, std::list<Sample> & samples);
		static bool save_sample_file(std::string filename, std::list<Sample> samples);
		static int load_controller_config(std::string filename, std::list<Pad> & pads);
		static bool save_controller_config(std::string filename, std::list<Pad> pads);
};

#endif // _LOADER_H_
