/*
 * bmp280_ex.h
 *
 *  Created on: May 8, 2020
 *      Author: fedepacher
 */

#ifndef INC_BMP280_EX_H_
#define INC_BMP280_EX_H_

#include <math.h>
#include "stm32l4xx_hal.h"

#define TIMEOUT						1000
#define BMP280_dev_address 			0xEE

#define F3_status_reg				0xF3

#define F4_ctrl_meas_reg			0xF4
//REGISTER 0xF4
#define F4_osrs_t_skipped			0x00
#define F4_osrs_t_oversampling1		0x20
#define F4_osrs_t_oversampling2		0x40
#define F4_osrs_t_oversampling4		0x60
#define F4_osrs_t_oversampling8		0x80
#define F4_osrs_t_oversampling16	0xA0

#define F4_osrs_p_skipped			0X00
#define F4_osrs_p_oversampling1		0X04
#define F4_osrs_p_oversampling2		0X08
#define F4_osrs_p_oversampling4		0X0C
#define F4_osrs_p_oversampling8		0X10
#define F4_osrs_p_oversampling16	0X14

#define F4_mode_sleep				0X00
#define F4_mode_forced				0X01
#define F4_mode_normal				0X03

#define F5_config_reg				0xF5
//REGISTER 0xF5
#define F5_t_sb_500us				0X00
#define F5_t_sb_62500us				0X20
#define F5_t_sb_125ms				0X40
#define F5_t_sb_250ms				0X60
#define F5_t_sb_500ms				0X80
#define F5_t_sb_1sec				0XA0
#define F5_t_sb_2sec				0XC0
#define F5_t_sb_4sec				0XE0

#define F5_filter_1					0X00
#define F5_filter_2					0X02
#define F5_filter_5					0X04
#define F5_filter_11				0X06
#define F5_filter_22				0X0A

#define F5_spi4w_en					0X00
#define F5_spi3w_en					0X01

#define F7_press_reg				0xF7

/*! @name Calibration parameters' relative position */
#define BMP280_DIG_T1_LSB_POS                0
#define BMP280_DIG_T1_MSB_POS                1
#define BMP280_DIG_T2_LSB_POS                2
#define BMP280_DIG_T2_MSB_POS                3
#define BMP280_DIG_T3_LSB_POS                4
#define BMP280_DIG_T3_MSB_POS                5
#define BMP280_DIG_P1_LSB_POS                6
#define BMP280_DIG_P1_MSB_POS                7
#define BMP280_DIG_P2_LSB_POS                8
#define BMP280_DIG_P2_MSB_POS                9
#define BMP280_DIG_P3_LSB_POS                10
#define BMP280_DIG_P3_MSB_POS                11
#define BMP280_DIG_P4_LSB_POS                12
#define BMP280_DIG_P4_MSB_POS                13
#define BMP280_DIG_P5_LSB_POS                14
#define BMP280_DIG_P5_MSB_POS                15
#define BMP280_DIG_P6_LSB_POS                16
#define BMP280_DIG_P6_MSB_POS                17
#define BMP280_DIG_P7_LSB_POS                18
#define BMP280_DIG_P7_MSB_POS                19
#define BMP280_DIG_P8_LSB_POS                20
#define BMP280_DIG_P8_MSB_POS                21
#define BMP280_DIG_P9_LSB_POS                22
#define BMP280_DIG_P9_MSB_POS                23
#define BMP280_CALIB_DATA_SIZE               24

/*! @name Calibration parameters structure */
typedef struct
{							//  LSB/MSB
    uint16_t dig_t1;		// 0x88/0x89
    int16_t dig_t2;			// 0x8A/0X8B
    int16_t dig_t3;			// 0X8C/0X8D
    uint16_t dig_p1;		// 0X8E/0X8F
    int16_t dig_p2;			// 0X90/0X91
    int16_t dig_p3;			// 0X92/0X93
    int16_t dig_p4;			// 0X94/0X95
    int16_t dig_p5;			// 0X96/0X97
    int16_t dig_p6;			// 0X98/0X99
    int16_t dig_p7;			// 0X9A/0X9B
    int16_t dig_p8;   		// 0X9C/0X9D
    int16_t dig_p9;			// 0X9E/0X9F
    int32_t t_fine;			// 0XA0/0XA1
}bmp280_calib_param;

/*! @name Uncompensated data structure */
typedef struct
{
    int32_t temperature_raw;		//raw temp data
    uint32_t pressure_raw;		//raw press data
}bmp280_uncomp_data;

/*! @name compensated data structure */
typedef struct
{
	float temperature;
	float pressure;
	float altitude;
	float init_height;
}bmp280_comp_data;

typedef struct
{
 //   uint8_t chip_id;
   // uint8_t dev_id;
    //uint8_t intf;
    //bmp280_com_fptr_t read;
   // bmp280_com_fptr_t write;
	//bmp280_delay_fptr_t delay_ms;
	bmp280_calib_param calib_param;
	bmp280_uncomp_data raw_data;
	bmp280_comp_data data;
//	struct bmp280_config conf;
}bmp280_dev;


/*
 * @brief I2C Write function
 * @param device_adr device address
 * @param internal_adr internal register's address
 * @param data data to be written
 */
void BMP280_i2c_write_reg(uint8_t device_adr, uint8_t internal_adr, uint8_t data);

/*
 * @brief Initialize the I2C module
 * @param dev structure of the sensor
 */
void BMP280_init(bmp280_dev *dev);

/*
 * @brief Calculate the temp press and altitud
 * @param dev structure of the sensor
 */
void BMP280_calc_values(bmp280_dev *dev);

/*
 * @brief I2C Read function
 * @param device_adr device address
 * @param internal_adr internal register's address
 * @param data data to be written
 * @param lenght data length
 * @param data data to be written
 */
uint8_t BMP280_i2c_read_reg(uint8_t device_adr, uint8_t internal_adr, uint8_t* data, uint16_t length);


#endif /* INC_BMP280_EX_H_ */
