#ifndef _LOADER_H_
#define _LOADER_H_

#include <string>
#include <vector>

#include "sample.h"

class Loader
{
	public:
		static int load_from_file(std::string filename, std::vector<Sample> & samples);
	protected:

	private:

};

#endif // _LOADER_H_
