/*
 * tasks.h
 *
 *  Created on: May 22, 2020
 *      Author: fedepacher
 */

#ifndef INC_MYTASKS_H_
#define INC_MYTASKS_H_

#include "main.h"
#include "stdint.h"


/*
 * @brief Initialize semaphore and one shot Task to connect to wifi
 */
void initTasks(void);

/*
 * @brief MEF to connect to wifi
 * @param argument not used
 */
void wifiConnectTask(void *argument);

/*
 * @brief blink led
 * @param argument not used
 */
void ledTask(void *argument);

/*
 * @brief Publish msg to the broker
 * @param argument not used
 */
void pubTask(void *argument);

/*
 * @brief Subscribe msg from the broker
 * @param argument not used
 */
void subTask(void *argument);

/*
 * @brief Analize packet received from the broker
 * @param argument not used
 */
void analizeTask(void *argument);

/*
 * @brief print data in the console
 * @param argument not used
 */
void printConsoleTask(void *argument);

/*
 * @brief Read button state and BMP280 sensor
 * @param argument not used
 */
void buttonsTask(void *argument);


#endif /* INC_MYTASKS_H_ */
