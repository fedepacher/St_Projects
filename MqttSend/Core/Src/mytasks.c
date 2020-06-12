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
#include "debounce.h"
#include "bmp280_ex.h"
#include "bmp280_ex_functions.h"
/* Private define ------------------------------------------------------------*/
#define STACK_SIZE			512UL

/* Private macro -------------------------------------------------------------*/

/* Private  Variables -------------------------------------------------------------*/
extern I2C_HandleTypeDef hi2c1;
static char network_host[32] = "broker.hivemq.com"; ///< HostName i.e. "test.mosquitto.org"//"broker.mqttdashboard.com";//
static unsigned short int network_port = 1883; ///< Remote port number.
static unsigned short int network_keepalive = 60; ///< Default keepalive time in seconds.
static char network_ssl = false; ///< SSL is disabled by default.
static char topic_sub1[] = "test/rtos";
static char topic_pub1[] = "test/rtos";
static char topic_sub2[] = "test/pdm";
static char topic_pub2[] = "test/pdm";


typedef struct{
	char topic[20];
	int32_t data;
}dataPubi_t;


button_t button_down;

TaskHandle_t wifiTask;
QueueHandle_t xSemaphorePub;
QueueHandle_t xSemaphoreSub;
QueueHandle_t xQueueDataRx;
QueueHandle_t xQueueDataPub;
QueueHandle_t xQueuePrintConsole;

SemaphoreHandle_t xSemaphoreMutexUart;

dataPubi_t dataSub;
bmp280_dev dev;


//void UartTxTask(void *argument);
//void UartRxTask(void *argument);
void wifiConnectTask(void *argument);
void ledTask(void *argument);
void pubTask(void *argument);
void subTask(void *argument);
void analizeTask(void *argument);
void printConsoleTask(void *argument);
void buttonsTask(void *argument);

void initTasks(void) {

	HAL_UART_F_Init();

	/* Led initialize */
	int i;
	uint8_t flag_error_mem = 0;

	//led init
	for(i = 0; i < LED_COUNT; i++){
		vLedWrite(i, GPIO_PIN_RESET);
	}

	//button set
	button_down.GPIOx = JOY_DOWN_GPIO_Port;
	button_down.GPIO_Pin = JOY_DOWN_Pin;

	//sensor check
	if (HAL_I2C_IsDeviceReady(&hi2c1, BMP280_dev_address, 2, 100) == HAL_OK) {
		BMP280_init(&dev);
		BMP280_calc_values(&dev);
		dev.data.init_height = dev.data.altitude;
	}


	strcpy((char*)dataSub.topic, topic_sub2);

	xSemaphoreMutexUart = xSemaphoreCreateMutex();
	//xSemaphorePub = xSemaphoreCreateBinary();

	xQueuePrintConsole = xQueueCreate(100, sizeof(uint8_t));

	xQueueDataRx = xQueueCreate(5, sizeof(int32_t));
	//xSemaphorePub != NULL && xSemaphoreSub != NULL &&
	if (xSemaphoreMutexUart != NULL && xQueueDataRx != NULL && xQueuePrintConsole != NULL) {
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
		res = xTaskCreate(printConsoleTask, "print", 128, 0,
						(osPriority_t) osPriorityAboveNormal, 0);
		if (res != pdPASS) {
			printf("error creacion de tarea led\r\n");
			flag_error_mem = 1;
		}



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
			Status = mqtt_SubscriberPacket(dataSub.topic);
			if (Status == ESP8266_OK) {
				internalState++;
				vLedWrite(LED_3, GPIO_PIN_RESET);
				vLedWrite(LED_2, GPIO_PIN_SET);


				vTaskDelayUntil(&t, pdMS_TO_TICKS(5000));


				xSemaphoreSub = xSemaphoreCreateBinary();
				//xSemaphorePub = xSemaphoreCreateBinary();

				xQueueDataPub = xQueueCreate(20, sizeof(dataPubi_t));

				if(xSemaphoreSub != NULL && xQueueDataPub != NULL){
					BaseType_t res;
					res = xTaskCreate(subTask, "subscribe", STACK_SIZE, 0,
							(osPriority_t) osPriorityAboveNormal1, 0);
					if (res != pdPASS) {
						printf("error creacion de tarea sub\r\n");
					}
					res = xTaskCreate(analizeTask, "analize data", 128, 0,
							(osPriority_t) osPriorityNormal, 0);
					if (res != pdPASS) {
						printf("error creacion de tarea analize\r\n");
					}
					res = xTaskCreate(pubTask, "publish", STACK_SIZE, 0,
							(osPriority_t) osPriorityNormal, 0);
					if (res != pdPASS) {
						printf("error creacion de tarea pub\r\n");
					}
					res = xTaskCreate(buttonsTask, "buttons", 128, 0,
									(osPriority_t) osPriorityAboveNormal1, 0);
					if (res != pdPASS) {
						printf("error creacion de tarea buttons\r\n");

					}
					//xSemaphoreGive(xSemaphorePub);
				}
				else{
					printf("error creacion de semaforo\r\n");
				}
			}
			else{
				if (Status == ESP8266_ERROR)
					vLedWrite(LED_3, GPIO_PIN_SET);
			}
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

void printConsoleTask(void *argument){
	int8_t dataQueuePrint;
/*	uint8_t dato1 = 'w';
	printf("%c",dato1);
	uint8_t a1 = 'a';
	printf("%c",(char*)a1);*/
	for(;;){
		xQueueReceive(xQueuePrintConsole, &dataQueuePrint, portMAX_DELAY);
		taskENTER_CRITICAL();
		printf("%c", dataQueuePrint);
		taskEXIT_CRITICAL();
		vTaskDelay(1 / portTICK_PERIOD_MS);
	}
}

void pubTask(void *argument) {
	ESP8266_StatusTypeDef Status;
	//uint32_t data = 0;
	//TickType_t t = xTaskGetTickCount();
	dataPubi_t dataQueuePub;
	//xSemaphoreTake(xSemaphorePub, portMAX_DELAY);
	for (;;) {
		xQueueReceive(xQueueDataPub, &dataQueuePub, portMAX_DELAY);

		xSemaphoreTake(xSemaphoreMutexUart, 20000);
		Status = mqtt_Publisher(dataQueuePub.topic, dataQueuePub.data);
		xSemaphoreGive(xSemaphoreMutexUart);
		if (Status == ESP8266_OK) {

		}
		//data++;
		vTaskDelay(1 / portTICK_PERIOD_MS);

		//vTaskDelayUntil(&t, pdMS_TO_TICKS(15000));
	}
}

void subTask(void *argument) {
	ESP8266_StatusTypeDef Status;


	for (;;) {
		xSemaphoreTake(xSemaphoreSub, portMAX_DELAY);

		xSemaphoreTake(xSemaphoreMutexUart, 20000);
		Status = mqtt_SubscriberReceive(dataSub.topic, &dataSub.data);
		xSemaphoreGive(xSemaphoreMutexUart);

		if (Status == ESP8266_OK) {
			if (dataSub.data != -1)
				xQueueSend(xQueueDataRx, &dataSub.data, 100);
		}
		vTaskDelay(1 / portTICK_PERIOD_MS);
	}
}

void analizeTask(void *argument) {
	int32_t dataQueueRx;
	for (;;) {
		xQueueReceive(xQueueDataRx, &dataQueueRx, portMAX_DELAY);
		if (dataQueueRx == 10009){
			vLedWrite(LED_7, GPIO_PIN_SET);
		}
		if (dataQueueRx == 20009){
			vLedWrite(LED_7, GPIO_PIN_RESET);
		}
		if (dataQueueRx % 2 == 0) {
			vLedWrite(LED_2, GPIO_PIN_SET);
		} else {
			vLedWrite(LED_2, GPIO_PIN_RESET);
		}

		vTaskDelay(1 / portTICK_PERIOD_MS);
	}
}

void buttonsTask(void *argument){

	int32_t data_pub = 0;
	uint32_t cont_aux = 0;
	uint32_t cont = 0;
	dataPubi_t data_st;
	bool flag = false;
	fsmButtonInit(&button_down);
	for (;;) {
		//update FSM button
		fsmButtonUpdate(&button_down);

		if(button_down.released){
			flag = !flag;
			if(flag){
				data_pub = 1000;
				vLedWrite(LED_6, GPIO_PIN_SET);
			}
			else{
				data_pub = 2000;
				vLedWrite(LED_6, GPIO_PIN_RESET);
			}
			cont = 0;
			strcpy((char*)data_st.topic, topic_pub2);
			data_st.data = data_pub;
			xQueueSend(xQueueDataPub, &data_st, 1000);
		}
		if(cont > 10000){
			BMP280_calc_values(&dev);
			cont = 0;
			cont_aux++;
			strcpy(data_st.topic, topic_pub1);
			data_st.data = dev.data.temperature;
			xQueueSend(xQueueDataPub, &data_st, 1000);
		}
		cont++;
		vTaskDelay(1 / portTICK_PERIOD_MS);


	}
}
