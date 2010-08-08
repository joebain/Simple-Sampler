#include "sample_choice_model.h"

SampleChoiceModel::SampleChoiceModel(std::list<Sample> & samples) {
	add(sample_column);
	add(name_column);
	add(is_looping_column);
	add(has_effect_column);
	add(has_timestretch_column);
    add(is_recording_column);
	
	ref_tree_model = Gtk::ListStore::create(*this);
	
	
	set_samples(samples);	
}

void SampleChoiceModel::set_samples(std::list<Sample> & samples) {
	ref_tree_model->clear();
	for (std::list<Sample>::iterator si = samples.begin() ;
			si != samples.end() ; ++si) {
		add_sample(&(*si));
	}
}

void SampleChoiceModel::add_sample(Sample* sample) {
	Gtk::TreeModel::Row row = *(ref_tree_model->append());
	row[sample_column] = sample;
	row[name_column] = sample->get_name();
	row[is_looping_column] = sample->sticky_loops;
	row[has_effect_column] = sample->effect_enabled();
	row[has_timestretch_column] = sample->timestretch_enabled();
    row[is_recording_column] = sample->is_recording();
}
