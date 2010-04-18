#ifndef STAGE_WINDOW_H
#define STAGE_WINDOW_H


#include <gtkmm.h>
#include <clutter-gtkmm.h>
#include <iostream>


class StageWindow : public Gtk::VBox
{
	public:
  StageWindow();
  virtual ~StageWindow();

private:
  Clutter::Gtk::Embed* embed_;
  Glib::RefPtr<Clutter::Stage> stage_;
  bool colored_;

  void on_button_clicked();
  static bool on_stage_button_press(Clutter::ButtonEvent* event);
};

#endif // STAGE_WINDOW_H
