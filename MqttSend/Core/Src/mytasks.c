/*
 * tasks.c
 *
 *  Created on: May 22, 2020
 *      Author: fedepacher
 */

#include <mytasks.h>
#include <stdio.h>
#include <string.h>
#include "cmsis_os.h"
#include "semphr.h"
#include "stdbool.h"
#include "led.h"
#include "ESP_Client.h"
#include "uart.h"
#include "wifiCredentials.h"
#include "mqtt.h"
/* Private define ------------------------------------------------------------*/
#define STACK_SIZE			512UL

/* Private macro -------------------------------------------------------------*/

/* Private  Variables -------------------------------------------------------------*/
static char network_host[32] = "broker.hivemq.com"; ///< HostName i.e. "test.mosquitto.org"//"broker.mqttdashboard.com";//
static unsigned short int network_port = 1883; ///< Remote port number.
static unsigned short int network_keepalive = 60; ///< Default keepalive time in seconds.
static char network_ssl = false; ///< SSL is disabled by default.
static char topic_sub[] = "test/rtos";
static char topic_pub[] = "test/rtos";
int32_t data_rx;

TaskHandle_t wifiTask;
QueueHandle_t xSemaphorePub;
QueueHandle_t xSemaphoreSub;
QueueHandle_t xQueueDataRx;

SemaphoreHandle_t xSemaphoreMutexUart;

//void UartTxTask(void *argument);
//void UartRxTask(void *argument);
void wifiConnectTask(void *argument);
void ledTask(void *argument);
void pubTask(void *argument);
void subTask(void *argument);
void analizeTask(void *argument);

void initTasks(void) {

	HAL_UART_F_Init();

	/* Led initialize */
	int i;
	uint8_t flag_error_mem = 0;
	for(i = 0; i < LED_COUNT; i++){
		vLedWrite(i, GPIO_PIN_RESET);
	}

	xSemaphoreMutexUart = xSemaphoreCreateMutex();
	//xSemaphorePub = xSemaphoreCreateBinary();

	xQueueDataRx = xQueueCreate(5, sizeof(int32_t));
	//xSemaphorePub != NULL && xSemaphoreSub != NULL &&
	if (xSemaphoreMutexUart != NULL && xQueueDataRx != NULL) {
		BaseType_t res = xTaskCreate(wifiConnectTask, "wifi", STACK_SIZE, 0,
				(osPriority_t) osPriorityAboveNormal, &wifiTask);
		if (res != pdPASS) {
			printf("error creacion de tarea wifi\r\n");
			flag_error_mem = 1;
		}
		res = xTaskCreate(ledTask, "led", 128, 0,
				(osPriority_t) osPriorityNormal, 0);
		if (res != pdPASS) {
			printf("error creacion de tarea led\r\n");
			flag_error_mem = 1;
		}
		/*res = xTaskCreate(pubTask, "publish", STACK_SIZE, 0,
				(osPriority_t) osPriorityAboveNormal, 0);
		if (res != pdPASS) {
			printf("error creacion de tarea pub\r\n");
			flag_error_mem = 1;
		}*/
		/*res = xTaskCreate(subTask, "subscribe", STACK_SIZE, 0,
				(osPriority_t) osPriorityAboveNormal1, 0);
		if (res != pdPASS) {
			printf("error creacion de tarea sub\r\n");
			flag_error_mem = 1;
		}*/
		/*res = xTaskCreate(analizeTask, "analize data", 256, 0,
				(osPriority_t) osPriorityAboveNormal, 0);
		if (res != pdPASS) {
			printf("error creacion de tarea analize\r\n");
			flag_error_mem = 1;

		}*/
	} else {
		printf("error creacion de semaforo\r\n");
		flag_error_mem = 1;
	}

	if(flag_error_mem == 1)
		vLedWrite(LED_4, GPIO_PIN_SET);

}

void wifiConnectTask(void *argument) {
	//uint8_t info[60] = "Conectando a wifi, Espere por favor.. \r\n";
	ESP8266_StatusTypeDef Status;
	//ESP8266_ConnectionInfoTypeDef connection_info;
	//esp_state state_esp = INIT;
	//esp8266_broker_setup(&connection_info);
	int internalState = 0;
	TickType_t t = xTaskGetTickCount();
	printf("Conectando a wifi, Espere por favor.. \r\n");
	for (;;) {
		switch (internalState) {
		case 0:
			// Turn the LED off.
			vLedWrite(LED_2, GPIO_PIN_RESET);
			// Connect to wifi (restore to default first).
			Status = ESP_ConnectWifi(true, WIFI_AP_SSID, WIFI_AP_PASS);
			if (Status == ESP8266_OK) {
				// To the next state.
				internalState++;
				vLedWrite(LED_3, GPIO_PIN_RESET);
			}
			else{
				if (Status == ESP8266_ERROR)
					vLedWrite(LED_3, GPIO_PIN_SET);
			}

			break;
		case 1:
			// Wait 1sec.
			Status = ESP_Delay(1000);
			if (Status == ESP8266_OK) {
				// To the next state.
				internalState++;
				vLedWrite(LED_3, GPIO_PIN_RESET);
			}
			else{
				if (Status == ESP8266_ERROR)
					vLedWrite(LED_3, GPIO_PIN_SET);
			}
			break;
		case 2:
			// Check the wifi connection status.
			Status = ESP_IsConnectedWifi();
			if (Status == ESP8266_OK) {
				// To the next state.
				internalState++;
				vLedWrite(LED_3, GPIO_PIN_RESET);
			}
			else{
				if (Status == ESP8266_ERROR)
					vLedWrite(LED_3, GPIO_PIN_SET);
			}
			break;
		case 3:
			// Start TCP connection.
			Status = ESP_StartTCP(network_host, network_port, network_keepalive,
					network_ssl);
			if (Status == ESP8266_OK) {
				// To the next state.
				internalState++;
				vLedWrite(LED_3, GPIO_PIN_RESET);
			}
			else{
				if (Status == ESP8266_ERROR)
					vLedWrite(LED_3, GPIO_PIN_SET);
			}
			break;
		case 4:
			// Send the mqtt data.
			//Status = ESP82_Send(address, bytes);
			Status = mqtt_Connect();
			if (Status == ESP8266_OK) {
				internalState++;
				vLedWrite(LED_3, GPIO_PIN_RESET);
			}
			else{
				if (Status == ESP8266_ERROR)
					vLedWrite(LED_3, GPIO_PIN_SET);
			}
			break;
		case 5:
			mqtt_SubscriberPacket(topic_sub);
			//Status = mqtt_SubscriberPacket();
			//Status = mqtt_Publisher();
			vTaskDelayUntil(&t, pdMS_TO_TICKS(5000));
			//Status = mqtt_Subscriber();
			//if (Status == ESP8266_OK) {
			vLedWrite(LED_2, GPIO_PIN_SET);
			//xSemaphoreGive(xSemaphorePub);
			//xSemaphoreGive(xSemaphoreSub);



			xSemaphoreSub = xSemaphoreCreateBinary();
			xSemaphorePub = xSemaphoreCreateBinary();

			if(xSemaphoreSub != NULL && xSemaphorePub != NULL){
				BaseType_t res = xTaskCreate(subTask, "subscribe", STACK_SIZE, 0,
					(osPriority_t) osPriorityAboveNormal, 0);
				if (res != pdPASS) {
					printf("error creacion de tarea sub\r\n");
				}
				res = xTaskCreate(analizeTask, "analize data", 256, 0,
						(osPriority_t) osPriorityNormal, 0);
				if (res != pdPASS) {
					printf("error creacion de tarea analize\r\n");
				}
				res = xTaskCreate(pubTask, "publish", STACK_SIZE, 0,
							(osPriority_t) osPriorityNormal, 0);
				if (res != pdPASS) {
					printf("error creacion de tarea pub\r\n");
				}
				xSemaphoreGive(xSemaphorePub);
			}
			else{
				printf("error creacion de semaforo\r\n");
			}


			internalState++;

			//}
			break;
		case 6:
			vTaskDelete(wifiTask);
			break;
		}

		osDelay(1 / portTICK_PERIOD_MS);
	}
	vTaskDelete(wifiTask);
}

void ledTask(void *argument) {

	TickType_t t = xTaskGetTickCount();
	for (;;) {
		vLedToggle(LED_1);
		vTaskDelayUntil(&t, pdMS_TO_TICKS(100));
	}
}

void pubTask(void *argument) {
	ESP8266_StatusTypeDef Status;
	uint8_t data = 0;
	TickType_t t = xTaskGetTickCount();
	xSemaphoreTake(xSemaphorePub, portMAX_DELAY);
	for (;;) {
		xSemaphoreTake(xSemaphoreMutexUart, 20000);
		Status = mqtt_Publisher(topic_pub, data);
		xSemaphoreGive(xSemaphoreMutexUart);
		if (Status == ESP8266_OK) {
			//xSemaphoreGive(xSemaphoreSub);
		}
		data++;

		vTaskDelayUntil(&t, pdMS_TO_TICKS(5000));
	}
}

void subTask(void *argument) {
	ESP8266_StatusTypeDef Status;

	for (;;) {
		xSemaphoreTake(xSemaphoreSub, portMAX_DELAY);

		xSemaphoreTake(xSemaphoreMutexUart, 20000);
		Status = mqtt_SubscriberReceive(topic_sub, &data_rx);
		xSemaphoreGive(xSemaphoreMutexUart);

		if (Status == ESP8266_OK) {
			if (data_rx != -1)
				xQueueSend(xQueueDataRx, &data_rx, 100);
			//xSemaphoreTake(xSemaphoreSub, portMAX_DELAY);
		}
		vTaskDelay(1 / portTICK_PERIOD_MS);
	}
}

void analizeTask(void *argument) {
	int32_t dataQueueRx;
	for (;;) {
		xQueueReceive(xQueueDataRx, &dataQueueRx, portMAX_DELAY);
		if (dataQueueRx > 100) {
			vLedToggle(LED_7);
		}
		if (dataQueueRx % 2 == 0) {
			vLedWrite(LED_2, GPIO_PIN_SET);
		} else {
			vLedWrite(LED_2, GPIO_PIN_RESET);
		}

		vTaskDelay(1 / portTICK_PERIOD_MS);
	}
}


