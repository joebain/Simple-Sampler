#ifndef _PAD_H_
#define _PAD_H_

class Pad {
	private:
		int event_number;
		int id;
	public:
		Pad(int event_number, int id);
		int get_event_number() { return event_number; }
		int get_id() { return id; }
};

#endif
