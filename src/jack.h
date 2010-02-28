#ifndef _JACK_H_
#define _JACK_H_

#include <jack/jack.h>
#include <string>

class Jack {
	private:
		jack_port_t *input_port;
		jack_port_t *output_port;
		jack_port_t *output_midi_port;
		jack_port_t *input_midi_port;
		jack_client_t *client;

		bool started;
		bool ready;
		
		std::string input_port_name;
		std::string output_port_name;
		std::string input_midi_port_name;
		std::string output_midi_port_name;
		std::string client_name;
	public:	
		Jack();
		~Jack();
		bool connect();
		bool close();
		bool start();
		
		bool is_ready() { return ready; }
		
		int buffer_size() { return jack_get_buffer_size(client); }
		int sample_rate() { return jack_get_sample_rate(client); }

		jack_port_t* get_input_port() { return input_port; }
		jack_port_t* get_output_port() { return output_port; }
		jack_port_t* get_input_midi_port() { return input_midi_port; }
		jack_port_t* get_output_midi_port() { return output_midi_port; }
};

int jack_process_cb (jack_nframes_t nframes, void *arg);
int jack_bufsize_cb(jack_nframes_t nframes, void *arg);
void jack_shutdown_cb (void *arg);
void jack_error_cb(const char* msg);

#endif
