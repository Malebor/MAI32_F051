/*
 * functions.c
 *
 *  Created on: 13 февр. 2024 г.
 *      Author: шшпгп
 */

#define filter_val 100

long int s_prev = 0;
unsigned long int summ_ARR = 0;
long int ArrFilt_buf[filter_val] =
{ 0 };

long int ds_dt(long int s_in, long int ds_max)
{
	if (s_in - s_prev > ds_max)
	{
		s_prev = s_in;
		return s_prev + ds_max;
	}
	else
	{
		if (s_prev - s_in > ds_max)
		{
			s_prev = s_in;
			return s_prev - ds_max;
		}
		else
		{
			s_prev = s_in;
			return s_in;
		}
	}

}

unsigned int ARR_filter(unsigned int input_ARR)
{
	summ_ARR -= ArrFilt_buf[0];
	for (int i = 0; i < filter_val; i++)
	{
		ArrFilt_buf[i] = ArrFilt_buf[i + 1];
	}
	summ_ARR += input_ARR;
	ArrFilt_buf[filter_val - 1] = input_ARR;
	return (unsigned int) summ_ARR / filter_val;
}

void ARRfilter_reset()
{
	for (int i = 0; i < filter_val; i++)
	{
		ArrFilt_buf[i] = 2000;
	}
	summ_ARR = 2000 * filter_val;
}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
	if (x >= in_max)
		return out_max;
	if (in_min > in_max)
		return map(x, in_max, in_min, out_max, out_min);
	if (out_min == out_max)
		return out_min;
	const long in_mid = (in_min + in_max) >> 1;
	const long out_mid = (out_min + out_max) >> 1;
	if (in_min == in_mid)
		return out_mid;
	if (x <= in_mid)
		return map(x, in_min, in_mid, out_min, out_mid);
	else
		return map(x, in_mid + 1, in_max, out_mid, out_max);
}

int getAbsDif(int number1, int number2)
{
	int result = number1 - number2;
	if (result < 0)
	{
		result = -result;
	}
	return result;
}

int constrain(int input, int min, int max)
{
	if (input>max) return max;
	if (input<min) return min;
	return input;
}
