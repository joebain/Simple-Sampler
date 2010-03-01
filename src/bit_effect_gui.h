#ifndef _BIT_EFFECT_GUI_H_
#define _BIT_EFFECT_GUI_H_

#include <gtkmm.h>
#include <string>

#include "bit_effect.h"

class BitEffectGui : public Gtk::Table {
	private:
		BitEffect* effect;

		Gtk::Adjustment offset_spinner_adjustment;
		Gtk::SpinButton offset_spinner;
		Gtk::Label offset_spinner_label;
		
		Gtk::Adjustment murder_scale_adjustment;
		Gtk::HScale murder_scale;
		Gtk::Label murder_scale_label;
		
		Gtk::Button one, two, three, four, five, six, seven, eight;
		void bit_clicked(Gtk::Button* button, int i);
				
		int twiddle(int i);
		std::string get_description_from_twiddle(int result);

	public:
		BitEffectGui(BitEffect* effect);
		void on_offset_changed();
		void on_scale_changed();
		void on_murder_changed();
};

#endif
