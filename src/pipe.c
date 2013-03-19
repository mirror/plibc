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
 * @file src/pipe.c
 * @brief pipe()
 */

#include "plibc_private.h"

/**
 * Create a pipe for reading and writing
 */
int _win_pipe(int *phandles)
{
  /* To get non-blocking pipes we could use CreateNamedPipe here. But it isn't
     implemented under Win9x. */
  if (!CreatePipe((HANDLE *) &phandles[0],(HANDLE *) &phandles[1], NULL, 0))
  {
    SetErrnoFromWinError(GetLastError());

    return -1;
  }
  else
  {
    errno = 0;
    __win_SetHandleType((DWORD) phandles[0], PIPE_HANDLE);
    __win_SetHandleType((DWORD) phandles[1], PIPE_HANDLE);
    
    return 0;
  }
}

/**
 * Make a FIFO special file
 * @todo use mode
 */
int _win_mkfifo(const char *path, mode_t mode)
{
  HANDLE ret;
  SECURITY_ATTRIBUTES sec;
  wchar_t *wpath = NULL;
  if (plibc_utf8_mode() == 1)
  {
    if (strtowchar (path, &wpath, CP_UTF8) < 0)
    {
      SetErrnoFromWinError(GetLastError());
      return -1;
    }
  }

  ZeroMemory(&sec, sizeof(sec));
  sec.nLength = sizeof(sec);
  sec.bInheritHandle = TRUE;

  if (plibc_utf8_mode() == 1)
  {
    int e;
    ret = CreateNamedPipeW(wpath, PIPE_ACCESS_DUPLEX |
        FILE_FLAG_OVERLAPPED, PIPE_TYPE_BYTE, 1, 0, 0, 0, &sec);
    e = errno;
    free (wpath);
    errno = e;
  }
  else
  {
    ret = CreateNamedPipe(path, PIPE_ACCESS_DUPLEX |
        FILE_FLAG_OVERLAPPED, PIPE_TYPE_BYTE, 1, 0, 0, 0, &sec);
  }
  if (ret == INVALID_HANDLE_VALUE)
  {
    SetErrnoFromWinError(GetLastError());
    return -1;
  }
  else
  {
    errno = 0;
    __win_SetHandleType((DWORD) ret, PIPE_HANDLE);

    return 0;
  }
}

/* end of pipe.c */
