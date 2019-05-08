/*
 * Event.h
 *
 *  Created on: 1 Feb 2018
 *      Author: Arvi
 */

#ifndef EVENT_H_
#define EVENT_H_

#include <atomic>

class Event {
public:
	enum eventType { Enter, Exit, Key, Tick, Error };
	Event(eventType t = Tick, int v = 0) : type(t), value(v) {}
	virtual ~Event() {}
	eventType type;
	int value;
};

#endif /* EVENT_H_ */
