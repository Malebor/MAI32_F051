/*
 * Sine_step.h
 *
 *  Created on: 30 янв. 2024 г.
 *      Author: шшпгп
 */

#ifndef INC_SINE_STEP_H_
#define INC_SINE_STEP_H_

void Sinesteps();

extern int pos_PWM_A;
extern unsigned int revs; //Количество проделанных оборотов
extern unsigned int steps_1ph;
extern float duty_k; //from 0 to 1

#endif /* INC_SINE_STEP_H_ */
