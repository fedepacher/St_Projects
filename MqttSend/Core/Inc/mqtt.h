/*
 * mqtt.h
 *
 *  Created on: May 26, 2020
 *      Author: fedepacher
 */

#ifndef INC_MQTT_H_
#define INC_MQTT_H_

#include "ESP_Client.h"


#define CONNECTION_KEEPALIVE_S	60UL
#define MQTT_BUFFERSIZE			128UL

/*
 * @brief Send connect packet to the broker mqtt.
 */
ESP8266_StatusTypeDef mqtt_Connect(void);

/*
 * @brief Send publish packet to the broker mqtt.
 */
ESP8266_StatusTypeDef mqtt_Publisher(char *topic, int32_t data);

/*
 * @brief Send subcribe packet to the broker mqtt.
 */
ESP8266_StatusTypeDef mqtt_Subscriber();

ESP8266_StatusTypeDef mqtt_SubscriberPacket(char *topic);

ESP8266_StatusTypeDef mqtt_SubscriberReceive(char topic[], int32_t *pData);

#endif /* INC_MQTT_H_ */
