/*
     This file is part of PlibC.
     (C) 2010 Nils Durner (and other contributing authors)

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
 * @file src/sysconf.c
 * @brief get configurable system variables
 * @author Nils Durner
 */

#include "plibc_private.h"

/**
 * @brief get configurable system variables
 * @param system variable to be queried
 * @return -1 on error, current variable value on the system otherwise
 */
long _win_sysconf(int name)
{
  switch(name)
  {
    case _SC_PAGE_SIZE:
      {
        SYSTEM_INFO sys_info;
        GetSystemInfo(&sys_info);
        return sys_info.dwAllocationGranularity;
      }
    default:
      errno = EINVAL;
      return -1;
  }
}
