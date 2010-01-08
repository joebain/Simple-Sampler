#include "jack.h"

#include <jack/jack.h>
#include <iostream>

Jack::Jack() {
	input_port_name = "input";
	output_port_name = "output";
	client_name = "sampler";
}

bool Jack::connect() {
	started = 0;
	
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

	/* tell the JACK server to call `process()' whenever
	   there is work to be done.
	*/

	jack_set_process_callback (client, jack_process_cb, 0);

	/* tell the JACK server to call `jack_shutdown()' if
	   it ever shuts down, either entirely, or if it
	   just decides to stop calling us.
	*/

	jack_on_shutdown (client, jack_shutdown_cb, 0);

	/* display the current sample rate. 
	 */

	//std::cout << "engine sample rate: " << jack_get_sample_rate (client) << std::endl;
		

	/* create two ports */

	input_port = jack_port_register (client, input_port_name.c_str(),
					 JACK_DEFAULT_AUDIO_TYPE,
					 JackPortIsInput, 0);
	
	output_port = jack_port_register (client, output_port_name.c_str(),
					  JACK_DEFAULT_AUDIO_TYPE,
					  JackPortIsOutput, 0);
	
	if ((input_port == NULL) || (output_port == NULL)) {
		std::cerr << "no more JACK ports available" << std::endl;
		return false;
	}

	/* Tell the JACK server that we are ready to roll.  Our
	 * process() callback will start running now. */

	if (jack_activate (client)) {
		std::cerr << "cannot activate client" << std::endl;
		return false;
	}
	

	/* Connect the ports.  You can't do this before the client is
	 * activated, because we can't make connections to clients
	 * that aren't running.  Note the confusing (but necessary)
	 * orientation of the driver backend ports: playback ports are
	 * "input" to the backend, and capture ports are "output" from
	 * it.
	 */
	/*
	ports = jack_get_ports (client, NULL, NULL,
				JackPortIsPhysical|JackPortIsOutput);
	if (ports == NULL) {
		fprintf(stderr,"no physical capture ports\n");
		exit (1);
	}
	
	if (jack_connect (client, ports[0], jack_port_name (input_port))) {
		fprintf(stderr,"cannot connect input ports\n");
	}

	free (ports);
	*/
	/*
	ports = jack_get_ports (client, NULL, NULL,
				JackPortIsPhysical|JackPortIsInput);
	if (ports == NULL) {
		fprintf(stderr, "no physical playback ports\n");
		exit (1);
	}
	
	if (jack_connect (client, jack_port_name (output_port), ports[0])) {
		fprintf (stderr, "cannot connect output ports\n");
	}
	
	free (ports);
	*/

	return true;
}

/* we call this when jack is closed on purpose*/
bool Jack::close() {
	jack_client_close (client);
	return true;
}

/**
 * The process callback for this JACK application is called in a
 * special realtime thread once for each audio cycle.
 *
 * This client follows a simple rule: when the JACK transport is
 * running, copy the input port to the output.  When it stops, exit.
 */
int jack_process_cb (jack_nframes_t nframes, void *arg) {
	//for now we do nothing
	
	return 0;      
}

/**
 * JACK calls this shutdown_callback if the server ever shuts down or
 * decides to disconnect the client.
 */
void jack_shutdown_cb (void *arg) {
	//for now we do nothing
}
