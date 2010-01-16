#include "loader.h"

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#include <libxml++/libxml++.h>

#include "pad.h"

int Loader::load_samples_from_file(std::string filename, std::list<Sample> & samples) {
	
	try {
		xmlpp::DomParser parser;
		parser.parse_file(filename);
		if(parser) {
			const xmlpp::Node* root_node = parser.get_document()->get_root_node();
			std::list<xmlpp::Node*> sample_nodes = root_node->get_children("sample");
			std::list<xmlpp::Node*>::iterator sample_node = sample_nodes.begin();
			while (sample_node != sample_nodes.end()) {
				std::string sample_filename = get_attribute(*sample_node,"filename");
				Sample sample;
				if (sample.load("data/" + sample_filename)) {
					samples.push_back(sample);
					
					std::list<xmlpp::Node*> pad_nodes = (*sample_node)->get_children("pad");
					std::list<xmlpp::Node*>::iterator pad_node = pad_nodes.begin();
					while (pad_node != pad_nodes.end()) {
						std::string pad_id_string = get_attribute(*pad_node, "id");
						std::string pad_position_string = get_attribute(*pad_node, "position");
						
						std::stringstream ss (std::stringstream::in | std::stringstream::out);
						ss << pad_id_string;
						int pad_id;
						ss >> pad_id;
						
						ss.clear();
						
						ss << pad_position_string;
						float pad_position;
						ss >> pad_position;
						
						samples.back().add_pad(pad_id,pad_position);
						++pad_node;
					}
				}
				
				++sample_node;
			}
		}
	} catch(const std::exception& ex) {
		std::cout << "Exception caught parsing samples xml file: " << ex.what() << std::endl;
	}
	
	return samples.size();
}

bool Loader::load_sample(std::string filename, Sample& sample) {
	return sample.load(filename);
}

bool Loader::save_sample_file(std::string filename, std::list<Sample> samples) {
	return true;
}

int Loader::load_controller_config(std::string filename, std::list<Pad> & pads) {
	
	try {
		xmlpp::DomParser parser;
		parser.parse_file(filename);
		if(parser) {
			const xmlpp::Node* root_node = parser.get_document()->get_root_node();
			
			std::list<xmlpp::Node*> pads_parent = root_node->get_children("pads");
			std::list<xmlpp::Node*> row_nodes = pads_parent.front()->get_children("row");
			
			int row_count = 0;
			for (std::list<xmlpp::Node*>::iterator row_node = row_nodes.begin() ;
					row_node != row_nodes.end() ; ++row_node) {
				
				std::list<xmlpp::Node*> pad_nodes = (*row_node)->get_children("pad");
				
				int col_count = 0;
				for (std::list<xmlpp::Node*>::iterator pad_node = pad_nodes.begin() ;
						pad_node != pad_nodes.end() ; ++pad_node) {
					
					Pad pad;
					if (parse_pad_node(*pad_node, &pad)) {
						pad.set_pos(row_count,col_count);
						pads.push_back(pad);
						col_count++;
					}
				}
				row_count ++;
			}
		}
	} catch(const std::exception& ex) {
		std::cout << "Exception caught parsing controller xml file: " << ex.what() << std::endl;
	}
	
	return pads.size();
}

bool Loader::parse_pad_node(xmlpp::Node* pad_node, Pad* pad) {
	std::string event_number_string = get_attribute(pad_node, "event_number");
	if (event_number_string.empty()) return false;
	std::string id_string = get_attribute(pad_node, "id");
	if (id_string.empty()) return false;
	
	std::stringstream ss (std::stringstream::in | std::stringstream::out);

	ss << event_number_string;
	int event_number;
	ss >> std::hex >> event_number;

	ss.clear();

	ss << id_string;
	int id;
	ss >> std::dec >> id;

	pad = new Pad(event_number,id);
	
	return true;
}

bool Loader::save_controller_config(std::string filename, std::list<Pad> pads) {
	return true;
}

std::string Loader::get_attribute(xmlpp::Node* node, std::string name) {
	const xmlpp::Element* node_element = dynamic_cast<const xmlpp::Element*>(node);
	if (node_element)
		return node_element->get_attribute_value(name);
	else
		return "";
}
