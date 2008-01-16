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
 * @file src/write.c
 * @brief write()
 */

#include "plibc_private.h"

DWORD WINAPI __win_Write(TReadWriteInfo *pInfo)
{
  int iRet;
  
  if (__win_GetHandleType((DWORD) pInfo->fildes) == FD_HANDLE)
  {
    _setmode(pInfo->fildes, _O_BINARY);
		errno = 0;
		iRet = _write(pInfo->fildes, pInfo->buf, pInfo->nbyte);
    free(pInfo);
    
    return iRet;
	}
	else
	{
    DWORD dwWritten;
    if (!WriteFile((HANDLE) pInfo->fildes, pInfo->buf, pInfo->nbyte,
      &dwWritten, NULL))
    {
      SetErrnoFromWinError(GetLastError());
      free(pInfo);
      
      return -1;
    }
    else
    {
      free(pInfo);      
      return dwWritten;
    }
	}
}

/**
 * @brief Write on a file
 *        If the handle is in non-blocking mode, this function
 *        always returns 1 for non-sockets.
 */
int _win_write(int fildes, const void *buf, size_t nbyte)
{
  if (__win_GetHandleType((DWORD) fildes) == SOCKET_HANDLE)
  {
    return _win_send(fildes, buf, nbyte, 0);
  }
  else
  {
    TReadWriteInfo *pInfo;
    
    pInfo = malloc(sizeof(TReadWriteInfo));
    pInfo->fildes = fildes;
    pInfo->buf = malloc(nbyte);
    memcpy(pInfo->buf, buf, nbyte);
    pInfo->nbyte = nbyte;

    if (__win_IsHandleMarkedAsBlocking(fildes))
      return __win_Write(pInfo);
    else
    {
      DWORD dwTID; /* Last ptr of CreateThread my not be NULL under Win9x */
      HANDLE h;
      
      h = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) __win_Write, pInfo, 0, &dwTID);
      CloseHandle(h);
      return nbyte;
    }
  }
}

/* end of write.c */
