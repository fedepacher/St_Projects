/*
 * esp8266_io.h
 *
 *  Created on: May 21, 2020
 *      Author: fedepacher
 */

#ifndef INC_ESP8266_IO_H_
#define INC_ESP8266_IO_H_

#include "main.h"
#include "stdint.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

 /* Private functions ---------------------------------------------------------*/



 /**
  * @brief  Uart Initalization.
  *         This function inits the UART interface to deal with the esp8266,
  *         then starts asynchronous listening on the RX port.
  * @param None
  * @retval None.
  */
void HAL_UART_F_Init(void);

/**
 * @brief  Uart Deinitialization.
 *         This function Deinits the UART interface of the ESP8266. When called
 *         the esp8266 commands can't be executed anymore.
 * @param  None.
 * @retval None.
 */
void HAL_UART_F_DeInit(void);

/**
 * @brief  Send Data to the ESP8266 module over the UART interface.
 *         This function allows sending data to the  ESP8266 WiFi Module, the
 *          data can be either an AT command or raw data to send over
 a pre-established WiFi connection.
 * @param pData: data to send.
 * @param Length: the data length.
 * @retval 0 on success, -1 otherwise.
 */
int8_t HAL_UART_F_Send(const char* Buffer, const uint8_t Length);



#endif /* INC_ESP8266_IO_H_ */
