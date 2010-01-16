#ifndef _GUI_CLIENT_H_
#define _GUI_CLIENT_H_

#include <list>

#include <gtkmm.h>

#include "client.h"
#include "server.h"

#include "pad_gui.h"

class GuiClient : public Client, public Gtk::Window {
	private:
		void on_add_sample_button_clicked();
		void on_load_pad_config_button_clicked();
		
		void refresh_pads();
		
		Gtk::Button add_sample_button;
		Gtk::Button load_pad_config_button;
		Gtk::Button test_button;
		Gtk::TextView message_window;
		Gtk::VBox v_box;
		Gtk::ScrolledWindow scrolled_window;
		Gtk::Paned v_paned;
		Gtk::Table sample_table;
		Gtk::HButtonBox button_box;
		
		std::list<PadGui*> pad_guis;
	public:
		GuiClient(Server* server);
		~GuiClient();
		
		void start();
	
	protected:
	  	
};

#endif
