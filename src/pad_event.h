#ifndef _PAD_EVENT_H_
#define _PAD_EVENT_H_

class PadEvent {
	public:
		int pad_id;
		int velocity;
		bool on;
		float position;
};

#endif
