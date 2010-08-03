#include "sample_edit_window.h"

SampleEditWindow::SampleEditWindow(SampleChoiceModel* samples) :
	Gtk::Table(2,1),
	button("Change color")
{
   sample = NULL;
   
   this->samples = samples;
   
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
   win_height = 300;
   win_width = 300;
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
      
      sleep(1.0);
   }
}

void SampleEditWindow::update() {
   
   glClearColor(1.0f,1.0f,1.0f,1.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();
   
   glLoadIdentity();
   
   glTranslatef(-1.0f,-1.0f,0.0f);
   glScalef(2.0f,2.0f,0.0f);
   
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
   float spacing = 2.0f / length;
   const float* data = sample->get_frames();
   for(int i = 0; i < length; i++) {
      glVertex2f(i*spacing, data[i]/2.0f + 0.5f);
   }
   glEnd();
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
