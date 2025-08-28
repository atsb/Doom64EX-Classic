// Emacs style mode select   -*- C++ -*- 
//-----------------------------------------------------------------------------
//
// Copyright(C) 2005 Simon Howard
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
// 02111-1307, USA.
//
// DESCRIPTION:
//     Networking module which uses SDL_net
//
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL3/SDL_net.h>

#include "doomdef.h"
#include "i_system.h"
#include "m_misc.h"
#include "net_defs.h"
#include "net_io.h"
#include "net_packet.h"
#include "net_sdl.h"
#include "z_zone.h"

//
// NETWORKING
//

#define DEFAULT_PORT 2342

static int port = DEFAULT_PORT;

typedef struct
{
    net_addr_t net_addr;
} addrpair_t;

static addrpair_t **addr_table;
static int addr_table_size = -1;

// Initialises the address table

static void NET_SDL_InitAddrTable(void)
{

}

static dboolean AddressesEqual(char *a, char *b)
{
    return 0;
}

// Finds an address by searching the table.  If the address is not found,
// it is added to the table.

static net_addr_t *NET_SDL_FindAddress(char *addr)
{
    return 0;
}

static void NET_SDL_FreeAddress(net_addr_t *addr)
{

}

static dboolean NET_SDL_InitClient(void)
{
    return 0;
}

static dboolean NET_SDL_InitServer(void)
{
    return 0;
}

static dboolean NET_SDL_RecvPacket(net_addr_t **addr, net_packet_t **packet)
{
    return 0;
}

void NET_SDL_AddrToString(net_addr_t *addr, char *buffer, int buffer_len)
{

}

// Complete module

net_module_t net_sdl_module =
{
    NET_SDL_InitClient,
    NET_SDL_InitServer,
    NET_SDL_RecvPacket,
    NET_SDL_AddrToString,
    NET_SDL_FreeAddress,
};

