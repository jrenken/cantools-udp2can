/*
 * can.h
 *
 *  Created on: 21.02.2014
 *      Author: jrenken
 */

#ifndef CAN_H_
#define CAN_H_


int can_setParamter(unsigned char mode, unsigned int br, unsigned char br2btr);
int can_setMsgFilter(unsigned long int mask, unsigned long int code);
int can_getDriverInfo(struct DriverInfo * info);


#endif /* CAN_H_ */
