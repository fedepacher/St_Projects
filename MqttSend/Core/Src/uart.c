/*
 * esp8266_io.c
 *
 *  Created on: May 21, 2020
 *      Author: fedepacher
 */

#include "uart.h"
#include <stdio.h>
#include <string.h>
#include "cmsis_os.h"
#include "semphr.h"
#include "ESP_Client.h"
#include "led.h"

/* Extern variables -------------------------------------------------------------*/
extern UART_HandleTypeDef huart1;
extern QueueHandle_t xSemaphoreSub;
extern QueueHandle_t xQueuePrintConsole;

/* Private function prototypes -----------------------------------------------*/
static void WIFI_Handler(void);

/**
 * @brief  Rx Callback when new data is received on the UART.
 * @param  UartHandle: Uart handle receiving the data.
 * @retval None.
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	// Set transmission flag: transfer complete
	static BaseType_t xHigherPriorityTaskWoken;

	xHigherPriorityTaskWoken = pdFALSE;

	if (huart->Instance == USART1) {
		uint8_t dato = WiFiRxBuffer.data[WiFiRxBuffer.tail];


		if (++WiFiRxBuffer.tail >= ESP_BUFFERSIZE_CIRCULAR) {
			WiFiRxBuffer.tail = 0;
		}
		// Receive one byte in interrupt mode
		HAL_UART_Receive_IT(huart, (uint8_t*) &WiFiRxBuffer.data[WiFiRxBuffer.tail], 1);
		if(xSemaphoreSub != NULL){
			if(dato == '\n')
				xSemaphoreGiveFromISR(xSemaphoreSub, &xHigherPriorityTaskWoken);
		}
#if DEBUG == 1
		//if(dato == '\n')
			xQueueSendFromISR(xQueuePrintConsole, &dato, &xHigherPriorityTaskWoken);
#endif
	}
	/* If xHigherPriorityTaskWoken was set to true you
	    we should yield.  The actual macro used here is
	    port specific. */
	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

/**
 * @brief  Function called when error happens on the UART.
 * @param  UartHandle: Uart handle receiving the data.
 * @retval None.
 */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
	WIFI_Handler();
}

void HAL_UART_F_Init(void) {

	WiFiRxBuffer.head = 0;
	WiFiRxBuffer.tail = 0;

	HAL_UART_Receive_IT(&huart1,
			(uint8_t*) &WiFiRxBuffer.data[WiFiRxBuffer.tail], 1);

}



void HAL_UART_F_DeInit(void) {
	/* Reset USART configuration to default */
	HAL_UART_DeInit(&huart1);
}

int8_t HAL_UART_F_Send(const char* Buffer, const uint8_t Length) {
	/* It is using a blocking call to ensure that the AT commands were correctly sent. */
	if (HAL_UART_Transmit_IT(&huart1, (uint8_t*) Buffer, Length) != HAL_OK){//, ESP_DEFAULT_TIME_OUT
	//if (HAL_UART_Transmit(&huart1, (uint8_t*) Buffer, Length, ESP_DEFAULT_TIME_OUT) != HAL_OK){
		return -1;
	}
	return 0;
}


/**
 * @brief  Handler to deinialize the ESP8266 UART interface in case of errors.
 * @param  None
 * @retval None.
 */
static void WIFI_Handler(void) {
	HAL_UART_DeInit(&huart1);
	vLedWrite(LED_5, GPIO_PIN_SET);
	while (1) {
	}
}

