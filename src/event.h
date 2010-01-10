#ifndef _EVENT_H_
#define _EVENT_H_

class Event {
	public:
		int pad_id;
		int velocity;
		bool on;
		float position;

		static bool sort(Event a, Event b);
};

#endif
