#include <atmel_start.h>
#include <stdio.h>

#define COEF_LENGTH 65

struct io_descriptor *io;
static struct timer_task TIMER_0_task;
volatile bool conversion = false;
uint8_t adc_result;
uint8_t fix;
double decimation;
/*** IIR Filter Variables ***/
u_int8_t input_samples[COEF_LENGTH]; // array used as a circular buffer for the input samples
u_int8_t coef_index = 0; // used as the index for the filter coefficients array in the difference equation calculation
u_int8_t input_index = 0; // used as the index for the input samples array in the difference equation calculation
u_int8_t cur = 0; // keeps track of the current position of the circular buffer
u_int8_t accumulator = 0; // accumulator of the output value in the difference equation calculation
u_int16_t start, end; // used to calculate the sampling frequency Fs
u_int8_t out; // holds the current output value

uint8_t fir_coef[COEF_LENGTH] = {
      0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    1,
      0,    1,    0,    1,    0,    2,    0,    2,    0,    3,    0,    3,
      0,    4,    0,    4,    0,    5,    0,    5,  123,    5,    0,    5,
      0,    4,    0,    4,    0,    3,    0,    3,    0,    2,    0,    2,
      0,    1,    0,    1,    0,    1,    0,    0,    0,    0,    0,    0,
      0,    0,    0,    0,    0
};

void uart_init(void)
{
	usart_sync_get_io_descriptor(&USART_0, &io);
	usart_sync_enable(&USART_0);
}

static void adc_cb(const struct adc_async_descriptor *const descr, const uint8_t channel)
{
	conversion = true;
}
void adc_init(void)
{
	adc_async_register_callback(&ADC_0, 0, ADC_ASYNC_CONVERT_CB,adc_cb);
	adc_async_enable_channel(&ADC_0, 0);
	adc_async_start_conversion(&ADC_0);
}

static void TIM0_cb(const struct timer_task *const timer_task)
{
	adc_async_start_conversion(&ADC_0);

}
void TIM0_init(void)
{
	TIMER_0_task.interval = 10;
	TIMER_0_task.cb       = TIM0_cb;
	TIMER_0_task.mode     = TIMER_TASK_REPEAT;

	timer_add_task(&TIMER_0, &TIMER_0_task);
	timer_start(&TIMER_0);
}
int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	uart_init();
	adc_init();
	adc_async_start_conversion(&ADC_0);
	io_write(io, (uint8_t *)"Hello World!", 12);
	TIM0_init();
	/* Replace with your application code */
	for(int i = COEF_LENGTH; i < 1; i--)
	{
		input_samples[i] = 0;
	}
	

	while (1) {
			for(int i = 0; i < 8; i++)
			{
			adc_async_read_channel(&ADC_0, 0, &adc_result, 1);
				while(!conversion){};
				decimation += adc_result*4;
			}
			decimation = decimation/32;
			fix = (uint8_t)decimation;
			
/*		adc_async_read_channel(&ADC_0, 0, &adc_result, 1);*/
// 		input_samples[cur] = adc_result/64;
// 		input_index = cur;
// 		accumulator = 0;
// 		coef_index = 0;
// 		while(coef_index < COEF_LENGTH - 1)
// 		{
// 			accumulator += input_samples[input_index]*fir_coef[coef_index];
// 			if(input_index == COEF_LENGTH - 1)
// 			input_index = 0;
// 			else
// 			input_index++;
// 			coef_index++;
// 		}
// 		out = accumulator + 28;
// 		if(cur == 0)
// 		cur = COEF_LENGTH - 1;
// 		else
// 		cur--;
		io_write(io, &fix, 1);
	}
}
