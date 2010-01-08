#include "sample.h"

#include <iostream>

#include <sndfile.h>

Sample::Sample() {
	file = 0;
	filename = "";
}

bool Sample::load(std::string filename)
{
	if (file) sf_close(file);

	this->filename = filename;
	
	SF_INFO sfinfo;
	file = sf_open(filename.c_str(),SFM_READ, &sfinfo);
	
	return true;
}

bool Sample::play()
{
	std::cout << "playing a song" << std::endl;
	
	return false;
}

bool Sample::stop()
{
	return false;
}

Sample::~Sample() {
	stop();
	
	if (file) sf_close(file);
}

bool Sample::operator==(const Sample &other) const {
	return (this->filename == other.filename);
}

Sample & Sample::operator=(const Sample &other) {
	if (this == &other)
		return *this;
	
	this->filename = other.filename;
	this->file = other.file;
	
	return *this; 
}
