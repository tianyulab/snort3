/****************************************************************************
 *
 *  Copyright (C) 2014 Cisco and/or its affiliates. All rights reserved.
 *  Copyright (C) 2005-2013 Sourcefire, Inc.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License Version 2 as
 *  published by the Free Software Foundation.  You may not use, modify or
 *  distribute this program under any other version of the GNU General
 *  Public License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *****************************************************************************/

#ifndef IP_SESSION_H
#define IP_SESSION_H

#include <sys/time.h>
#include "flow/session.h"

struct Fragment;
struct FragEngine;

/* tracker for a fragmented packet set */
struct FragTracker
{
    uint8_t protocol;      /* IP protocol */

    uint8_t ttl;           /* ttl used to detect evasions */
    uint8_t alerted;
    uint32_t frag_flags;   /* bit field */

    uint32_t frag_bytes;   /* number of fragment bytes stored, based
                             * on aligned fragment offsets/sizes
                             */

    uint32_t calculated_size; /* calculated size of reassembled pkt, based on
                                * last frag offset
                                */

    uint32_t frag_pkts;   /* nummber of frag pkts stored under this tracker */

    struct timeval frag_time; /* time we started tracking this frag */

    Fragment *fraglist;      /* list of fragments */
    Fragment *fraglist_tail; /* tail ptr for easy appending */
    int fraglist_count;       /* handy dandy counter */

    uint8_t ip_options_len;  /* length of ip options for this set of frags */
    uint8_t *ip_options_data; /* ip options from offset 0 packet */
    uint8_t copied_ip_options_len;  /* length of 'copied' ip options */

    FragEngine *engine;

    int ordinal;
    int ipprotocol;
    int application_protocol;
    uint32_t frag_policy;

    // Count of IP fragment overlap for each packet id.
    uint32_t overlap_count;
};

class IpSession : public Session
{
public:
    IpSession(Flow*);

    bool setup(Packet*) override;
    int process(Packet*) override;
    void clear() override;

public:
    FragTracker tracker;
};

#endif

