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
 * @file src/read.c
 * @brief read()
 */

#include "plibc_private.h"

DWORD WINAPI __win_Read(TReadWriteInfo *pInfo)
{
  int iRet;
  
  if (__win_GetHandleType((DWORD) pInfo->fildes) == FD_HANDLE)
  {
    _setmode(pInfo->fildes, _O_BINARY);
		errno = 0;
		iRet = _read(pInfo->fildes, pInfo->buf, pInfo->nbyte);
    free(pInfo);
    
    return iRet;
	}
	else
	{
    DWORD dwRead;
    
    errno = 0;
    if (!ReadFile((HANDLE) pInfo->fildes, pInfo->buf, pInfo->nbyte, &dwRead,
      NULL))
    {
      free(pInfo);
      SetErrnoFromWinError(GetLastError());
      return -1;
    }
    else
    {
      free(pInfo);
      return dwRead;
    }
	}
}

/**
 * @brief Reads data from a file.
 *        If the handle is in non-blocking mode, this function
 *        always returns 1 for non-sockets.
 */
int _win_read(int fildes, void *buf, size_t nbyte)
{
  if (__win_GetHandleType((DWORD) fildes) == SOCKET_HANDLE)
    return _win_recv(fildes, (char *) buf, nbyte, 0);
  else
  {
    TReadWriteInfo *pInfo;
    
    pInfo = malloc(sizeof(TReadWriteInfo));
    pInfo->fildes = fildes;
    pInfo->buf = buf;
    pInfo->nbyte = nbyte;

    if (__win_IsHandleMarkedAsBlocking(fildes))
      return __win_Read(pInfo);
    else
    {
      DWORD dwTID; /* Last ptr of CreateThread my not be NULL under Win9x */
      HANDLE h;
      
      h = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) __win_Read, pInfo, 0, &dwTID);
      CloseHandle(h);
      return 1;
    }
  }
}

/* end of read.c */
