#include "config.h"
#include "ih2.h"
#include "ih2_io.h"

uchar ih2_inputs[_INPUT_REGS_NO][_INPUT_FILTER_DEPTH];
ushort ih2_inputs_idx;

uchar ih2_inputs_state[_INPUT_REGS_NO];
uchar ih2_inputs_prv_state[_INPUT_REGS_NO];
uchar ih2_rising_edge[_INPUT_REGS_NO];
uchar ih2_falling_edge[_INPUT_REGS_NO];
uchar ih2_any_edge[_INPUT_REGS_NO];


volatile uchar ih2_input_regs[16];
volatile uchar ih2_output_regs[16];

int ih2_init()
{
	int i,j;
	for(i=0;i<_INPUT_REGS_NO;i++)
	{
		ih2_inputs_state[i]=0;
		ih2_inputs_prv_state[i]=0;
		
		for(j=0;j<_INPUT_FILTER_DEPTH;j++)
		{
			ih2_inputs[i][j]=0;
		}
	}

	ih2_inputs_idx=0;

	for(i=0;i<sizeof(ih2_input_regs);i++)
	{
		ih2_input_regs[i]=0;
	}

	for(i=0;i<sizeof(ih2_output_regs);i++)
	{
		ih2_output_regs[i]=0;
	}
	
	return 0;
}


uchar ih2_read_single_reg(uchar adr)
{
	uchar rv;
	if (adr<sizeof(ih2_input_regs))
	{
		rv=ih2_input_regs[adr];

		if (adr==1)
		{
			//clear edge reg after reading
			ih2_input_regs[adr]=0;
			ih2_rising_edge[0]=0;
		}
		if (adr==2)
		{
			//clear edge reg after reading
			ih2_input_regs[adr]=0;
			ih2_falling_edge[0]=0;
		}

		if (adr==3)
		{
			//clear edge reg after reading
			ih2_input_regs[adr]=0;
			ih2_any_edge[0]=0;
		}

		if (adr==5)
		{
			//clear edge reg after reading
			ih2_input_regs[adr]=0;
			ih2_rising_edge[1]=0;
		}
		if (adr==6)
		{
			//clear edge reg after reading
			ih2_input_regs[adr]=0;
			ih2_falling_edge[1]=0;
		}

		if (adr==7)
		{
			//clear edge reg after reading
			ih2_input_regs[adr]=0;
			ih2_any_edge[1]=0;
		}
		if (adr==9)
		{
			ih2_input_regs[9]=0;
			ih2_rising_edge[2]=0;

		}
		if (adr==10)
		{
			ih2_input_regs[10]=0;
			ih2_falling_edge[2]=0;
		}
		if (adr==11)
		{
			ih2_input_regs[11]=0;
			ih2_any_edge[2]=0;
		}
		return rv;
	}
	return 0;
}

int ih2_write_single_reg(uchar adr,uchar data)
{
	if (adr<sizeof(ih2_output_regs))
	{
		ih2_output_regs[adr]=data;
	}
	return 0;
}

int ih2_input_filter()
{
	ulong in;
	int i,j;
	uchar is;
	uchar n1[8];
	
	//sample inputs
	in=ih2_io_get_inputs();
	
	ih2_inputs[0][ih2_inputs_idx]=in&0xff;
	ih2_inputs[1][ih2_inputs_idx++]=(in>>8)&0xff;
	ih2_inputs[2][ih2_inputs_idx++]=(in>>16)&0xff;
	
	//round buffer
	if (ih2_inputs_idx>=_INPUT_FILTER_DEPTH) ih2_inputs_idx=0;
	
	
	//count 1s
	for(i=0;i<_INPUT_REGS_NO;i++)
	{
		for(j=0;j<8;j++) 
		{
			n1[j]=0;
		}
		
		for(j=0;j<_INPUT_FILTER_DEPTH;j++)
		{
			is=ih2_inputs[i][j];
			if (is&1) n1[0]++;
			if (is&2) n1[1]++;
			if (is&4) n1[2]++;
			if (is&8) n1[3]++;
			if (is&16) n1[4]++;
			if (is&32) n1[5]++;
			if (is&64) n1[6]++;
			if (is&128) n1[7]++;
		}
		
		ih2_inputs_state[i]=0;

		if (n1[0]>=(_INPUT_FILTER_DEPTH/2))
		{
			ih2_inputs_state[i]|=1;
		}
		if (n1[1]>=(_INPUT_FILTER_DEPTH/2))
		{
			ih2_inputs_state[i]|=2;
		}
		if (n1[2]>=(_INPUT_FILTER_DEPTH/2))
		{
			ih2_inputs_state[i]|=4;
		}
		if (n1[3]>=(_INPUT_FILTER_DEPTH/2))
		{
			ih2_inputs_state[i]|=8;
		}
		if (n1[4]>=(_INPUT_FILTER_DEPTH/2))
		{
			ih2_inputs_state[i]|=16;
		}
		if (n1[5]>=(_INPUT_FILTER_DEPTH/2))
		{
			ih2_inputs_state[i]|=32;
		}
		if (n1[6]>=(_INPUT_FILTER_DEPTH/2))
		{
			ih2_inputs_state[i]|=64;
		}
		if (n1[7]>=(_INPUT_FILTER_DEPTH/2))
		{
			ih2_inputs_state[i]|=128;
		}
	}
	
	for(i=0;i<_INPUT_REGS_NO;i++)
	{
		
		for(j=0;j<8;j++)
		{
			if ((!(ih2_inputs_prv_state[i]&(1<<j)))&&(ih2_inputs_state[i]&(1<<j)))
			{
				ih2_rising_edge[i]|=(1<<j);
				ih2_any_edge[i]|=(1<<j);
			}
			if ((ih2_inputs_prv_state[i]&(1<<j))&&(!(ih2_inputs_state[i]&(1<<j))))
			{
				ih2_falling_edge[i]|=(1<<j);
				ih2_any_edge[i]|=(1<<j);
			}
		}
		
		ih2_inputs_prv_state[i]=ih2_inputs_state[i];
	}
	
	return 0;
}

int ih2_input_cycle()
{
		
	ih2_input_filter();
	
	ih2_input_regs[0]=ih2_inputs_state[0];
	ih2_input_regs[1]=ih2_rising_edge[0];
	ih2_input_regs[2]=ih2_falling_edge[0];
	ih2_input_regs[3]=ih2_any_edge[0];
	
	ih2_input_regs[4]=ih2_inputs_state[1];
	ih2_input_regs[5]=ih2_rising_edge[1];
	ih2_input_regs[6]=ih2_falling_edge[1];
	ih2_input_regs[7]=ih2_any_edge[1];

	ih2_input_regs[8]=ih2_inputs_state[2];
	ih2_input_regs[9]=ih2_rising_edge[2];
	ih2_input_regs[10]=ih2_falling_edge[2];
	ih2_input_regs[11]=ih2_any_edge[2];

	return 0;
}

int ih2_output_cycle()
{
	uchar c;
	c=ih2_output_regs[0];
	
	ih2_io_set_output(0,c&1);
	ih2_io_set_output(1,c&2);
	ih2_io_set_output(2,c&4);
	ih2_io_set_output(3,c&8);
	ih2_io_set_output(4,c&16);
	ih2_io_set_output(5,c&32);
	ih2_io_set_output(6,c&64);
	ih2_io_set_output(7,c&128);

	c=ih2_output_regs[1];

	ih2_io_set_output(8,c&1);
	ih2_io_set_output(9,c&2);
	ih2_io_set_output(10,c&4);
	ih2_io_set_output(11,c&8);
	ih2_io_set_output(12,c&16);
	ih2_io_set_output(13,c&32);
	ih2_io_set_output(14,c&64);
	ih2_io_set_output(15,c&128);

	c=ih2_output_regs[2];

	ih2_io_set_output(16,c&1);
	ih2_io_set_output(17,c&2);
	ih2_io_set_output(18,c&4);
	ih2_io_set_output(19,c&8);
	ih2_io_set_output(20,c&16);
	ih2_io_set_output(21,c&32);
	ih2_io_set_output(22,c&64);
	ih2_io_set_output(23,c&128);


	return 0;
}
