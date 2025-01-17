//==========================================================================
//
//      include/netinet6/in6.h
//
//==========================================================================
//####BSDCOPYRIGHTBEGIN####
//
// -------------------------------------------
//
// Portions of this software may have been derived from OpenBSD, 
// FreeBSD or other sources, and are covered by the appropriate
// copyright disclaimers included herein.
//
// Portions created by Red Hat are
// Copyright (C) 2002 Red Hat, Inc. All Rights Reserved.
//
// -------------------------------------------
//
//####BSDCOPYRIGHTEND####
//==========================================================================

/*	$KAME: in6.h,v 1.115 Exp $	*/

/*
 * Copyright (C) 1995, 1996, 1997, and 1998 WIDE Project.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the project nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE PROJECT AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * Copyright (c) 1982, 1986, 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)in.h	8.3 (Berkeley) 1/3/94
 */

#ifndef __KAME_NETINET_IN_H_INCLUDED_
#error "do not include netinet6/in6.h directly, include netinet/in.h.  see RFC2553"
#endif

#ifndef _NETINET6_IN6_H_
#define _NETINET6_IN6_H_

#include <sys/param.h>

/*
 * Identification of the network protocol stack
 * for *BSD-current/release: http://www.kame.net/dev/cvsweb.cgi/kame/COVERAGE
 * has the table of implementation/integration differences.
 */
#define __KAME__
#define __KAME_VERSION		"SNAP 20020107"

/*
 * Local port number conventions:
 *
 * Ports < IPPORT_RESERVED are reserved for privileged processes (e.g. root),
 * unless a kernel is compiled with IPNOPRIVPORTS defined.
 *
 * When a user does a bind(2) or connect(2) with a port number of zero,
 * a non-conflicting local port address is chosen.
 *
 * The default range is IPPORT_ANONMIN to IPPORT_ANONMAX, although
 * that is settable by sysctl(3); net.inet.ip.anonportmin and
 * net.inet.ip.anonportmax respectively.
 *
 * A user may set the IPPROTO_IP option IP_PORTRANGE to change this
 * default assignment range.
 *
 * The value IP_PORTRANGE_DEFAULT causes the default behavior.
 *
 * The value IP_PORTRANGE_HIGH is the same as IP_PORTRANGE_DEFAULT,
 * and exists only for FreeBSD compatibility purposes.
 *
 * The value IP_PORTRANGE_LOW changes the range to the "low" are
 * that is (by convention) restricted to privileged processes.
 * This convention is based on "vouchsafe" principles only.
 * It is only secure if you trust the remote host to restrict these ports.
 * The range is IPPORT_RESERVEDMIN to IPPORT_RESERVEDMAX.
 */

#define	IPV6PORT_RESERVED	1024
#define	IPV6PORT_ANONMIN	49152
#define	IPV6PORT_ANONMAX	65535
#define	IPV6PORT_RESERVEDMIN	600
#define	IPV6PORT_RESERVEDMAX	(IPV6PORT_RESERVED-1)

/*
 * IPv6 address
 */
struct in6_addr {
	union {
		u_int8_t   __u6_addr8[16];
		u_int16_t  __u6_addr16[8];
		u_int32_t  __u6_addr32[4];
	} __u6_addr;			/* 128-bit IP6 address */
};

#define s6_addr   __u6_addr.__u6_addr8
#ifdef _KERNEL	/* XXX nonstandard */
#define s6_addr8  __u6_addr.__u6_addr8
#define s6_addr16 __u6_addr.__u6_addr16
#define s6_addr32 __u6_addr.__u6_addr32
#endif

#define INET6_ADDRSTRLEN	46

/*
 * Socket address for IPv6
 */
#ifndef _XOPEN_SOURCE
#define SIN6_LEN
#endif
struct sockaddr_in6 {
	u_int8_t	sin6_len;	/* length of this struct(sa_family_t)*/
	u_int8_t	sin6_family;	/* AF_INET6 (sa_family_t) */
	u_int16_t	sin6_port;	/* Transport layer port # (in_port_t)*/
	u_int32_t	sin6_flowinfo;	/* IP6 flow information */
	struct in6_addr	sin6_addr;	/* IP6 address */
	u_int32_t	sin6_scope_id;	/* scope zone index */
};

/*
 * Local definition for masks
 */
#ifdef _KERNEL	/* XXX nonstandard */
#define IN6MASK0	{{{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }}}
#define IN6MASK32	{{{ 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, \
			    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }}}
#define IN6MASK64	{{{ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, \
			    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }}}
#define IN6MASK96	{{{ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, \
			    0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00 }}}
#define IN6MASK128	{{{ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, \
			    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff }}}
#endif

#ifdef _KERNEL
extern const struct sockaddr_in6 sa6_any;

extern const struct in6_addr in6mask0;
extern const struct in6_addr in6mask32;
extern const struct in6_addr in6mask64;
extern const struct in6_addr in6mask96;
extern const struct in6_addr in6mask128;
#endif /* _KERNEL */

/*
 * Macros started with IPV6_ADDR is KAME local
 */
#ifdef _KERNEL	/* XXX nonstandard */
#if BYTE_ORDER == BIG_ENDIAN
#define IPV6_ADDR_INT32_ONE	1
#define IPV6_ADDR_INT32_TWO	2
#define IPV6_ADDR_INT32_MNL	0xff010000
#define IPV6_ADDR_INT32_MLL	0xff020000
#define IPV6_ADDR_INT32_SMP	0x0000ffff
#define IPV6_ADDR_INT16_ULL	0xfe80
#define IPV6_ADDR_INT16_USL	0xfec0
#define IPV6_ADDR_INT16_MLL	0xff02
#elif BYTE_ORDER == LITTLE_ENDIAN
#define IPV6_ADDR_INT32_ONE	0x01000000
#define IPV6_ADDR_INT32_TWO	0x02000000
#define IPV6_ADDR_INT32_MNL	0x000001ff
#define IPV6_ADDR_INT32_MLL	0x000002ff
#define IPV6_ADDR_INT32_SMP	0xffff0000
#define IPV6_ADDR_INT16_ULL	0x80fe
#define IPV6_ADDR_INT16_USL	0xc0fe
#define IPV6_ADDR_INT16_MLL	0x02ff
#endif
#endif

/*
 * Definition of some useful macros to handle IP6 addresses
 */
#define IN6ADDR_ANY_INIT \
	{{{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }}}
#define IN6ADDR_LOOPBACK_INIT \
	{{{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 }}}
#define IN6ADDR_NODELOCAL_ALLNODES_INIT \
	{{{ 0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 }}}
#define IN6ADDR_INTFACELOCAL_ALLNODES_INIT \
	{{{ 0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 }}}
#define IN6ADDR_LINKLOCAL_ALLNODES_INIT \
	{{{ 0xff, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 }}}
#define IN6ADDR_LINKLOCAL_ALLROUTERS_INIT \
	{{{ 0xff, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02 }}}

extern const struct in6_addr in6addr_any;
extern const struct in6_addr in6addr_loopback;
extern const struct in6_addr in6addr_nodelocal_allnodes;
extern const struct in6_addr in6addr_linklocal_allnodes;
extern const struct in6_addr in6addr_linklocal_allrouters;

/*
 * Equality
 * NOTE: Some of kernel programming environment (for example, openbsd/sparc)
 * does not supply memcmp().  For userland memcmp() is preferred as it is
 * in ANSI standard.
 */
#ifdef _KERNEL
#define IN6_ARE_ADDR_EQUAL(a, b)			\
    (bcmp(&(a)->s6_addr[0], &(b)->s6_addr[0], sizeof(struct in6_addr)) == 0)
#else
#define IN6_ARE_ADDR_EQUAL(a, b)			\
    (memcmp(&(a)->s6_addr[0], &(b)->s6_addr[0], sizeof(struct in6_addr)) == 0)
#endif

#ifdef _KERNEL			/* non standard */
/* see if two addresses are equal in a scope-conscious manner. */
#define SA6_ARE_ADDR_EQUAL(a, b) \
	(((a)->sin6_scope_id == 0 || (b)->sin6_scope_id == 0 || \
	  ((a)->sin6_scope_id == (b)->sin6_scope_id)) && \
	 (bcmp(&(a)->sin6_addr, &(b)->sin6_addr, sizeof(struct in6_addr)) == 0))
#endif

/*
 * Unspecified
 */
#define IN6_IS_ADDR_UNSPECIFIED(a)	\
	((*(const u_int32_t *)(const void *)(&(a)->s6_addr[0]) == 0) &&	\
	 (*(const u_int32_t *)(const void *)(&(a)->s6_addr[4]) == 0) &&	\
	 (*(const u_int32_t *)(const void *)(&(a)->s6_addr[8]) == 0) &&	\
	 (*(const u_int32_t *)(const void *)(&(a)->s6_addr[12]) == 0))

/*
 * Loopback
 */
#define IN6_IS_ADDR_LOOPBACK(a)		\
	((*(const u_int32_t *)(const void *)(&(a)->s6_addr[0]) == 0) &&	\
	 (*(const u_int32_t *)(const void *)(&(a)->s6_addr[4]) == 0) &&	\
	 (*(const u_int32_t *)(const void *)(&(a)->s6_addr[8]) == 0) &&	\
	 (*(const u_int32_t *)(const void *)(&(a)->s6_addr[12]) == ntohl(1)))

/*
 * IPv4 compatible
 */
#define IN6_IS_ADDR_V4COMPAT(a)		\
	((*(const u_int32_t *)(const void *)(&(a)->s6_addr[0]) == 0) &&	\
	 (*(const u_int32_t *)(const void *)(&(a)->s6_addr[4]) == 0) &&	\
	 (*(const u_int32_t *)(const void *)(&(a)->s6_addr[8]) == 0) &&	\
	 (*(const u_int32_t *)(const void *)(&(a)->s6_addr[12]) != 0) &&	\
	 (*(const u_int32_t *)(const void *)(&(a)->s6_addr[12]) != ntohl(1)))

/*
 * Mapped
 */
#define IN6_IS_ADDR_V4MAPPED(a)		      \
	((*(const u_int32_t *)(const void *)(&(a)->s6_addr[0]) == 0) &&	\
	 (*(const u_int32_t *)(const void *)(&(a)->s6_addr[4]) == 0) &&	\
	 (*(const u_int32_t *)(const void *)(&(a)->s6_addr[8]) == ntohl(0x0000ffff)))

/*
 * KAME Scope Values
 */

#ifdef _KERNEL	/* XXX nonstandard */
#define IPV6_ADDR_SCOPE_NODELOCAL	0x01
#define IPV6_ADDR_SCOPE_INTFACELOCAL	0x01
#define IPV6_ADDR_SCOPE_LINKLOCAL	0x02
#define IPV6_ADDR_SCOPE_SITELOCAL	0x05
#define IPV6_ADDR_SCOPE_ORGLOCAL	0x08	/* just used in this file */
#define IPV6_ADDR_SCOPE_GLOBAL		0x0e
#else
#define __IPV6_ADDR_SCOPE_NODELOCAL	0x01
#define __IPV6_ADDR_SCOPE_INTFACELOCAL	0x01
#define __IPV6_ADDR_SCOPE_LINKLOCAL	0x02
#define __IPV6_ADDR_SCOPE_SITELOCAL	0x05
#define __IPV6_ADDR_SCOPE_ORGLOCAL	0x08	/* just used in this file */
#define __IPV6_ADDR_SCOPE_GLOBAL	0x0e
#endif

/*
 * Unicast Scope
 * Note that we must check topmost 10 bits only, not 16 bits (see RFC2373).
 */
#define IN6_IS_ADDR_LINKLOCAL(a)	\
	(((a)->s6_addr[0] == 0xfe) && (((a)->s6_addr[1] & 0xc0) == 0x80))
#define IN6_IS_ADDR_SITELOCAL(a)	\
	(((a)->s6_addr[0] == 0xfe) && (((a)->s6_addr[1] & 0xc0) == 0xc0))

/*
 * Multicast
 */
#define IN6_IS_ADDR_MULTICAST(a)	((a)->s6_addr[0] == 0xff)

#ifdef _KERNEL	/* XXX nonstandard */
#define IPV6_ADDR_MC_SCOPE(a)		((a)->s6_addr[1] & 0x0f)
#else
#define __IPV6_ADDR_MC_SCOPE(a)		((a)->s6_addr[1] & 0x0f)
#endif

/*
 * Multicast Scope
 */
#ifdef _KERNEL	/* refers nonstandard items */
#define IN6_IS_ADDR_MC_NODELOCAL(a)	\
	(IN6_IS_ADDR_MULTICAST(a) &&	\
	 (IPV6_ADDR_MC_SCOPE(a) == IPV6_ADDR_SCOPE_NODELOCAL))
#define IN6_IS_ADDR_MC_INTFACELOCAL(a)	\
	(IN6_IS_ADDR_MULTICAST(a) &&	\
	 (IPV6_ADDR_MC_SCOPE(a) == IPV6_ADDR_SCOPE_INTFACELOCAL))
#define IN6_IS_ADDR_MC_LINKLOCAL(a)	\
	(IN6_IS_ADDR_MULTICAST(a) &&	\
	 (IPV6_ADDR_MC_SCOPE(a) == IPV6_ADDR_SCOPE_LINKLOCAL))
#define IN6_IS_ADDR_MC_SITELOCAL(a)	\
	(IN6_IS_ADDR_MULTICAST(a) && 	\
	 (IPV6_ADDR_MC_SCOPE(a) == IPV6_ADDR_SCOPE_SITELOCAL))
#define IN6_IS_ADDR_MC_ORGLOCAL(a)	\
	(IN6_IS_ADDR_MULTICAST(a) &&	\
	 (IPV6_ADDR_MC_SCOPE(a) == IPV6_ADDR_SCOPE_ORGLOCAL))
#define IN6_IS_ADDR_MC_GLOBAL(a)	\
	(IN6_IS_ADDR_MULTICAST(a) &&	\
	 (IPV6_ADDR_MC_SCOPE(a) == IPV6_ADDR_SCOPE_GLOBAL))
#else
#define IN6_IS_ADDR_MC_NODELOCAL(a)	\
	(IN6_IS_ADDR_MULTICAST(a) &&	\
	 (__IPV6_ADDR_MC_SCOPE(a) == __IPV6_ADDR_SCOPE_NODELOCAL))
#define IN6_IS_ADDR_MC_LINKLOCAL(a)	\
	(IN6_IS_ADDR_MULTICAST(a) &&	\
	 (__IPV6_ADDR_MC_SCOPE(a) == __IPV6_ADDR_SCOPE_LINKLOCAL))
#define IN6_IS_ADDR_MC_SITELOCAL(a)	\
	(IN6_IS_ADDR_MULTICAST(a) && 	\
	 (__IPV6_ADDR_MC_SCOPE(a) == __IPV6_ADDR_SCOPE_SITELOCAL))
#define IN6_IS_ADDR_MC_ORGLOCAL(a)	\
	(IN6_IS_ADDR_MULTICAST(a) &&	\
	 (__IPV6_ADDR_MC_SCOPE(a) == __IPV6_ADDR_SCOPE_ORGLOCAL))
#define IN6_IS_ADDR_MC_GLOBAL(a)	\
	(IN6_IS_ADDR_MULTICAST(a) &&	\
	 (__IPV6_ADDR_MC_SCOPE(a) == __IPV6_ADDR_SCOPE_GLOBAL))
#endif

#ifdef _KERNEL	/* nonstandard */
/*
 * KAME Scope
 */
#define IN6_IS_SCOPE_LINKLOCAL(a)	\
	((IN6_IS_ADDR_LINKLOCAL(a)) ||	\
	 (IN6_IS_ADDR_MC_LINKLOCAL(a)))

#if (defined(__FreeBSD__) && __FreeBSD__ >= 3)
#define IFA6_IS_DEPRECATED(a) \
	((a)->ia6_lifetime.ia6t_pltime != ND6_INFINITE_LIFETIME && \
	 (u_int32_t)((time_second - (a)->ia6_updatetime)) > \
	 (a)->ia6_lifetime.ia6t_pltime)
#define IFA6_IS_INVALID(a) \
	((a)->ia6_lifetime.ia6t_vltime != ND6_INFINITE_LIFETIME && \
	 (u_int32_t)((time_second - (a)->ia6_updatetime)) > \
	 (a)->ia6_lifetime.ia6t_vltime)
#else
#define IFA6_IS_DEPRECATED(a) \
	((a)->ia6_lifetime.ia6t_pltime != ND6_INFINITE_LIFETIME && \
	 (u_int32_t)((time.tv_sec - (a)->ia6_updatetime)) > \
	 (a)->ia6_lifetime.ia6t_pltime)
#define IFA6_IS_INVALID(a) \
	((a)->ia6_lifetime.ia6t_vltime != ND6_INFINITE_LIFETIME && \
	 (u_int32_t)((time.tv_sec - (a)->ia6_updatetime)) > \
	 (a)->ia6_lifetime.ia6t_vltime)
#endif
#endif /* _KERNEL */

/*
 * IP6 route structure
 */
#if !defined(__NetBSD__) && !defined(__OpenBSD__) && !defined(__FreeBSD__)
#ifndef _XOPEN_SOURCE
#ifndef NEW_STRUCT_ROUTE
struct route_in6 {
	struct	rtentry *ro_rt;
	struct	sockaddr_in6 ro_dst;
};
#endif /* !NEW_STRUCT_ROUTE */
#endif
#endif

/*
 * Options for use with [gs]etsockopt at the IPV6 level.
 * First word of comment is data type; bool is stored in int.
 */
/* no hdrincl */
#if 0 /* the followings are relic in IPv4 and hence are disabled */
#define IPV6_OPTIONS		1  /* buf/ip6_opts; set/get IP6 options */
#define IPV6_RECVOPTS		5  /* bool; receive all IP6 opts w/dgram */
#define IPV6_RECVRETOPTS	6  /* bool; receive IP6 opts for response */
#define IPV6_RECVDSTADDR	7  /* bool; receive IP6 dst addr w/dgram */
#define IPV6_RETOPTS		8  /* ip6_opts; set/get IP6 options */
#endif
#define IPV6_SOCKOPT_RESERVED1	3  /* reserved for future use */
#define IPV6_UNICAST_HOPS	4  /* int; IP6 hops */
#define IPV6_MULTICAST_IF	9  /* u_int; set/get IP6 multicast i/f */
#define IPV6_MULTICAST_HOPS	10 /* int; set/get IP6 multicast hops */
#define IPV6_MULTICAST_LOOP	11 /* u_int; set/get IP6 multicast loopback */
#define IPV6_JOIN_GROUP		12 /* ip6_mreq; join a group membership */
#define IPV6_LEAVE_GROUP	13 /* ip6_mreq; leave a group membership */
#define IPV6_PORTRANGE		14 /* int; range to choose for unspec port */
#define ICMP6_FILTER		18 /* icmp6_filter; icmp6 filter */
/* RFC2292 options */
#ifdef _KERNEL
#define IPV6_2292PKTINFO	19 /* bool; send/recv if, src/dst addr */
#define IPV6_2292HOPLIMIT	20 /* bool; hop limit */
#define IPV6_2292NEXTHOP	21 /* bool; next hop addr */
#define IPV6_2292HOPOPTS	22 /* bool; hop-by-hop option */
#define IPV6_2292DSTOPTS	23 /* bool; destinaion option */
#define IPV6_2292RTHDR		24 /* bool; routing header */
#define IPV6_2292PKTOPTIONS	25 /* buf/cmsghdr; set/get IPv6 options */
#endif

#define IPV6_CHECKSUM		26 /* int; checksum offset for raw socket */
#define IPV6_V6ONLY		27 /* bool; make AF_INET6 sockets v6 only */
#ifndef _KERNEL
#define IPV6_BINDV6ONLY		IPV6_V6ONLY
#endif

#if 1 /* IPSEC */
#define IPV6_IPSEC_POLICY	28 /* struct; get/set security policy */
#endif
#define IPV6_FAITH		29 /* bool; accept FAITH'ed connections */

#if 1 /* IPV6FIREWALL */
#define IPV6_FW_ADD		30 /* add a firewall rule to chain */
#define IPV6_FW_DEL		31 /* delete a firewall rule from chain */
#define IPV6_FW_FLUSH		32 /* flush firewall rule chain */
#define IPV6_FW_ZERO		33 /* clear single/all firewall counter(s) */
#define IPV6_FW_GET		34 /* get entire firewall rule chain */
#endif

/* new socket options introduced in RFC2292bis */
#define IPV6_RTHDRDSTOPTS	35 /* ip6_dest; send dst option before rthdr */

#define IPV6_RECVPKTINFO	36 /* bool; recv if, dst addr */
#define IPV6_RECVHOPLIMIT	37 /* bool; recv hop limit */
#define IPV6_RECVRTHDR		38 /* bool; recv routing header */
#define IPV6_RECVHOPOPTS	39 /* bool; recv hop-by-hop option */
#define IPV6_RECVDSTOPTS	40 /* bool; recv dst option after rthdr */
#ifdef _KERNEL
#define IPV6_RECVRTHDRDSTOPTS	41 /* bool; recv dst option before rthdr */
#endif

#define IPV6_USE_MIN_MTU	42 /* bool; send packets at the minimum MTU */
#define IPV6_RECVPATHMTU	43 /* bool; notify an according MTU */

#define IPV6_PATHMTU		44 /* mtuinfo; get the current path MTU (sopt),
				      4 bytes int; MTU notification (cmsg) */
#define IPV6_REACHCONF		45 /* no data; ND reachability confirm
				      (cmsg only) */

/* more new socket options introduced in RFC2292bis */
#define IPV6_PKTINFO		46 /* in6_pktinfo; send if, src addr */
#define IPV6_HOPLIMIT		47 /* int; send hop limit */
#define IPV6_NEXTHOP		48 /* sockaddr; next hop addr */
#define IPV6_HOPOPTS		49 /* ip6_hbh; send hop-by-hop option */
#define IPV6_DSTOPTS		50 /* ip6_dest; send dst option befor rthdr */
#define IPV6_RTHDR		51 /* ip6_rthdr; send routing header */
#if 0
#define IPV6_PKTOPTIONS		52 /* buf/cmsghdr; set/get IPv6 options */
				   /* obsoleted by 2292bis */
#endif

#ifdef __OpenBSD__
#define IPV6_AUTH_LEVEL		53   /* int; authentication used */
#define IPV6_ESP_TRANS_LEVEL	54   /* int; transport encryption */
#define IPV6_ESP_NETWORK_LEVEL	55   /* int; full-packet encryption */
#define IPSEC6_OUTSA		56   /* set the outbound SA for a socket */
#endif

#define IPV6_RECVTCLASS		57 /* bool; recv traffic class values */
#ifdef _KERNEL
#define IPV6_OTCLASS		58 /* u_int8_t; send traffic class value */
#endif
#define IPV6_AUTOFLOWLABEL	59 /* bool; attach flowlabel automagically */

#ifdef __OpenBSD__
#define IPV6_IPCOMP_LEVEL	60   /* int; compression */
#endif

#define IPV6_TCLASS		61 /* int; send traffic class value */
#define IPV6_DONTFRAG		62 /* bool; disable IPv6 fragmentation */

/* to define items, should talk with KAME guys first, for *BSD compatibility */

#define IPV6_RTHDR_LOOSE     0 /* this hop need not be a neighbor. XXX old spec */
#define IPV6_RTHDR_STRICT    1 /* this hop must be a neighbor. XXX old spec */
#define IPV6_RTHDR_TYPE_0    0 /* IPv6 routing header type 0 */

/*
 * Defaults and limits for options
 */
#define IPV6_DEFAULT_MULTICAST_HOPS 1	/* normally limit m'casts to 1 hop */
#define IPV6_DEFAULT_MULTICAST_LOOP 1	/* normally hear sends if a member */

/*
 * Argument structure for IPV6_JOIN_GROUP and IPV6_LEAVE_GROUP.
 */
struct ipv6_mreq {
	struct in6_addr	ipv6mr_multiaddr;
	unsigned int	ipv6mr_interface;
};

/*
 * IPV6_PKTINFO: Packet information(RFC2292 sec 5)
 */
struct in6_pktinfo {
	struct in6_addr	ipi6_addr;	/* src/dst IPv6 address */
	unsigned int	ipi6_ifindex;	/* send/recv interface index */
};

/*
 * Control structure for IPV6_RECVPATHMTU socket option.
 * Note that this structure is not fully conformant to rfc2292bis-02.
 * However, since how to tell the destination of the path (or the path itself)
 * to the application is not determined, we use our original structure.  
 */
struct ip6_mtuinfo {
	struct sockaddr_in6 ip6m_addr;	/* or sockaddr_storage? */
	u_int32_t ip6m_mtu;
};

/*
 * Argument for IPV6_PORTRANGE:
 * - which range to search when port is unspecified at bind() or connect()
 */
#define	IPV6_PORTRANGE_DEFAULT	0	/* default range */
#define	IPV6_PORTRANGE_HIGH	1	/* "high" - request firewall bypass */
#define	IPV6_PORTRANGE_LOW	2	/* "low" - vouchsafe security */

#ifndef _XOPEN_SOURCE
/*
 * Definitions for inet6 sysctl operations.
 *
 * Third level is protocol number.
 * Fourth level is desired variable within that protocol.
 */
#define IPV6PROTO_MAXID	(IPPROTO_PIM + 1)	/* don't list to IPV6PROTO_MAX */

#define CTL_IPV6PROTO_NAMES { \
	{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, \
	{ 0, 0 }, \
	{ "tcp6", CTLTYPE_NODE }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ "udp6", CTLTYPE_NODE }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, \
	{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, \
	{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, \
	{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, \
	{ 0, 0 }, \
	{ "ip6", CTLTYPE_NODE }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, \
	{ 0, 0 }, \
	{ "ipsec6", CTLTYPE_NODE }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ "icmp6", CTLTYPE_NODE }, \
	{ 0, 0 }, \
	{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, \
	{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, \
	{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, \
	{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, \
	{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, \
	{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, \
	{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, \
	{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ "pim6", CTLTYPE_NODE }, \
}

/*
 * Names for IP sysctl objects
 */
#define IPV6CTL_FORWARDING	1	/* act as router */
#define IPV6CTL_SENDREDIRECTS	2	/* may send redirects when forwarding*/
#define IPV6CTL_DEFHLIM		3	/* default Hop-Limit */
#ifdef notyet
#define IPV6CTL_DEFMTU		4	/* default MTU */
#endif
#define IPV6CTL_FORWSRCRT	5	/* forward source-routed dgrams */
#define IPV6CTL_STATS		6	/* stats */
#define IPV6CTL_MRTSTATS	7	/* multicast forwarding stats */
#define IPV6CTL_MRTPROTO	8	/* multicast routing protocol */
#define IPV6CTL_MAXFRAGPACKETS	9	/* max packets reassembly queue */
#define IPV6CTL_SOURCECHECK	10	/* verify source route and intf */
#define IPV6CTL_SOURCECHECK_LOGINT 11	/* minimume logging interval */
#define IPV6CTL_ACCEPT_RTADV	12
#define IPV6CTL_KEEPFAITH	13
#define IPV6CTL_LOG_INTERVAL	14
#define IPV6CTL_HDRNESTLIMIT	15
#define IPV6CTL_DAD_COUNT	16
#define IPV6CTL_AUTO_FLOWLABEL	17
#define IPV6CTL_DEFMCASTHLIM	18
#define IPV6CTL_GIF_HLIM	19	/* default HLIM for gif encap packet */
#define IPV6CTL_KAME_VERSION	20
#define IPV6CTL_USE_DEPRECATED	21	/* use deprecated addr (RFC2462 5.5.4) */
#define IPV6CTL_RR_PRUNE	22	/* walk timer for router renumbering */
#if 0	/* obsolete */
#define IPV6CTL_MAPPED_ADDR	23
#endif
#define IPV6CTL_V6ONLY		24
#ifdef __FreeBSD__
#define IPV6CTL_RTEXPIRE	25	/* cloned route expiration time */
#define IPV6CTL_RTMINEXPIRE	26	/* min value for expiration time */
#define IPV6CTL_RTMAXCACHE	27	/* trigger level for dynamic expire */
#endif
#ifdef __NetBSD__
#define IPV6CTL_ANONPORTMIN	28	/* minimum ephemeral port */
#define IPV6CTL_ANONPORTMAX	29	/* maximum ephemeral port */
#define IPV6CTL_LOWPORTMIN	30	/* minimum reserved port */
#define IPV6CTL_LOWPORTMAX	31	/* maximum reserved port */
#endif
#define IPV6CTL_USETEMPADDR	32	/* use temporary addresses for
					   privacy extension */
#define IPV6CTL_TEMPPLTIME	33	/* preferred lifetime for tmpaddrs */
#define IPV6CTL_TEMPVLTIME	34	/* valid lifetime for tmpaddrs */
#define IPV6CTL_AUTO_LINKLOCAL	35	/* automatic link-local addr assign */
#define IPV6CTL_RIP6STATS	36	/* raw_ip6 stats */
#define IPV6CTL_PREFER_TEMPADDR	37	/* prefer temporary addr as src */
#define IPV6CTL_ADDRCTLPOLICY	38	/* get/set address selection policy */
#define IPV6CTL_USE_DEFAULTZONE	39	/* use default scope zone */

/* New entries should be added here from current IPV6CTL_MAXID value. */
/* to define items, should talk with KAME guys first, for *BSD compatibility */
#define IPV6CTL_MAXID		40

#ifdef IPV6CTL_RTEXPIRE
#define __IPV6CTL_NAMES_RTEXPIRE	"rtexpire"
#define __IPV6CTL_TYPE_RTEXPIRE		CTLTYPE_INT
#define __IPV6CTL_VARS_RTEXPIRE		0	/* &rtq_reallyold */
#else
#define __IPV6CTL_NAMES_RTEXPIRE	0
#define __IPV6CTL_TYPE_RTEXPIRE		0
#define __IPV6CTL_VARS_RTEXPIRE		0
#endif

#ifdef IPV6CTL_RTMINEXPIRE
#define __IPV6CTL_NAMES_RTMINEXPIRE	"minexpire"
#define __IPV6CTL_TYPE_RTMINEXPIRE	CTLTYPE_INT
#define __IPV6CTL_VARS_RTMINEXPIRE	0	/* &rtq_minreallyold */
#else
#define __IPV6CTL_NAMES_RTMINEXPIRE	0
#define __IPV6CTL_TYPE_RTMINEXPIRE	0
#define __IPV6CTL_VARS_RTMINEXPIRE	0
#endif

#ifdef IPV6CTL_RTMAXCACHE
#define __IPV6CTL_NAMES_RTMAXCACHE	"maxcache"
#define __IPV6CTL_TYPE_RTMAXCACHE	CTLTYPE_INT
#define __IPV6CTL_VARS_RTMAXCACHE	0	/* &rtq_toomany */
#else
#define __IPV6CTL_NAMES_RTMAXCACHE	0
#define __IPV6CTL_TYPE_RTMAXCACHE	0
#define __IPV6CTL_VARS_RTMAXCACHE	0
#endif

#ifdef IPV6CTL_ANONPORTMIN
#define __IPV6CTL_NAMES_ANONPORTMIN	"anonportmin"
#define __IPV6CTL_TYPE_ANONPORTMIN	CTLTYPE_INT
#define __IPV6CTL_VARS_ANONPORTMIN	0
#else
#define __IPV6CTL_NAMES_ANONPORTMIN	0
#define __IPV6CTL_TYPE_ANONPORTMIN	0
#define __IPV6CTL_VARS_ANONPORTMIN	0
#endif

#ifdef IPV6CTL_ANONPORTMAX
#define __IPV6CTL_NAMES_ANONPORTMAX	"anonportmax"
#define __IPV6CTL_TYPE_ANONPORTMAX	CTLTYPE_INT
#define __IPV6CTL_VARS_ANONPORTMAX	0
#else
#define __IPV6CTL_NAMES_ANONPORTMAX	0
#define __IPV6CTL_TYPE_ANONPORTMAX	0
#define __IPV6CTL_VARS_ANONPORTMAX	0
#endif

#ifdef IPV6CTL_LOWPORTMIN
#define __IPV6CTL_NAMES_LOWPORTMIN	"lowportmin"
#define __IPV6CTL_TYPE_LOWPORTMIN	CTLTYPE_INT
#define __IPV6CTL_VARS_LOWPORTMIN	0
#else
#define __IPV6CTL_NAMES_LOWPORTMIN	0
#define __IPV6CTL_TYPE_LOWPORTMIN	0
#define __IPV6CTL_VARS_LOWPORTMIN	0
#endif

#ifdef IPV6CTL_LOWPORTMAX
#define __IPV6CTL_NAMES_LOWPORTMAX	"lowportmax"
#define __IPV6CTL_TYPE_LOWPORTMAX	CTLTYPE_INT
#define __IPV6CTL_VARS_LOWPORTMAX	0
#else
#define __IPV6CTL_NAMES_LOWPORTMAX	0
#define __IPV6CTL_TYPE_LOWPORTMAX	0
#define __IPV6CTL_VARS_LOWPORTMAX	0
#endif


#define IPV6CTL_NAMES { \
	{ 0, 0 }, \
	{ "forwarding", CTLTYPE_INT }, \
	{ "redirect", CTLTYPE_INT }, \
	{ "hlim", CTLTYPE_INT }, \
	{ "mtu", CTLTYPE_INT }, \
	{ "forwsrcrt", CTLTYPE_INT }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ "mrtproto", CTLTYPE_INT }, \
	{ "maxfragpackets", CTLTYPE_INT }, \
	{ "sourcecheck", CTLTYPE_INT }, \
	{ "sourcecheck_logint", CTLTYPE_INT }, \
	{ "accept_rtadv", CTLTYPE_INT }, \
	{ "keepfaith", CTLTYPE_INT }, \
	{ "log_interval", CTLTYPE_INT }, \
	{ "hdrnestlimit", CTLTYPE_INT }, \
	{ "dad_count", CTLTYPE_INT }, \
	{ "auto_flowlabel", CTLTYPE_INT }, \
	{ "defmcasthlim", CTLTYPE_INT }, \
	{ "gifhlim", CTLTYPE_INT }, \
	{ "kame_version", CTLTYPE_STRING }, \
	{ "use_deprecated", CTLTYPE_INT }, \
	{ "rr_prune", CTLTYPE_INT }, \
	{ 0, 0 }, \
	{ "v6only", CTLTYPE_INT }, \
	{ __IPV6CTL_NAMES_RTEXPIRE, __IPV6CTL_TYPE_RTEXPIRE }, \
	{ __IPV6CTL_NAMES_RTMINEXPIRE, __IPV6CTL_TYPE_RTMINEXPIRE }, \
	{ __IPV6CTL_NAMES_RTMAXCACHE, __IPV6CTL_TYPE_RTMAXCACHE }, \
	{ __IPV6CTL_NAMES_ANONPORTMIN, __IPV6CTL_TYPE_ANONPORTMIN }, \
	{ __IPV6CTL_NAMES_ANONPORTMAX, __IPV6CTL_TYPE_ANONPORTMAX }, \
	{ __IPV6CTL_NAMES_LOWPORTMIN, __IPV6CTL_TYPE_LOWPORTMIN }, \
	{ __IPV6CTL_NAMES_LOWPORTMAX, __IPV6CTL_TYPE_LOWPORTMAX }, \
	{ "use_tempaddr", CTLTYPE_INT }, \
	{ "temppltime", CTLTYPE_INT }, \
	{ "tempvltime", CTLTYPE_INT }, \
	{ "auto_linklocal", CTLTYPE_INT }, \
	{ 0, 0 }, \
	{ "prefer_tempaddr", CTLTYPE_INT }, \
	{ 0, 0 }, \
	{ "use_defaultzone", CTLTYPE_INT }, \
}

#ifdef __bsdi__
#define IPV6CTL_VARS { \
	0, \
	&ip6_forwarding, \
	&ip6_sendredirects, \
	&ip6_defhlim, \
	0, \
	&ip6_forward_srcrt, \
	0, \
	0, \
	0, \
	&ip6_maxfragpackets, \
	&ip6_sourcecheck, \
	&ip6_sourcecheck_interval, \
	&ip6_accept_rtadv, \
	&ip6_keepfaith, \
	&ip6_log_interval, \
	&ip6_hdrnestlimit, \
	&ip6_dad_count, \
	&ip6_auto_flowlabel, \
	&ip6_defmcasthlim, \
	&ip6_gif_hlim, \
	0, \
	&ip6_use_deprecated, \
	&ip6_rr_prune, \
	0, \
	&ip6_v6only, \
	__IPV6CTL_VARS_RTEXPIRE, \
	__IPV6CTL_VARS_RTMINEXPIRE, \
	__IPV6CTL_VARS_RTMAXCACHE, \
	__IPV6CTL_VARS_ANONPORTMIN, \
	__IPV6CTL_VARS_ANONPORTMAX, \
	__IPV6CTL_VARS_LOWPORTMIN, \
	__IPV6CTL_VARS_LOWPORTMAX, \
	&ip6_use_tempaddr, \
	&ip6_temp_preferred_lifetime, \
	&ip6_temp_valid_lifetime, \
	&ip6_auto_linklocal, \
	0, \
	&ip6_prefer_tempaddr, \
	0, \
	&ip6_use_defzone,\
}
#endif
#endif /* !_XOPEN_SOURCE */

/*
 * Redefinition of mbuf flags
 */
#define	M_AUTHIPHDR	M_PROTO2
#define	M_DECRYPTED	M_PROTO3
#define	M_LOOP		M_PROTO4
#define	M_AUTHIPDGM	M_PROTO5

#ifdef _KERNEL
struct cmsghdr;
struct mbuf;
struct ifnet;

int	in6_cksum __P((struct mbuf *, u_int8_t, u_int32_t, u_int32_t));
int	in6_localaddr __P((struct in6_addr *));
int	in6_addrscope __P((struct in6_addr *));
struct	in6_ifaddr *in6_ifawithifp __P((struct ifnet *, struct in6_addr *));
extern void in6_if_up __P((struct ifnet *));
struct sockaddr;

void	in6_sin6_2_sin __P((struct sockaddr_in *sin,
			    struct sockaddr_in6 *sin6));
void	in6_sin_2_v4mapsin6 __P((struct sockaddr_in *sin,
				 struct sockaddr_in6 *sin6));
void	in6_sin6_2_sin_in_sock __P((struct sockaddr *nam));
void	in6_sin_2_v4mapsin6_in_sock __P((struct sockaddr **nam));
extern void addrsel_policy_init __P((void));

#define	satosin6(sa)	((struct sockaddr_in6 *)(sa))
#define	sin6tosa(sin6)	((struct sockaddr *)(sin6))
#define	ifatoia6(ifa)	((struct in6_ifaddr *)(ifa))
#endif /* _KERNEL */

__BEGIN_DECLS
struct cmsghdr;

extern int inet6_option_space __P((int));
extern int inet6_option_init __P((void *, struct cmsghdr **, int));
extern int inet6_option_append __P((struct cmsghdr *, const u_int8_t *,
	int, int));
extern u_int8_t *inet6_option_alloc __P((struct cmsghdr *, int, int, int));
extern int inet6_option_next __P((const struct cmsghdr *, u_int8_t **));
extern int inet6_option_find __P((const struct cmsghdr *, u_int8_t **, int));

extern size_t inet6_rthdr_space __P((int, int));
extern struct cmsghdr *inet6_rthdr_init __P((void *, int));
extern int inet6_rthdr_add __P((struct cmsghdr *, const struct in6_addr *,
		unsigned int));
extern int inet6_rthdr_lasthop __P((struct cmsghdr *, unsigned int));
#if 0 /* not implemented yet */
extern int inet6_rthdr_reverse __P((const struct cmsghdr *, struct cmsghdr *));
#endif
extern int inet6_rthdr_segments __P((const struct cmsghdr *));
extern struct in6_addr *inet6_rthdr_getaddr __P((struct cmsghdr *, int));
extern int inet6_rthdr_getflags __P((const struct cmsghdr *, int));

extern int inet6_opt_init __P((void *, size_t));
extern int inet6_opt_append __P((void *, size_t, int, u_int8_t,
				 size_t, u_int8_t, void **));
extern int inet6_opt_finish __P((void *, size_t, int));
extern int inet6_opt_set_val __P((void *, size_t, void *, int));

extern int inet6_opt_next __P((void *, size_t, int, u_int8_t *,
			       size_t *, void **));
extern int inet6_opt_find __P((void *, size_t, int, u_int8_t,
			  size_t *, void **));
extern int inet6_opt_get_val __P((void *, size_t, void *, int));
extern size_t inet6_rth_space __P((int, int));
extern void *inet6_rth_init __P((void *, int, int, int));
extern int inet6_rth_add __P((void *, const struct in6_addr *));
extern int inet6_rth_reverse __P((const void *, void *));
extern int inet6_rth_segments __P((const void *));
extern struct in6_addr *inet6_rth_getaddr __P((const void *, int));

/* the followings two are experimental.  should not merge them to *BSD. */
struct sockaddr;		/* XXX */

extern int addr2scopetype __P((struct sockaddr *));
extern int inet_zoneid __P((int, int, char *, u_int32_t *));
__END_DECLS

#endif /* !_NETINET6_IN6_H_ */
