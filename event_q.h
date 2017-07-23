

/*
	A data structure for storing events as they are received.
	Implemented as a circular buffer, so if events are not
	processed quickly enough, the new events overwrite the
	old events.
*/

#ifndef EVENT_Q_H
#define EVENT_Q_H

	#include "event.h"

	// Maximum number of events that can be stored.
	#define QUEUE_SIZE 50
	
	typedef struct Event_Q *Event_Q;
	
	// Constructing an event queue.
	Event_Q make_eventq(void);
	void free_eventq(Event_Q eq);
	
	// Pushing and popping events.
	void push_event(Event_Q eq, Event_Type ev);
	Event pop_event(Event_Q eq);

#endif

