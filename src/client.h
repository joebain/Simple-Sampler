#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "server.h"

class Client
{
public:
	Client(Server* server);

	virtual void start();
	virtual void stop();
	virtual void update();

	bool running;

protected:
	Server* server;
	
	void load_pads(std::string filename);
	bool save_pads(std::string filename);
	bool load_samples(std::string filename);
	bool save_samples(std::string filename);
	bool load_sample(std::string filename);
	bool link_pads_to_samples(std::string filename);

private:

};

#endif // _CLIENT_H_
