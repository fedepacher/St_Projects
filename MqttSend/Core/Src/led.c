/*
 * led.c
 *
 *  Created on: May 26, 2020
 *      Author: fedepacher
 */

// Include.
#include "led.h"
#include <assert.h>
#include "main.h"


/*
 * @brief Controls various LEDs.
 * @param ledID The id number of the LED to modify.
 * @param ledON If true, the LED is on, otherwise off.
 */
void vLedWrite(led_t led, GPIO_PinState ledON) {
	//GPIO_InitTypeDef GPIO_InitStructure;

	// Check the id.
	assert(led < LED_COUNT);

	// Pick the LED.
	switch (led) {
/*	case LED_RED:
		HAL_GPIO_WritePin(LEDR_Port, LEDR_Pin, ledON);
		break;
	case LED_GREEN:
		HAL_GPIO_WritePin(LEDG_Port, LEDG_Pin, ledON);
		break;
	case LED_BLUE:
		HAL_GPIO_WritePin(LEDB_Port, LEDB_Pin, ledON);
		break;
	case LED_ORANGE:
		HAL_GPIO_WritePin(LEDO_Port, LEDO_Pin, ledON);
		break;*/
	case LED_1:
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, ledON);
			break;
	case LED_2:
			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, ledON);
			break;
	case LED_3:
		HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, ledON);
		break;
	case LED_4:
		HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, ledON);
		break;
	case LED_5:
		HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, ledON);
		break;
	case LED_6:
		HAL_GPIO_WritePin(LED6_GPIO_Port, LED6_Pin, ledON);
		break;
	case LED_7:
		HAL_GPIO_WritePin(LED7_GPIO_Port, LED7_Pin, ledON);
		break;
	}
}

/*
 * @brief Controls various LEDs.
 * @param ledID The id number of the LED to modify.
 */
void vLedToggle(led_t led) {
	//GPIO_InitTypeDef GPIO_InitStructure;

	// Check the id.
	assert(led < LED_COUNT);

	// Pick the LED.
	switch (led) {
/*	case LED_RED:
		HAL_GPIO_TogglePin(LEDR_Port, LEDR_Pin);
		break;
	case LED_GREEN:
		HAL_GPIO_TogglePin(LEDG_Port, LEDG_Pin);
		break;
	case LED_BLUE:
		HAL_GPIO_TogglePin(LEDB_Port, LEDB_Pin);
		break;
	case LED_ORANGE:
		HAL_GPIO_TogglePin(LEDO_Port, LEDO_Pin);
		break;*/
	case LED_1:
		HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
		break;
	case LED_2:
		HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
		break;
	case LED_3:
		HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
		break;
	case LED_4:
		HAL_GPIO_TogglePin(LED4_GPIO_Port, LED4_Pin);
		break;
	case LED_5:
		HAL_GPIO_TogglePin(LED5_GPIO_Port, LED5_Pin);
		break;
	case LED_6:
		HAL_GPIO_TogglePin(LED6_GPIO_Port, LED6_Pin);
		break;
	case LED_7:
		HAL_GPIO_TogglePin(LED7_GPIO_Port, LED7_Pin);
		break;
	}

}

/*
 * @brief Controls various LEDs.
 * @param ledID The id number of the LED to modify.
 * @param ledON If true, the LED is on, otherwise off.
 */
GPIO_PinState vLedRead(led_t led) {
	GPIO_PinState result;

	// Check the id.
	assert(led < LED_COUNT);

	// Pick the LED.
	switch (led) {
/*	case LED_RED:
		result = HAL_GPIO_ReadPin(LEDR_Port, LEDR_Pin, ledON);
		break;
	case LED_GREEN:
		result = HAL_GPIO_ReadPin(LEDG_Port, LEDG_Pin, ledON);
		break;
	case LED_BLUE:
		result = HAL_GPIO_ReadPin(LEDB_Port, LEDB_Pin, ledON);
		break;
	case LED_ORANGE:
		result = HAL_GPIO_ReadPin(LEDO_Port, LEDO_Pin, ledON);
		break;*/
	case LED_1:
		result = HAL_GPIO_ReadPin(LED1_GPIO_Port, LED1_Pin);
		break;
	case LED_2:
		result = HAL_GPIO_ReadPin(LED2_GPIO_Port, LED2_Pin);
			break;
	case LED_3:
		result = HAL_GPIO_ReadPin(LED3_GPIO_Port, LED3_Pin);
		break;
	case LED_4:
		result = HAL_GPIO_ReadPin(LED4_GPIO_Port, LED4_Pin);
		break;
	case LED_5:
		result = HAL_GPIO_ReadPin(LED5_GPIO_Port, LED5_Pin);
		break;
	case LED_6:
		result = HAL_GPIO_ReadPin(LED6_GPIO_Port, LED6_Pin);
		break;
	case LED_7:
		result = HAL_GPIO_ReadPin(LED7_GPIO_Port, LED7_Pin);
		break;
	}
	return result;
}
