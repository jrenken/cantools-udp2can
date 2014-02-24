/*
 *  file:		udp2can.c
 *  author:		jrenken
 *  date:		24.02.2014
 *
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "can.h"
#include "can_driver.h"




void testCan(void)
{
	int fd;
	int i;
	unsigned char can_error;
	struct DriverInfo info;
	unsigned char mode = CAN_STANDARD;


	printf("> Open device /dev/can0\n");
	fd = can_openDevice("/dev/can0");
	if (fd < 0)
		return;

	can_setOnOff(fd, 0);

	if (can_getDriverInfo(fd, &info) == 1)  {
	    printf("\nDriver info:\n");
	    printf("Name: '%s'\n",  info.version);
	    printf("Received since last parameter setting :%lu\n",   info.total_rcv);
	    printf("Send since last parameter setting     :%lu\n",   info.total_snd);
	    printf("CAN-msg. Receive buffer size          :%lu\n",   info.rcv_buffer_size);
	    printf("CAN-msg. Send buffer size             :%lu\n\n", info.snd_buffer_size);
	}

	if (can_getStatus(fd, &can_error) == 1){
		can_dumpStatus(can_error);
	}

	printf("> Set parameter\n");
	can_setParameter(fd, CAN_STANDARD, 125, 0);
	printf("> Driver on\n");



//	printf("> Test Leds\n");
//	can_setLed(fd, AUTO_LED_OFF);
//
//	for ( i = 0; i < 16; i++) {
//		can_setLed(fd, (unsigned char) i);
//		usleep(500000L);
//	}
//	can_setLed(fd, 0);
	can_setLed(fd, AUTO_LED_ON);

	can_setOnOff(fd, 1);


	struct TCANMsg msg;
	for (i = 0; i < 8; i++)
		msg.DATA[i] = (unsigned char) i + 16;
	msg.LEN = 8;
	msg.RTR = 0;
	for (i = 0; i < 32; i += 2) {
		msg.ID = i + 64;
		can_sendMessage(fd, &msg);

		if (can_getStatus(fd, &can_error) == 1){
			can_dumpStatus(can_error);
		}
	}
	usleep(2000000L);

	if (can_getStatus(fd, &can_error) == 1){
		can_dumpStatus(can_error);
	}
	if (can_getDriverInfo(fd, &info) == 1)  {
	    printf("\nDriver info:\n");
	    printf("Name: '%s'\n",  info.version);
	    printf("Received since last parameter setting :%lu\n",   info.total_rcv);
	    printf("Send since last parameter setting     :%lu\n",   info.total_snd);
	    printf("CAN-msg. Receive buffer size          :%lu\n",   info.rcv_buffer_size);
	    printf("CAN-msg. Send buffer size             :%lu\n\n", info.snd_buffer_size);
	  }

	struct TCANMsgT msgT;
	while (can_getRecMessages(fd) > 0) {
		if (can_readMessageT(fd, &msgT) > 0) {
			can_dumpMessageT(&msgT, mode);
		}
	}
	can_setOnOff(fd, 0);
	close(fd);
}




int main(void) {

	can_setDebug(1);

	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */
	testCan();
	return EXIT_SUCCESS;
}
