#ifndef _SERVER_H_
#define _SERVER_H_

#include <vector>

#include "jack.h"
#include "sample.h"

class Server {
	private:
		Jack* jack;

		std::vector<Sample> samples;
	public:
		Server();
		bool start();
		bool stop();

		bool add_sample(Sample sample);
		bool add_samples(std::vector<Sample> sample);
		bool remove_sample(Sample sample);
};

#endif
