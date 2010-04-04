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
			
			for (std::list<xmlpp::Node*>::iterator sample_node = sample_nodes.begin() ;
					sample_node != sample_nodes.end() ; ++sample_node) {
				std::string sample_filename = get_attribute(*sample_node,"filename");
				Sample sample;
				sample.id = parse_int(get_attribute(*sample_node,"id"));
				if (sample.load("data/" + sample_filename)) {
					samples.push_back(sample);
				}
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

std::string Loader::to_string(int i) {
	std::stringstream ss;
	ss << i;
	return ss.str();
}

std::string Loader::to_string(float i) {
	std::stringstream ss;
	ss << i;
	return ss.str();
}

bool Loader::save_sample_file(std::string filename, std::list<Sample> & samples) {
	try {
		xmlpp::Document document;
		
		xmlpp::Node* root_node = document.create_root_node("samples");
				
		for (std::list<Sample>::iterator sample = samples.begin() ;
				sample != samples.end() ; ++sample) {
			std::string sample_filename = sample->get_filename();
			std::string id = to_string(sample->id);
						
			xmlpp::Element* child = root_node->add_child("sample");
			child->set_attribute("filename",sample_filename);
			child->set_attribute("id",id);
		}
		
		document.write_to_file(filename);
		
	} catch(const std::exception& ex) {
		std::cout << "Exception caught writing samples xml file: " << ex.what() << std::endl;
		return false;
	}
	
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
					
					Pad pad = parse_pad_node(*pad_node);
					
					pad.set_pos(row_count,col_count);
					pads.push_back(pad);
					
					col_count++;
					
				}
				row_count ++;
			}
		}
	} catch(const std::exception& ex) {
		std::cout << "Exception caught parsing controller xml file: " << ex.what() << std::endl;
	}
	
	return pads.size();
}

Pad Loader::parse_pad_node(xmlpp::Node* pad_node) {
	std::string event_number_string = get_attribute(pad_node, "event_number");
	if (event_number_string.empty()) {
		std::exception e;
		throw e;
	}
	std::string id_string = get_attribute(pad_node, "id");
	if (id_string.empty()) {
		std::exception e;
		throw e;
	}
	
	std::stringstream ss (std::stringstream::in | std::stringstream::out);

	ss << event_number_string;
	int event_number;
	ss >> std::hex >> event_number;

	ss.clear();

	ss << id_string;
	int id;
	ss >> std::dec >> id;

	Pad pad(event_number,id);
		
	return pad;
}

bool Loader::save_controller_config(std::string filename, std::list<Pad> & pads) {
	try {
		xmlpp::Document document;
		
		xmlpp::Node* root_node = document.create_root_node("controller");
		xmlpp::Node* pads_node = root_node->add_child("pads");
		
		std::map<int, xmlpp::Node*> rows;
		
		for (std::list<Pad>::iterator pad = pads.begin() ;
				pad != pads.end() ; ++pad) {
			std::string event_number = to_string(pad->get_event_number());
			std::string key = "x";
			std::string id = to_string(pad->get_id());
			
			int row = pad->get_x();
			
			xmlpp::Node* row_node;
			
			std::map<int, xmlpp::Node*>::iterator it = rows.find(row);
			if (it == rows.end()) {
				row_node = pads_node->add_child("row");
			} else {
				row_node = it->second;
			}
			
			xmlpp::Element* pad_el = row_node->add_child("pad");
			pad_el->set_attribute("event_number",event_number);
			pad_el->set_attribute("id",id);
		}
		
		document.write_to_file(filename);
		
	} catch(const std::exception& ex) {
		std::cout << "Exception caught parsing controller xml file: " << ex.what() << std::endl;
		return false;
	}
	
	return true;
}

std::string Loader::get_attribute(xmlpp::Node* node, std::string name) {
	const xmlpp::Element* node_element = dynamic_cast<const xmlpp::Element*>(node);
	if (node_element)
		return node_element->get_attribute_value(name);
	else
		return "";
}

bool Loader::link_pads_to_samples(std::string filename, std::list<Pad> & pads, std::list<Sample> & samples) {
	//create a map of pad ids to pads
	std::map<int,Pad*> pad_ids_to_pads;
	for (std::list<Pad>::iterator pi = pads.begin() ;
			pi != pads.end() ; ++pi)
		pad_ids_to_pads[pi->get_id()] = &(*pi);
		
	//create a map of sample ids to samples
	std::map<int,Sample*> sample_ids_to_samples;
	for (std::list<Sample>::iterator si = samples.begin() ;
			si != samples.end() ; ++si)
		sample_ids_to_samples[si->id] = &(*si);
	
	try {
		xmlpp::DomParser parser;
		parser.parse_file(filename);
		if(parser) {
			const xmlpp::Node* root_node = parser.get_document()->get_root_node();
			std::list<xmlpp::Node*> pad_nodes = root_node->get_children("pad");
			
			for (std::list<xmlpp::Node*>::iterator pad_node = pad_nodes.begin() ;
					pad_node != pad_nodes.end() ; ++pad_node) {
				int pad_id = parse_int(get_attribute(*pad_node,"id"));
				int sample_id = parse_int(get_attribute(*pad_node,"sample_id"));
				float start = parse_float(get_attribute(*pad_node,"start"));
				float end = parse_float(get_attribute(*pad_node,"end"));
				
				pad_ids_to_pads[pad_id]->set_sample(sample_ids_to_samples[sample_id]);
				pad_ids_to_pads[pad_id]->start_position = start;
				pad_ids_to_pads[pad_id]->end_position = end;
			}
		}
	} catch(const std::exception& ex) {
		std::cout << "Exception caught parsing samples xml file: " << ex.what() << std::endl;
	}
	
	return true;
}

float Loader::parse_float(std::string string_value) {
	std::stringstream ss (std::stringstream::in | std::stringstream::out);

	ss << string_value;
	float float_value;
	ss >> float_value;
	
	return float_value;
}

int Loader::parse_int(std::string string_value) {
	std::stringstream ss (std::stringstream::in | std::stringstream::out);

	ss << string_value;
	int int_value;
	ss >> int_value;
	
	return int_value;
}
