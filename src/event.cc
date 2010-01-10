#include "event.h"

bool Event::sort(Event a, Event b) {
	return (a.position < b.position);
}
