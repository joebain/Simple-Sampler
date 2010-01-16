#ifndef _PAD_H_
#define _PAD_H_

class Pad {
	private:
		int event_number;
		int id;
		int x,y;
	public:
		Pad() { Pad(0,0); }
		Pad(int event_number, int id);
		int get_event_number() { return event_number; }
		int get_id() { return id; }
		void set_pos(int x, int y) { this->x = x; this->y = y; }
		int get_x() { return x; }
		int get_y() { return y; }
};

#endif
