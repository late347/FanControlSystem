/*
 * DigitalloPin.h
 *
 *  Created on: 23 Jan 2018
 *      Author: Arvi
 */

#ifndef DIGITALIOPIN_H_
#define DIGITALIOPIN_H_

#include "board.h"
#include "chip.h"
#include <string>

using namespace std;

class DigitalIoPin {
public:
	DigitalIoPin();
	DigitalIoPin(int port0, int pin0, string name0="", bool input0 = false, bool pullup0 = false, bool invert0 = false);
	DigitalIoPin& operator=(const DigitalIoPin& pin);
	void init(int port0, int pin0, string name0="", bool input0 = false, bool pullup0 = false, bool invert0 = false);
	virtual ~DigitalIoPin();
	bool read() const;
	void write(bool value);
	string getName() const;
private:
	string name;
	int port;
	int pin;
	bool input;
	bool pullup;
	bool invert;
};

#endif /* DIGITALIOPIN_H_ */
