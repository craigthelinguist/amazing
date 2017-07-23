

#include "event_q.h"

#include <stdlib.h>
#include <stdio.h>

#include <time.h>

struct Event_Q {
	int16_t head;
	int16_t tail;
	int16_t size;
	Event buffer[QUEUE_SIZE];
};


Event_Q make_eventq(void) {
	
	// Allocate memory for the event queue.
	struct Event_Q *eq = malloc (sizeof (struct Event_Q));
	
	// Initialise its fields.
	eq->head = 0;
	eq->tail = 0;
	eq->size = 0;
	
	// Zero out all of the memory.
	for (int i = 0; i < QUEUE_SIZE; i++) {
		eq->buffer[i] = (const Event){0};
	}
	
	return eq;
	
}

void free_eventq(Event_Q eq) {
	free(eq);
	eq = NULL;
}

void push_event(Event_Q eq, Event_Type event) {
	
	// Get a reference to the correct struct in the array.
	// Then write new details to it.
	Event *ev = &eq->buffer[eq->tail];
	time(&ev->time);
	ev->type = event;
	
	// Update indices. If it wraps around on itself, don't
	// update size, but move the head along.
	eq->tail = (eq->tail + 1) % QUEUE_SIZE;
	if (eq->size == QUEUE_SIZE) {
		eq->head = (eq->head + 1) % QUEUE_SIZE;
	}
	else {
		eq->size++;
	}
	
}

Event pop_event(Event_Q eq) {
	
	// When there are no events in the queue.
	if (!eq->size)
		return make_event(NO_EVENT);

	// Copy the event to a new struct. Zero out that
	// position in the array.
	Event ev = eq->buffer[eq->head];
	eq->buffer[eq->head] = (const Event){0};
	
	// Adjust indices, wrapping around if needbe.
	eq->head = (eq->head + 1) % QUEUE_SIZE;
	eq->size--;

	return ev;
	
}








