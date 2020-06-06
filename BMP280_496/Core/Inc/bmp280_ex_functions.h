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


uint8_t string_compare(char array1[], char array2[], uint16_t lenght);

// reverses a string 'str' of length 'len'
void reverse(char *str, int len);

// Converts a given integer x to string str[].  d is the number
// of digits required in output. If d is more than the number
// of digits in x, then 0s are added at the beginning.
int intToStr(int x, char str[], int d);

// Converts a floating point number to string.
void ftoa(float n, char *res, int afterpoint);

#endif /* INC_BMP280_EX_FUNCTIONS_H_ */
