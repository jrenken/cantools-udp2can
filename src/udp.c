/*
 * udp.c
 *
 *  Created on: 24.02.2014
 *      Author: jrenken
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <stdio.h>

#include <string.h>
#include "udp.h"

#define UDP_DEBUG(format, ...)		if (udpDebug) { fprintf(stderr, format, ##__VA_ARGS__); }

int udpDebug = 0;
struct sockaddr udpTarget;
socklen_t		udpTargetAddrLen = 0;

void udp_setDebug(int debug)
{
	udpDebug = debug;
}

int udp_openSocket(unsigned short port)
{
	int fd;
	struct sockaddr_in saddr, baddr;

	if((fd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
		UDP_DEBUG("can't create UDP socket:%d: %s\n", errno, strerror(errno))
		return fd;
	}

	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	saddr.sin_port = htons(port);

	if (bind(fd,(struct sockaddr*)&saddr, sizeof(saddr)) < 0) {
		UDP_DEBUG("can't bind UDP socket:%d: %s\n", errno, strerror(errno))
		return fd;
	}

	UDP_DEBUG("Bind to port %d\n", port);
	return fd;
}

int udp_setTarget(const char* host, unsigned short port)
{
	struct addrinfo *result;
	char sport[16];

	snprintf(sport, 10, "%d", port);

	struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */


	int ret = getaddrinfo(host, sport, &hints, &result);

	if (ret < 0) {
		UDP_DEBUG("can't resolve target host %s:%d: %s\n", host, errno, strerror(errno))
		return ret;
	}

	memcpy( &udpTarget, result->ai_addr, result->ai_addrlen);
	udpTargetAddrLen = result->ai_addrlen;
    freeaddrinfo(result);
    return 0;
}


int udp_sendDatagram(int fd, void* data, size_t len)
{
	int ret = sendto(fd, data, len, 0, &udpTarget, udpTargetAddrLen);
	if (ret < 0) {
		UDP_DEBUG("can't write data:%d: %s\n", errno, strerror(errno))

	}
	return ret;
}

//        struct addrinfo *result;
//        string port_as_string = boost::lexical_cast<string>(port);
//        int ret = getaddrinfo(NULL, port_as_string.c_str(), &hints, &result);
//        if (ret != 0)
//            throw UnixError("cannot resolve server port " + port_as_string);
//
//        int sfd = -1;
//        struct addrinfo *rp;
//        for (rp = result; rp != NULL; rp = rp->ai_next) {
//            sfd = socket(rp->ai_family, rp->ai_socktype,
//                    rp->ai_protocol);
//            if (sfd == -1)
//                continue;
//
//            if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
//                break;                  /* Success */
//
//            ::close(sfd);
//        }
//        freeaddrinfo(result);
//
//        if (rp == NULL)
//            throw UnixError("cannot open server socket on port " + port_as_string);
//
//        setMainStream(new UDPServerStream(sfd,true));
//
//
//
