#ifndef _PAD_GUI_H_
#define _PAD_GUI_H_

#include <list>

#include <gtkmm.h>

#include "pad.h"
#include "sample.h"

class PadGui : public Gtk::VBox {
	private:
		Pad& pad;

		Gtk::Button play_button;
		Gtk::ComboBox sample_choice;
		Glib::RefPtr<Gtk::ListStore> ref_tree_model;
		Gtk::TreeModelColumn<int> id_column;
		Gtk::TreeModelColumn<Glib::ustring> name_column;

		class ModelColumns : public Gtk::TreeModel::ColumnRecord {
			public:
				ModelColumns() {
					add(col_id);
					add(col_name);
				}

				Gtk::TreeModelColumn<int> col_id;
				Gtk::TreeModelColumn<Glib::ustring> col_name;
		};
		ModelColumns model_columns;

	public:
		PadGui(Pad& pad, std::list<Sample> & samples);
		void on_sample_choice_changed();
		void on_play_button_clicked();
};

#endif
