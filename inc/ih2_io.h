#ifndef _IH2_IO_H
#define _IH2_IO_H
#include "config.h"

//in1 p0.7
#define IN1_PIN 7
//in2 p0.8
#define IN2_PIN 8
//in3 p0.9
#define IN3_PIN 9
//in4 p2.0
#define IN4_PIN 0
//in5 p2.1
#define IN5_PIN 1
//in6 p2.2
#define IN6_PIN 2
//in7 p2.3
#define IN7_PIN 3
//in8 p2.4
#define IN8_PIN 4
//in9 p2.5
#define IN9_PIN 5
//in10 p2.6
#define IN10_PIN 6
//in11 p2.7
#define IN11_PIN 7
//in12 p2.8
#define IN12_PIN 8
//in13 p0.19
#define IN13_PIN 19
//in14 p0.20
#define IN14_PIN 20
//in15 p0.21
#define IN15_PIN 21
//in16 p0.22
#define IN16_PIN 22
//in17 p2.11
#define IN17_PIN 11
//in18 p2.12
#define IN18_PIN 12
//in19 p2.13
#define IN19_PIN 13
//in20 p0.11
#define IN20_PIN 11


//out1 p1.15
#define OUT1_PIN 15
//out2 p1.14
#define OUT2_PIN 14
//out3 p1.10
#define OUT3_PIN 10
//out4 p1.9
#define OUT4_PIN 9
//out5 p1.8
#define OUT5_PIN 8
//out6 p1.4
#define OUT6_PIN 4
//out7 p1.1
#define OUT7_PIN 1
//out8 p1.0
#define OUT8_PIN 0
//out9 p0.26
#define OUT9_PIN 26
//out10 p0.25
#define OUT10_PIN 25
//out11 p0.24
#define OUT11_PIN 24
//out12 p0.23
#define OUT12_PIN 23
//out13 p1.31
#define OUT13_PIN 31
//out14 p1.30
#define OUT14_PIN 30
//out15 p1.18
#define OUT15_PIN 18
//out16 p1.19
#define OUT16_PIN 19
//out17 p1.20
#define OUT17_PIN 20
//out18 p1.21
#define OUT18_PIN 21
//out19 p1.22
#define OUT19_PIN 22
//out20 p1.23
#define OUT20_PIN 23
//out21 p1.24
#define OUT21_PIN 24
//out22 p1.25
#define OUT22_PIN 25
//out23 p1.26
#define OUT23_PIN 26
//out24 p1.27
#define OUT24_PIN 27




//rs485_tx_on p0.4
#define RS485_TX_ON_PIN 4


#define IN1 (1<<IN1_PIN)
#define IN2 (1<<IN2_PIN)
#define IN3 (1<<IN3_PIN)
#define IN4 (1<<IN4_PIN)
#define IN5 (1<<IN5_PIN)
#define IN6 (1<<IN6_PIN)
#define IN7 (1<<IN7_PIN)
#define IN8 (1<<IN8_PIN)

#define IN9 (1<<IN9_PIN)
#define IN10 (1<<IN10_PIN)
#define IN11 (1<<IN11_PIN)
#define IN12 (1<<IN12_PIN)
#define IN13 (1<<IN13_PIN)
#define IN14 (1<<IN14_PIN)
#define IN15 (1<<IN15_PIN)
#define IN16 (1<<IN16_PIN)

#define IN17 (1<<IN17_PIN)
#define IN18 (1<<IN18_PIN)
#define IN19 (1<<IN19_PIN)
#define IN20 (1<<IN20_PIN)

#define OUT1 (1<<OUT1_PIN)
#define OUT2 (1<<OUT2_PIN)
#define OUT3 (1<<OUT3_PIN)
#define OUT4 (1<<OUT4_PIN)
#define OUT5 (1<<OUT5_PIN)
#define OUT6 (1<<OUT6_PIN)
#define OUT7 (1<<OUT7_PIN)
#define OUT8 (1<<OUT8_PIN)

#define OUT9 (1<<OUT9_PIN)
#define OUT10 (1<<OUT10_PIN)
#define OUT11 (1<<OUT11_PIN)
#define OUT12 (1<<OUT12_PIN)
#define OUT13 (1<<OUT13_PIN)
#define OUT14 (1<<OUT14_PIN)
#define OUT15 (1<<OUT15_PIN)
#define OUT16 (1<<OUT16_PIN)

#define OUT17 (1<<OUT17_PIN)
#define OUT18 (1<<OUT18_PIN)
#define OUT19 (1<<OUT19_PIN)
#define OUT20 (1<<OUT20_PIN)
#define OUT21 (1<<OUT21_PIN)
#define OUT22 (1<<OUT22_PIN)
#define OUT23 (1<<OUT23_PIN)
#define OUT24 (1<<OUT24_PIN)

#define RS485_TX_ON (1<<RS485_TX_ON_PIN)

//pwr led p1.28
#define LED_PWR (1<<28)

//tx led p1.29
#define LED_TX (1<<29)

//rx led p0.0
#define LED_RX (1<<0)

//err led p0.1
#define LED_ERR (1<<1)

int ih2_io_rs485_tx_on(void);
int ih2_io_rs485_tx_off(void);
int ih2_rx_led(ushort on);
int ih2_tx_led(ushort on);

int ih2_io_init(void);
ulong ih2_io_get_inputs(void);
int ih2_io_set_output(uchar output_no,uchar value);


#endif
