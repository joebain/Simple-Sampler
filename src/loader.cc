#include "loader.h"

#include <string>
#include <fstream>
#include <iostream>
#include <vector>

int Loader::load_from_file(std::string filename, std::vector<Sample> & samples) {
	
	std::ifstream samplefile(filename.c_str());

	if (!samplefile.is_open()) {
		std::cerr << "File " << filename << " could not be opened." << std::endl;
		return -1;
	}

	std::string line;
	int lines = 0;
	while (!samplefile.eof()) {
		++lines;
		getline(samplefile, line);
	}
	
	samplefile.clear();
	samplefile.close();
	samplefile.open(filename.c_str());

	lines -= 1;

	samples = std::vector<Sample>(lines);
	for (int i = 0 ; i < lines; i++) {
		getline(samplefile, line);
		samples[i] = Sample();
		samples[i].load(line);
	}
	
	samplefile.close();
	
	return lines;
}


