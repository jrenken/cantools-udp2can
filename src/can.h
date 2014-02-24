/*
 * can.h
 *
 *  Created on: 21.02.2014
 *      Author: jrenken
 */

#ifndef CAN_H_
#define CAN_H_

#include "can_driver.h"



#define AUTO_LED_ON		128
#define AUTO_LED_OFF		64




/* Set debug mode on/off
 *
 */
void can_setDebug(int debug);


/* Open the CAN device
 * @return file descriptor
 */
int can_openDevice(const char* dev);


/* Set CAN interface parameter
 *
 */
int can_setParameter(int fd, unsigned char mode, unsigned int br, unsigned char br2btr);

/* Set message filter for receiving messages
 *
 */
int can_setMsgFilter(int fd, unsigned long int mask, unsigned long int code);

/* Get info about the driver
 *
 */
int can_getDriverInfo(int fd, struct DriverInfo * info);

/* Set the bus communication on or off
 *
 */
int can_setOnOff(int fd, unsigned char on);

/* Set the debug LEDs
 *
 */
int can_setLed(int fd, unsigned char led);

/* Send a CAN message
 *
 */
int can_sendMessage(int fd, struct TCANMsg *msg);

/* Get the interface status
 *
 */
int can_getStatus(int fd, unsigned char *status);

/* Get the number of available messages in receive buffer
 * @return -1 if error or number of available messages
 */
int can_getRecMessages(int fd);

/* Read a message without timestamp
 *
 */
int can_readMessage(int fd, struct TCANMsg* msg);

/* Read a message with timestamp
 *
 */
int can_readMessageT(int fd, struct TCANMsgT* msg);

void can_dumpStatus(unsigned char status);

void can_dumpMessage(struct TCANMsg *msg, unsigned char mode);

void can_dumpMessageT(struct TCANMsgT *msg, unsigned char mode);

#endif /* CAN_H_ */
