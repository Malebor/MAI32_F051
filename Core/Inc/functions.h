/*
 * functions.h
 *
 *  Created on: 13 февр. 2024 г.
 *      Author: шшпгп
 */

#ifndef INC_FUNCTIONS_H_
#define INC_FUNCTIONS_H_
#include <main.h>

long int ds_dt(int s_in, int ds_max); //Ограничение быстрорастущих значений, линейно по значению ds_max (предыдущее значение не будет больше следующего больше чем на ds_max)
unsigned int ARR_filter(unsigned int input_ARR);
long map(long x, long in_min, long in_max, long out_min, long out_max);
int getAbsDif(int number1, int number2);
int constrain(int input, int min, int max);



#endif /* INC_FUNCTIONS_H_ */
