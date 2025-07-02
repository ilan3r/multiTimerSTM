/*
 * state_machine.h
 *
 *  Created on: Mar 8, 2025
 *      Author: ilan1
 */

#ifndef INC_STATE_MACHINE_H_
#define INC_STATE_MACHINE_H_


#define BUZZER_LED_GPIO_Port GPIOB
#define BUZZER_LED_Pin GPIO_PIN_4

#define HAPTIC_LED_GPIO_Port GPIOC
#define HAPTIC_LED_Pin GPIO_PIN_7

#include <stdint.h>

// define all states for the state machine
typedef enum {
	MENU,
	STOPWATCH_PRIMED,
	STOPWATCH_PAUSED,
	STOPWATCH_RUN,
	TIMER_RUN,
	TIMER_PAUSED
} State;

typedef enum {
	TIMER,
	STOPWATCH,
} Mode;

typedef enum {
	NO_OUTPUT,
	BUZZER,
	HAPTIC,
	BUZZER_HAPTIC
} OutputType;

extern State currentState;
extern State prevState;
extern Mode mode;
extern OutputType output;





// function prototypes
const char* modeToString(Mode mode);
const char* outputToString(OutputType output);

void handleMenu();
void handleStopwatchPrimed();
void handleStopwatchPaused();
void handleStopwatchRun();
void handleTimerRun();
void handleTimerPaused();
void updateState();
void trigger_output(OutputType output);
void update_oled_display();


#endif /* INC_STATE_MACHINE_H_ */
