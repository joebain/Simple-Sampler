#include "server.h"

#include <vector>
#include <iostream>

#include "jack.h"

Server::Server() {
	jack = new Jack();
}

bool Server::start() {
	jack->connect();
	
	while (true) {
		std::cout << "tick" << std::endl;
		sleep(1);
	}
	
	return true;
}

bool Server::stop() {
	jack->close();

	return true;
}

bool Server::add_sample(Sample sample) {
	samples.push_back(sample);
	std::cout << "added a sample, name: " << sample.filename << std::endl;
	return true;
}

bool Server::add_samples(std::vector<Sample> new_samples) {
	std::vector<Sample>::iterator i = new_samples.begin();
	
	while (i != new_samples.end()) {
		if (!add_sample(*i)) return false;
		++i;
	}
	
	return true;
}

bool Server::remove_sample(Sample sample) {
	uint size = samples.size();

	std::vector<Sample>::iterator i = samples.begin();

	while (i != samples.end()) {
		if (*i == sample) {
			samples.erase(i);
			break;
		}
		++i;
	}

	if (size > samples.size())
		return true;
	else
		return false;
}
