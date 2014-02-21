/*
 * can.c
 *
 *  Created on: 21.02.2014
 *      Author: jrenken
 */

#include "can_driver.h"
#include "can.h"

int can_setParamter(unsigned char mode, unsigned int br, unsigned char br2btr)
{
	return 0;
}

int can_setMsgFilter(unsigned long int mask, unsigned long int code)
{
	return 0;
}

int can_getDriverInfo(struct DriverInfo* info)
{
	return 0;

}
