/*
 * key_generic.h
 *
 *  Created on: Apr 1, 2020
 *      Author: fedepacher
 */

#ifndef FEDE_CLASE5PM_INC_KEY_GENERIC_H_
#define FEDE_CLASE5PM_INC_KEY_GENERIC_H_

#include "cmsis_os.h"
#include "stdbool.h"
#include "stm32l4xx.h"

/*=====[C++ comienzo]========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Macros de definicion de constantes publicas]=========================*/

//#define PI 3.14

/*=====[Macros estilo funcion publicas]======================================*/

#define DEBOUNCE_TIME	40		/*tiempo del antirebote*/

/*=====[Definiciones de tipos de datos publicos]=============================*/

// Tipo de datos que renombra un tipo basico


// Tipo de datos de puntero a funcion


// Tipo de datos enumerado
typedef enum {
	STATE_BUTTON_UP, STATE_BUTTON_FALLING, STATE_BUTTON_DOWN, STATE_BUTTON_RISING,		/*estados de la MEF*/
} fsmButtonState_t;



// Tipo de datos estructua, union o campo de bits

typedef struct{
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
//	gpioMap_t button;
	fsmButtonState_t fsmButtonState;
	TickType_t messure_time;
	TickType_t time_down;
	TickType_t time_up;
	uint8_t countFalling;
	uint8_t countRising;
	bool pressed;
	bool released;
}button_t;
/*=====[Prototipos de funciones publicas]====================================*/


void fsmButtonError(button_t* config);
void fsmButtonInit(button_t* config);
void fsmButtonUpdate(button_t* config);
void buttonPressed(button_t* config);
void buttonReleased(button_t* config);




/*=====[Prototipos de funciones publicas de interrupcion]====================*/



/*=====[C++ fin]=============================================================*/

#ifdef __cplusplus
}
#endif



#endif /* FEDE_CLASE5PM_INC_KEY_GENERIC_H_ */
