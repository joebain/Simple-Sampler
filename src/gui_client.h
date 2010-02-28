#ifndef _GUI_CLIENT_H_
#define _GUI_CLIENT_H_

#include <list>
#include <map>

#include <gtkmm.h>

#include "client.h"
#include "server.h"

#include "pad_gui.h"

class GuiClient : public Client, public Gtk::Window {
	private:
		void on_add_sample_button_clicked();
		void on_load_pad_config_button_clicked();
		void on_sample_is_looping_toogled(const Glib::ustring& path);
		
		void init();
		void refresh();
		
		Gtk::Button add_sample_button;
		Gtk::Button load_pad_config_button;
		Gtk::Button test_button;
		Gtk::TextView message_window;
		Gtk::VBox v_box;
		Gtk::ScrolledWindow scrolled_window;
		Gtk::Paned v_paned;
		Gtk::Table sample_table;
		Gtk::HButtonBox button_box;
		Gtk::Frame sample_frame;
		Gtk::TreeView sample_list_view;
		
		SampleChoiceModel* choice_model;
		
		std::list<PadGui*> pad_guis;
	public:
		GuiClient(Server* server);
		~GuiClient();
		
		void start();
		
		static void init_gtk(int argc, char *argv[]);
	
	protected:
	  	
};

#endif
