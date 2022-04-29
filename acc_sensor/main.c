#include <atmel_start.h>


volatile bool set = false;
struct timer_task TIMER_task1;



void SPIwrite(uint8_t address, uint8_t value)
{
	uint16_t temp =(address << 1) &~(1<<0);
	gpio_set_pin_level(slave_select, false);
	io_write(&SPI.io, &temp, 1);
	io_write(&SPI.io, &value, 1);
	gpio_set_pin_level(slave_select, true);
}

void SPIread(const uint8_t address, uint8_t* buf, const uint16_t length)
{
	uint8_t temp = (address << 1) | (1<<0);
	
	uint8_t test[4];
		
	gpio_set_pin_level(slave_select, false);
	io_write(&SPI.io, &temp, 1);
	volatile int32_t readlaenge = io_read(&SPI.io, buf, length);
	gpio_set_pin_level(slave_select, true);	
}

static void button_on_PA10_pressed(void)
{
	
	timer_stop(&TIMER);
	gpio_set_pin_level(led, false);
	
}

static void rx_cb_USART(const struct usart_async_descriptor *const io_descr)
{
	uint8_t ch;
	

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
	uint8_t temp[4];
	
	SPIread(0x0E, temp, 4);
	
	io_write(&Serial.io, temp, 4);
	
	gpio_toggle_pin_level(led);
}

void acc_init()
{
	uint8_t temp;
	
	SPIwrite(0x2F, 0x52);
	
	SPIwrite(0x2c,0b01);	// 2g
	
	//SPIread(0x2b, &temp, 1);
	//SPIwrite(0x2b, temp | 0b10);	// Externer Trigger
	
	SPIread(0x2D, &temp, 1);
	SPIwrite(0x2d, temp &~(1<<0));	// Measurement aktivieren

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
		
	// SPI
	spi_m_sync_enable(&SPI);
	
	acc_init();
	
	// Timer
	TIMER_task1.interval = 3;
	TIMER_task1.cb = TIMER_task1_cb;
	TIMER_task1.mode = TIMER_TASK_REPEAT;
		
	timer_add_task(&TIMER, &TIMER_task1);
	timer_start(&TIMER);
	
	while (1) {
		

		



	}
}

