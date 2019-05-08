/*
 * FanControl.h
 *
 *  Created on: 5 Mar 2018
 *      Author: Arvi
 */

#ifndef FANCONTROL_H_
#define FANCONTROL_H_

#include "ModbusMaster.h"
#include "I2C.h"
#include "Sleep.h"

class FanControl {
public:
	FanControl(I2C& i2c0, ModbusMaster& node0);
	virtual ~FanControl();
	bool setFrequency(uint16_t freq0);
	void printRegister(uint16_t reg);
	bool autoAdjustFrequency(uint16_t target);
	FanControl& operator++(); //prefix
	FanControl operator++(int);
	FanControl& operator--(); //prefix
	FanControl operator--(int);
	uint16_t getFrequency() const;
	uint16_t getPercentage() const;
	int16_t getMedianPressure() const;
private:
	int16_t getPressure() const;
	I2C& i2c;
	ModbusMaster& node;
	const uint16_t maxFreq = 20000;
	const uint16_t threshold = 30; //when pressure difference is larger than this, autoAdjustFrequency uses quickChange
	const uint16_t quickChange = 4; //autoAdjustFrequency uses this percentage to change when the pressure difference is larger than threshold
};

#endif /* FANCONTROL_H_ */
