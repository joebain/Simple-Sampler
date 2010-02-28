#include "gui_client.h"

#include <iostream>

#include <gtkmm.h>

#include "pad_gui.h"
#include "sample_choice_model.h"

GuiClient::GuiClient(Server* server) : Client(server) {
		
	//window settings
	set_border_width(10);
	
	//put everything together
	add(v_paned);
	v_paned.add1(sample_table);
	v_paned.add2(v_box);
	
	v_box.pack_start(button_box);
	v_box.pack_start(scrolled_window);
	v_box.pack_start(sample_frame);
	
	button_box.add(add_sample_button);
	button_box.add(load_pad_config_button);

	scrolled_window.add(message_window);
	
	sample_frame.add(sample_list_view);

	//init all the widgets
	add_sample_button.set_label("Add sample");
	add_sample_button.signal_clicked().connect(sigc::mem_fun(*this,
              &GuiClient::on_add_sample_button_clicked));
	
	load_pad_config_button.set_label("Load pads");
	load_pad_config_button.signal_clicked().connect(sigc::mem_fun(*this,
              &GuiClient::on_load_pad_config_button_clicked));

	scrolled_window.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	
	sample_frame.set_label("Samples");
	
	sample_table.set_spacings(10);

	//show everything
	show_all_children();
	
	load_samples("data/samples.xml");
	load_pads("data/controller.xml");
	link_pads_to_samples("data/pads_to_samples.xml");
		
	init();
}

GuiClient::~GuiClient() {
	for (std::list<PadGui*>::iterator pi = pad_guis.begin() ;
			pi != pad_guis.end() ; ++pi) {
		delete *pi;
	}
	pad_guis.clear();
	delete choice_model;
}

void GuiClient::start() {
	Client::start();
	
	Gtk::Main::run(*this);
	
	//when main exits
	server->stop();
}

void GuiClient::on_load_pad_config_button_clicked() {
	Gtk::FileChooserDialog dialog(
		"Please choose a file",
		Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.set_transient_for(*this);

	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

	Gtk::FileFilter filter;
	filter.set_name("Pad files");
	filter.add_mime_type("text/xml");
	dialog.add_filter(filter);
	
	Gtk::FileFilter any_filter;
	any_filter.set_name("Any files");
	any_filter.add_pattern("*");
	dialog.add_filter(any_filter);

	if (dialog.run() == Gtk::RESPONSE_OK) {
		load_pads(dialog.get_filename());
		refresh();
		message_window.get_buffer()->insert_at_cursor("Loaded pads\n");
	}
}

void GuiClient::on_add_sample_button_clicked() {
	Gtk::FileChooserDialog dialog(
		"Please choose a file",
		Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.set_transient_for(*this);

	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);
	
	Gtk::FileFilter audio_filter;
	audio_filter.set_name("Audio files");
	audio_filter.add_mime_type("audio/x-wav");
	audio_filter.add_mime_type("audio/x-aiff");
	audio_filter.add_mime_type("audio/basic");
	audio_filter.add_mime_type("audio/mpeg");
	audio_filter.add_mime_type("audio/ogg");
	audio_filter.add_mime_type("audio/flac");
	dialog.add_filter(audio_filter);
	
	Gtk::FileFilter any_filter;
	any_filter.set_name("Any files");
	any_filter.add_pattern("*");
	dialog.add_filter(any_filter);

	if (dialog.run() == Gtk::RESPONSE_OK) {
		if (load_sample(dialog.get_filename())) {
			refresh();
			message_window.get_buffer()->insert_at_cursor("Loaded sample (" + dialog.get_filename() + ")\n");
		}
	}
}

void GuiClient::on_sample_is_looping_toogled(const Glib::ustring& path) {
	std::cout << "path: " << path << std::endl;
	Gtk::TreeModel::iterator iter = sample_list_view.get_model()->get_iter(path);
	if(iter) {
		Gtk::TreeModel::Row row = *iter;
		if(row)	{
			Sample* sample = row[choice_model->sample_column];
			bool looping_ticked = row[choice_model->is_looping_column];
			sample->set_looping(looping_ticked);
			
			std::cout << "ticking: " << (looping_ticked?"true":"false") << std::endl;
		}
	} else {
		std::cout << "no selection" << std::endl;
	}
}

void GuiClient::init() {
	//sample model
	choice_model = new SampleChoiceModel (server->get_samples());
	
	//main sample list
	sample_list_view.set_model(choice_model->ref_tree_model);
	sample_list_view.append_column("name", choice_model->name_column);
	
	int view_column = sample_list_view.append_column_editable("loop?", choice_model->is_looping_column);	
	Gtk::CellRenderer *renderer = sample_list_view.get_column_cell_renderer(view_column - 1);
	Gtk::CellRendererToggle *toggle_renderer =
		dynamic_cast<Gtk::CellRendererToggle *>(renderer);
	if (toggle_renderer)
	{
		toggle_renderer->signal_toggled().connect
			( sigc::mem_fun(*this, &GuiClient::on_sample_is_looping_toogled) );
	}

	//pads
	std::list<Pad> & pads = server->get_pads();
	for (std::list<Pad>::iterator pi = pads.begin() ;
			pi != pads.end() ; ++pi) {
		
		PadGui* pad_gui = new PadGui(*pi, choice_model);
		pad_guis.push_back(pad_gui);
		int x = pi->get_x();
		int y = pi->get_y();
		sample_table.attach(*(pad_guis.back()),x,x+1,y,y+1);
	}
	
	show_all_children();
}

void GuiClient::refresh() {
	//refresh model
	choice_model->set_samples(server->get_samples());
	
	//refresh pads
	for (std::list<PadGui*>::iterator pi = pad_guis.begin() ;
			pi != pad_guis.end() ; ++pi) {
		(*pi)->refresh();		
	}
}

void GuiClient::init_gtk(int argc, char *argv[]) {
	Gtk::RC::add_default_file("data/themes/Elegant Brit/gtk-2.0/gtkrc");
	Gtk::Main kit(argc, argv);
}
