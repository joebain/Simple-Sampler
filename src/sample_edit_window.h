#ifndef _SAMPLE_EDIT_WINDOW_H_
#define _SAMPLE_EDIT_WINDOW_H_

#include <gtkmm.h>

#include <cluttermm.h>
#include <clutter-gtkmm.h>
#include <stdlib.h>

class SampleEditWindow : public Gtk::VBox
{
public:
    SampleEditWindow();
private:
    Clutter::Gtk::Embed embed_;
	Glib::RefPtr<Clutter::Stage> stage_;
	Gtk::Button button;
	bool colored_;

	void on_button_clicked();
	static bool on_stage_button_press(Clutter::ButtonEvent* event);

};

#endif
