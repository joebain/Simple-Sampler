#ifndef _SAMPLE_EDIT_WINDOW_H_
#define _SAMPLE_EDIT_WINDOW_H_

#include <gtkmm.h>

#include <gtk/gtk.h>
#include <clutter/clutter.h>
#include <clutter-gtk/clutter-gtk.h>
#include <stdlib.h>

class SampleEditWindow : public Gtk::Table
{
public:
    SampleEditWindow();
private:
    ClutterActor *stage;
    void on_button_clicked();

    Gtk::Button button;

    bool already_changed;

};

static gboolean on_stage_button_press (ClutterStage *stage, ClutterEvent *event, gpointer user_data);

#endif
