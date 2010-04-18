#include "sample_edit_window.h"

SampleEditWindow::SampleEditWindow() :
	Gtk::Table(2,1),
	button("Change color")
{
	already_changed = false;
	ClutterColor stage_color = { 0x00, 0x00, 0x00, 0xff }; /* Black */

	attach(button, 0,1,1,2);
	
	button.signal_clicked().connect(sigc::mem_fun(*this,
              &SampleEditWindow::on_button_clicked));
	

	/* Create the clutter widget: */
	Gtk::Widget *clutter_widget = Glib::wrap(gtk_clutter_embed_new ());
	attach(*clutter_widget, 0,1,0,1);
	
	/* Set the size of the widget, 
	* because we should not set the size of its stage when using GtkClutterEmbed.
	*/ 
	//clutter_widget->set_size_request(200,200);
	
	/* Get the stage and set its size and color: */
	//stage = gtk_clutter_embed_get_stage (GTK_CLUTTER_EMBED (clutter_widget->gobj()));
	//clutter_stage_set_color (CLUTTER_STAGE (stage), &stage_color);

	/* Show the stage: */
	//clutter_actor_show (stage);

	/* Connect a signal handler to handle mouse clicks and key presses on the stage: */ 
	//g_signal_connect (stage, "button-press-event",
	//G_CALLBACK (on_stage_button_press), NULL);
	
	show_all();
}

void SampleEditWindow::on_button_clicked ()
{
  if(already_changed)
  {
    //~ ClutterColor stage_color = { 0x00, 0x00, 0x00, 0xff }; /* Black */
    //~ clutter_stage_set_color (CLUTTER_STAGE (stage), &stage_color);
  }
  else
  {
    //~ ClutterColor stage_color = { 0x20, 0x20, 0xA0, 0xff };
    //~ clutter_stage_set_color (CLUTTER_STAGE (stage), &stage_color);
  }

  already_changed = !already_changed;
}

static gboolean on_stage_button_press (ClutterStage *stage, ClutterEvent *event, gpointer user_data) {
  gfloat x = 0;
  gfloat y = 0;
  clutter_event_get_coords (event, &x, &y);

  g_print ("Stage clicked at (%f, %f)\n", x, y);

  return TRUE; /* Stop further handling of this event. */
}
