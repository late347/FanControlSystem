/*
 * EventQueue.h
 *
 *  Created on: 1 Feb 2018
 *      Author: Arvi
 */

#ifndef EVENTQUEUE_H_
#define EVENTQUEUE_H_

#include "Event.h"
#include <queue>
using namespace std;

class EventQueue {
public:
	EventQueue();
	virtual ~EventQueue();
	void publish(const Event& e);
	Event consume();
	bool pending() const;
private:
	queue<Event> eq;
};

#endif /* EVENTQUEUE_H_ */
