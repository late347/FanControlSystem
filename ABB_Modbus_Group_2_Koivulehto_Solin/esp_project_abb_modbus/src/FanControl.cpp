/*
 * FanControl.cpp
 *
 *  Created on: 5 Mar 2018
 *      Author: Arvi
 */

#include "FanControl.h"
#include <cstdlib>
#include <vector>
#include <algorithm>

using namespace std;

FanControl::FanControl(I2C& i2c0, ModbusMaster& node0) : i2c(i2c0), node(node0) {

	node.begin(9600); // set transmission rate - other parameters are set inside the object and can't be changed here

	node.writeSingleRegister(0, 0x0406); // prepare for starting

	Sleep(1000); // give converter some time to set up
	// note: we should have a startup state machine that check converter status and acts per current status
	//       but we take the easy way out and just wait a while and hope that everything goes well

	node.writeSingleRegister(0, 0x047F); // set drive to start mode

	Sleep(1000); // give converter some time to set up
	// note: we should have a startup state machine that check converter status and acts per current status
	//       but we take the easy way out and just wait a while and hope that everything goes well
}

FanControl::~FanControl() {

}


bool FanControl::setFrequency(uint16_t freq0) {
	auto val = freq0;
	if (val > maxFreq) {
		val = maxFreq;
	}
	uint8_t result;
	int ctr;
	bool atSetpoint;
	const int delay = 500;

	node.writeSingleRegister(1, val); // set motor frequency

	// wait until we reach set point or timeout occurs
	ctr = 0;
	atSetpoint = false;
	do {
		Sleep(delay);
		// read status word
		result = node.readHoldingRegisters(3, 1);
		// check if we are at setpoint
		if (result == node.ku8MBSuccess) {
			if(node.getResponseBuffer(0) & 0x0100) atSetpoint = true;
		}
		ctr++;
	} while(ctr < 20 && !atSetpoint);

	return atSetpoint;
}

void FanControl::printRegister(uint16_t reg) {
	uint8_t result;
	// slave: read 16-bit registers starting at reg to RX buffer
	result = node.readHoldingRegisters(reg, 1);

	// do something with data if read is successful
	if (result == node.ku8MBSuccess)
	{
		printf("R%d=%04X\n", reg, node.getResponseBuffer(0));
	}
	else {
		printf("R%d=???\n", reg);
	}
}

bool FanControl::autoAdjustFrequency(uint16_t target) {
	bool result = false;
	auto pressure = getMedianPressure();
	auto freq = getFrequency();
	if (pressure < target) {
		if (target - pressure > threshold) {
			setFrequency(freq + maxFreq / 100 * quickChange);
		}
		else {
			++*this;
		}
	}
	else if (pressure > target) {
		if (pressure - target > threshold && freq >= maxFreq / 100 * quickChange) {
			setFrequency(freq - maxFreq / 100 * quickChange);
		}
		else {
			--*this;
		}
	}
	else {
		result = true;
	}
	return result;
}

FanControl& FanControl::operator++() {
	setFrequency(getFrequency() + maxFreq / 100);
	return *this;
}

FanControl FanControl::operator++(int) {
	auto result = *this;
	++*this;
	return result;
}

FanControl& FanControl::operator--() { //prefix
	uint16_t val = 0;
	if (getPercentage() < getFrequency()) {
		val = getFrequency() - maxFreq / 100;
	}
	setFrequency(val);
	return *this;
}

FanControl FanControl::operator--(int) {
	auto result = *this;
	--*this;
	return result;
}

uint16_t FanControl::getFrequency() const {
	uint8_t result;

	// slave: read (2) 16-bit registers starting at register 102 to RX buffer
	int j = 0;
	do {
		result = node.readHoldingRegisters(102, 2);
		j++;
	} while(j < 3 && result != node.ku8MBSuccess);
	// note: sometimes we don't succeed on first read so we try up to threee times
	// if read is successful print frequency and current (scaled values)
	if (result == node.ku8MBSuccess) {
		return node.getResponseBuffer(0);
	}
	return 0;
}

uint16_t FanControl::getPercentage() const {
	return uint16_t(double(getFrequency()) / maxFreq * 100);
}

int16_t FanControl::getMedianPressure() const{
	vector<int16_t> pressures;
	int size = 10;
	for (int i = 0; i < size; ++i) {
		pressures.push_back(getPressure());
	}
	sort(pressures.begin(), pressures.end());
	return pressures[size/2];
}

int16_t FanControl::getPressure() const {
	uint8_t pressureData[3];
	uint8_t readPressureCmd = 0xF1;
	int16_t pressure = 0;

	if (i2c.transaction(0x40, &readPressureCmd, 1, pressureData, 3)) {
		pressure = (pressureData[0] << 8) | pressureData[1];
	};
	Sleep(1); //to reduce the risk of I2C locking
	pressure = (pressure / 240) * 0.95 ;

	return pressure;
}
