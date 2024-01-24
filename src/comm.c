 
#include "config.h"
#include "comm.h"
#include "lpc_uart.h"
#include "ih2_io.h"
#include "ih2.h"
#include "dbg_printf_arm.h"

enum TRcvState {trs_wait_for_soh,trs_from,trs_to,trs_cmd,trs_dlen,trs_hdrsum,trs_data,trs_data_sum,
				trs_eop};


volatile uchar comm_tx_buffer[256];
volatile uchar comm_rx_buffer[512];
volatile uchar comm_tmp_rx_buffer[512];
volatile ulong comm_tmp_rx_idx;
volatile ushort comm_rcv_bad_packet;

volatile enum TRcvState RcvState;
volatile ushort comm_rcv_data_length;
volatile ushort comm_rcv_escaped;
volatile ushort comm_rcv_data_idx;	
volatile ushort comm_rcv_packet_received;
volatile uchar comm_rcv_cmd;
volatile uchar comm_rcv_from;
volatile uchar comm_rcv_to;

volatile uchar comm_rcv_eor;

volatile uchar comm_dev_address;
volatile ulong comm_rx_bytes;
volatile ulong comm_rejected_bytes;
volatile ushort comm_tx_idx;
volatile ulong comm_hdrsum_errors;


int comm_init()
{
	int i;
	
	RcvState=trs_wait_for_soh;
	comm_rcv_data_length=0;
	comm_rcv_escaped=0;
	comm_rcv_data_idx=0;
	comm_rcv_packet_received=0;
	comm_rcv_from=0;
	comm_rcv_to=0;
	comm_rcv_cmd=0;
	comm_rx_bytes=0;
	comm_rejected_bytes=0;
	comm_hdrsum_errors=0;
	
	comm_dev_address=_COMM_DEV_ADRESS;
	comm_tmp_rx_idx=0;
	comm_rcv_bad_packet=0;

	return 0;
}

int comm_send_buffer(uchar *buf,ushort len)
{
	ushort i;
	uchar rc;
	uchar collision;
	volatile ulong j;
	
	//loght tx led
	ih2_tx_led(1);

	//disable rx interrupt
	LPC_UART3->IER&=(0xffffffff^UIER_ERBFI);	//disable rx interrupt

	lpc_uart3_getc(&rc);

	ih2_io_rs485_tx_on();

	for(j=0;j<1000;j++);
	
	collision=0;
	
	for(i=0;i<len;i++)
	{
		//wait'til buf empty
		while (!(LPC_UART3->LSR & ULSR_THRE));

		//send char
		LPC_UART3->THR = (uchar)buf[i];

		//wait'til char transmitted
		while (!(LPC_UART3->LSR & ULSR_THRE));


		//receive char
		j=0;
		while(lpc_uart3_getc(&rc))
		{
			j++;
			if (j>1000) {collision|=1;break;}
		}

		if (rc!=buf[i])
		{
			collision|=1;
		}
	}
	
	for(j=0;j<1000;j++);
	
	//disable transmitter
	ih2_io_rs485_tx_off();

	LPC_UART3->IIR;             // clear interrupt ID register
	//
	LPC_UART3->LSR;             // clear line status register

	#ifdef UART3_RX_IRQ
	LPC_UART3->IER|=UIER_ERBFI;	//enable rx interrupt
	#endif

	//turn off tx led
	ih2_tx_led(0);

//	if (collision) dprintf("COL! ");
	return collision;
}


int comm_tx_insert(unsigned char c,uchar *comm_snd_eor)
{
   *comm_snd_eor=*comm_snd_eor^c;

	if (c>=_COMM_ESC)
	{
		comm_tx_buffer[comm_tx_idx]=_COMM_ESC;
		comm_tx_idx++;
		comm_tx_buffer[comm_tx_idx]=c-_COMM_ESC;
		comm_tx_idx++;
	}else{
		comm_tx_buffer[comm_tx_idx]=c;
		comm_tx_idx++;
	}
	return 0;
}


int comm_send(uchar to_address,uchar cmd)
{
	ushort i;
	volatile ulong j;	
	uchar eor;
	
	i=0;
	
	comm_tx_idx=0;
	comm_tx_buffer[comm_tx_idx++]=_COMM_SOH;		//soh
	eor=_COMM_EOR_BASE;
	
	comm_tx_insert(comm_dev_address,&eor);	//from
	comm_tx_insert(to_address,&eor);			//to
	comm_tx_insert(cmd,&eor);			//cmd
	comm_tx_insert(0,&eor);				//dlen=0;
	comm_tx_insert(eor,&eor);			//eor
	comm_tx_buffer[comm_tx_idx++]=_COMM_EOP;		//eop

	return comm_send_buffer(comm_tx_buffer,comm_tx_idx);
}

int comm_send_data(uchar to_address,uchar cmd,uchar *data,uchar data_len)
{
	ushort i;
	ushort j;	
	uchar eor;
	
	i=0;
	
	comm_tx_idx=0;
	comm_tx_buffer[comm_tx_idx++]=_COMM_SOH;		//soh
	eor=_COMM_EOR_BASE;
	
	comm_tx_insert(comm_dev_address,&eor);	//from
	comm_tx_insert(to_address,&eor);			//to
	comm_tx_insert(cmd,&eor);			//cmd
	comm_tx_insert(data_len,&eor);		//dlen
	comm_tx_insert(eor,&eor);			//eor

	eor=_COMM_EOR_BASE;
	
	for(j=0;j<data_len;j++)
	{
		comm_tx_insert(data[j],&eor);			//data
	}
	comm_tx_insert(eor,&eor);			//data eor
	
	comm_tx_buffer[comm_tx_idx++]=_COMM_EOP;		//eop
	
	return comm_send_buffer(comm_tx_buffer,comm_tx_idx);
}



int comm_receiver(uchar rx)
{
	uchar c;
	
	//light rx led
	ih2_rx_led(1);
				
	if (comm_rcv_packet_received)
	{
		comm_rejected_bytes++;
		return 0;
	}
	
	if (comm_tmp_rx_idx<512)
	{
		if (comm_rcv_bad_packet==0)
		{
			comm_tmp_rx_buffer[comm_tmp_rx_idx++]=rx;
		}
	}
	comm_rx_bytes++;
	
	if (comm_rcv_escaped==0)
	{
		c=rx;
		
		//receiver fsm
		if (rx==_COMM_SOH)
		{
			RcvState=trs_wait_for_soh;
		}else if(rx==_COMM_ESC)
		{
			comm_rcv_escaped=1;
			return 0;
		}
	}else{
	
		//escaped char -> add 0xf0 to it
		c=rx+_COMM_ESC;
		comm_rcv_escaped=0;
	}
	
	switch(RcvState)
	{
		case trs_wait_for_soh:

			
			if (rx==_COMM_SOH)
			{
				comm_tmp_rx_idx=0;
				comm_tmp_rx_buffer[comm_tmp_rx_idx++]=_COMM_SOH;

				comm_rcv_data_length=0;
				comm_rcv_data_idx=0;
				comm_rcv_eor=_COMM_EOR_BASE;
				
				RcvState=trs_from;
			}
			break;
	
		case trs_from:

		    comm_rcv_eor^=c;
			comm_rcv_from=c;
			
			RcvState=trs_to;
			
			break;
			
		case trs_to:
			
			comm_rcv_eor^=c;
			comm_rcv_to=c;
			
			RcvState=trs_cmd;
			break;

		case trs_cmd:
			
			comm_rcv_cmd=c;
			comm_rcv_eor^=c;

			RcvState=trs_dlen;
			break;

		case trs_dlen:
			
			comm_rcv_eor^=c;
			comm_rcv_data_length=c;
			RcvState=trs_hdrsum;
			break;
			
		case trs_hdrsum:
		
			if (c!=comm_rcv_eor)
			{
				comm_hdrsum_errors++;
				comm_rcv_bad_packet=1;

				RcvState=trs_wait_for_soh;
			}else{

				comm_rcv_eor=_COMM_EOR_BASE;
				
				if (comm_rcv_data_length>0)
				{
					RcvState=trs_data;
				}else{
				
					comm_rcv_packet_received=1;				
					RcvState=trs_wait_for_soh;

				}
			
			}
			
			break;
			
		case trs_data:
		
			comm_rcv_eor^=c;
			
			comm_rx_buffer[comm_rcv_data_idx++]=c;
			comm_rcv_data_length--;
			
			if (comm_rcv_data_length==0)
			{
				RcvState=trs_data_sum;
			}
			
			break;
			
		case trs_data_sum:
		
			if (comm_rcv_data_idx==0)
			{
				comm_rcv_packet_received=1;
			}else{
				if (c==comm_rcv_eor)
				{
					comm_rcv_packet_received=1;
				}else{
					comm_rcv_packet_received=0;
				}
			}
			
			RcvState=trs_wait_for_soh;				
			break;	
	}
	

	return 0;
}


int comm_check_command()
{
	int rv;
	uchar rd;
	int i;
	uchar local_tx_buf[256];
		



	if (comm_rcv_bad_packet)
	{

		dprintf("\nBP");
		for(i=0;i<comm_tmp_rx_idx;i++)
		{
			dprintf("%02X ",comm_tmp_rx_buffer[i]);

		}
		dprintf("\n");
		comm_rcv_bad_packet=0;
	}

	if (comm_rcv_packet_received==0) return 0;


	if ((comm_rcv_from==0)&&(comm_rcv_to==comm_dev_address))
	{
		switch(comm_rcv_cmd)
		{
			case _COMM_CMD_REQ_DEV_INFO:
//				dprintf("RDI ");
				local_tx_buf[0]=0xa1;	//digital i/o module
				local_tx_buf[1]=comm_dev_address;	//device address
				local_tx_buf[2]=_VERSION/0x100;		//software version
				local_tx_buf[3]=_VERSION%0x100;
				local_tx_buf[4]=_NUM_INPUTS;
				local_tx_buf[5]=_NUM_OUTPUTS;
				local_tx_buf[6]=_NUM_ANALOG_INPUTS;
				local_tx_buf[7]=_NUM_ANALOG_OUTPUTS;
				local_tx_buf[8]=_NUM_TEMP_SENSORS;
				
				comm_send_data(0,_COMM_CMD_REQ_DEV_INFO|0x80,local_tx_buf,9);
			
				break;

			case _COMM_CMD_WRITE_SINGLE_REG:

//				dprintf("WSR %d ",comm_hdrsum_errors);

				if (comm_rcv_data_idx>1)
				{
					ih2_write_single_reg(comm_rx_buffer[0],comm_rx_buffer[1]);
					comm_send(0,_COMM_CMD_WRITE_SINGLE_REG|0x80);
				}else{
					comm_send(0,_COMM_CMD_WRITE_SINGLE_REG|0x80|0x40);
				}
				break;
				
			case _COMM_CMD_READ_SINGLE_REG:

//				dprintf("RSR %d ",comm_hdrsum_errors);

				if (comm_rcv_data_idx>0)
				{
					rd=ih2_read_single_reg(comm_rx_buffer[0]);
					local_tx_buf[0]=rd;
					comm_send_data(0,_COMM_CMD_READ_SINGLE_REG|0x80,local_tx_buf,1);
				}else{
					comm_send(0,_COMM_CMD_READ_SINGLE_REG|0x80|0x40);				
				}
				
				break;

			case _COMM_CMD_READ_MULTIPLE_REG:

	//			dprintf("RMR %d ",comm_hdrsum_errors);
				
				if (comm_rcv_data_idx>1)
				{
					for(i=0;i<comm_rx_buffer[1];i++)
					{
						rd=ih2_read_single_reg(comm_rx_buffer[0]+i);
						local_tx_buf[i]=rd;
					}
					
					comm_send_data(0,_COMM_CMD_READ_MULTIPLE_REG|0x80,local_tx_buf,comm_rx_buffer[1]);

				}else{
					comm_send(0,_COMM_CMD_READ_MULTIPLE_REG|0x80|0x40);				
				}
			
				break;
			case _COMM_CMD_WRITE_MULTIPLE_REG:

				if (comm_rcv_data_idx>2)
				{
					for(i=0;i<comm_rx_buffer[1];i++)
					{
						ih2_write_single_reg(comm_rx_buffer[0]+i,comm_rx_buffer[2+i]);
					}
					comm_send(0,_COMM_CMD_WRITE_MULTIPLE_REG|0x80);
				}else{
					comm_send(0,_COMM_CMD_WRITE_MULTIPLE_REG|0x80|0x40);
				}

				break;
			default:
				comm_send(0,_COMM_CMD_UNSUPPORTED_COMMAND);
			break;
		}
	}
	
	comm_rcv_packet_received=0;
	return 0;
}


