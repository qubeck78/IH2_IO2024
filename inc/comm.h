#ifndef _COMM_H
#define _COMM_H

#define _COMM_SOH 0xff
#define _COMM_EOP 0xfe
#define _COMM_ESC 0xfd

#define _COMM_EOR_BASE 0xa5

#define _COMM_CMD_REQ_DEV_INFO				0x01
#define _COMM_CMD_READ_SINGLE_REG 			0x02
#define _COMM_CMD_READ_MULTIPLE_REG 		0x03
#define _COMM_CMD_WRITE_SINGLE_REG			0x04
#define _COMM_CMD_WRITE_MULTIPLE_REG		0x05
#define _COMM_CMD_START_PAIRING				0x06
#define _COMM_CMD_SET_DEV_ID				0x07
#define _COMM_CMD_SET_CHAIN_PREDECESOR		0x08
#define _COMM_CMD_CHAIN_READ_SIGNLE_REG		0x09
#define _COMM_CMD_CHAIN_READ_MULTIPLE_REG	0x0a

#define _COMM_CMD_UNSUPPORTED_COMMAND		0xff

int comm_init(void);
int comm_tx_insert(unsigned char c,uchar *comm_snd_eor);
int comm_receiver(uchar rx);


#endif
