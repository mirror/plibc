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
 * @file src/flock.c
 * @brief flock()
 */

#include "plibc_private.h"

extern const char *errlist;

/**
 * Apply or remove an advisory lock on an open file
 */
int flock(int fd, int operation)
{
  DWORD dwFlags;
  HANDLE hFile;
  OVERLAPPED theOvInfo;
  BOOL bRet;

  errno = 0;

  hFile = (HANDLE) _get_osfhandle(fd);
  memset(&theOvInfo, 0, sizeof(OVERLAPPED));

  /* Don't deadlock ourselves */
  if (IsWinNT())
    bRet = UnlockFileEx(hFile, 0, 1, 0, &theOvInfo);
  else
    bRet = UnlockFile(hFile, 0, 0, 1, 0);

  if (operation & LOCK_UN)
  {
    if (!bRet && ((dwFlags = GetLastError()) != ERROR_NOT_LOCKED))
    {
      SetErrnoFromWinError(dwFlags);
      return -1;
    }
    else
      return 0;
  }

  if (operation & LOCK_EX)
  {
    dwFlags = LOCKFILE_EXCLUSIVE_LOCK;
  }
  else if (operation & LOCK_SH)
  {
    dwFlags = 0;
  }
  else
  {
    errno = EINVAL;
    return -1;
  }

  if (operation & LOCK_NB)
    dwFlags |= LOCKFILE_FAIL_IMMEDIATELY;

  if (IsWinNT())
    bRet = LockFileEx(hFile, dwFlags, 0, 1, 0, &theOvInfo);
  else
    bRet = LockFile(hFile, 0, 0, 1, 0);

  if (! bRet)
  {
    SetErrnoFromWinError(GetLastError());
    return -1;
  }
  else
    return 0;
}

/* end of flock.c */
