#include "bit_effect_gui.h"

BitEffectGui::BitEffectGui(BitEffect* effect) :
	Gtk::Table(2,6),
	offset_spinner_adjustment(0.0, -1.0, 1.0, 0.05, 0.25, 0),
	offset_spinner(offset_spinner_adjustment,0.2,2),
	offset_spinner_label("offset"),
  	murder_scale_adjustment(0.0, 1.0, 32.0, 1, 8, 0),
  	murder_scale(murder_scale_adjustment),
  	murder_scale_label("murder"),
  	one("one"),
  	two("two"),
  	three("three"),
  	four("four"),
  	five("five"),
  	six("six"),
  	seven("seven"),
  	eight("eight")
{
	this->effect = effect;
	
	set_spacings(5);
	
	//put everything in
	attach(offset_spinner_label,2,4,0,1);
	attach(offset_spinner,2,4,1,2);
	attach(murder_scale_label,0,4,2,3);
	attach(murder_scale,0,4,3,4);
		
	//offset_spinner
	offset_spinner.set_value(effect->offset);
	offset_spinner_adjustment.signal_value_changed().connect(
			sigc::mem_fun(*this, &BitEffectGui::on_offset_changed)
			);
	
	//murder_scale
	murder_scale.set_value(effect->quant);
	murder_scale_adjustment.signal_value_changed().connect(
			sigc::mem_fun(*this, &BitEffectGui::on_murder_changed)
			);
			
	//bit buttons
	attach(one, 0,1,4,5);
	attach(two, 1,2,4,5);
	attach(three, 2,3,4,5);
	attach(four, 3,4,4,5);
	attach(five, 0,1,5,6);
	attach(six, 1,2,5,6);
	attach(seven, 2,3,5,6);
	attach(eight, 3,4,5,6);
	
	one.signal_clicked().connect(
		sigc::bind(sigc::mem_fun(*this, &BitEffectGui::bit_clicked),&one,0));
	two.signal_clicked().connect(
		sigc::bind(sigc::mem_fun(*this, &BitEffectGui::bit_clicked),&two,1));
	three.signal_clicked().connect(
		sigc::bind(sigc::mem_fun(*this, &BitEffectGui::bit_clicked),&three,2));
	four.signal_clicked().connect(
		sigc::bind(sigc::mem_fun(*this, &BitEffectGui::bit_clicked),&four,3));
	five.signal_clicked().connect(
		sigc::bind(sigc::mem_fun(*this, &BitEffectGui::bit_clicked),&five,4));
	six.signal_clicked().connect(
		sigc::bind(sigc::mem_fun(*this, &BitEffectGui::bit_clicked),&six,5));
	seven.signal_clicked().connect(
		sigc::bind(sigc::mem_fun(*this, &BitEffectGui::bit_clicked),&seven,6));
	eight.signal_clicked().connect(
		sigc::bind(sigc::mem_fun(*this, &BitEffectGui::bit_clicked),&eight,7));
}

void BitEffectGui::on_offset_changed() {
	effect->offset = offset_spinner.get_value();
}

void BitEffectGui::on_murder_changed() {
	effect->quant = murder_scale.get_value();
}

void BitEffectGui::bit_clicked(Gtk::Button* button, int i) {
	std::string title = button->get_label();
	int pos = title.find_first_of(".");
	title = title.substr(0,pos);
	button->set_label(title + "." + get_description_from_twiddle(twiddle(i)));
}

int BitEffectGui::twiddle(int i) {
	bool off = effect->off[i];
	bool flip = effect->flip[i];
	if (off){
		effect->off[i] = 0;
		effect->flip[i] = 1;
		return 2;
	} else if (flip) {
		effect->flip[i] = 0;
		return 0;
	} else {
		effect->off[i] = 1;
		return 1;
	}
}

std::string BitEffectGui::get_description_from_twiddle(int result) {
	switch (result) {
		case 1:
			return "(off)";
			break;
		case 2:
			return "(flip)";
			break;
		default:
			return "";
			break;
	}
	
	return "";
}
