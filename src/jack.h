#ifndef JACK_H
#define JACK_H

#include <jack/jack.h>
#include <string>

class Jack {
	private:
		jack_port_t *input_port;
		jack_port_t *output_port;
		jack_client_t *client;

		int count;
		int started;
	public:
		Jack();
		bool connect();
		bool close();

		std::string input_port_name;
		std::string output_port_name;
		std::string client_name;
};

int jack_process_cb (jack_nframes_t nframes, void *arg);
void jack_shutdown_cb (void *arg);

#endif
