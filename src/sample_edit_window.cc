#include "sample_edit_window.h"

SampleEditWindow::SampleEditWindow(SampleChoiceModel* samples, Server* server) :
	Gtk::Table(2,1),
	button("Change color")
{
   sample = NULL;
    click_distance = 6;
    view_offset = 50;
   
    is_dragging_pad = false;
   dragged_pad = NULL;
   drag_location_y = 0.0f;
   
   this->samples = samples;
   this->server = server;
   
   //combo box
   attach(sample_choice, 0,1,0,1);
   
   sample_choice.set_model(samples->ref_tree_model);
   sample_choice.pack_start(samples->name_column);
	sample_choice.signal_changed().connect(
			sigc::mem_fun(*this, &SampleEditWindow::on_sample_choice_changed)
			);
   
   //a button
	attach(button, 0,1,1,2);
	
	button.signal_clicked().connect(sigc::mem_fun(*this,
              &SampleEditWindow::on_button_clicked));
         
	show_all();
   
   pthread_create( &gl_thread, NULL, run_edit_window, this);
}

void SampleEditWindow::on_sample_choice_changed() {
   Gtk::TreeModel::iterator iter = sample_choice.get_active();
	if(!iter) return;
	Gtk::TreeModel::Row row = *iter;
	if(!row) return;
	
	sample = row[samples->sample_column];
}

void SampleEditWindow::init_gfx() {
   if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		fprintf(stderr,"Couldn't initialize SDL: %s\n",SDL_GetError());
   }
   
   if(!SDL_WasInit(SDL_INIT_EVERYTHING)&SDL_INIT_VIDEO) {
     printf("Video is not initialized.\n");
  }


   //SDL_WM_SetIcon(SDL_LoadBMP(("img/" + win_icon).c_str()), NULL);
   std::string win_name = "Splice";
   SDL_WM_SetCaption(win_name.c_str(), win_name.c_str());

   const SDL_VideoInfo *videoInfo = SDL_GetVideoInfo( );

   int videoFlags;
    /* the flags to pass to SDL_SetVideoMode */
    videoFlags  = SDL_OPENGL;          /* Enable OpenGL in SDL */
    videoFlags |= SDL_GL_DOUBLEBUFFER; /* Enable double buffering */
    videoFlags |= SDL_HWPALETTE;       /* Store the palette in hardware */

    /* This checks to see if surfaces can be stored in memory */
    if (videoInfo->hw_available)
    videoFlags |= SDL_HWSURFACE;
    else
    videoFlags |= SDL_SWSURFACE;

    /* This checks if hardware blits can be done */
    if (videoInfo->blit_hw)
    videoFlags |= SDL_HWACCEL;

    /* Sets up OpenGL double buffering */
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    /* get a SDL surface */
   int screen_bpp = 32;
   win_height = 200;
   win_width = 800;
   surface = SDL_SetVideoMode(win_width, win_height, screen_bpp, videoFlags);
}

void* run_edit_window(void* args) {
   SampleEditWindow* window = (SampleEditWindow*)args;
   window->init_gfx();
   window->run();
   return NULL;
}

void SampleEditWindow::run() {
   running = true;
   while (running) {
      SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch(event.type){
				case SDL_QUIT:
               running = false;
               break;
               case SDL_MOUSEBUTTONDOWN:
               if (event.button.button == SDL_BUTTON_LEFT) {
                   on_mouse_down(event.button.x, event.button.y);
               }
               break;
               case SDL_MOUSEBUTTONUP:
               if (event.button.button == SDL_BUTTON_LEFT) {
                   on_mouse_up(event.button.x, event.button.y);
               }
               break;
               case SDL_MOUSEMOTION:
               on_mouse_drag(event.motion.x, event.motion.y);
               break;
         }
      }
      
      update();
      
      /* Check for error conditions. */
		GLenum gl_error = glGetError( );

		if( gl_error != GL_NO_ERROR ) {
			fprintf( stderr, "OpenGL error: %d\n", gl_error );
		}

		char* sdl_error = SDL_GetError( );

		if( sdl_error[0] != '\0' ) {
			fprintf(stderr, "SDL error '%s'\n", sdl_error);
			SDL_ClearError();
		}
      
      usleep(10);
   }
}

void SampleEditWindow::update() {
   
   glClearColor(1.0f,1.0f,1.0f,1.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();
   
   glLoadIdentity();
   
   float scale = ((float)win_width-view_offset*2.0f)/(float)win_width;
   float offset = screen_width_to_gl(view_offset);
   
   glTranslatef(-1.0f,-1.0f,0.0f);
   glScalef(2.0f,2.0f,1.0f);
   
   
   glScalef(scale,1.0f,1.0f);
   glTranslatef(offset,0.0f,0.0f);
   
   glColor3f(0.5f,0.5f,0.5f);
   
   glBegin(GL_TRIANGLE_STRIP);
   glVertex2f(-offset, 0.0f);
   glVertex2f(-offset, 1.0f);
   glVertex2f(0.0f, 0.0f);
   glVertex2f(0.0f, 1.0f);
   glEnd();
   
   glBegin(GL_TRIANGLE_STRIP);
   glVertex2f(1.0f+offset, 0.0f);
   glVertex2f(1.0f+offset, 1.0f);
   glVertex2f(1.0f, 0.0f);
   glVertex2f(1.0f, 1.0f);
   glEnd();
   
   if (sample != NULL) {
      draw_sample(sample);
   }
   
   glPopMatrix();
   SDL_GL_SwapBuffers();
}

void SampleEditWindow::draw_sample(Sample* sample) {
   glColor3f(1.0f,0.0f,0.0f);
   glBegin(GL_POINTS);
   int length = sample->get_length();
   float spacing = 1.0f / length;
   const float* data = sample->get_frames();
   for(int i = 0; i < length; i+=5) {
      glVertex2f(i*spacing, data[i]/2.0f + 0.5f);
   }
   glEnd();
   
   glBegin(GL_LINES);
   glColor3f(0.0f,0.0f,1.0f);
   float pos = sample->get_position();
   glVertex2f(pos, 0.0f);
   glVertex2f(pos, 1.0f);
   glEnd();
   
   float fin_width = screen_width_to_gl(10);
   float fin_height = screen_height_to_gl(10);
   
   std::list<Pad*> pads = server->get_pads_for_sample(sample);
    for (std::list<Pad*>::iterator pad = pads.begin()
            ; pad != pads.end() ; ++pad) {
        
        float start_pos = (*pad)->start_position;
        float end_pos = (*pad)->end_position;
        
        glBegin(GL_LINES);
        
        glColor3f(0.0f,1.0f,0.0f);
        glVertex2f(start_pos, 1.0f);
        glVertex2f(start_pos, 0.5f);
        
        glColor3f(1.0f,0.0f,1.0f);
        glVertex2f(end_pos, 0.5f);
        glVertex2f(end_pos, 0.0f);
        
        glEnd();
        
        glBegin(GL_TRIANGLES);
        glColor3f(0.0f,1.0f,0.0f);
        glVertex2f(start_pos + fin_width, 1.0f);
        glVertex2f(start_pos, 1.0f);
        glVertex2f(start_pos, 1.0f - fin_height);
        
        glVertex2f(start_pos, 0.5f);
        glVertex2f(start_pos + fin_width, 0.5f);
        glVertex2f(start_pos, 0.5f + fin_height);
        
        glColor3f(1.0f,0.0f,1.0f);
        
        glVertex2f(end_pos - fin_width, 0.5f);
        glVertex2f(end_pos, 0.5f);
        glVertex2f(end_pos, 0.5f - fin_height);
        
        glVertex2f(end_pos, 0.0f);
        glVertex2f(end_pos - fin_width, 0.0f);
        glVertex2f(end_pos, 0.0f + fin_height);
        
        glEnd();
    }
}

float SampleEditWindow::screen_x_to_gl(int x) {
    return (float)(x-view_offset)/(win_width-view_offset*2);
}

float SampleEditWindow::screen_y_to_gl(int y) {
    return (float)y/win_height;
}

int SampleEditWindow::gl_width_to_screen(float w) {
    return round(w * (win_width-view_offset*2));
}

float SampleEditWindow::screen_width_to_gl(int w) {
    return (float)w / (win_width-view_offset*2);
}

float SampleEditWindow::screen_height_to_gl(int h) {
    return (float)h / win_height;
}

Pad* SampleEditWindow::clicked_on_pad(int x, int y) {
    std::list<Pad*> pads = server->get_pads_for_sample(sample);
    float x_pos = screen_x_to_gl(x);
    float y_pos = screen_y_to_gl(y);
    for (std::list<Pad*>::iterator pad = pads.begin()
            ; pad != pads.end() ; ++pad) {
        if (y_pos < 0.5f) {
            if (fabs(x_pos - (*pad)->start_position) < screen_width_to_gl(click_distance)) {
                return *pad;
            }
        } else {
            if (fabs(x_pos - (*pad)->end_position) < screen_width_to_gl(click_distance)) {
                return *pad;
            }
        }
    }
    return NULL;
}

void SampleEditWindow::on_mouse_down(int x, int y) {    
    if (sample != NULL) {
        Pad* clicked_pad = clicked_on_pad(x, y);
        if (clicked_pad != NULL) {
            is_dragging_pad = true;
            dragged_pad = clicked_pad;
            drag_location_y = screen_y_to_gl(y);
        } else {
            sample->play(screen_x_to_gl(x));
        }
    }
}

void SampleEditWindow::on_mouse_drag(int x, int y) {
    float x_pos = screen_x_to_gl(x);
    if (is_dragging_pad && x_pos >= 0.0f && x_pos <= 1.0f) {
        if (drag_location_y < 0.5f) {
            dragged_pad->start_position = x_pos;
        } else {
            dragged_pad->end_position = x_pos;
        }
    }
}

void SampleEditWindow::on_mouse_up(int x, int y) {
    if (is_dragging_pad) {
        is_dragging_pad = false;
        dragged_pad->hit(1.0f);
        dragged_pad->release();
        dragged_pad = NULL;
    }
}

void SampleEditWindow::on_button_clicked ()
{
  if(already_changed)
  {
    
  }
  else
  {
    
  }

  already_changed = !already_changed;
}
