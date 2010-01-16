#ifndef _PAD_GUI_H_
#define _PAD_GUI_H_

#include <gtkmm.h>

#include "pad.h"

class PadGui : public Gtk::VBox {
	private:
		Pad& pad;
		
		Gtk::Button play_button;
		Gtk::ComboBox sample_choices;
		Glib::RefPtr<Gtk::ListStore> ref_tree_model;
		Gtk::TreeModelColumn<int> id_column;
		Gtk::TreeModelColumn<Glib::ustring> name_column;
	public:
		PadGui(Pad& pad);
};

#endif
