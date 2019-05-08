/*
===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
 */

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include <cr_section_macros.h>

// TODO: insert other include files here

// TODO: insert other definitions and declarations here

#include <cstring>
#include <cstdio>
#include <string>

#include "ModbusMaster.h"
#include "I2C.h"
#include "DigitalIoPin.h"
#include "LiquidCrystal.h"
#include "StateMachine.h"
#include "EventQueue.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/


/*****************************************************************************
 * Public functions
 ****************************************************************************/
static volatile int counter;
static volatile uint32_t systicks;

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief	Handle interrupt from SysTick timer
 * @return	Nothing
 */
void SysTick_Handler(void)
{
	systicks++;
	if(counter > 0) counter--;
}
#ifdef __cplusplus
}
#endif

void Sleep(int ms)
{
	counter = ms;
	while(counter > 0) {
		__WFI();
	}
}

/* this function is required by the modbus library */
uint32_t millis() {
	return systicks;
}

/**
 * @brief	Main program body
 * @return	Always returns 1
 */
int main(void)
{

#if defined (__USE_LPCOPEN)
	// Read clock settings and update SystemCoreClock variable
	SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
	// Set up and initialize all required blocks and
	// functions related to the board hardware
	Board_Init();
	// Set the LED to the state of "On"
	Board_LED_Set(0, true);
#endif
#endif

	/* Enable and setup SysTick Timer at a periodic rate */
	SysTick_Config(SystemCoreClock / 1000);

	Chip_RIT_Init(LPC_RITIMER);

	DigitalIoPin rs(0, 8, "", false, false, false);
	DigitalIoPin en(1, 6, "", false, false, false);
	DigitalIoPin d4(1, 8, "", false, false, false);
	DigitalIoPin d5(0, 5, "", false, false, false);
	DigitalIoPin d6(0, 6, "", false, false, false);
	DigitalIoPin d7(0, 7, "", false, false, false);

	LiquidCrystal lcd(&rs, &en, &d4, &d5, &d6, &d7);

	I2C i2c(0, 100000);
	ModbusMaster node(2);
	FanControl ctl(i2c, node);
	EventQueue events;
	StateMachine machine(ctl, lcd);

	DigitalIoPin b0(1, 3, "b0", true, true, true);
	DigitalIoPin b1(0, 16, "b1", true, true, true);
	DigitalIoPin b2(0, 10, "b2", true, true, true);
	DigitalIoPin b3(0, 9, "b3", true, true, true);

	while (1) {
		if (systicks >= 1) {
			events.publish(Event());
		}
		if (b0.read()) {
			events.publish(Event(Event::Key, 0));
			Sleep(500);
		}
		else if (b1.read()) {
			events.publish(Event(Event::Key, 1));
		}
		else if (b2.read()) {
			events.publish(Event(Event::Key, 2));
		}
		else if (b3.read()) {
			events.publish(Event(Event::Key, 3));
			Sleep(500);
		}
		while (events.pending()) {
			machine.handleState(events.consume());
		}
	}
}

