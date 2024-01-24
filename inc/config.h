/*
 * config.h
 *
 *  Created on: 2 wrz 2014
 *      Author: qubeck
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

#include <stdio.h>

//device config

#define _COMM_DEV_ADRESS 6

#define _VERSION 0x0020
#define _NUM_INPUTS 20
#define _NUM_OUTPUTS 24
#define _NUM_ANALOG_INPUTS 0
#define _NUM_ANALOG_OUTPUTS 0
#define _NUM_TEMP_SENSORS 0

#define UART3_RX_IRQ

#define _BUILD "20150514"

#define uchar unsigned char
#define ushort unsigned short
#define ulong unsigned long



#endif /* CONFIG_H_ */
