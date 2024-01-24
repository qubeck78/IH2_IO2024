/*
===============================================================================
 Name        : IH2_IO2024.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#include "config.h"
#include "lpc_uart.h"
#include "dbg_printf_arm.h"
#include "ih2_io.h"
#include "ih2.h"
#include "comm.h"

int main(void)
{
	volatile ulong j;
	int i;
	char buf[1024];
	uchar c;
	ulong rx_led_clear_counter;

	LPC_SC->PCLKSEL0|=(1<<6);	//full clock for uart0
	LPC_SC->PCLKSEL1|=(1<<18);	//full clock for uart3


	lpc_uart0_init(115200,UART_8N1,UART_FIFO_OFF);
	lpc_uart3_init(115200,UART_8N1,UART_FIFO_OFF);

	comm_init();
	ih2_io_init();
	ih2_init();

	dprintf("IH2_IO2024 BUILD "_BUILD" (c)2014 Mechatronika.net\n\n");
	dprintf("Adres: %d\n",_COMM_DEV_ADRESS);
/*
	do
	{
		if (!lpc_uart3_getc(&c))
		{
			dprintf("%02x ",c);
		}

	}while(1);
*/
	NVIC_EnableIRQ(UART3_IRQn);

    __enable_irq();

    rx_led_clear_counter=1000;

	do
	{
		ih2_input_cycle();

		comm_check_command();
		if (rx_led_clear_counter)
		{
		    rx_led_clear_counter--;
		}else{
		    rx_led_clear_counter=1000;

			//turn off rx led
			ih2_rx_led(0);
		}
		ih2_output_cycle();

	}while(1);



    return 0 ;
}
