

#include "event.h"

Event make_event(Event_Type type) {
	Event ev;
	time(&ev.time);
	ev.type = type;
	return ev;
}


const char *event_to_str(Event_Type type) {
	switch(type) {
		case NO_EVENT: return "No event";
		case MOVE_UP: return "Move Up";
		case MOVE_DOWN: return "Move Down";
		case MOVE_LEFT: return "Move Left";
		case MOVE_RIGHT: return "Move Right";
		default: return "ERR: Unknown Event";
	}
}
