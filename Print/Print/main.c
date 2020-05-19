#include <atmel_start.h>

//struct io_descriptor *io;
static struct timer_task TIMER_0_task;

void uart_init(void)
{
	//usart_sync_get_io_descriptor(&USART_0, &io);
	usart_sync_enable(&USART_0);
}

static void adc_cb(const struct adc_async_descriptor *const descr, const uint8_t channel)
{
	//conversion = true;
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
	adc_init();
	uart_init();
	TIM0_init();
	/* Replace with your application code */
	while (1) {
		printf("Testing");
	}
}
