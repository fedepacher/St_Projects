/*
 * bmp280_ex.c
 *
 *  Created on: May 8, 2020
 *      Author: fedepacher
 */


#include "bmp280_ex.h"


extern I2C_HandleTypeDef hi2c1;

uint8_t BMP280_i2c_read_reg(uint8_t device_adr, uint8_t internal_adr, uint8_t* data, uint16_t length)
{
	uint8_t tx_buff[1];

	tx_buff[0] = internal_adr;

	HAL_I2C_Master_Transmit(&hi2c1, device_adr, &tx_buff[0], 1, TIMEOUT);
	HAL_I2C_Master_Receive(&hi2c1, device_adr + 1, data, length, TIMEOUT);

	return 0;
}


void BMP280_i2c_write_reg(uint8_t device_adr, uint8_t internal_adr, uint8_t data)
{
	uint8_t tx_buff[2];

	tx_buff[0] = internal_adr;
	tx_buff[1] = data;

	HAL_I2C_Master_Transmit(&hi2c1, device_adr, tx_buff, sizeof(tx_buff), TIMEOUT);
}

void BMP280_get_calib_values(bmp280_dev *dev)
{
	uint8_t temp[BMP280_CALIB_DATA_SIZE], starting_address=0x88;

	BMP280_i2c_read_reg(BMP280_dev_address, starting_address, temp, BMP280_CALIB_DATA_SIZE);

	dev->calib_param.dig_t1 =
			(uint16_t) (((uint16_t) temp[BMP280_DIG_T1_MSB_POS] << 8) | ((uint16_t) temp[BMP280_DIG_T1_LSB_POS]));
	dev->calib_param.dig_t2 =
			(int16_t) (((int16_t) temp[BMP280_DIG_T2_MSB_POS] << 8) | ((int16_t) temp[BMP280_DIG_T2_LSB_POS]));
	dev->calib_param.dig_t3 =
			(int16_t) (((int16_t) temp[BMP280_DIG_T3_MSB_POS] << 8) | ((int16_t) temp[BMP280_DIG_T3_LSB_POS]));
	dev->calib_param.dig_p1 =
			(uint16_t) (((uint16_t) temp[BMP280_DIG_P1_MSB_POS] << 8) | ((uint16_t) temp[BMP280_DIG_P1_LSB_POS]));
	dev->calib_param.dig_p2 =
			(int16_t) (((int16_t) temp[BMP280_DIG_P2_MSB_POS] << 8) | ((int16_t) temp[BMP280_DIG_P2_LSB_POS]));
	dev->calib_param.dig_p3 =
			(int16_t) (((int16_t) temp[BMP280_DIG_P3_MSB_POS] << 8) | ((int16_t) temp[BMP280_DIG_P3_LSB_POS]));
	dev->calib_param.dig_p4 =
			(int16_t) (((int16_t) temp[BMP280_DIG_P4_MSB_POS] << 8) | ((int16_t) temp[BMP280_DIG_P4_LSB_POS]));
	dev->calib_param.dig_p5 =
			(int16_t) (((int16_t) temp[BMP280_DIG_P5_MSB_POS] << 8) | ((int16_t) temp[BMP280_DIG_P5_LSB_POS]));
	dev->calib_param.dig_p6 =
			(int16_t) (((int16_t) temp[BMP280_DIG_P6_MSB_POS] << 8) | ((int16_t) temp[BMP280_DIG_P6_LSB_POS]));
	dev->calib_param.dig_p7 =
			(int16_t) (((int16_t) temp[BMP280_DIG_P7_MSB_POS] << 8) | ((int16_t) temp[BMP280_DIG_P7_LSB_POS]));
	dev->calib_param.dig_p8 =
			(int16_t) (((int16_t) temp[BMP280_DIG_P8_MSB_POS] << 8) | ((int16_t) temp[BMP280_DIG_P8_LSB_POS]));
	dev->calib_param.dig_p9 =
			(int16_t) (((int16_t) temp[BMP280_DIG_P9_MSB_POS] << 8) | ((int16_t) temp[BMP280_DIG_P9_LSB_POS]));

}

void BMP280_init(bmp280_dev *dev)
{
	uint8_t register_F4 = 0b01010111;
	uint8_t register_F5 = 0b10010000;

	BMP280_i2c_write_reg(BMP280_dev_address, F4_ctrl_meas_reg, register_F4);// osrs_t 010 x2, osrs_p 16 101, mode normal 11
	BMP280_i2c_write_reg(BMP280_dev_address, F5_config_reg, register_F5);// standby time 500ms 100, filter 16 100, SPI DIS 0

	BMP280_get_calib_values(dev);
}

void BMP280_calc_values(bmp280_dev *dev)
{
	uint8_t rx_buff[6], starting_address = F7_press_reg;
	uint8_t * data = NULL;

	do
	{
		BMP280_i2c_read_reg(BMP280_dev_address, F3_status_reg, data, 1);
	} while(((*(data)&0b00001000) == 8)||((*(data)&0b00000001) == 1));


	BMP280_i2c_read_reg(BMP280_dev_address, starting_address, rx_buff, sizeof(rx_buff));

	volatile uint32_t temp[3];
	temp[2] = rx_buff[3];
	temp[1] = rx_buff[4];
	temp[0] = rx_buff[5];
	dev->raw_data.temperature_raw = (temp[2] << 12) + (temp[1] << 4) + (temp[0] >> 4);

	temp[2] = rx_buff[0];
	temp[1] = rx_buff[1];
	temp[0] = rx_buff[2];
	dev->raw_data.pressure_raw=(temp[2] << 12) + (temp[1] << 4) + (temp[0] >> 4);

	double var1, var2;
	var1 = (((double)dev->raw_data.temperature_raw) / 16384.0 - ((double)dev->calib_param.dig_t1) / 1024.0) * ((double)dev->calib_param.dig_t2);
	var2 = ((((double)dev->raw_data.temperature_raw) / 131072.0-((double)dev->calib_param.dig_t1)/8192.0)*(((double)dev->raw_data.temperature_raw)/131072.0-((double)dev->calib_param.dig_t1)/8192.0))*((double)dev->calib_param.dig_t3);
	dev->calib_param.t_fine = (int32_t)(var1 + var2);
volatile	float T = (var1+var2)/5120.0;

	var1 = ((double)dev->calib_param.t_fine / 2.0) - 64000.0;
	var2 = var1 * var1 * ((double)dev->calib_param.dig_p6) / 32768.0;
	var2 = var2 + var1 * ((double)dev->calib_param.dig_p5) * 2.0;
	var2 = (var2 / 4.0) + (((double)dev->calib_param.dig_p4) * 65536.0);
	var1 = (((double)dev->calib_param.dig_p3) * var1 * var1 / 524288.0 + ((double)dev->calib_param.dig_p2) * var1) / 524288.0;
	var1 = (1.0+var1/32768.0) * ((double)dev->calib_param.dig_p1);
volatile	double p = 1048576.0 - (double)dev->raw_data.pressure_raw;
	p = (p - (var2 / 4096.0)) * 6250.0 / var1;
	var1 = ((double)dev->calib_param.dig_p9) * p * p / 2147483648.0;
	var2 = p * ((double)dev->calib_param.dig_p8) / 32768.0;
	p = p + (var1 + var2 + ((double)dev->calib_param.dig_p7)) / 16.0;

	dev->data.temperature = T;
	dev->data.pressure = p;
	dev->data.altitude = 44330.0f * (1 - powf(dev->data.pressure / 101325.0f,1.0f / 5.255f));//altitude=((powf(101325.0/pressure, 1/5.257f)-1)*(temperature+273.15f))/0.0065f;
}
