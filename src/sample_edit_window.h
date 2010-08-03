#ifndef _SAMPLE_EDIT_WINDOW_H_
#define _SAMPLE_EDIT_WINDOW_H_

#include <gtkmm.h>

#include <gtk/gtk.h>
#include <stdlib.h>
#include <iostream>

#include <SDL/SDL.h>
#include <GL/gl.h>

#include "sample_choice_model.h"

class SampleEditWindow : public Gtk::Table
{
public:
    SampleEditWindow(SampleChoiceModel* samples);
    
    void run();
    void init_gfx();
    bool running;
private:
    void on_button_clicked();
   void update();
   void draw_sample(Sample* sample);
   void on_sample_choice_changed();
    
   Gtk::ComboBox sample_choice; 
    Gtk::Button button;
    SDL_Surface *surface;

    bool already_changed;
    
    int win_height;
   int win_width;
   
   pthread_t gl_thread;
   
   SampleChoiceModel* samples;
   Sample* sample;
};

void* run_edit_window(void* args);

#endif
