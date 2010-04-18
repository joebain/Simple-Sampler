#include "sample_edit_window.h"

#include <iostream>

SampleEditWindow::SampleEditWindow() :
	Gtk::VBox(false, 6),
	button("Change colour")
{
	pack_end(button, Gtk::PACK_SHRINK);

	pack_start(embed_, Gtk::PACK_EXPAND_WIDGET);
 	embed_.set_size_request(200, 200);

	button.signal_clicked().connect(sigc::mem_fun(*this, &SampleEditWindow::on_button_clicked));

	stage_ = embed_.get_stage();
	stage_->reference();
	stage_->set_color(Clutter::Color(0, 0, 0)); // black
	stage_->signal_button_press_event().connect(&SampleEditWindow::on_stage_button_press);

	show_all();
	stage_->show();
}

bool SampleEditWindow::on_stage_button_press(Clutter::ButtonEvent* event)
{
	float x = 0;
	float y = 0;
	// TODO: Wrap properly
	clutter_event_get_coords(reinterpret_cast<Clutter::Event*>(event), &x, &y);

	std::cout << "Stage clicked at (" << x << ", " << y << ")\n";

	return true; // stop further handling of this event
}


void SampleEditWindow::on_button_clicked ()
{
	colored_ = !colored_;
	stage_->set_color((colored_) ? Clutter::Color(32, 32, 160)
		: Clutter::Color(0, 0, 0));
}
