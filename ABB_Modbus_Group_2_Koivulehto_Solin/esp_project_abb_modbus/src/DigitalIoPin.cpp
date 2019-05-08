/*
 * DigitalloPin.cpp
 *
 *  Created on: 23 Jan 2018
 *      Author: Arvi
 */

#include "DigitalIoPin.h"

DigitalIoPin::DigitalIoPin() {
	init(0, 0, "", false, false, false);
}

DigitalIoPin::DigitalIoPin(int port0, int pin0, string name0, bool input0, bool pullup0, bool invert0) {
	init(port0, pin0, name0, input0, pullup0, invert0);
}

DigitalIoPin& DigitalIoPin::operator=(const DigitalIoPin& pin) {
	if (this != &pin) {
		init(pin.port, pin.pin, pin.getName(), pin.input, pin.pullup, pin.invert);
	}

	return *this;
}

void DigitalIoPin::init(int port0, int pin0, string name0, bool input0, bool pullup0, bool invert0) {
	name = name0;
	port = port0;
	pin = pin0;
	input = input0;
	pullup = pullup0;
	invert = invert0;
	auto modes = IOCON_DIGMODE_EN;
	if (pullup) {
		modes |= IOCON_MODE_PULLUP;
	}
	if (invert) {
		modes |= IOCON_INV_EN;
	}
	if (!input) {
		modes |= IOCON_MODE_INACT;
	}
	Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin, modes); //sets IOCON

	if (input) {
		Chip_GPIO_SetPinDIRInput(LPC_GPIO, port, pin);
	}
	else {
		Chip_GPIO_SetPinDIROutput(LPC_GPIO, port, pin);
		Chip_GPIO_SetPinState(LPC_GPIO, port, pin, true); //default state is shutoff
	}
}

DigitalIoPin::~DigitalIoPin() {

}
bool DigitalIoPin::read() const {
	bool result = Chip_GPIO_GetPinState(LPC_GPIO, port, pin);
	return result;
}
void DigitalIoPin::write(bool value) {
	bool input = value;
	Chip_GPIO_SetPinState(LPC_GPIO, port, pin, input);
}

string DigitalIoPin::getName() const {
	return name;
}
