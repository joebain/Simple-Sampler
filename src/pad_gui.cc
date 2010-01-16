#include "pad_gui.h"

PadGui::PadGui(Pad& pad) : pad(pad){
	pack_start(play_button);
	
	play_button.set_label("Play");
}
