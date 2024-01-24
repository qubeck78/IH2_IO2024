#include "config.h"
#include "ih2_io.h"

int ih2_io_init(void)
{
	

	LPC_GPIO0->FIODIR&=~(IN20|IN16|IN15|IN14|IN13|IN3|IN2|IN1);
	LPC_GPIO2->FIODIR&=~(IN19|IN18|IN17|IN12|IN11|IN10|IN9|IN8|IN7|IN6|IN5|IN4);

	LPC_GPIO0->FIODIR|=(OUT12|OUT11|OUT10|OUT9);
	LPC_GPIO1->FIODIR|=(OUT24|OUT23|OUT22|OUT21|OUT20|OUT19|OUT18|OUT17|OUT16|OUT15|OUT14|OUT13|OUT8|OUT7|OUT6|OUT5|OUT4|OUT3|OUT2|OUT1);

	
	LPC_GPIO0->FIODIR|=RS485_TX_ON;
		
	LPC_GPIO1->FIODIR|=(LED_PWR|LED_TX);
	LPC_GPIO0->FIODIR|=(LED_RX|LED_ERR);

	LPC_GPIO1->FIOSET=LED_TX;
	LPC_GPIO1->FIOCLR=LED_PWR;

	LPC_GPIO0->FIOSET=LED_RX|LED_ERR;

	ih2_io_rs485_tx_off();
	return 0;
}

int ih2_io_rs485_tx_on(void)
{
	LPC_GPIO0->FIOSET=RS485_TX_ON;
	return 0;
}

int ih2_io_rs485_tx_off(void)
{
	LPC_GPIO0->FIOCLR=RS485_TX_ON;
	return 0;
}

int ih2_tx_led(ushort on)
{
	if (on)
	{
		LPC_GPIO1->FIOCLR=LED_TX;

	}else{
		LPC_GPIO1->FIOSET=LED_TX;

	}
	return 0;
}

int ih2_rx_led(ushort on)
{
	if (on)
	{
		LPC_GPIO0->FIOCLR=LED_RX;

	}else{
		LPC_GPIO0->FIOSET=LED_RX;

	}
	return 0;
}


ulong ih2_io_get_inputs()
{
	ulong rv;
	
	rv=0;
	/*
	  LPC_GPIO0->FIODIR&=~(IN20|IN16|IN15|IN14|IN13|IN3|IN2|IN1);
	  LPC_GPIO2->FIODIR&=~(IN19|IN18|IN17|IN12|IN11|IN10|IN9|IN8|IN7|IN6|IN5|IN4);
	 */
	
	if (!(LPC_GPIO0->FIOPIN&IN1)) rv|=(1<<0);
	if (!(LPC_GPIO0->FIOPIN&IN2)) rv|=(1<<1);
	if (!(LPC_GPIO0->FIOPIN&IN3)) rv|=(1<<2);
	if (!(LPC_GPIO2->FIOPIN&IN4)) rv|=(1<<3);
	if (!(LPC_GPIO2->FIOPIN&IN5)) rv|=(1<<4);
	if (!(LPC_GPIO2->FIOPIN&IN6)) rv|=(1<<5);
	if (!(LPC_GPIO2->FIOPIN&IN7)) rv|=(1<<6);
	if (!(LPC_GPIO2->FIOPIN&IN8)) rv|=(1<<7);
	if (!(LPC_GPIO2->FIOPIN&IN9)) rv|=(1<<8);
	if (!(LPC_GPIO2->FIOPIN&IN10)) rv|=(1<<9);
	if (!(LPC_GPIO2->FIOPIN&IN11)) rv|=(1<<10);
	if (!(LPC_GPIO2->FIOPIN&IN12)) rv|=(1<<11);
	if (!(LPC_GPIO0->FIOPIN&IN13)) rv|=(1<<12);
	if (!(LPC_GPIO0->FIOPIN&IN14)) rv|=(1<<13);
	if (!(LPC_GPIO0->FIOPIN&IN15)) rv|=(1<<14);
	if (!(LPC_GPIO0->FIOPIN&IN16)) rv|=(1<<15);
	if (!(LPC_GPIO2->FIOPIN&IN17)) rv|=(1<<16);
	if (!(LPC_GPIO2->FIOPIN&IN18)) rv|=(1<<17);
	if (!(LPC_GPIO2->FIOPIN&IN19)) rv|=(1<<18);
	if (!(LPC_GPIO0->FIOPIN&IN20)) rv|=(1<<19);

	return rv;
}

int ih2_io_set_output(uchar output_no,uchar value)
{
	/*
	 LPC_GPIO0->FIODIR|=(OUT12|OUT11|OUT10|OUT9);
	 LPC_GPIO1->FIODIR|=(OUT24|OUT23|OUT22|OUT21|OUT20|OUT19|OUT18|OUT17|OUT16|OUT15|OUT14|OUT13|OUT8|OUT7|OUT6|OUT5|OUT4|OUT3|OUT2|OUT1);
	 */

	switch(output_no)
	{
		case 0:
			if (!value)
			{
				LPC_GPIO1->FIOCLR=OUT1;
			}else{
				LPC_GPIO1->FIOSET=OUT1;
			}
			break;
		case 1:
			if (!value)
			{
				LPC_GPIO1->FIOCLR=OUT2;
			}else{
				LPC_GPIO1->FIOSET=OUT2;
			}
			break;
			
		case 2:
			if (!value)
			{
				LPC_GPIO1->FIOCLR=OUT3;
			}else{
				LPC_GPIO1->FIOSET=OUT3;
			}
			break;
			
		case 3:
			if (!value)
			{
				LPC_GPIO1->FIOCLR=OUT4;
			}else{
				LPC_GPIO1->FIOSET=OUT4;
			}
			break;
			
		case 4:
			if (!value)
			{
				LPC_GPIO1->FIOCLR=OUT5;
			}else{
				LPC_GPIO1->FIOSET=OUT5;
			}
			break;

		case 5:
			if (!value)
			{
				LPC_GPIO1->FIOCLR=OUT6;
			}else{
				LPC_GPIO1->FIOSET=OUT6;
			}
			break;

		case 6:
			if (!value)
			{
				LPC_GPIO1->FIOCLR=OUT7;
			}else{
				LPC_GPIO1->FIOSET=OUT7;
			}
			break;

		case 7:
			if (!value)
			{
				LPC_GPIO1->FIOCLR=OUT8;
			}else{
				LPC_GPIO1->FIOSET=OUT8;
			}
			break;

		case 8:
			if (!value)
			{
				LPC_GPIO0->FIOCLR=OUT9;
			}else{
				LPC_GPIO0->FIOSET=OUT9;
			}
			break;

		case 9:
			if (!value)
			{
				LPC_GPIO0->FIOCLR=OUT10;
			}else{
				LPC_GPIO0->FIOSET=OUT10;
			}
			break;

		case 10:
			if (!value)
			{
				LPC_GPIO0->FIOCLR=OUT11;
			}else{
				LPC_GPIO0->FIOSET=OUT11;
			}
			break;

		case 11:
			if (!value)
			{
				LPC_GPIO0->FIOCLR=OUT12;
			}else{
				LPC_GPIO0->FIOSET=OUT12;
			}
			break;

		case 12:
			if (!value)
			{
				LPC_GPIO1->FIOCLR=OUT13;
			}else{
				LPC_GPIO1->FIOSET=OUT13;
			}
			break;

		case 13:
			if (!value)
			{
				LPC_GPIO1->FIOCLR=OUT14;
			}else{
				LPC_GPIO1->FIOSET=OUT14;
			}
			break;

		case 14:
			if (!value)
			{
				LPC_GPIO1->FIOCLR=OUT15;
			}else{
				LPC_GPIO1->FIOSET=OUT15;
			}
			break;

		case 15:
			if (!value)
			{
				LPC_GPIO1->FIOCLR=OUT16;
			}else{
				LPC_GPIO1->FIOSET=OUT16;
			}
			break;

		case 16:
			if (!value)
			{
				LPC_GPIO1->FIOCLR=OUT17;
			}else{
				LPC_GPIO1->FIOSET=OUT17;
			}
			break;

		case 17:
			if (!value)
			{
				LPC_GPIO1->FIOCLR=OUT18;
			}else{
				LPC_GPIO1->FIOSET=OUT18;
			}
			break;

		case 18:
			if (!value)
			{
				LPC_GPIO1->FIOCLR=OUT19;
			}else{
				LPC_GPIO1->FIOSET=OUT19;
			}
			break;

		case 19:
			if (!value)
			{
				LPC_GPIO1->FIOCLR=OUT20;
			}else{
				LPC_GPIO1->FIOSET=OUT20;
			}
			break;

		case 20:
			if (!value)
			{
				LPC_GPIO1->FIOCLR=OUT21;
			}else{
				LPC_GPIO1->FIOSET=OUT21;
			}
			break;

		case 21:
			if (!value)
			{
				LPC_GPIO1->FIOCLR=OUT22;
			}else{
				LPC_GPIO1->FIOSET=OUT22;
			}
			break;

		case 22:
			if (!value)
			{
				LPC_GPIO1->FIOCLR=OUT23;
			}else{
				LPC_GPIO1->FIOSET=OUT23;
			}
			break;

		case 23:
			if (!value)
			{
				LPC_GPIO1->FIOCLR=OUT24;
			}else{
				LPC_GPIO1->FIOSET=OUT24;
			}
			break;

		default:
			return 1;
			break;
	}
	return 0;

}
