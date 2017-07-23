


#ifndef EVENT_H
#define EVENT_H

#include <time.h>

enum Event_Type {
	NO_EVENT,
	MOVE_UP,
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_DOWN
};

typedef enum Event_Type Event_Type;

struct Event {
	time_t time;
	Event_Type type;
};
	
typedef struct Event Event;

Event make_event(Event_Type type);

const char *event_to_str(Event_Type type);

#endif