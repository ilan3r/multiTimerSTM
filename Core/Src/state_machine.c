#include "state_machine.h"
#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"
#include <stdlib.h>   // For rand() and srand()
#include <stdint.h>   // For uint32_t type
#include <time.h>     // (Optional) For time-based seeding, but not needed in STM32

#include "ssd1306.h"
#include "ssd1306_fonts.h"



State currentState = MENU;
State prevState = MENU;
Mode mode = STOPWATCH;
OutputType output;

extern TIM_HandleTypeDef htim9;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim2;


extern uint32_t okButtonPressTime;
extern uint32_t okButtonReleaseTime;
extern uint32_t okButtonPressedDuration;
extern uint32_t okButtonReleasedDuration;
extern uint32_t backButtonPressTime;
extern uint32_t backButtonReleaseTime;
extern uint32_t backButtonPressedDuration;
extern uint32_t backButtonReleasedDuration;


extern uint8_t okButtonState;
extern uint8_t okDebounceFlag;
extern uint8_t backButtonState;
extern uint8_t backDebounceFlag;
volatile uint8_t okJustPressed = 0;
volatile uint8_t backJustPressed = 0;

const uint32_t EXIT_MENU_THRESHOLD = 2000;
const uint32_t REFRESH_RATE = 5;
const uint32_t OUTPUT_TIME = 500;
const uint32_t CYCLE1_TIME[6] = {999, 9, 0, 0, 0, 0};  // 9s 999ms
const uint32_t CYCLE2_TIME[6] = {999, 4, 0, 0, 0, 0};  // 4s 999ms


volatile uint32_t timeArray[6] = {0, 0, 0, 0, 0, 0};  // week, day, hour, min, s, ms}


uint8_t timerPaused = 1;	// 1 means paused, 0 means running
uint8_t cycle1 = 1;
uint8_t randomStartInitialized = 0;
uint32_t primedTimerCounter = 0; // changed from volatile
uint32_t primedDelay;		// changed from volatile
uint8_t primed = 0;
uint8_t primedOutput = 0;
uint8_t outputActive = 0;
uint8_t cursor = 0;
uint32_t outputTriggerTime = 0;







void updateState(){
	switch(currentState){
	case MENU:
		handle_menu();
		break;
    case STOPWATCH_PAUSED:
        handle_stopwatch_paused();
        break;

    case STOPWATCH_PRIMED:
        handle_stopwatch_primed();
        break;

    case STOPWATCH_RUN:
        handle_stopwatch_run();
        break;

    case TIMER_RUN:
        handle_timer_run();
        break;

    case TIMER_PAUSED:
        handle_timer_paused();
        break;
	}
}

const char* modeToString(Mode mode) {
    switch (mode) {
        case STOPWATCH: return "STOPWATCH";
        case TIMER: return "TIMER";
        default: return "UNKNOWN";
    }
}

const char* outputToString(OutputType output) {
    switch (output) {
        case NO_OUTPUT: return "NO_OUTPUT";
        case BUZZER: return "BUZZER";
        case HAPTIC: return "HAPTIC";
        case BUZZER_HAPTIC: return "BUZZER_HAPTIC";
        default: return "UNKNOWN";
    }
}

uint32_t get_random_delay() {
    return (rand() % 3001) + 2000;   // Generates a number between 2000 and 5000
}



void update_oled_display() {
    ssd1306_Fill(Black); // Clear screen
    static char timer_buf[20];
    static char stopwatch_buf[20];

    switch (currentState) {
        case MENU:
            ssd1306_SetCursor(0, 0);
            ssd1306_WriteString("MENU", Font_7x10, White);
            ssd1306_SetCursor(0, 12);
            ssd1306_WriteString("Mode: ", Font_7x10, White);
            ssd1306_WriteString((char*) modeToString(mode), Font_7x10, White);
            ssd1306_SetCursor(0, 24);
            ssd1306_WriteString("Output: ", Font_7x10, White);
            ssd1306_WriteString((char*) outputToString(output), Font_7x10, White);
            break;

        case TIMER_RUN:
            ssd1306_SetCursor(0, 0);
            ssd1306_WriteString("TIMER RUN", Font_7x10, White);
            ssd1306_SetCursor(0, 12);
            if (cycle1){
                sprintf(timer_buf, "Time: %02lu:%03lu", CYCLE1_TIME[1] - timeArray[1], CYCLE1_TIME[0] - timeArray[0]);
            }
            else if (!cycle1){
                sprintf(timer_buf, "Time: %02lu:%03lu", CYCLE2_TIME[1] - timeArray[1], CYCLE2_TIME[0] - timeArray[0]);
            }
            ssd1306_WriteString((char*) timer_buf, Font_7x10, White);
            break;

        case TIMER_PAUSED:
            ssd1306_SetCursor(0, 0);
            ssd1306_WriteString("TIMER RUN", Font_7x10, White);
            ssd1306_SetCursor(0, 12);
            if (cycle1){
            	sprintf(timer_buf, "Time: %02lu:%03lu", CYCLE1_TIME[1] - timeArray[1], CYCLE1_TIME[0] - timeArray[0]);
            }
            else if (!cycle1){
            	sprintf(timer_buf, "Time: %02lu:%03lu", CYCLE2_TIME[1] - timeArray[1], CYCLE2_TIME[0] - timeArray[0]);
            }
            ssd1306_WriteString((char*) timer_buf, Font_7x10, White);
            break;

        case STOPWATCH_RUN:
            ssd1306_SetCursor(0, 0);
            ssd1306_WriteString("STOPWATCH", Font_7x10, White);
            ssd1306_SetCursor(0, 12);
            sprintf(stopwatch_buf, "%02lu:%02lu:%03lu", timeArray[2], timeArray[1], timeArray[0]);
            ssd1306_WriteString((char*) stopwatch_buf, Font_7x10, White);
            break;

        case STOPWATCH_PAUSED:
            ssd1306_SetCursor(0, 0);
            ssd1306_WriteString("STOPWATCH", Font_7x10, White);
            ssd1306_SetCursor(0, 12);
            sprintf(stopwatch_buf, "%02lu:%02lu:%03lu", timeArray[2], timeArray[1], timeArray[0]);
            ssd1306_WriteString((char*) stopwatch_buf, Font_7x10, White);
            break;

        case STOPWATCH_PRIMED:
            ssd1306_SetCursor(0, 0);
            ssd1306_WriteString("STOPWATCH PRIMED", Font_7x10, White);
            ssd1306_SetCursor(0, 12);
            sprintf(stopwatch_buf, "%02lu:%02lu:%03lu", timeArray[2], timeArray[1], timeArray[0]);
            ssd1306_WriteString((char*) stopwatch_buf, Font_7x10, White);
            break;

        default:
            ssd1306_SetCursor(0, 0);
            ssd1306_WriteString("UNKNOWN STATE", Font_7x10, White);
            break;
    }

    ssd1306_UpdateScreen(); // Push updates to display
}

void trigger_output(OutputType output) {
	if (outputActive){
		return;
	}
	else{
		outputActive = 1;
	}
	if (__HAL_TIM_GET_COUNTER(&htim9) > 0) {
	    printf("TIM9 was already running! Counter: %lu\r\n", __HAL_TIM_GET_COUNTER(&htim9));
	}

	// originally tried using a case switch statement but this didn't work
    if (output == BUZZER || output == BUZZER_HAPTIC) {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
    }
    if (output == HAPTIC || output == BUZZER_HAPTIC) {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
    }


    HAL_TIM_Base_Stop_IT(&htim9);   // Stop the timer
    __HAL_TIM_SET_COUNTER(&htim9, 0);  // Reset counter
    HAL_TIM_Base_Start_IT(&htim9);  // Start timer with interrupt
}



void handle_menu() {
//	printf("State: MENU");
//    printf(" Mode: %s", modeToString(mode));
//	printf(" Output: %s\r\n", outputToString(output));

	update_oled_display();

	if (backJustPressed){
		backJustPressed = 0;
		cursor = !cursor;
	}

	if (okJustPressed && cursor){
		okJustPressed = 0;
		if (output == NO_OUTPUT){
			output = BUZZER;
		}
		else if (output == BUZZER){
			output = HAPTIC;
		}
		else if (output == HAPTIC){
			output = BUZZER_HAPTIC;
		}
		else if (output == BUZZER_HAPTIC){
			output = NO_OUTPUT;
		}

	}

	if (okJustPressed && !cursor){
		okJustPressed = 0;
		// might need to add ok
		if (mode == STOPWATCH){
			mode = TIMER;
		}
		else if (mode == TIMER){
			mode = STOPWATCH;
		}
	}

	if (backButtonPressedDuration > EXIT_MENU_THRESHOLD){
		if (mode == TIMER){
			currentState = TIMER_PAUSED;
		}
		else if (mode == STOPWATCH){
			currentState = STOPWATCH_PAUSED;
		}
		backButtonPressedDuration = 0;

	}
}

void handle_stopwatch_primed() {


	  if (!primed){
		  primedDelay = get_random_delay();
		  HAL_TIM_Base_Stop_IT(&htim2);  // Ensure timer is stopped first
		  __HAL_TIM_SET_COUNTER(&htim2, 0);  // Reset the counter
		  HAL_TIM_Base_Start_IT(&htim2);  // Start the timer with interrupt
//		  printf("random time is: %d\r\n", primedDelay);
		  primed = 1;
		  primedTimerCounter = 0;
	  }
//	  else if (primed){
//		 printf("State: STOPWATCH_PRIMED\r\n");
//	  }

	  if (backJustPressed){
		  primed = 0;
		  backJustPressed = 0;
		  currentState = STOPWATCH_PAUSED;
	  }

	  update_oled_display();

}

void handle_stopwatch_paused() {
//	  printf("Paused at time: %02lu:%02lu:%03lu\r\n",
//	             timeArray[2], timeArray[1], timeArray[0]);
	update_oled_display();


	if (!randomStartInitialized){
		    srand(HAL_GetTick());  // Seed the RNG with a unique value
		    randomStartInitialized = 1;
	}

	if (backButtonPressedDuration > EXIT_MENU_THRESHOLD){
		currentState = MENU;
		backButtonPressedDuration = 0;
	}
	if (okJustPressed){
		okJustPressed = 0;
		primed = 0;
		currentState = STOPWATCH_PRIMED;
	}

	if (backJustPressed){
		backJustPressed = 0;
		HAL_TIM_Base_Stop_IT(&htim3);  // Ensure timer is stopped first
		__HAL_TIM_SET_COUNTER(&htim3, 0);  // Reset the counter
		HAL_TIM_Base_Start_IT(&htim3);  // Start the timer with interrupt
		for (int i = 0; i < 6; i++){
			timeArray[i] = 0;
		}

	}
}

void handle_stopwatch_run() {

	HAL_TIM_Base_Stop_IT(&htim3);  // Ensure timer is stopped first
	__HAL_TIM_SET_COUNTER(&htim3, 0);  // Reset the counter
	HAL_TIM_Base_Start_IT(&htim3);  // Start the timer with interrupt

	if (!primedOutput){
		primedOutput = 1;
		trigger_output(output);
	}


//	  printf("Elapsed Time: %02lu:%02lu:%03lu\r\n",
//	             timeArray[2], timeArray[1], timeArray[0]);
	update_oled_display();



	  if (backJustPressed){
		  backJustPressed = 0;
		  currentState = STOPWATCH_PAUSED;
	  }
	  if (okJustPressed){
		  okJustPressed = 0;
		  currentState = STOPWATCH_PAUSED;
	  }
}

void handle_timer_run() {

	  if (backJustPressed){
		  backJustPressed = 0;
		  timerPaused = 1;
		  HAL_TIM_Base_Stop_IT(&htim3);
		  currentState = TIMER_PAUSED;
		  return;
	  }
	  if (okJustPressed){
		  okJustPressed = 0;

		  if (timerPaused){
			  timerPaused = 0;
		  }
		  else if (!timerPaused){
			  HAL_TIM_Base_Stop_IT(&htim3);
			  timerPaused = 1;
			  currentState = TIMER_PAUSED;
			  return;
		  }
	  }

	  if (timerPaused == 1){
		  HAL_TIM_Base_Start_IT(&htim3);
		  timerPaused = 0;
	  }

	if (cycle1 && !timerPaused){
		if (timeArray[1] >= CYCLE1_TIME[1]){
			cycle1 = 0;
			for (int i = 0; i < 6; i++){
				timeArray[i] = 0;
			}
			__HAL_TIM_SET_COUNTER(&htim3, 0);
			outputActive = 0;
			trigger_output(output);
		}
		else{
//			printf("TIMER_RUN - Elapsed: %02lu:%03lu\r\n",
//					CYCLE1_TIME[1] - timeArray[1], CYCLE1_TIME[0] - timeArray[0]);
			update_oled_display();
		}
	}

	else if (!cycle1 && !timerPaused){
		if (timeArray[1] >= CYCLE2_TIME[1]){
			cycle1 = 1;
			for (int i = 0; i < 6; i++){
				timeArray[i] = 0;
			}
			__HAL_TIM_SET_COUNTER(&htim3, 0);
			outputActive = 0;
			trigger_output(output);
		}
		else{
//			printf("TIMER_RUN - Elapsed: %02lu:%03lu\r\n",
//				   CYCLE2_TIME[1]- timeArray[1], CYCLE2_TIME[0]-timeArray[0]);
			update_oled_display();
		}


	}


}

void handle_timer_paused() {
//	  printf("State: TIMER_PAUSED\r\n");
	update_oled_display();

	if (backButtonPressedDuration > EXIT_MENU_THRESHOLD){
		currentState = MENU;
		backButtonPressedDuration = 0;
	}

	if (okJustPressed){
		currentState = TIMER_RUN;
		okJustPressed = 0;
	}

	if (backJustPressed){
		for (int i = 0; i < 6; i++){
			timeArray[i] = 0;
		}

		backJustPressed = 0;
	}
}
