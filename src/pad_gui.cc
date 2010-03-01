#include "pad_gui.h"

#include <iostream>

PadGui::PadGui(Pad& pad, SampleChoiceModel* choice_model) :
	Gtk::Table(2,3) , pad(pad),
	start_position_spinner_adjustment(0.0, 0.0, 1.0, 0.05, 0.25, 0),
  	start_position_spinner(start_position_spinner_adjustment,0.2,2),
  	end_position_spinner_adjustment(0.0, 0.0, 1.0, 0.05, 0.25, 0),
  	end_position_spinner(end_position_spinner_adjustment,0.2,2)
{
	this->choice_model = choice_model;
	
	//put everything in
	attach(play_button,0,2,0,1);
	attach(start_position_spinner,0,1,1,2);
	attach(end_position_spinner,1,2,1,2);
	attach(sample_choice,0,2,2,3);
	
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
	
	//position spinners
	start_position_spinner_adjustment.signal_value_changed().connect(
			sigc::mem_fun(*this, &PadGui::on_start_position_spinner_changed)
			);
	end_position_spinner_adjustment.signal_value_changed().connect(
			sigc::mem_fun(*this, &PadGui::on_end_position_spinner_changed)
			);
			
	refresh();
}

void PadGui::on_sample_choice_changed() {
	Gtk::TreeModel::iterator iter = sample_choice.get_active();
	if(!iter) return;
	Gtk::TreeModel::Row row = *iter;
	if(!row) return;
	
	Glib::ustring name = row[choice_model->name_column];
	pad.set_sample(row[choice_model->sample_column]);
}

void PadGui::on_play_button_clicked() {
	pad.hit(1.0);
	pad.release();
}

void PadGui::on_start_position_spinner_changed() {
	pad.start_position = start_position_spinner.get_value();
}

void PadGui::on_end_position_spinner_changed() {
	pad.end_position = end_position_spinner.get_value();
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
	
	start_position_spinner.set_value(pad.start_position);
	end_position_spinner.set_value(pad.end_position);
}
