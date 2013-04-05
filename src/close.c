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
 * @file src/close.c
 * @brief close()
 */

#include "plibc_private.h"

int _win_close(int fd)
{
  THandleType theType;
  int ret;

  theType = __win_GetHandleType((DWORD) fd);
  switch(theType)
  {
    case SOCKET_HANDLE:
      ret = closesocket(fd);
      if (ret == SOCKET_ERROR)
        SetErrnoFromWinsockError(WSAGetLastError());
      break;
    case PIPE_HANDLE:
      if (!CloseHandle((HANDLE) fd))
      {
        SetErrnoFromWinError(GetLastError());
        ret = -1;
      }
      else
        ret = 0;
      break;
    case FD_HANDLE:
      ret = close(fd);
      break;
    default:
      theType = UNKNOWN_HANDLE;
    case UNKNOWN_HANDLE:
      ret = -1;
      errno = EBADF;
      SetLastError(ERROR_INVALID_HANDLE);
      __plibc_panic(5, "Cannot close() unknown handle\n");
      break;
  }

  if (theType != UNKNOWN_HANDLE)
  {
    __win_DiscardHandleBlockingMode(fd);
    __win_DiscardHandleType(fd);
  }

  return ret;
}

/* end of close.c */
