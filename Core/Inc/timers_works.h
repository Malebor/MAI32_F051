/*
 * timers_works.h
 *
 *  Created on: Feb 9, 2024
 *      Author: шшпгп
 */

#ifndef INC_TIMERS_WORKS_H_
#define INC_TIMERS_WORKS_H_

#include <main.h>


extern char PWM_error; //Ошибка при захвате
extern int PWM_duty; //Период ШИМ
extern int PWM_us; //Длинна импульса
extern int prev_PWM_duty;
extern char RunReady;



void PWM_TIM_Init(); //Доп инициализация таймера ШИМ
void ServoPWM_TIM_Init(); //Доп инициализация таймера для захвата серво ШИМ
void ServoPWM_Calc();
void COMMUTATE_TIM_Init(); //Доп инициализация таймера тайминга/SINE режма
void COMMUTATE_TIM_arr(uint8_t ArrVal); //Запись в значение arr таймера тайминга
void OutOff(); //Выключение выхода на драйвер
void OutOn(); //Включение выхода на драйвер
void ToPosA(); //Перейти к позиции с начало в А

#endif /* INC_TIMERS_WORKS_H_ */
