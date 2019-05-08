/*
 * EventQueue.cpp
 *
 *  Created on: 1 Feb 2018
 *      Author: Arvi
 */

#include "EventQueue.h"

EventQueue::EventQueue() {


}

EventQueue::~EventQueue() {

}

void EventQueue::publish(const Event& e) {
	eq.push(e);
}

Event EventQueue::consume() {
	if (eq.empty()) {
		return Event(Event::Error);
	}
	Event e(eq.front());
	eq.pop();
	return e;
}
bool EventQueue::pending() const {
	return !eq.empty();
}
