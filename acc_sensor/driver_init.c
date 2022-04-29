/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_init.h"
#include <peripheral_clk_config.h>
#include <utils.h>
#include <hal_init.h>
#include <hpl_gclk_base.h>
#include <hpl_pm_base.h>

/*! The buffer size for USART */
#define SERIAL_BUFFER_SIZE 16

struct spi_m_sync_descriptor  SPI;
struct usart_async_descriptor Serial;
struct timer_descriptor       TIMER;

static uint8_t Serial_buffer[SERIAL_BUFFER_SIZE];

void EXTERNAL_IRQ_init(void)
{
	_gclk_enable_channel(EIC_GCLK_ID, CONF_GCLK_EIC_SRC);

	// Set pin direction to input
	gpio_set_pin_direction(button, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(button,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_UP);

	gpio_set_pin_function(button, PINMUX_PA10A_EIC_EXTINT10);

	ext_irq_init();
}

void SPI_PORT_init(void)
{

	// Set pin direction to input
	gpio_set_pin_direction(PA05, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(PA05,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PA05, PINMUX_PA05D_SERCOM0_PAD1);

	gpio_set_pin_level(PA06,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(PA06, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(PA06, PINMUX_PA06D_SERCOM0_PAD2);

	gpio_set_pin_level(PA07,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(PA07, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(PA07, PINMUX_PA07D_SERCOM0_PAD3);
}

void SPI_CLOCK_init(void)
{
	_pm_enable_bus_clock(PM_BUS_APBC, SERCOM0);
	_gclk_enable_channel(SERCOM0_GCLK_ID_CORE, CONF_GCLK_SERCOM0_CORE_SRC);
}

void SPI_init(void)
{
	SPI_CLOCK_init();
	spi_m_sync_init(&SPI, SERCOM0);
	SPI_PORT_init();
}

/**
 * \brief USART Clock initialization function
 *
 * Enables register interface and peripheral clock
 */
void Serial_CLOCK_init()
{

	_pm_enable_bus_clock(PM_BUS_APBC, SERCOM4);
	_gclk_enable_channel(SERCOM4_GCLK_ID_CORE, CONF_GCLK_SERCOM4_CORE_SRC);
}

/**
 * \brief USART pinmux initialization function
 *
 * Set each required pin to USART functionality
 */
void Serial_PORT_init()
{

	gpio_set_pin_function(PB08, PINMUX_PB08D_SERCOM4_PAD0);

	gpio_set_pin_function(PB09, PINMUX_PB09D_SERCOM4_PAD1);
}

/**
 * \brief USART initialization function
 *
 * Enables USART peripheral, clocks and initializes USART driver
 */
void Serial_init(void)
{
	Serial_CLOCK_init();
	usart_async_init(&Serial, SERCOM4, Serial_buffer, SERIAL_BUFFER_SIZE, (void *)NULL);
	Serial_PORT_init();
}

/**
 * \brief Timer initialization function
 *
 * Enables Timer peripheral, clocks and initializes Timer driver
 */
static void TIMER_init(void)
{
	_pm_enable_bus_clock(PM_BUS_APBC, TC3);
	_gclk_enable_channel(TC3_GCLK_ID, CONF_GCLK_TC3_SRC);

	timer_init(&TIMER, TC3, _tc_get_timer());
}

void system_init(void)
{
	init_mcu();

	// GPIO on PA09

	gpio_set_pin_level(slave_select,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(slave_select, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(slave_select, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA17

	gpio_set_pin_level(led,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(led, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(led, GPIO_PIN_FUNCTION_OFF);

	EXTERNAL_IRQ_init();

	SPI_init();
	Serial_init();

	TIMER_init();
}
