#include "pad_gui.h"

#include <iostream>

PadGui::PadGui(Pad& pad, std::list<Sample> & samples) : pad(pad){
	//put everything in
	pack_start(play_button);
	pack_start(sample_choice);
	
	//play button
	play_button.set_label("Play");
	play_button.signal_clicked().connect(
			sigc::mem_fun(*this, &PadGui::on_play_button_clicked)
			);

	//combo box
	ref_tree_model = Gtk::ListStore::create(model_columns);
	sample_choice.set_model(ref_tree_model);

	for (std::list<Sample>::iterator si = samples.begin() ;
			si != samples.end() ; ++si) {
		Gtk::TreeModel::Row row = *(ref_tree_model->append());
		row[model_columns.col_id] = si->id;
		row[model_columns.col_name] = si->get_name();
	}

	//sample_choice.pack_start(model_columns.col_id);
	sample_choice.pack_start(model_columns.col_name);
	sample_choice.signal_changed().connect(
			sigc::mem_fun(*this, &PadGui::on_sample_choice_changed)
			);

}

void PadGui::on_sample_choice_changed() {
	Gtk::TreeModel::iterator iter = sample_choice.get_active();
	if(iter) {
		Gtk::TreeModel::Row row = *iter;
		if(row)	{
			int id = row[model_columns.col_id];
			Glib::ustring name = row[model_columns.col_name];

			std::cout << "sample chosen is " << name <<
				" with id " << id << std::endl;
		}
	} else {
		std::cout << "no selection" << std::endl;
	}
}

void PadGui::on_play_button_clicked() {
	std::cout << "play clicked" << std::endl;
}
