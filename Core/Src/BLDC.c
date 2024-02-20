/*
 * BLDC.c
 *
 *  Created on: Feb 14, 2024
 *      Author: шшпгп
 */
#include <main.h>
#include <BLDC.h>
#include <timers_works.h>
#include <routine.h>

#define filter_level 10

extern char rising; //какой фронт мы должны засечь, 1 = восходящий, 0 - нисходящий
extern unsigned int PWM_ARR;
extern int throttle_lvl;


char rising = 0;
const char forward = 1;
char step = 1;
int commutation_interval = 0;
int thiszctime = 0;
int waitTime = 0;

void comStep(char newStep)
{
	switch (newStep)
	{
	case 1: // A-B
		phaseCFLOAT();
		phaseBLOW();
		phaseA_HIGH_PWM();
		COMP->CSR = PHASE_C_COMP;
		EXTI->FTSR = 0x0;
		EXTI->RTSR = 0x200000;
		break;

	case 2: // C-B
		phaseAFLOAT();
		phaseBLOW();
		phaseC_HIGH_PWM();
		COMP->CSR = PHASE_A_COMP;
		EXTI->RTSR = 0x0;
		EXTI->FTSR = 0x200000;
		break;

	case 3: // C-A
		phaseBFLOAT();
		phaseALOW();
		phaseC_HIGH_PWM();
		COMP->CSR = PHASE_B_COMP;
		EXTI->FTSR = 0x0;
		EXTI->RTSR = 0x200000;
		break;

	case 4: // B-A
		phaseCFLOAT();
		phaseALOW();
		phaseB_HIGH_PWM();
		COMP->CSR = PHASE_C_COMP;
		EXTI->RTSR = 0x0;
		EXTI->FTSR = 0x200000;
		break;

	case 5: // B-C
		phaseAFLOAT();
		phaseCLOW();
		phaseB_HIGH_PWM();
		COMP->CSR = PHASE_A_COMP;
		EXTI->FTSR = 0x0;
		EXTI->RTSR = 0x200000;
		break;

	case 6: // A-C
		phaseBFLOAT();
		phaseCLOW();
		phaseA_HIGH_PWM();
		COMP->CSR = PHASE_B_COMP;
		EXTI->RTSR = 0x0;
		EXTI->FTSR = 0x200000;
		break;
	}
}

void allOff()
{
	phaseAFLOAT();
	phaseBFLOAT();
	phaseCFLOAT();
}

void fullBrake()
{ // full braking shorting all low sides
	phaseALOW();
	phaseBLOW();
	phaseCLOW();
}

//---------------------LOW-----------------------------------

void phaseALOW()
{
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_7, LL_GPIO_MODE_OUTPUT); //Modify to output (not PWM)
	GPIOA->BSRR = GPIO_BSRR_BS_7; //ON

	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_8, LL_GPIO_MODE_OUTPUT); //Modify to output (not PWM)
	GPIOA->BSRR = GPIO_BSRR_BR_8; //OFF
}

void phaseBLOW()
{
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_0, LL_GPIO_MODE_OUTPUT); //Modify to output (not PWM)
	GPIOB->BSRR = GPIO_BSRR_BS_0; //ON

	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_9, LL_GPIO_MODE_OUTPUT); //Modify to output (not PWM)
	GPIOA->BSRR = GPIO_BSRR_BR_9; //OFF
}

void phaseCLOW()
{
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_1, LL_GPIO_MODE_OUTPUT); //Modify to output (not PWM)
	GPIOB->BSRR = GPIO_BSRR_BS_1; //ON

	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_10, LL_GPIO_MODE_OUTPUT); //Modify to output (not PWM)
	GPIOA->BSRR = GPIO_BSRR_BR_10; //OFF
}

//---------------------PWM-------------------------------------

void phaseA_HIGH_PWM()
{	//LOW SIDE
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_7, LL_GPIO_MODE_OUTPUT); //Modify to output (not PWM)
	GPIOA->BSRR = GPIO_BSRR_BR_7; //OFF
	//HIGH SIDE
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_8, LL_GPIO_MODE_ALTERNATE);//Modify to PWM
}

void phaseB_HIGH_PWM()
{	//LOW SIDE
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_0, LL_GPIO_MODE_OUTPUT); //Modify to output (not PWM)
	GPIOB->BSRR = GPIO_BSRR_BR_0; //OFF
	//HIGH SIDE
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_9, LL_GPIO_MODE_ALTERNATE);//Modify to PWM
}

void phaseC_HIGH_PWM()
{	//LOW SIDE
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_7, LL_GPIO_MODE_OUTPUT); //Modify to output (not PWM)
	GPIOA->BSRR = GPIO_BSRR_BR_7; //OFF
	//HIGH SIDE
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_10, LL_GPIO_MODE_ALTERNATE);//Modify to PWM
}

void allPWM()
{
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_8, LL_GPIO_MODE_ALTERNATE);//Modify to PWM A
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_9, LL_GPIO_MODE_ALTERNATE);//Modify to PWM B
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_10, LL_GPIO_MODE_ALTERNATE);//Modify to PWM C
}

//-----------------FLOAT------------------------------

void phaseAFLOAT()
{	//LOW SIDE
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_7, LL_GPIO_MODE_OUTPUT); //Modify to output (not PWM)
	GPIOA->BSRR = GPIO_BSRR_BR_7; //OFF
	//HIGH SIDE
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_8, LL_GPIO_MODE_OUTPUT);
	GPIOA->BSRR = GPIO_BSRR_BR_8;
}

void phaseBFLOAT()
{	//LOW SIDE
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_0, LL_GPIO_MODE_OUTPUT); //Modify to output (not PWM)
	GPIOB->BSRR = GPIO_BSRR_BR_0; //OFF
	//HIGH SIDE
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_9, LL_GPIO_MODE_OUTPUT);
	GPIOA->BSRR = GPIO_BSRR_BR_9;
}

void phaseCFLOAT()
{	//LOW SIDE
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_1, LL_GPIO_MODE_OUTPUT); //Modify to output (not PWM)
	GPIOB->BSRR = GPIO_BSRR_BR_1; //OFF
	//HIGH SIDE
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_10, LL_GPIO_MODE_OUTPUT);
	GPIOA->BSRR = GPIO_BSRR_BR_10;
}

//----COPM WORK------

void ChangeCopmInput()
{
	/*if (step == 1 || step == 4)
	{ // c floating
		COMP->CSR = PHASE_C_COMP;
	}
	if (step == 2 || step == 5)
	{ // a floating
		COMP->CSR = PHASE_A_COMP;
	}
	if (step == 3 || step == 6)
	{ // b floating
		COMP->CSR = PHASE_B_COMP;
	}*/
	if (rising)
	{
		EXTI->RTSR = 0x0;
		EXTI->FTSR = 0x200000;

	}
	else
	{
		// falling bemf
		EXTI->FTSR = 0x0;
		EXTI->RTSR = 0x200000;
	}
}

uint8_t getCompOutputLevel()
{
	return LL_COMP_ReadOutputLevel(COMP1);
}

void maskPhaseInterrupts()
{
	EXTI->IMR &= ~(1 << 21);
	LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_21);
}

void enableCompInterrupts()
{
	EXTI->IMR |= (1 << 21);
}

//---ARR_VALUE----

void BLDC_power()
{
	TIM6->CCR1 = throttle_lvl * PWM_ARR / 1024; //Деление на 1024 можно заменить на >>10
	TIM6->CCR2 = throttle_lvl * PWM_ARR / 1024;
	TIM6->CCR3 = throttle_lvl * PWM_ARR / 1024;
}

void interruptRoutine()
{

	if (rising) //Проверяем действительно ли нашли ноль. ЭДС должн анаходится ниже/выше псевдонейтрали
	{
		for (int i = 0; i < filter_level; i++)
		{
			if (!getCompOutputLevel())
			{
				return; //При несовпадении включаем обратно прерывания и вхыодим
			}
		}
	}
	else
	{
		for (int i = 0; i < filter_level; i++)
		{

			if (!getCompOutputLevel())
			{
				return;
			}
		}
	}
	commutate();

}

void PeriodElapsedCallback()
{
	TIM14->CNT = 0;
	TIM14->CR1 &= ~TIM_CR1_CEN;
	commutate();
	commutation_interval = ((3 * commutation_interval) + thiszctime) >> 2;
	enableCompInterrupts();
}

void commutate()
{
	if (forward == 1)
	{
		step++;
		if (step > 6)
		{
			step = 1;
		}
		rising = step % 2;
	}
	else
	{
		step--;
		if (step < 1)
		{
			step = 6;
		}
		rising = !(step % 2);
	}
	comStep(step);
	//ChangeCopmInput();
}


