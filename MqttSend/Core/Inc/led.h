/*
 * led.h
 *
 *  Created on: May 26, 2020
 *      Author: fedepacher
 */

#ifndef _LED_H_
#define _LED_H_

#include "main.h"

/*#define LEDG_Pin GPIO_PIN_12
#define LEDG_Port GPIOD
#define LEDO_Pin GPIO_PIN_13
#define LEDO_Port GPIOD
#define LEDR_Pin GPIO_PIN_14
#define LEDR_Port GPIOD
#define LEDB_Pin GPIO_PIN_15
#define LEDB_Port GPIOD
*/

#define LED_COUNT 7

typedef enum{
	/*LED_RED,
	LED_GREEN,
	LED_BLUE,
	LED_ORANGE,*/
	LED_1,
	LED_2,
	LED_3,		//NO WIFI CONNECTION ERROR LED
	LED_4,		//NO MEMORY AVAILABLE RTOS ERROR LED
	LED_5,		//HARDFAULT/UART ERROR LED
	LED_6,
	LED_7
}led_t;


/*
 * @brief Controls various LEDs.
 * @param ledID The id number of the LED to modify.
 * @param ledON If true, the LED is on, otherwise off.
 */
void vLedWrite(led_t led, GPIO_PinState ledON);

/*
 * @brief Controls various LEDs.
 * @param ledID The id number of the LED to modify.
 */
void vLedToggle(led_t led);

/*
 * @brief Controls various LEDs.
 * @param ledID The id number of the LED to modify.
 * @param ledON If true, the LED is on, otherwise off.
 */
GPIO_PinState vLedRead(led_t led);
#endif
