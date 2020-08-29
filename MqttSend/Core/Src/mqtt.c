/*
 * mqtt.c
 *
 *  Created on: May 26, 2020
 *      Author: fedepacher
 */
#include "mqtt.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "MQTTPacket.h"



#define TRIAL_CONNECTION_TIME	5
#define BUFFERSIZE_CMD 		128

//int32_t transport_socket;


ESP8266_StatusTypeDef mqtt_Connect(void) {
	int32_t length;
	unsigned char buffer[BUFFERSIZE_CMD];
	uint8_t sessionPresent, connack_rc;
	ESP8266_StatusTypeDef Status = ESP8266_OK;
	int32_t internalState = 0;
	int32_t trial = 0;
	MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

	while (trial < TRIAL_CONNECTION_TIME) {
		switch (internalState) {
		case 0:
			connectData.MQTTVersion = 3; //4
			connectData.clientID.cstring = "fede";
			connectData.keepAliveInterval = CONNECTION_KEEPALIVE_S * 2;
			//connectData.willFlag = 1;
			//connectData.will.qos = 2;
			memset((char*) buffer, '\0', BUFFERSIZE_CMD);
			length = MQTTSerialize_connect(buffer, sizeof(buffer),
					&connectData);

			// Send CONNECT to the mqtt broker.

			Status = ESP_SendData(buffer, length);

			if (Status == ESP8266_OK) {

				internalState++;
			} else {
				internalState = 0;
				if(Status == ESP8266_ERROR)
					trial++;
			}
			break;
		case 1:
			memset((char*) buffer, '\0', BUFFERSIZE_CMD);
			uint32_t Retlength;
			Status = ESP_ReceiveData(buffer, BUFFERSIZE_CMD, &Retlength);

			if (Status == ESP8266_OK) {
				if (MQTTDeserialize_connack(&sessionPresent, &connack_rc,
						buffer, strlen((char*) buffer)) != 1) {
					internalState++;
				} else {
					Status = ESP8266_OK;
					trial = TRIAL_CONNECTION_TIME;
					break;
				}
			} else {
				internalState++;
			}
			break;

		case 2:
			memset((char*) buffer, '\0', BUFFERSIZE_CMD);
			length = MQTTSerialize_disconnect(buffer, sizeof(buffer));
			Status = ESP_SendData(buffer, length);
			internalState++;
			break;
		case 3:
			Status = ESP8266_ConnectionClose();
			trial = TRIAL_CONNECTION_TIME;//aca deberia devolver para reconectarse
			break;
		}

	}
	return Status;
}


ESP8266_StatusTypeDef mqtt_Publisher(dataMqtt_t *data){
	unsigned char buffer[BUFFERSIZE_CMD];
	int32_t length;
	int32_t trial = 0;
	int32_t internalState = 0;
	ESP8266_StatusTypeDef Status = ESP8266_OK;

	// Populate the publish message.
	MQTTString topicString = MQTTString_initializer;
	topicString.cstring = data->topic;
	int qos = 0;
	memset((char*)buffer, '\0', BUFFERSIZE_CMD);
	//strcat((char*)data->data, "\r\n");// OJO QUE PUEDE QUE ALGUNOS ENVIOS NECESITEN ESTE \R\N
	length = MQTTSerialize_publish(buffer, sizeof(buffer), 0, qos, 0, 0,
			topicString, (char*)data->data, strlen((char*)data->data));

	// Send PUBLISH to the mqtt broker.
	while (trial < TRIAL_CONNECTION_TIME) {
		switch (internalState) {
				case 0:
					Status = ESP_SendData(buffer, length);

					if (Status == ESP8266_OK){//(result = transport_sendPacketBuffer(transport_socket, buffer, length)) == length) {
						internalState++;
					} else {
						internalState = 0;
						trial++;
					}
				break;
				case 1:
					Status = ESP8266_OK;
					trial = TRIAL_CONNECTION_TIME;
					break;
		}
	}
	return Status;
}

ESP8266_StatusTypeDef mqtt_Subscriber() {

	int length;
	unsigned char buffer[128];
	ESP8266_StatusTypeDef Status = ESP8266_OK;

	// Populate the subscribe message.
	MQTTString topicFilters[1] = { MQTTString_initializer };
	topicFilters[0].cstring = "test/rgb";
	int requestedQoSs[1] = { 0 };
	length = MQTTSerialize_subscribe(buffer, sizeof(buffer), 0, 1, 1,
			topicFilters, requestedQoSs);

	// Send SUBSCRIBE to the mqtt broker.
	Status = ESP_SendData(buffer, length);


	//envio a otro topic
	/*memset(buffer, '\0', sizeof(buffer));
	topicFilters[0].cstring = "test/rgb1";
	length = MQTTSerialize_subscribe(buffer, sizeof(buffer), 0, 1, 1,
				topicFilters, requestedQoSs);
	Status = ESP8266_SendData(buffer, length);*/


	uint32_t RetLength;
	uint8_t* dato;
	if (Status == ESP8266_OK) {
		while (1) {
			//alocate memory bor the receiving buffer
			dato = (uint8_t*) malloc(128 * sizeof(uint8_t));
			memset(dato, '\0', 128);
			ESP_ReceiveData(dato, 128, &RetLength);
			free(dato);
		}
	} else {

	}
	return Status;
}

ESP8266_StatusTypeDef mqtt_SubscriberPacket(char *topic) {
	int length;
	unsigned char buffer[128];
	ESP8266_StatusTypeDef Status = ESP8266_OK;
	int32_t trial = 0;
	int32_t internalState = 0;

	// Populate the subscribe message.
	MQTTString topicFilters[1] = { MQTTString_initializer };
	topicFilters[0].cstring = topic;//"test/rgb";
	int requestedQoSs[1] = { 0 };
	length = MQTTSerialize_subscribe(buffer, sizeof(buffer), 0, 1, 1,
			topicFilters, requestedQoSs);

	// Send SUBSCRIBE to the mqtt broker.
	while (trial < TRIAL_CONNECTION_TIME) {
		switch (internalState) {
		case 0:
			Status = ESP_SendData(buffer, length);

			if (Status == ESP8266_OK){//(result = transport_sendPacketBuffer(transport_socket, buffer, length)) == length) {
				internalState++;
			} else {
				internalState = 0;
				trial++;
			}
			break;
		case 1:
			Status = ESP8266_OK;
			trial = TRIAL_CONNECTION_TIME;
			break;
		}
	}
	return Status;
}

/*ESP8266_StatusTypeDef mqtt_SubscriberReceive(char topic[], int32_t* pData) {
	ESP8266_StatusTypeDef Status = ESP8266_OK;
	uint32_t RetLength;
	uint8_t* dato;

	//alocate memory for the receiving buffer
	dato = (uint8_t*) malloc(MQTT_BUFFERSIZE * sizeof(uint8_t));
	memset(dato, '\0', MQTT_BUFFERSIZE);
	ESP_ReceiveData(dato, MQTT_BUFFERSIZE, &RetLength);
	//pData = findIntData(topic, dato, RetLength);
	free(dato);

	return Status;
}*/


ESP8266_StatusTypeDef mqtt_SubscriberReceive(dataMqtt_t *data){//char topic[], char *pData, uint32_t *length) {
	ESP8266_StatusTypeDef Status = ESP8266_OK;
	//uint32_t RetLength;
	//uint8_t dato[MQTT_BUFFERSIZE];


	//alocate memory for the receiving buffer
	//dato = (uint8_t*) malloc(MQTT_BUFFERSIZE * sizeof(uint8_t));
	memset(data->data, '\0', MQTT_BUFFERSIZE);
	ESP_ReceiveData(data->data, MQTT_BUFFERSIZE, &data->length);
	//strcpy(*(pData), (char*)dato);	//ACA NO ME DEVUELVE EL ARREGLO
	//*(length) = RetLength;
	//*(pData) = findIntData(topic, dato, RetLength);
	//free(dato);

	return Status;
}










