/*
 * can.c
 *
 *  Created on: 21.02.2014
 *      Author: jrenken
 */

#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>

#include "can.h"

#define CAN_DEBUG(format, ...)		if (canDebug) { fprintf(stderr, format, ##__VA_ARGS__); }


int canDebug = 0;

void can_setDebug(int debug)
{
	canDebug = debug;
}

int can_openDevice(const char* dev)
{
	int fd = open(dev, O_RDWR);
	if (fd < 0) {
		CAN_DEBUG("can't open CAN device:%d: %s\n", errno, strerror(errno));
	} else {
		ioctl(fd, CAN_SET_DEBUGMODE, canDebug);
	}
	return fd;
}

int can_setParameter(int fd, unsigned char mode, unsigned int br, unsigned char br2btr)
{
	int ret;
	struct CANSettings cs;

	ret = ioctl(fd, CAN_GET_SETTINGS, &cs);
	if (ret < 0) {
		CAN_DEBUG("can't get device settings:%d: %s\n", errno, strerror(errno));
		return ret;
	}

	// copy data to CANSettings structure
	cs.mode            = mode;
	cs.baudrate        = br;
	cs.baudrate_to_btr = br2btr;

	// call kernel driver
	ret = ioctl(fd, CAN_SET_SETTINGS, &cs);
	if (ret < 0) {
		CAN_DEBUG("can't set device settings:%d: %s\n", errno, strerror(errno));
	}

	return ret;
}

int can_setMsgFilter(int fd, unsigned long int mask, unsigned long int code)
{
	int ret;
	struct CANSettings cs;

	ret = ioctl(fd, CAN_GET_SETTINGS, &cs);
	if (ret < 0) {
		CAN_DEBUG("can't get device settings:%d: %s\n", errno, strerror(errno));
		return ret;
	}

	cs.mask            = mask;
	cs.code            = code;


	ret = ioctl(fd, CAN_SET_SETTINGS, &cs);
	if (ret < 0) {
		CAN_DEBUG("can't set message filter:%d: %s\n", errno, strerror(errno));
	}

	return ret;
}

int can_getDriverInfo(int fd, struct DriverInfo* info)
{
	int ret;

	ret = ioctl(fd, CAN_GET_DRIVER_INFO, info);
	if (ret < 0) {
		CAN_DEBUG("can't get driver info:%d: %s\n", errno, strerror(errno));
	}

	return ret;
}

int can_setOnOff(int fd, unsigned char on)
{
	int ret;

	ret = ioctl(fd, CAN_SET_ON_OFF, on);
	if (ret < 0) {
		CAN_DEBUG("can't set can communication %s:%d: %s\n", on ? "on" : "off", errno, strerror(errno));
	}

	return ret;
}

int can_setLed(int fd, unsigned char led)
{
	int ret;

	if (led == AUTO_LED_OFF) {
		ret = ioctl(fd, CAN_SET_AUTO_LED, 0);
	} else if (led == AUTO_LED_ON) {
		ret = ioctl(fd, CAN_SET_AUTO_LED, 1);
	} else {
		ret = ioctl(fd, CAN_SET_LED, led);
	}
	if (ret < 0) {
		CAN_DEBUG("can't set LED:%d: %s\n", errno, strerror(errno));
	}

	return ret;
}


int can_sendMessage(int fd, struct TCANMsg *msg)
{
	int ret;

	ret = write(fd, (void*) msg, sizeof(*msg));

	if (ret < 0) {
		CAN_DEBUG("can't send message:%d: %s\n", errno, strerror(errno));
	}
	return ret;

}

int can_getStatus(int fd, unsigned char *status)
{
	int ret = ioctl(fd, CAN_GET_STATE, status);
	if (ret < 0) {
		CAN_DEBUG("can't get device status:%d: %s\n", errno, strerror(errno));
	}
	return ret;
}

int can_getRecMessages(int fd)
{
	int msgs;
	int ret = ioctl(fd, CAN_GET_RECEIVE_MSG, &msgs);
	if (ret >= 0) {
		return msgs;
	} else {
		CAN_DEBUG("can't get available messages:%d: %s\n", errno, strerror(errno));
	}
	return ret;
}

int can_readMessage(int fd, struct TCANMsg* msg)
{
	int ret = 0;

	if (can_getRecMessages > 0) {
		ret = read(fd, (void*) msg, sizeof(*msg));
		if (ret == sizeof(*msg)) {
			return ret;
		} else {
			CAN_DEBUG("reading message failed:%d: %s", errno, strerror(errno));
		}
	}
	return ret;
}

int can_readMessageT(int fd, struct TCANMsgT* msg)
{
	int ret = 0;

	if (can_getRecMessages > 0) {
		ret = read(fd, (void*) msg, sizeof(*msg));
		if (ret == sizeof(*msg)) {
			return ret;
		} else {
			CAN_DEBUG("reading message failed:%d: %s", errno, strerror(errno));
		}
	}
	return ret;
}

void can_dumpStatus(unsigned char status)
{
	if ( (status & 0xFC) != CAN_ERR_OK) {
		printf("CAN-Error %02X: ", status);
		status &= 0xFC;
		if (status & CAN_ERR_OVERRUN)              printf(" OVERRUN");
		if (status & CAN_ERR_BUSERROR)             printf(" BUSERROR");
		if (status & CAN_ERR_BUSOFF)               printf(" BUSOFF");
		if (status & CAN_ERR_RECEIVEBUF_OVERFLOW)  printf(" RECEIVEBUF_OVERFLOW");
		if (status & CAN_ERR_TRANSMITBUF_OVERFLOW) printf(" TRANSMITBUF_OVERFLOW");

		printf("\n");
	} else {
		printf("CAN ok\n");
	}

}

void can_dumpMessage(struct TCANMsg *msg, unsigned char mode)
{
	int i = 0;
	char str[200], str2[20];

	sprintf(str,"ID:%08lX RTR:%d Len:%d ", 	msg->ID, msg->RTR, (msg->LEN&0x0F));

	if (msg->RTR == 0) {
		for (i = 0; i <= (msg->LEN & 0x0F) - 1; i++) {
			sprintf(str2, " %02X", msg->DATA[i]);
			strcat(str,str2);
		}
	}

	if (mode == CAN_MIXED) {
		if(msg->LEN & 0x40)
		{
			if(msg->LEN & 0x20)
				strcat(str, " (CAN2.0B)");
			else
				strcat(str, " (CAN2.0A)");
		}
	}

	strcat(str,"\n");
	printf(str);
}

void can_dumpMessageT(struct TCANMsgT *msg, unsigned char mode)
{
	int i = 0;
	char str[200], str2[20];

	sprintf(str,"%10ld.%06ld  ID:%08lX RTR:%d Len:%d ", msg->TIMES, msg->TIMEUS,
			msg->ID, msg->RTR, (msg->LEN&0x0F));

	if (msg->RTR == 0) {
		for (i = 0; i <= (msg->LEN & 0x0F) - 1; i++) {
			sprintf(str2, " %02X", msg->DATA[i]);
			strcat(str,str2);
		}
	}

	if(mode == CAN_MIXED)
	{ // CAN2.0A/CAN2.0B mixed mode
		if(msg->LEN & 0x40)
		{
			if(msg->LEN & 0x20)
				strcat(str, " (CAN2.0B)");
			else
				strcat(str, " (CAN2.0A)");
		}
	}

	strcat(str,"\n");
	printf(str);
}
