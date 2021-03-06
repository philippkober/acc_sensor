/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_examples.h"
#include "driver_init.h"
#include "utils.h"

static void button_on_PA10_pressed(void)
{
}

/**
 * Example of using EXTERNAL_IRQ
 */
void EXTERNAL_IRQ_example(void)
{

	ext_irq_register(PIN_PA10, button_on_PA10_pressed);
}

/**
 * Example of using SPI to write "Hello World" using the IO abstraction.
 */
static uint8_t example_SPI[12] = "Hello World!";

void SPI_example(void)
{
	struct io_descriptor *io;
	spi_m_sync_get_io_descriptor(&SPI, &io);

	spi_m_sync_enable(&SPI);
	io_write(io, example_SPI, 12);
}

/**
 * Example of using Serial to write "Hello World" using the IO abstraction.
 *
 * Since the driver is asynchronous we need to use statically allocated memory for string
 * because driver initiates transfer and then returns before the transmission is completed.
 *
 * Once transfer has been completed the tx_cb function will be called.
 */

static uint8_t example_Serial[12] = "Hello World!";

static void tx_cb_Serial(const struct usart_async_descriptor *const io_descr)
{
	/* Transfer completed */
}

void Serial_example(void)
{
	struct io_descriptor *io;

	usart_async_register_callback(&Serial, USART_ASYNC_TXC_CB, tx_cb_Serial);
	/*usart_async_register_callback(&Serial, USART_ASYNC_RXC_CB, rx_cb);
	usart_async_register_callback(&Serial, USART_ASYNC_ERROR_CB, err_cb);*/
	usart_async_get_io_descriptor(&Serial, &io);
	usart_async_enable(&Serial);

	io_write(io, example_Serial, 12);
}

static struct timer_task TIMER_task1, TIMER_task2;

/**
 * Example of using TIMER.
 */
static void TIMER_task1_cb(const struct timer_task *const timer_task)
{
}

static void TIMER_task2_cb(const struct timer_task *const timer_task)
{
}

void TIMER_example(void)
{
	TIMER_task1.interval = 100;
	TIMER_task1.cb       = TIMER_task1_cb;
	TIMER_task1.mode     = TIMER_TASK_REPEAT;
	TIMER_task2.interval = 200;
	TIMER_task2.cb       = TIMER_task2_cb;
	TIMER_task2.mode     = TIMER_TASK_REPEAT;

	timer_add_task(&TIMER, &TIMER_task1);
	timer_add_task(&TIMER, &TIMER_task2);
	timer_start(&TIMER);
}
