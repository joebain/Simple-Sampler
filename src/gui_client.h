#ifndef _GUI_CLIENT_H_
#define _GUI_CLIENT_H_

#include <list>
#include <map>

#include <gtkmm.h>

#include "client.h"
#include "server.h"

#include "pad_gui.h"
#include "bit_effect_gui.h"

class GuiClient : public Client, public Gtk::Window {
	private:
		void on_add_sample_button_clicked();
		void on_load_pad_config_button_clicked();
		void on_edit_bit_effect_button_clicked();
		void on_sample_is_looping_toogled(const Glib::ustring& path);
		void on_has_effect_toogled(const Glib::ustring& path);
		void on_has_timestretch_toogled(const Glib::ustring& path);
		void on_save_sample_config_button_clicked();
		void on_save_pad_config_button_clicked();
		
		void init();
		void refresh();
		
		bool get_xml_file(std::string* filename);
		
		Gtk::Button add_sample_button;
		Gtk::Button load_pad_config_button;
		Gtk::Button save_sample_config_button;
		Gtk::Button save_pad_config_button;
		Gtk::Button edit_bit_effect_button;
		
		Gtk::TextView message_window;
		Gtk::VBox v_box;
		Gtk::ScrolledWindow scrolled_window;
		Gtk::Paned v_paned;
		Gtk::Table sample_table;
		Gtk::HButtonBox button_box;
		Gtk::Frame sample_frame;
		Gtk::TreeView sample_list_view;
		
		BitEffectGui* bit_effect_gui;
		Gtk::Window edit_bit_effect_window;
		
		SampleChoiceModel* choice_model;
		
		std::list<PadGui*> pad_guis;
		PadGui* get_pad_gui(Pad pad);
	public:
		GuiClient(Server* server);
		~GuiClient();
		
		void start();
		void stop();
		void update();
		bool update_gtk();
		
		static void init_gtk(int argc, char *argv[]);
	
	protected:
	  	
};

#endif
