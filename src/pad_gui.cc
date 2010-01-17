#include "pad_gui.h"

#include <iostream>

PadGui::PadGui(Pad& pad, SampleChoiceModel* choice_model) :
	Gtk::Table(2,2) , pad(pad),
	position_spinner_adjustment(0.0, 0.0, 1.0, 0.05, 0.25, 0),
  	position_spinner(position_spinner_adjustment,0.2,2)
{
	this->choice_model = choice_model;
	
	//put everything in
	attach(play_button,0,1,0,1);
	attach(position_spinner,1,2,0,1);
	attach(sample_choice,0,2,1,2);
	
	//play button
	play_button.set_label("Play");
	play_button.signal_clicked().connect(
			sigc::mem_fun(*this, &PadGui::on_play_button_clicked)
			);

	sample_choice.set_model(choice_model->ref_tree_model);

	//combo box
	sample_choice.pack_start(choice_model->name_column);
	sample_choice.signal_changed().connect(
			sigc::mem_fun(*this, &PadGui::on_sample_choice_changed)
			);
	
	//position spinner
	position_spinner_adjustment.signal_value_changed().connect(
			sigc::mem_fun(*this, &PadGui::on_position_spinner_changed)
			);
			
	refresh();
}

void PadGui::on_sample_choice_changed() {
	Gtk::TreeModel::iterator iter = sample_choice.get_active();
	if(iter) {
		Gtk::TreeModel::Row row = *iter;
		if(row)	{
			Glib::ustring name = row[choice_model->name_column];
			std::cout << "sample chosen is " << name << std::endl;
			
			pad.set_sample(row[choice_model->sample_column]);
		}
	} else {
		std::cout << "no selection" << std::endl;
	}
}

void PadGui::on_play_button_clicked() {
	std::cout << "play clicked" << std::endl;
	pad.hit(1.0);
	pad.release();
}

void PadGui::on_position_spinner_changed() {
	std::cout << "position changed to " << position_spinner.get_value() << std::endl;
	pad.set_position(position_spinner.get_value());
}

void PadGui::refresh() {
	Gtk::TreeNodeChildren kids = sample_choice.get_model()->children();
	for (Gtk::TreeModel::iterator ci = kids.begin() ;
			ci != kids.end() ; ++ci) {
		Gtk::TreeModel::Row row = *ci;
		if (row[choice_model->sample_column] == pad.get_sample()) {
			sample_choice.set_active(ci);
			break;
		}	
	}
	
	position_spinner.set_value(pad.get_position());
}
