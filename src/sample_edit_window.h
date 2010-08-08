#ifndef _SAMPLE_EDIT_WINDOW_H_
#define _SAMPLE_EDIT_WINDOW_H_

#include <gtkmm.h>

#include <gtk/gtk.h>
#include <stdlib.h>
#include <iostream>

#include <SDL/SDL.h>
#include <GL/gl.h>

#include "sample_choice_model.h"
#include "server.h"

class SampleEditWindow : public Gtk::Table
{
public:
    SampleEditWindow(SampleChoiceModel* samples, Server* server);
    
    void run();
    void init_gfx();
    bool running;
private:
    void on_button_clicked();
    void on_mouse_down(int x, int y);
    void on_mouse_up(int x, int y);
    void on_mouse_drag(int x, int y);
   void update();
   void draw_sample(Sample* sample);
   void on_sample_choice_changed();
    
    float screen_x_to_gl(int x);
    float screen_y_to_gl(int y);
    float screen_width_to_gl(int w);
    float screen_height_to_gl(int h);
    int gl_width_to_screen(float w);
    
    Pad* clicked_on_pad(int x, int y);
    
   Gtk::ComboBox sample_choice; 
    Gtk::Button button;
    SDL_Surface *surface;

    bool already_changed;
    
    int win_height;
   int win_width;
    int click_distance;
    int view_offset;
    float view_scale;
   
   pthread_t gl_thread;
   
   SampleChoiceModel* samples;
   Sample* sample;
   Server* server;
   
   bool is_dragging_pad;
   float drag_location_y;
   Pad* dragged_pad;
};

void* run_edit_window(void* args);

#endif
