/*
 * bmp280_ex_functions.h
 *
 *  Created on: Jun 5, 2020
 *      Author: fedepacher
 */

#ifndef INC_BMP280_EX_FUNCTIONS_H_
#define INC_BMP280_EX_FUNCTIONS_H_

#include "stdio.h"
#include "string.h"

/*
 * @brief Compare two string.
 * @param array1 first string to be compared
 * @param array2 second string to be compared
 * @param lenght length of the array
 * @return 1 if it is equal, 0 if it is no equal
 */
uint8_t string_compare(char array1[], char array2[], uint16_t lenght);

/*
 * @brief Reverses a string 'str' of length 'len'
 * @param str string to reverse.
 * @param len lengt of string.
 */
void reverse(char *str, int32_t len);


/*
 * @brief Converts a given integer to string str[].
 * @param x integer to convert
 * @param str store the integer
 * @param d  number of digits required in output. If d is more than the number
 * of digits in x, then 0s are added at the beginning.
 * @return length of array
 */
int intToStr(int32_t x, char str[], int32_t d);


/*
 * @brief Converts a floating point number to string.
 * @param n floating number to convert
 * @param res store the converted number
 * @param afterpoint amount decimals
 */
void ftoa(float n, char *res, int afterpoint);

#endif /* INC_BMP280_EX_FUNCTIONS_H_ */
