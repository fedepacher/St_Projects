/*
 * bmp280.h
 *
 *  Created on: Jun 5, 2020
 *      Author: fedepacher
 */

#ifndef INC_BMP280_H_
#define INC_BMP280_H_

#include "stdio.h"

/*! @name Bit-slicing macros */
#define BMP280_GET_BITS(bitname, x)                    ((x & bitname##_MASK) \
                                                        >> bitname##_POS)
#define BMP280_SET_BITS(regvar, bitname, val)          ((regvar & \
                                                         ~bitname##_MASK) | ((val << bitname##_POS) & bitname##_MASK))
#define BMP280_SET_BITS_POS_0(reg_data, bitname, data) ((reg_data & \
                                                         ~(bitname##_MASK)) | (data & bitname##_MASK))
#define BMP280_GET_BITS_POS_0(bitname, reg_data)       (reg_data & \
                                                        (bitname##_MASK))
/* highest API revision supported is revision 0. */
#define BMP280_ST_MAX_APIREVISION            0x00

/*! @brief Macros holding the minimum and maximum for trimming values */
/* 0x00000 is minimum output value */
#define BMP280_ST_ADC_T_MIN                  0x00000

/* 0xFFFF0 is maximum 20-bit output value without over sampling */
#define BMP280_ST_ADC_T_MAX                  0xFFFF0

/* 0x00000 is minimum output value */
#define BMP280_ST_ADC_P_MIN                  0x00000

/* 0xFFFF0 is maximum 20-bit output value without over sampling */
#define BMP280_ST_ADC_P_MAX                  0xFFFF0


/*! @name I2C addresses */
#define BMP280_I2C_ADDR                 	0xEE


/*! @name Interface selection macros */
#define BMP280_SPI_INTF                      0
#define BMP280_I2C_INTF                      1

/*! @name SPI 3-Wire macros */
#define BMP280_SPI3_WIRE_DISABLE             0

/*! @name Return codes */
/*! @name Success code*/
#define BMP280_OK                            0


/*! @name Error codes */
#define BMP280_E_NULL_PTR                    -1
#define BMP280_E_DEV_NOT_FOUND               -2
#define BMP280_E_INVALID_LEN                 -3
#define BMP280_E_COMM_FAIL                   -4
#define BMP280_E_INVALID_MODE                -5
#define BMP280_E_BOND_WIRE                   -6
#define BMP280_E_IMPLAUS_TEMP                -7
#define BMP280_E_IMPLAUS_PRESS               -8
#define BMP280_E_CAL_PARAM_RANGE             -9
#define BMP280_E_UNCOMP_TEMP_RANGE           -10
#define BMP280_E_UNCOMP_PRES_RANGE           -11
#define BMP280_E_UNCOMP_TEMP_AND_PRESS_RANGE -12
#define BMP280_E_UNCOMP_DATA_CALC            -13
#define BMP280_E_32BIT_COMP_TEMP             -14
#define BMP280_E_32BIT_COMP_PRESS            -15
#define BMP280_E_64BIT_COMP_PRESS            -16
#define BMP280_E_DOUBLE_COMP_TEMP            -17
#define BMP280_E_DOUBLE_COMP_PRESS           -18

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

/*! @name Position and mask macros */
#define BMP280_STATUS_IM_UPDATE_POS          0
#define BMP280_STATUS_IM_UPDATE_MASK         0x01
#define BMP280_STATUS_MEAS_POS               3
#define BMP280_STATUS_MEAS_MASK              0x08
#define BMP280_OS_TEMP_POS                   5
#define BMP280_OS_TEMP_MASK                  0xE0
#define BMP280_OS_PRES_POS                   2
#define BMP280_OS_PRES_MASK                  0x1C
#define BMP280_POWER_MODE_POS                0
#define BMP280_POWER_MODE_MASK               0x03
#define BMP280_STANDBY_DURN_POS              5
#define BMP280_STANDBY_DURN_MASK             0xE0
#define BMP280_FILTER_POS                    2
#define BMP280_FILTER_MASK                   0x1C
#define BMP280_SPI3_ENABLE_POS               0
#define BMP280_SPI3_ENABLE_MASK              0x01

/*! @name Over-sampling macros */
#define	BMP280_OS_NONE						0X00
#define BMP280_OS_1X						0X01		//x1 oversamping 16bits/0.0050 ºC
#define BMP280_OS_2X						0X02		//x2 oversamping 17bits/0.0025 ºC
#define BMP280_OS_4X						0X03		//x4 oversamping 18bits/0.0012 ºC
#define BMP280_OS_8X						0X04		//x8 oversamping 19bits/0.0006 ºC
#define BMP280_OS_16X						0X05		//16 oversamping 20bits/0.0003 ºC

/*! @name Filter coefficient macros */
#define BMP280_FILTER_OFF					0X00
#define BMP280_FILTER_COEFF_2				0X02
#define BMP280_FILTER_COEFF_4				0X04
#define BMP280_FILTER_COEFF_8				0X08
#define BMP280_FILTER_COEFF_16				0X10

/*! @name Power modes */
#define BMP280_SLEEP_MODE					0X00
#define	BMP280_FORCED_MODE					0X01 //or 0X10
#define BMP280_NORMAL_MODE					0X11

/*! @name ODR options */
#define BMP280_ODR_0_5_MS					0X00 //t=0.5ms
#define BMP280_ODR_62_5_MS					0X01 //t=62.5ms
#define BMP280_ODR_125_MS					0X02 //t=125ms
#define BMP280_ODR_250_MS					0X03 //t=250ms
#define BMP280_ODR_500_MS					0X04 //t=500ms
#define BMP280_ODR_1000_MS					0X05 //t=1000ms
#define BMP280_ODR_2000_MS					0X06 //t=2000ms
#define BMP280_ODR_4000_MS					0X07 //t=4000ms

/*! @name Calibration parameter register addresses*/
#define BMP280_DIG_T1_LSB_ADDR               0x88
#define BMP280_DIG_T1_MSB_ADDR               0x89
#define BMP280_DIG_T2_LSB_ADDR               0x8A
#define BMP280_DIG_T2_MSB_ADDR               0x8B
#define BMP280_DIG_T3_LSB_ADDR               0x8C
#define BMP280_DIG_T3_MSB_ADDR               0x8D
#define BMP280_DIG_P1_LSB_ADDR               0x8E
#define BMP280_DIG_P1_MSB_ADDR               0x8F
#define BMP280_DIG_P2_LSB_ADDR               0x90
#define BMP280_DIG_P2_MSB_ADDR               0x91
#define BMP280_DIG_P3_LSB_ADDR               0x92
#define BMP280_DIG_P3_MSB_ADDR               0x93
#define BMP280_DIG_P4_LSB_ADDR               0x94
#define BMP280_DIG_P4_MSB_ADDR               0x95
#define BMP280_DIG_P5_LSB_ADDR               0x96
#define BMP280_DIG_P5_MSB_ADDR               0x97
#define BMP280_DIG_P6_LSB_ADDR               0x98
#define BMP280_DIG_P6_MSB_ADDR               0x99
#define BMP280_DIG_P7_LSB_ADDR               0x9A
#define BMP280_DIG_P7_MSB_ADDR               0x9B
#define BMP280_DIG_P8_LSB_ADDR               0x9C
#define BMP280_DIG_P8_MSB_ADDR               0x9D
#define BMP280_DIG_P9_LSB_ADDR               0x9E
#define BMP280_DIG_P9_MSB_ADDR               0x9F

/*! @name Memory map */
#define BMP280_CHIP_ID_ADDR                  0xD0
#define BMP280_SOFT_RESET_ADDR               0xE0
#define BMP280_STATUS_ADDR                   0xF3
#define BMP280_CTRL_MEAS_ADDR                0xF4
#define BMP280_CONFIG_ADDR                   0xF5
#define BMP280_PRES_MSB_ADDR                 0xF7
#define BMP280_PRES_LSB_ADDR                 0xF8
#define BMP280_PRES_XLSB_ADDR                0xF9
#define BMP280_TEMP_MSB_ADDR                 0xFA
#define BMP280_TEMP_LSB_ADDR                 0xFB
#define BMP280_TEMP_XLSB_ADDR                0xFC


/*! @name Chip IDs */
#define BMP280_CHIP_ID1                      0x56
#define BMP280_CHIP_ID2                      0x57
#define BMP280_CHIP_ID3                      0x58

/*! @name Soft reset command */
#define BMP280_SOFT_RESET_CMD                0xB6


/*! @name Measurement status */
#define BMP280_MEAS_DONE                     0
#define BMP280_MEAS_ONGOING                  1

/*! @name Image update */
#define BMP280_IM_UPDATE_DONE                0
#define BMP280_IM_UPDATE_ONGOING             1

/*! @name Function pointer type definitions */
typedef int8_t (*bmp280_com_fptr_t)(uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len);
typedef void (*bmp280_delay_fptr_t)(uint32_t period);

/*! @name Calibration parameters structure */
struct bmp280_calib_param
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
};




/*! @name Sensor status structure */
struct bmp280_status
{
    uint8_t measuring;			//1 when a conversion is running, 0 whe the resul have been transferred to the data reg
    uint8_t im_update;			//1 when the non volatile memory data are being copied to img reg, 0 when the copying is done
};

/*! @name Uncompensated data structure */
struct bmp280_uncomp_data
{
    int32_t uncomp_temp;		//raw temp data
    uint32_t uncomp_press;		//raw press data
};

/*! @name Sensor configuration structure */
struct bmp280_config
{
    uint8_t os_temp;			//oversampling temp
    uint8_t os_pres;			//oversampling press
    uint8_t odr;				//output data rate
    uint8_t filter;				//control the time constant of the IIR filter
    uint8_t spi3w_en;			//enable 3-wire SPI interface when set to '1'
};

/*! @name API device structure */
struct bmp280_dev
{
    uint8_t chip_id;
    uint8_t dev_id;
    uint8_t intf;
    bmp280_com_fptr_t read;
    bmp280_com_fptr_t write;
	//struct bmp280_status status;
	//struct bmp280_uncomp_data data;
	bmp280_delay_fptr_t delay_ms;
	struct bmp280_calib_param calib_param;
	struct bmp280_config conf;
};

/*!
 * @brief This API reads the data from the given register address of the
 * sensor.
 *
 * @param[in] reg_addr : Register address from where the data to be read
 * @param[out] reg_data : Pointer to data buffer to store the read data.
 * @param[in] len : No of bytes of data to be read.
 * @param[in] dev : Structure instance of bmp280_dev.
 *
 * @return Result of API execution
 * @retval Zero for Success, non-zero otherwise.
 */
int8_t bmp280_get_regs(uint8_t reg_addr, uint8_t *reg_data, uint8_t len, const struct bmp280_dev *dev);

/*!
 * @brief This API writes the given data to the register addresses
 * of the sensor.
 *
 * @param[in] reg_addr : Register address from where the data to be written.
 * @param[in] reg_data : Pointer to data buffer which is to be written
 * in the sensor.
 * @param[in] len : No of bytes of data to write..
 * @param[in] dev : Structure instance of bmp280_dev.
 *
 * @return Result of API execution
 * @retval Zero for Success, non-zero otherwise.
 */
int8_t bmp280_set_regs(uint8_t *reg_addr, const uint8_t *reg_data, uint8_t len, const struct bmp280_dev *dev);

/*!
 * @brief This API triggers the soft reset of the sensor.
 *
 * @param[in] dev : Structure instance of bmp280_dev.
 *
 * @return Result of API execution
 * @retval Zero for Success, non-zero otherwise..
 */
int8_t bmp280_soft_reset(const struct bmp280_dev *dev);

/*!
 *  @brief This API is the entry point.
 *  It reads the chip-id and calibration data from the sensor.
 *
 *  @param[in,out] dev : Structure instance of bmp280_dev
 *
 *  @return Result of API execution
 *  @retval zero -> Success / +ve value -> Warning / -ve value -> Error
 */
int8_t bmp280_init(struct bmp280_dev *dev);

/*!
 * @brief This API reads the data from the ctrl_meas register and config
 * register. It gives the currently set temperature and pressure over-sampling
 * configuration, power mode configuration, sleep duration and
 * IIR filter coefficient.
 *
 * @param[out] conf : Current configuration of the bmp280
 * conf.osrs_t, conf.osrs_p = BMP280_OS_NONE, BMP280_OS_1X,
 *     BMP280_OS_2X, BMP280_OS_4X, BMP280_OS_8X, BMP280_OS_16X
 * conf.odr = BMP280_ODR_0_5_MS, BMP280_ODR_62_5_MS, BMP280_ODR_125_MS,
 *     BMP280_ODR_250_MS, BMP280_ODR_500_MS, BMP280_ODR_1000_MS,
 *     BMP280_ODR_2000_MS, BMP280_ODR_4000_MS
 * conf.filter = BMP280_FILTER_OFF, BMP280_FILTER_COEFF_2,
 *     BMP280_FILTER_COEFF_4, BMP280_FILTER_COEFF_8, BMP280_FILTER_COEFF_16
 * conf.spi3w_en = BMP280_SPI3_WIRE_ENABLE, BMP280_SPI3_WIRE_DISABLE
 * @param[in] dev : Structure instance of bmp280_dev
 *
 * @return Result of API execution
 * @retval Zero for Success, non-zero otherwise.
 */
int8_t bmp280_get_config(struct bmp280_config *conf, struct bmp280_dev *dev);

/*!
 * @brief This API writes the data to the ctrl_meas register and config register.
 * It sets the temperature and pressure over-sampling configuration,
 * power mode configuration, sleep duration and IIR filter coefficient.
 *
 * @param[in] conf : Desired configuration to the bmp280
 * conf.osrs_t, conf.osrs_p = BMP280_OS_NONE, BMP280_OS_1X,
 *     BMP280_OS_2X, BMP280_OS_4X, BMP280_OS_8X, BMP280_OS_16X
 * conf.odr = BMP280_ODR_0_5_MS, BMP280_ODR_62_5_MS, BMP280_ODR_125_MS,
 *     BMP280_ODR_250_MS, BMP280_ODR_500_MS, BMP280_ODR_1000_MS,
 *     BMP280_ODR_2000_MS, BMP280_ODR_4000_MS
 * conf.filter = BMP280_FILTER_OFF, BMP280_FILTER_COEFF_2,
 *     BMP280_FILTER_COEFF_4, BMP280_FILTER_COEFF_8, BMP280_FILTER_COEFF_16
 * conf.spi3w_en = BMP280_SPI3_WIRE_ENABLE, BMP280_SPI3_WIRE_DISABLE
 * @param[in] dev : Structure instance of bmp280_dev
 *
 * @return Result of API execution
 * @retval Zero for Success, non-zero otherwise.
 */
int8_t bmp280_set_config(const struct bmp280_config *conf, struct bmp280_dev *dev);

/*!
 * @brief This API writes the power mode.
 *
 * @param[out] mode : BMP280_SLEEP_MODE, BMP280_NORMAL_MODE,
 *     BMP280_FORCED_MODE
 * @param[in] dev : Structure instance of bmp280_dev
 *
 * @return Result of API execution
 * @retval Zero for Success, non-zero otherwise.
 */
int8_t bmp280_set_power_mode(uint8_t mode, struct bmp280_dev *dev);

/*!
 * @brief This API reads the temperature and pressure data registers.
 * It gives the raw temperature and pressure data.
 *
 * @param[in] uncomp_data : Structure instance of bmp280_uncomp_data
 * @param[in] dev : Structure instance of bmp280_dev
 *
 * @return Result of API execution
 * @retval Zero for Success, non-zero otherwise.
 */
int8_t bmp280_get_uncomp_data(struct bmp280_uncomp_data *uncomp_data, const struct bmp280_dev *dev);

/*!
 * @brief This API is used to get the compensated temperature from
 * uncompensated temperature. This API uses 32 bit integers.
 * Temperature in degC, resolution is 0.01 DegC. output value of
 * "5123" equals 51.23 degree Celsius
 *
 * @param[out] comp_temp : 32 bit compensated temperature
 * @param[in] uncomp_temp : Raw temperature values from the sensor
 * @param[in] dev : Structure instance of bmp280_dev
 *
 * @return Result of API execution
 * @retval Zero for Success, non-zero otherwise.
 */
int8_t bmp280_get_comp_temp_32bit(int32_t *comp_temp, int32_t uncomp_temp, struct bmp280_dev *dev);


#endif /* INC_BMP280_H_ */
