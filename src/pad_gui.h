#ifndef _PAD_GUI_H_
#define _PAD_GUI_H_

#include <list>

#include <gtkmm.h>

#include "pad.h"
#include "sample.h"
#include "sample_choice_model.h"

class PadGui : public Gtk::Table {
	private:
		Pad& pad;

		Gtk::Button play_button;
		Gtk::Adjustment start_position_spinner_adjustment;
		Gtk::SpinButton start_position_spinner;
		Gtk::Adjustment end_position_spinner_adjustment;
		Gtk::SpinButton end_position_spinner;
		Gtk::ComboBox sample_choice;
				
		SampleChoiceModel* choice_model;

		bool highlighted;

	public:
		PadGui(Pad& pad, SampleChoiceModel* choice_model);
		void on_sample_choice_changed();
		void on_play_button_clicked();
		void on_start_position_spinner_changed();
		void on_end_position_spinner_changed();

		void refresh();
		
		const Pad& get_pad() { return pad; }
		
		void update();
		
		void highlight();
		void unhighlight();
};

#endif
