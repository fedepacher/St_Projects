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

const uint8_t END_STRING_R_N[] = "\r\n";
const uint8_t END_STRING_N[] = "\n";
static unsigned char buffer[128];
//int32_t transport_socket;
static int strpos(char *hay, char *needle, int offset);
static int32_t findIntData(char topic[], uint8_t *data, uint32_t lenght);

ESP8266_StatusTypeDef mqtt_Connect(void) {
	//unsigned char buffer[128];
	//MQTTTransport transporter;
	//int32_t result;
	int32_t length;

	ESP8266_StatusTypeDef Status = ESP8266_OK;
	int32_t internalState = 0;
	int32_t trial = 0;
	MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

	while (trial < TRIAL_CONNECTION_TIME) {
		switch (internalState) {
		case 0:
			// Populate the transporter.
			//transporter.sck = &transport_socket;
			//transporter.getfn = transport_getdatanb;
			//transporter.state = 0;

			// Populate the connect struct.


			connectData.MQTTVersion = 3; //4
			connectData.clientID.cstring = "fede";
			connectData.keepAliveInterval = CONNECTION_KEEPALIVE_S * 2;
			//connectData.willFlag = 1;
			//connectData.will.qos = 2;
			length = MQTTSerialize_connect(buffer, sizeof(buffer),
					&connectData);

			// Send CONNECT to the mqtt broker.

			Status = ESP_SendData(buffer, length);

			//if ((result = transport_sendPacketBuffer(transport_socket, buffer,length)) == length) {
			if (Status == ESP8266_OK) {
				//Status = ESP8266_OK;
				internalState = 2;			//internalState++;
			} else {
				//Status = ESP8266_ERROR;
				internalState = 0;
				trial++;
			}
			break;
		case 1:
			//not implemented yet

			// Wait for CONNACK response from the mqtt broker.
			/*while (TRUE) {
				// Wait until the transfer is done.
				if ((result = MQTTPacket_readnb(buffer, sizeof(buffer),
						&transporter)) == CONNACK) {
					// Check if the connection was accepted.
					unsigned char sessionPresent, connack_rc;
					if ((MQTTDeserialize_connack(&sessionPresent, &connack_rc,
							buffer, sizeof(buffer)) != 1)
							|| (connack_rc != 0)) {
						// Start over.
						internalState = 0;
						break;
					} else {
						// To the next state.

						internalState++;
						break;
					}
				} else if (result == -1) {
					// Start over.
					internalState = 0;
					trial++;
					break;
				}
			}*/
			break;
		case 2:
			Status = ESP8266_OK;
			trial = TRIAL_CONNECTION_TIME;
			break;
		}
	}

	return Status;
}

ESP8266_StatusTypeDef mqtt_Publisher(char *topic, uint8_t data) {
	//unsigned char buffer[128];
	//MQTTTransport transporter;
	//int32_t result;
	int32_t length;
	//uint32_t tickStart;// = HAL_GetTick();


	ESP8266_StatusTypeDef Status = ESP8266_OK;

	// Populate the publish message.
	MQTTString topicString = MQTTString_initializer;
	topicString.cstring = topic;//"test/rgb";
	unsigned char payload[16];
	int qos = 0;
	length = MQTTSerialize_publish(buffer, sizeof(buffer), 0, qos, 0, 0,
			topicString, payload,
			(length = sprintf((char *)payload, "%d%c%c", (int) data , '\r', '\n')));

	// Send PUBLISH to the mqtt broker.

	Status = ESP_SendData(buffer, length);

	//tickStart = HAL_GetTick();
	/*if (Status == ESP8266_OK){//(result = transport_sendPacketBuffer(transport_socket, buffer, length)) == length) {

	} else {
		// Start over.
		Status = ESP8266_ERROR;
	}*/

	return Status;
}

ESP8266_StatusTypeDef mqtt_Subscriber() {

	int length;

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

		ESP8266_StatusTypeDef Status = ESP8266_OK;

		// Populate the subscribe message.
		MQTTString topicFilters[1] = { MQTTString_initializer };
		topicFilters[0].cstring = topic;//"test/rgb";
		int requestedQoSs[1] = { 0 };
		length = MQTTSerialize_subscribe(buffer, sizeof(buffer), 0, 1, 1,
				topicFilters, requestedQoSs);

		// Send SUBSCRIBE to the mqtt broker.
		Status = ESP_SendData(buffer, length);
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


ESP8266_StatusTypeDef mqtt_SubscriberReceive(char topic[], int32_t* pData) {
	ESP8266_StatusTypeDef Status = ESP8266_OK;
	uint32_t RetLength;
	uint8_t dato[MQTT_BUFFERSIZE];

	//alocate memory for the receiving buffer
	//dato = (uint8_t*) malloc(MQTT_BUFFERSIZE * sizeof(uint8_t));
	memset(dato, '\0', MQTT_BUFFERSIZE);
	ESP_ReceiveData(dato, MQTT_BUFFERSIZE, &RetLength);
	*(pData) = findIntData(topic, dato, RetLength);
	//free(dato);

	return Status;
}





static int strpos(char *hay, char *needle, int offset) {
	char haystack[strlen(hay)];
	strncpy(haystack, hay + offset, strlen(hay) - offset);
	char *p = strstr(haystack, needle);
	if (p)
		return p - haystack + offset;
	return -1;
}

static int32_t findIntData(char topic[], uint8_t *data, uint32_t lenght) {
	uint8_t newdata[lenght];
	uint32_t i = 0;
	uint32_t j = 0;
	int32_t data_begin;
	int32_t data_end;

	/* remove \0 */
	memset((char*) newdata, '\0', lenght);
	for (i = 0; i < lenght; i++) {
		if (*(data + i) != '\0') {
			newdata[j++] = *(data + i);
		}
	}
	data_begin = strpos((char*)newdata, (char*)topic, 0);	//get index where string topic begins
	data_end = strpos((char*)newdata, (char*)END_STRING_R_N, 0);		//get index where \r\n ends

	if(data_end == -1){
		data_end = strpos((char*)newdata, (char*)END_STRING_N, 0);		//get index where \n ends
	}


	if (data_begin != -1 && data_end != -1) {
		i = 0;
		int value = 0;
		int32_t index_start = data_begin + strlen(topic);
		int32_t st_lenght = data_end - index_start + 1; //sume uno porque el atoi me convertia mal si el st tenia un solo valor, es por eso que inicializo el primer elemtento del arreglo con el '0'
		uint8_t st[st_lenght];
		st[0] = '0';//inicializado en cero el primer elemento para que el atoi convierta los numero como el 1, 2 ....9 bien sino los convertia 10, 20 ..... 90
		for (i = 1; i < st_lenght; i++) {
			st[i] = *(newdata + index_start + i - 1);
		}

		value = atoi((char*) st);

		return value;

	}
	return -1;

}




