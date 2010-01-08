#ifndef _SAMPLE_H_
#define _SAMPLE_H_

#include <sndfile.h>
#include <string>

class Sample
{
public:
	Sample();
	~Sample();
	
	bool load(std::string filename);
	bool play();
	bool stop();

	bool operator==(const Sample &other) const;
	Sample & operator=(const Sample &other);
	
	std::string filename;
protected:

private:
	SNDFILE* file;

};

#endif // _SAMPLE_H_
