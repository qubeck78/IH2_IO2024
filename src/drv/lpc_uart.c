/*
 * lpc_uart.c
 *
 *  Created on: 12-12-2013
 *      Author: qubeck
 */

#include "config.h"
#include "lpc_uart.h"
#include "comm.h"

int lpc_uart0_init(ulong baudrate,uchar uart_mode,uchar fifo_mode)
{
	ulong Fdiv;
	ulong pclkdiv, pclk;

	//Setup uC pins
	LPC_PINCON->PINSEL0 &= ~0x000000F0;
	LPC_PINCON->PINSEL0 |= 0x00000050;  /* RxD0 is P0.3 and TxD0 is P0.2 */

	/* By default, the PCLKSELx value is zero, thus, the PCLK for
	all the peripherals is 1/4 of the SystemFrequency. */
	/* Bit 6~7 is for UART0 */
	pclkdiv = (LPC_SC->PCLKSEL0 >> 6) & 0x03;
	switch ( pclkdiv )
	{
	  case 0x00:
	  default:
		pclk = SystemCoreClock/4;
		break;
	  case 0x01:
		pclk = SystemCoreClock;
		break;
	  case 0x02:
		pclk = SystemCoreClock/2;
		break;
	  case 0x03:
		pclk = SystemCoreClock/8;
		break;
	}

	Fdiv = ( pclk / 16 ) / baudrate ;	/*baud rate */


	LPC_UART0->IER = 0x00;             // disable all interrupts

//	LPC_UART0->IIR = 0x00;             // clear interrupt ID register
//	LPC_UART0->LSR = 0x00;             // clear line status register

	// set the baud rate - DLAB must be set to access DLL/DLM
	LPC_UART0->LCR = (UART_LCR_DLAB); // set divisor latches (DLAB)

	LPC_UART0->DLL = (uchar)Fdiv;         // set for baud low byte
	LPC_UART0->DLM = (uchar)(Fdiv >> 8);  // set for baud high byte

	// set the number of characters and other
	// user specified operating parameters
	// Data bits, Parity, Stop bits - Settings in Line Control Register
	LPC_UART0->LCR = (uart_mode & ~(UART_LCR_DLAB)); // clear DLAB "on-the-fly"
	// setup FIFO Control Register (fifo - enabled + xx trig)
	LPC_UART0->FCR = fifo_mode;

	#ifdef UART0_RX_IRQ
	LPC_UART0->IER|=UIER_ERBFI;	//enable rx interrupt
	#endif


	return 0;
}

int lpc_uart0_putc(uchar c)
{
	//wait for empty tx buffer
	while (!(LPC_UART0->LSR & ULSR_THRE))
		continue;

	//send char
	LPC_UART0->THR = (uchar)c;

	return 0;	//ok
}

int lpc_uart0_send_buf(uchar *buf,int length)
{
	int i;
	for(i=0;i<length;i++)
	{
		lpc_uart0_putc(*buf++);
	}
	return 0;
}


int lpc_uart0_getc(uchar *c)
{

	if (LPC_UART0->LSR & ULSR_RDR)
	{
		if (c) {*c=LPC_UART0->RBR;}
		return 0;
	}else{
		if (c) {*c=0;}
		return 1;
	}
}

int lpc_uart1_init(ulong baudrate,uchar uart_mode,uchar fifo_mode)
{
	ulong Fdiv;
	ulong pclkdiv, pclk;

	//Setup uC pins

	LPC_PINCON->PINSEL0 &= ~0xC0000000;
	LPC_PINCON->PINSEL0 |= 0x40000000;	//P0.15 is TXD1

	LPC_PINCON->PINSEL1 &= ~0x03;
	LPC_PINCON->PINSEL1 |= 0x1;			//P0.16 is RXD1

	/* By default, the PCLKSELx value is zero, thus, the PCLK for
	all the peripherals is 1/4 of the SystemFrequency. */
	/* Bit 8,9 are for UART1 */
	pclkdiv = (LPC_SC->PCLKSEL0 >> 8) & 0x03;
	switch ( pclkdiv )
	{
	  case 0x00:
	  default:
		pclk = SystemCoreClock/4;
		break;
	  case 0x01:
		pclk = SystemCoreClock;
		break;
	  case 0x02:
		pclk = SystemCoreClock/2;
		break;
	  case 0x03:
		pclk = SystemCoreClock/8;
		break;
	}

	Fdiv = ( pclk / 16 ) / baudrate ;	/*baud rate */


	LPC_UART1->IER = 0x00;             // disable all interrupts

//	LPC_UART0->IIR = 0x00;             // clear interrupt ID register
//	LPC_UART0->LSR = 0x00;             // clear line status register

	// set the baud rate - DLAB must be set to access DLL/DLM
	LPC_UART1->LCR = (UART_LCR_DLAB); // set divisor latches (DLAB)

	LPC_UART1->DLL = (uchar)Fdiv;         // set for baud low byte
	LPC_UART1->DLM = (uchar)(Fdiv >> 8);  // set for baud high byte

	// set the number of characters and other
	// user specified operating parameters
	// Data bits, Parity, Stop bits - Settings in Line Control Register
	LPC_UART1->LCR = (uart_mode & ~(UART_LCR_DLAB)); // clear DLAB "on-the-fly"
	// setup FIFO Control Register (fifo - enabled + xx trig)
	LPC_UART1->FCR = fifo_mode;

	#ifdef UART1_RX_IRQ
	LPC_UART1->IER|=UIER_ERBFI;	//enable rx interrupt
	#endif


	return 0;
}

int lpc_uart1_putc(uchar c)
{
	//wait for empty tx buffer
	while (!(LPC_UART1->LSR & ULSR_THRE))
		continue;

	//send char
	LPC_UART1->THR = (uchar)c;

	return 0;	//ok
}

int lpc_uart1_send_buf(uchar *buf,int length)
{
	int i;
	for(i=0;i<length;i++)
	{
		lpc_uart1_putc(*buf++);
	}
	return 0;
}


int lpc_uart1_getc(uchar *c)
{

	if (LPC_UART1->LSR & ULSR_RDR)
	{
		if (c) {*c=LPC_UART1->RBR;}
		return 0;
	}else{
		if (c) {*c=0;}
		return 1;
	}
}

int lpc_uart2_init(ulong baudrate,uchar uart_mode,uchar fifo_mode)
{
	ulong Fdiv;
	ulong pclkdiv, pclk;

	//Setup uC pins

//	LPC_PINCON->PINSEL0 &= ~0xC0000000;
//	LPC_PINCON->PINSEL0 |= 0x40000000;	//P0.15 is TXD1

//	LPC_PINCON->PINSEL1 &= ~0x03;
//	LPC_PINCON->PINSEL1 |= 0x1;			//P0.16 is RXD1

	/* By default, the PCLKSELx value is zero, thus, the PCLK for
	all the peripherals is 1/4 of the SystemFrequency. */
	/* Bit 8,9 are for UART1 */
	pclkdiv = (LPC_SC->PCLKSEL1 >> 16) & 0x03;
	switch ( pclkdiv )
	{
	  case 0x00:
	  default:
		pclk = SystemCoreClock/4;
		break;
	  case 0x01:
		pclk = SystemCoreClock;
		break;
	  case 0x02:
		pclk = SystemCoreClock/2;
		break;
	  case 0x03:
		pclk = SystemCoreClock/8;
		break;
	}

	Fdiv = ( pclk / 16 ) / baudrate ;	/*baud rate */


	LPC_UART2->IER = 0x00;             // disable all interrupts

//	LPC_UART0->IIR = 0x00;             // clear interrupt ID register
//	LPC_UART0->LSR = 0x00;             // clear line status register

	// set the baud rate - DLAB must be set to access DLL/DLM
	LPC_UART2->LCR = (UART_LCR_DLAB); // set divisor latches (DLAB)

	LPC_UART2->DLL = (uchar)Fdiv;         // set for baud low byte
	LPC_UART2->DLM = (uchar)(Fdiv >> 8);  // set for baud high byte

	// set the number of characters and other
	// user specified operating parameters
	// Data bits, Parity, Stop bits - Settings in Line Control Register
	LPC_UART2->LCR = (uart_mode & ~(UART_LCR_DLAB)); // clear DLAB "on-the-fly"
	// setup FIFO Control Register (fifo - enabled + xx trig)
	LPC_UART2->FCR = fifo_mode;

	#ifdef UART2_RX_IRQ
	LPC_UART2->IER|=UIER_ERBFI;	//enable rx interrupt
	#endif


	return 0;
}

int lpc_uart2_putc(uchar c)
{
	//wait for empty tx buffer
	while (!(LPC_UART2->LSR & ULSR_THRE))
		continue;

	//send char
	LPC_UART2->THR = (uchar)c;

	return 0;	//ok
}

int lpc_uart2_send_buf(uchar *buf,int length)
{
	int i;
	for(i=0;i<length;i++)
	{
		lpc_uart2_putc(*buf++);
	}
	return 0;
}


int lpc_uart2_getc(uchar *c)
{

	if (LPC_UART2->LSR & ULSR_RDR)
	{
		if (c) {*c=LPC_UART2->RBR;}
		return 0;
	}else{
		if (c) {*c=0;}
		return 1;
	}
}

int lpc_uart3_init(ulong baudrate,uchar uart_mode,uchar fifo_mode)
{
	ulong Fdiv;
	ulong pclkdiv, pclk;

	//Setup uC pins

	//P4.28 is TXD3
	//P4.29 is RXD3
	LPC_SC->PCONP|=(1<<25);	//PCUART3

	LPC_PINCON->PINSEL9|=((1<<24)|(1<<25)|(1<<26)|(1<<27));

	/* By default, the PCLKSELx value is zero, thus, the PCLK for
	all the peripherals is 1/4 of the SystemFrequency. */
	/* Bit 8,9 are for UART1 */
	pclkdiv = (LPC_SC->PCLKSEL1 >> 18) & 0x03;
	switch ( pclkdiv )
	{
	  case 0x00:
	  default:
		pclk = SystemCoreClock/4;
		break;
	  case 0x01:
		pclk = SystemCoreClock;
		break;
	  case 0x02:
		pclk = SystemCoreClock/2;
		break;
	  case 0x03:
		pclk = SystemCoreClock/8;
		break;
	}

	Fdiv = ( pclk / 16 ) / baudrate ;	/*baud rate */


	LPC_UART3->IER = 0x00;             // disable all interrupts

	//
	LPC_UART3->IIR;             // clear interrupt ID register
	//
	LPC_UART3->LSR;             // clear line status register

	// set the baud rate - DLAB must be set to access DLL/DLM
	LPC_UART3->LCR = (UART_LCR_DLAB); // set divisor latches (DLAB)

	LPC_UART3->DLL = (uchar)Fdiv;         // set for baud low byte
	LPC_UART3->DLM = (uchar)(Fdiv >> 8);  // set for baud high byte

	// set the number of characters and other
	// user specified operating parameters
	// Data bits, Parity, Stop bits - Settings in Line Control Register
	LPC_UART3->LCR = (uart_mode & ~(UART_LCR_DLAB)); // clear DLAB "on-the-fly"
	// setup FIFO Control Register (fifo - enabled + xx trig)
	LPC_UART3->FCR = fifo_mode;

	#ifdef UART3_RX_IRQ
	LPC_UART3->IER=UIER_ERBFI;	//enable rx interrupt
	#endif


	return 0;
}

int lpc_uart3_putc(uchar c)
{
	//wait for empty tx buffer
	while (!(LPC_UART3->LSR & ULSR_THRE))
		continue;

	//send char
	LPC_UART3->THR = (uchar)c;

	return 0;	//ok
}

int lpc_uart3_send_buf(uchar *buf,int length)
{
	int i;
	for(i=0;i<length;i++)
	{
		lpc_uart3_putc(*buf++);
	}
	return 0;
}


int lpc_uart3_getc(uchar *c)
{

	if (LPC_UART3->LSR & ULSR_RDR)
	{
		if (c) {*c=LPC_UART3->RBR;}
		return 0;
	}else{
		if (c) {*c=0;}
		return 1;
	}
}


void UART3_IRQHandler(void)
{
	uchar c;
	ulong iir;

	iir=LPC_UART3->IIR;

	if (iir&UIIR_RDA_INT)
	{
		//rx byte

		//line status
		if (LPC_UART3->LSR&ULSR_RDR)
		{
			//receive char
			c=LPC_UART3->RBR;

			//receiver
			comm_receiver(c);
		}
	}
}
