/*
 * BLDC.h
 *
 *  Created on: 15 февр. 2024 г.
 *      Author: шшпгп
 */

#ifndef INC_BLDC_H_
#define INC_BLDC_H_

#define PHASE_A_COMP 0b1100001
#define PHASE_B_COMP 0b1000001
#define PHASE_C_COMP 0b1010001

void comStep(char newStep);
void allOff(); //Выключает все выходы на драйвер, двигатель в
void fullBrake(); //Включает все нижние ключи без ШИМ
void allPWM(); //Включает все выходы как ШИМ
void ChangeCopmImnput(); //Меняет вход компаратора и фронт срабатывания в зависимости от шага
uint8_t getCompOutputLevel(); //Возвращает состояние компаратора (BEMF относительно средней точки)
void maskPhaseInterrupts(); //выключение прерываний компаратор
void enableCompInterrupts(); //Включение прерываний компаратора

void phaseALOW();
void phaseBLOW();
void phaseCLOW();

void phaseA_HIGH_PWM();
void phaseB_HIGH_PWM();
void phaseC_HIGH_PWM();

void phaseAFLOAT();
void phaseBFLOAT();
void phaseCFLOAT();

void BLDC_power(); //Задание значения заполнения ШИМ в зависимости от уровня газа

#endif /* INC_BLDC_H_ */
