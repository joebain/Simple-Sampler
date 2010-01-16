#include "sample_choice_model.h"

SampleChoiceModel::SampleChoiceModel(std::list<Sample> & samples) {
	add(id_column);
	add(name_column);
	
	ref_tree_model = Gtk::ListStore::create(*this);
	
	set_samples(samples);	
}

void SampleChoiceModel::set_samples(std::list<Sample> & samples) {
	ref_tree_model->clear();
	for (std::list<Sample>::iterator si = samples.begin() ;
			si != samples.end() ; ++si) {
		Gtk::TreeModel::Row row = *(ref_tree_model->append());
		row[id_column] = si->id;
		row[name_column] = si->get_name();
	}
}
