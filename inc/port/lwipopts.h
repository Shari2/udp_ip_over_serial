// SPDX-FileCopyrightText: 2022 Marian Sauer
//
// SPDX-License-Identifier: BSD-2-Clause

#ifndef PORT_lwipopts_H
#define PORT_lwipopts_H

#define NO_SYS 1

#define SYS_LIGHTWEIGHT_PROT 0

#define LWIP_SOCKET 0
#define LWIP_NETCONN 0

#define LWIP_ETHERNET 0
#define LWIP_ARP 0

#define IP_REASSEMBLY 0
#define IP_FRAG 0
#define IP_FORWARD 1

#define LWIP_UDP 0
#define LWIP_TCP 0

//#define LWIP_DEBUG 1
//#define IP_DEBUG LWIP_DBG_ON

#endif
