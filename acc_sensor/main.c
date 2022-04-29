#include <atmel_start.h>


volatile bool set = false;
struct timer_task TIMER_task1;


void SPIwriteOne(uint8_t address, uint8_t value)
{
	uint8_t temp = (((address << 1) &~(1<<0)) << 8) | value;
	gpio_set_pin_level(slave_select, false);
	io_write(&SPI.io, &temp, 2);
	gpio_set_pin_level(slave_select, true);
}

uint8_t SPIreadOne(uint8_t address)
{
	uint8_t temp = (address << 1) | (1<<0);
	uint8_t value = 0;
	gpio_set_pin_level(slave_select, false);
	io_write(&SPI.io, &temp, 1);
	io_read(&SPI.io, &value, 1);
	gpio_set_pin_level(slave_select, true);
	return value;	
}

static void button_on_PA10_pressed(void)
{
	uint8_t temp = 9;
	//if(set == false){
		//io_write(&Serial.io, &temp, 1);
		//set = true;
	//}
	
	timer_stop(&TIMER);
	gpio_set_pin_level(led, false);
}

static void rx_cb_USART(const struct usart_async_descriptor *const io_descr)
{
	uint8_t ch;
	
	// Read  a Character
	io_read(&Serial.io, &ch, 1);
	
	if(ch == '1')
	{
		set = false;
		timer_start(&TIMER);
	}
}


static void tx_cb_USART(const struct usart_async_descriptor *const io_descr)
{
	
}

static void TIMER_task1_cb(const struct timer_task *const timer_task)
{
	
	uint8_t ergebnis = SPIreadOne(0x00);
	
	io_write(&Serial.io, &ergebnis, 1);
	
	gpio_toggle_pin_level(led);
}



int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	// UART
	usart_async_register_callback(&Serial, USART_ASYNC_TXC_CB, tx_cb_USART);
	usart_async_register_callback(&Serial, USART_ASYNC_RXC_CB, rx_cb_USART);
	usart_async_enable(&Serial);
	
	// Interrupt
	ext_irq_register(button, button_on_PA10_pressed);	
	
	// Timer
	TIMER_task1.interval = 3;
	TIMER_task1.cb = TIMER_task1_cb;
	TIMER_task1.mode = TIMER_TASK_REPEAT;
	
	timer_add_task(&TIMER, &TIMER_task1);
	timer_start(&TIMER);
	
	// SPI
	spi_m_sync_enable(&SPI);
	//io_write(&SPI.io, 8, 12);
	
	while (1) {
		
		
		
		



	}
}

