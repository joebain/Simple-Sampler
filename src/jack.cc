#include "jack.h"

#include <jack/jack.h>
#include <jack/midiport.h>
#include <iostream>
#include <string.h>

#include "main.h"
#include "server.h"

Jack::Jack() {
	input_port_name = "audio_out";
	output_port_name = "audio_in";
	input_midi_port_name = "midi_in";
	output_midi_port_name = "midi_out";
	client_name = "sampler";
	
	ready = false;
	started = false;
}

bool Jack::connect() {
		
	std::string server_name = "";
	jack_options_t options = JackNullOption;
	jack_status_t status;

	/* open a client connection to the JACK server */

	client = jack_client_open (client_name.c_str(), options, &status, server_name.c_str());
	if (client == NULL) {
		std::cerr <<  "jack_client_open() failed, "
			<< "status = " << status << std::endl;
		if (status & JackServerFailed) {
			std::cerr << "Unable to connect to JACK server"  << std::endl;
		}
		return false;
	}
	if (status & JackServerStarted) {
		std::cerr << "JACK server started" << std::endl;
	}
	if (status & JackNameNotUnique) {
		client_name = jack_get_client_name(client);
		std::cerr << "unique name " << client_name << " assigned" << std::endl;
	}

	/* register callbacks */
	jack_set_process_callback(client, jack_process_cb, this);
	jack_set_buffer_size_callback(client, jack_bufsize_cb, this);
	
	jack_on_shutdown(client, jack_shutdown_cb, this);


	/* create audio ports */
	input_port = jack_port_register (client, input_port_name.c_str(),
					JACK_DEFAULT_AUDIO_TYPE,
					JackPortIsInput, 0);
	
	output_port = jack_port_register (client, output_port_name.c_str(),
					JACK_DEFAULT_AUDIO_TYPE,
					JackPortIsOutput, 0);
	
	if ((output_port == NULL)) {// || (input_port == NULL)) {
		std::cerr << "unable to register jack audio ports" << std::endl;
		return false;
	}
	
	/* create midi ports */  
	input_midi_port = jack_port_register (client, input_midi_port_name.c_str(),
					JACK_DEFAULT_MIDI_TYPE,
					JackPortIsInput, 0);
	
	output_midi_port = jack_port_register (client, output_midi_port_name.c_str(),
					JACK_DEFAULT_MIDI_TYPE,
					JackPortIsOutput, 0);
	
	if ((input_midi_port == NULL) ){//|| (output_midi_port == NULL)) {
		std::cerr << "unable to register jack midi ports" << std::endl;
		return false;
	}
	
	ready = true;

	return true;
}

bool Jack::start() {
	/* start getting process call backs */
	if (jack_activate (client)) {
		std::cerr << "cannot activate client" << std::endl;
		return false;
	}

	/* connect output port */
	const char **ports;
	
	ports = jack_get_ports (client, NULL, NULL,
				JackPortIsPhysical|JackPortIsInput);
	if (ports == NULL) {
		std::cerr << "no physical playback ports" << std::endl;
		return false;
	}
	
	if (jack_connect (client, jack_port_name (output_port), ports[0])) {
		std::cerr << "cannot connect output port" << std::endl;
	}
	
	delete ports;
	
	started = true;
	
	return true;
}

/* we call this when jack is closed on purpose */
bool Jack::close() {
	jack_client_close (client);
	return true;
}

/* process data to and from jack */
int jack_process_cb (jack_nframes_t nframes, void *arg) {
	
	Jack* jack = (Jack*) arg;
	
	/* midi */
	void* port_buf = jack_port_get_buffer(jack->get_input_midi_port(), nframes);
	jack_nframes_t event_count = jack_midi_get_event_count(port_buf);
	jack_midi_event_t in_event;
	
	for (uint event = 0 ; event < event_count ; event++) {
		jack_midi_event_get(&in_event, port_buf, event);
		//~ std::cout << "midi event " << std::hex 
				//~ << (uint) in_event.buffer[0] << " "
				//~ << (uint) in_event.buffer[1] << " "
				//~ << (uint) in_event.buffer[2] << " "
				//~ << (uint) in_event.buffer[3]
				//~ << std::endl;
		if (in_event.buffer[0] == 0x90) { /* note on */
			server->midi_on(in_event.buffer[1],in_event.buffer[2]);
		} else if(in_event.buffer[0] == 0x80) { /* note off */
			server->midi_off(in_event.buffer[1]);
		} else if (in_event.buffer[0] == 0xe0) { /* pitch bend */
			server->pitch_bend(in_event.buffer[1], in_event.buffer[2]);
		} else if (in_event.buffer[0] == 0xb0) { /* controller change */
			server->controller_change(in_event.buffer[1],in_event.buffer[2]);
		}
	}
	
	/* audio */
	jack_default_audio_sample_t *out =
		(jack_default_audio_sample_t *) 
			jack_port_get_buffer(jack->get_output_port(), nframes);
	
	for (uint f = 0; f < nframes ; f++) {
		out[f] = 0.0f;
	}
	
	server->get_frames(out, nframes);
	
	return 0;      
}

int jack_bufsize_cb(jack_nframes_t nframes, void *arg) {
	
	//Jack* jack = (Jack*) arg;
	
	return 0;
}

/**
 * JACK calls this shutdown_callback if the server ever shuts down or
 * decides to disconnect the client.
 */
void jack_shutdown_cb (void *arg) {
	
	//Jack* jack = (Jack*) arg;
	
	//for now we do nothing
}
