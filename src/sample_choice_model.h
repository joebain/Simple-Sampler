#ifndef _SAMPLE_CHOICE_MODEL_H_
#define _SAMPLE_CHOICE_MODEL_H_

#include <list>

#include <gtkmm.h>

#include "sample.h"

class SampleChoiceModel : public Gtk::TreeModel::ColumnRecord {
	public:
		SampleChoiceModel(std::list<Sample> & samples);
		
		Glib::RefPtr<Gtk::ListStore> ref_tree_model;
		
		Gtk::TreeModelColumn<Sample*> sample_column;
		Gtk::TreeModelColumn<Glib::ustring> name_column;
		
		void set_samples(std::list<Sample> & samples);
		void add_sample(Sample* sample);
};


#endif
