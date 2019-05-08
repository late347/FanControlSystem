/*
 * StateMachine.h
 *
 *  Created on: 1 Feb 2018
 *      Author: Arvi
 */

#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

#include "Event.h"
#include "I2C.h"
#include "LiquidCrystal.h"
#include "FanControl.h"
#include "LimitedCounter.h"

#include <string>

using namespace std;

class StateMachine {
public:
	StateMachine(FanControl& ctl0, LiquidCrystal& lcd0);
	void handleState(const Event& e);
	virtual ~StateMachine();
private:
	enum state { Start, Manual, AutoStart, AutoTry, AutoSuccess, AutoFailure };
	state current;
	int timer;
	int accuracyTimer; //used to stabilize the autotry before entering autosuccess
	const int tryTime = 120;
	const int lcdWidth = 16;
	const int lcdHeight = 2;
	const int testTicks = 3; //how many ticks autotry should wait to make sure the pressure is stable
	const int limit = 5; //acceptable fluctuation in pressure
	//refresh rates based on testing what seems to be the right rate
	const int manualRefresh = 50000; //how often UI refreshed in manual state
	const int autoStartRefresh = 50000; //how often UI refreshed in autostart state
	const int autoTryRefresh = 0; //how often UI refreshed in manual state
	const int autoSuccessRefresh = 10; //how often UI refreshed in manual state
	const int autoFailureRefresh = 10; //how often UI refreshed in manual state
	FanControl& ctl;
	LiquidCrystal& lcd;
	LimitedCounter targetPressure;
	string status;

	void start(const Event& e);
	void manual(const Event& e);
	void autoStart(const Event& e);
	void autoTry(const Event& e);
	void autoSuccess(const Event& e);
	void autoFailure(const Event& e);
	void setState(state next);
	void manualUI();
	void autoUI();
};

#endif /* STATEMACHINE_H_ */
