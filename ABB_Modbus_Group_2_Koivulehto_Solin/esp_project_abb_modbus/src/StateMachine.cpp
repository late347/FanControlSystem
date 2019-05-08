/*
 * StateMachine.cpp
 *
 *  Created on: 1 Feb 2018
 *      Author: Arvi
 */

#include "StateMachine.h"

StateMachine::StateMachine(FanControl& ctl0, LiquidCrystal& lcd0) : ctl(ctl0), lcd(lcd0) {
	status = "try";
	timer = 0;
	accuracyTimer = 0;
	targetPressure = 60;

	lcd.begin(lcdWidth, lcdHeight);

	current = StateMachine::Start;
	start(Event::Enter);
}

StateMachine::~StateMachine() {

}

void StateMachine::handleState(const Event& e) {
	switch (current) {
	case Start:
		start(e);
		break;
	case Manual:
		manual(e);
		break;
	case AutoStart:
		autoStart(e);
		break;
	case AutoTry:
		autoTry(e);
		break;
	case AutoSuccess:
		autoSuccess(e);
		break;
	case AutoFailure:
		autoFailure(e);
		break;
	}
}

void StateMachine::start(const Event& e) {
	switch (e.type) {
	case Event::Enter:
		lcd.clear();
		ctl.setFrequency(0);
		lcd.setCursor(0,0);
		lcd.print("Start state");
		lcd.setCursor(0, 1);
		lcd.print("0: man  3: auto");
		break;
	case Event::Key:
		if (e.value == 0) {
			setState(Manual);
		}
		else if (e.value == 3) {
			setState(AutoStart);
		}
		break;
	default:
		break;
	}
}

void StateMachine::manual(const Event& e) {
	switch (e.type) {
	case Event::Enter:
		timer = 0;
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("Manual");
		lcd.setCursor(9,0);
		lcd.print("F=");
		lcd.setCursor(15,0);
		lcd.print("%");
		lcd.setCursor(9,1);
		lcd.print("P=");
		lcd.setCursor(14,1);
		lcd.print("Pa");
		manualUI();
		break;
	case Event::Key:
		if (e.value == 0) {
			setState(AutoStart);
		}
		else if (e.value == 1) {
			--ctl;
			manualUI();
		}
		else if (e.value == 2) {
			++ctl;
			manualUI();
		}
		break;
	case Event::Tick:
		++timer;
		if (timer > manualRefresh) {
			timer = 0;
			manualUI();
		}
		break;
	default:
		break;
	}
}

void StateMachine::autoStart(const Event& e) {
	switch (e.type) {
	case Event::Enter:
		status = "strt";
		timer=0;
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("Auto ");
		lcd.setCursor(10,0);
		lcd.print("F=");
		lcd.setCursor(15,0);
		lcd.print("%");
		lcd.setCursor(0,1);
		lcd.print("P=");
		lcd.setCursor(5,1);
		lcd.print("Pa T=");
		lcd.setCursor(13,1);
		lcd.print("Pa");
		autoUI();
		Sleep(200);
		break;
	case Event::Key:
		if (e.value == 0) {
			setState(Manual);
		}
		else if(e.value==1) {
			//decrement targetPressure
			--targetPressure;
			autoUI();
			Sleep(200);
		}
		if(e.value==2){
			//increment targetPressure
			++targetPressure;
			autoUI();
			Sleep(200);
		}
		else if(e.value==3){
			setState(AutoTry);
		}

		break;
	case Event::Tick:
		++timer;
		if (timer > autoStartRefresh) {
			timer = 0;
			autoUI();
		}
		break;
	default:
		break;
	}
}

void StateMachine::autoTry(const Event& e) {
	switch (e.type) {
	case Event::Enter:
		status = "try";
		timer = 0;
		accuracyTimer = 0;
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("Auto ");
		lcd.setCursor(10,0);
		lcd.print("F=");
		lcd.setCursor(15,0);
		lcd.print("%");
		lcd.setCursor(0,1);
		lcd.print("P=");
		lcd.setCursor(5,1);
		lcd.print("Pa T=");
		lcd.setCursor(13,1);
		lcd.print("Pa");
		autoUI();
		break;
	case Event::Key:
		if (e.value == 0) {
			lcd.clear();
			lcd.setCursor(0,0);
			lcd.print("Changing state");
			Sleep(2000);
			lcd.clear();
			setState(Manual);
		}
		else if (e.value == 3) {
			lcd.clear();
			lcd.setCursor(0,0);
			lcd.print("Changing state");
			Sleep(2000);
			lcd.clear();
			setState(AutoStart);
		}
		break;
	case Event::Tick:
		++timer;
		if (timer > autoTryRefresh) {
			autoUI();
		}
		if (timer < tryTime) {
			if (ctl.autoAdjustFrequency(targetPressure)) {
				if (accuracyTimer == 0){
					accuracyTimer = timer;
				}
				else if (timer - accuracyTimer >= testTicks) {
					accuracyTimer = 0;
					setState(AutoSuccess);
				}
			}
		}
		else {
			setState(AutoFailure);
		}
		break;
	case Event::Exit:
		break;
	default:
		break;
	}
}

void StateMachine::autoSuccess(const Event& e) {
	switch (e.type) {
	case Event::Enter:
		status = "ok";
		timer = 0;
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("Auto ");
		lcd.setCursor(10,0);
		lcd.print("F=");
		lcd.setCursor(15,0);
		lcd.print("%");
		lcd.setCursor(0,1);
		lcd.print("P=");
		lcd.setCursor(5,1);
		lcd.print("Pa T=");
		lcd.setCursor(13,1);
		lcd.print("Pa");
		autoUI();
		break;
	case Event::Key:
		if (e.value == 0) {
			setState(Manual);
		}
		else if (e.value == 3) {
			setState(AutoStart);
		}
		break;
	case Event::Tick:
		++timer;
		if (timer > autoSuccessRefresh) {
			timer = 0;
			autoUI();
		}
		if (abs(ctl.getMedianPressure() - targetPressure) >= limit) {
			setState(AutoTry);
		}
		break;
	default:
		break;
	}
}

void StateMachine::autoFailure(const Event& e) {
	switch (e.type) {
	case Event::Enter:
		status = "fail";
		timer = 0;
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("Auto ");
		lcd.setCursor(10,0);
		lcd.print("F=");
		lcd.setCursor(15,0);
		lcd.print("%");
		lcd.setCursor(0,1);
		lcd.print("P=");
		lcd.setCursor(5,1);
		lcd.print("Pa T=");
		lcd.setCursor(13,1);
		lcd.print("Pa");
		autoUI();
		ctl.setFrequency(2000);
		break;
	case Event::Key:
		if (e.value == 0) {
			setState(Manual);
		}
		else if (e.value == 3) {
			setState(AutoStart);
		}
		break;
	case Event::Tick:
		++timer;
		if (timer > autoFailureRefresh) {
			timer = 0;
			autoUI();
		}
		break;
	default:
		break;
	}
}

void StateMachine::setState(state next) {
	handleState(Event(Event::Exit));
	current = next;
	handleState(Event(Event::Enter));
}

void StateMachine::manualUI() {
	lcd.setCursor(11,0);
	string freq = to_string(ctl.getPercentage());
	freq.append(3 - freq.size(), ' ');
	lcd.print(freq);
	lcd.setCursor(11,1);
	string pressure = to_string(ctl.getMedianPressure());
	pressure.append(3 - pressure.size(), ' ');
	lcd.print(pressure);
}

void StateMachine::autoUI() {
	lcd.setCursor(5,0);
	string temp = status;
	temp.append(5 - temp.size(), ' ');
	lcd.print(temp);
	lcd.setCursor(12,0);
	string freq = to_string(ctl.getPercentage());
	freq.append(3 - freq.size(), ' ');
	lcd.print(freq);
	lcd.setCursor(2,1);
	string pressure = to_string(ctl.getMedianPressure());
	pressure.append(3 - pressure.size(), ' ');
	lcd.print(pressure);
	lcd.setCursor(10,1);
	string target = to_string(targetPressure);
	target.append(3 - target.size(), ' ');
	lcd.print(target);
}
