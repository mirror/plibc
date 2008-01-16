/*
     This file is part of PlibC.
     (C) 2005 Nils Durner (and other contributing authors)

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
 * @file src/fstat.c
 * @brief fstat()
 */

#include "plibc_private.h"

/**
 * @brief Get information about an open file.
 */
int _win_fstat(int handle, struct stat *buffer)
{
  errno = 0;

  /* File */
  if (fstat(handle, buffer) == -1)
  {
    /* We just check for a valid handle here */

    /* Handle */
    memset(buffer, 0, sizeof(struct stat));
    if (GetFileType((HANDLE) handle) == FILE_TYPE_UNKNOWN && GetLastError() != NO_ERROR)
    {
      /* socket */
      unsigned long lRead;
      if (ioctlsocket(handle, FIONREAD, &lRead) == SOCKET_ERROR)
        /* Invalid handle */
        return -1;
    }
  }

  return 0;
}

/* end of fstat.c */
