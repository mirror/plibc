/*
     This file is part of PlibC.
     (C) 2005, 2006, 2007, 2008 Nils Durner (and other contributing authors)

     This library is free software; you can redistribute it and/or
     modify it under the terms of the GNU Lesser General Public
     License as published by the Free Software Foundation; either
     version 2.1 of the License, or (at your option) any later version.
  
     This library is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
     Lesser General Public License for more details.
  
     You should have received a copy of the GNU Lesser General Public
     License along with this library; if not, write to the Free Software
     Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/**
 * @file src/resolv.c
 * @brief Resolver function
 */

#include "plibc_private.h"

extern Tgetnameinfo _plibc_getnameinfo;
extern Tfreeaddrinfo _plibc_freeaddrinfo;
extern Tgetaddrinfo _plibc_getaddrinfo;

int getnameinfo(const struct sockaddr *sa, socklen_t salen,
       char *node, socklen_t nodelen, char *service,
       socklen_t servicelen, int flags)
{
  return _plibc_getnameinfo(sa, salen, node, nodelen, service,
    servicelen, flags);
}

void freeaddrinfo(struct addrinfo *ai)
{
  _plibc_freeaddrinfo(ai);
}

int getaddrinfo(const char *nodename,
       const char *servname,
       const struct addrinfo *hints,
       struct addrinfo **res)
{
  return _plibc_getaddrinfo(nodename, servname, hints, res);
}
