#ifndef _IH2_H
#define _IH2_H

#include "config.h"

//low pass input filter depth
#define _INPUT_FILTER_DEPTH	8
//input regs no
#define _INPUT_REGS_NO 3


int ih2_init(void);
uchar ih2_read_single_reg(uchar adr);
int ih2_write_single_reg(uchar adr,uchar data);
int ih2_input_cycle(void);
int ih2_output_cycle(void);

#endif
