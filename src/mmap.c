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
 * @file src/mmap.c
 * @brief mmap() and munmap.c
 */

#include "plibc_private.h"

extern unsigned int uiMappingsCount;
extern TMapping *pMappings;
extern HANDLE hMappingsLock;

/**
 * @brief map files into memory
 * @author Cygwin team
 * @author Nils Durner
 */
void *_win_mmap(void *start, size_t len, int access, int flags, int fd,
                unsigned long long off) {
  DWORD protect, high, low, access_param;
  HANDLE h, hFile;
  SECURITY_ATTRIBUTES sec_none;
  void *base;
  BOOL bFound = FALSE;
  unsigned int uiIndex;

  errno = 0;

  switch(access)
  {
    case PROT_WRITE:
      protect = PAGE_READWRITE;
      access_param = FILE_MAP_WRITE;
      break;
    case PROT_READ:
      protect = PAGE_READONLY;
      access_param = FILE_MAP_READ;
      break;
    default:
      protect = PAGE_WRITECOPY;
      access_param = FILE_MAP_COPY;
      break;
  }

  sec_none.nLength = sizeof(SECURITY_ATTRIBUTES);
  sec_none.bInheritHandle = TRUE;
  sec_none.lpSecurityDescriptor = NULL;

  hFile = (HANDLE) _get_osfhandle(fd);

  h = CreateFileMapping(hFile, &sec_none, protect, 0, 0, NULL);

  if (! h)
  {
    SetErrnoFromWinError(GetLastError());
    return MAP_FAILED;
  }

  high = off >> 32;
  low = off & ULONG_MAX;
  base = NULL;

  /* If a non-zero start is given, try mapping using the given address first.
     If it fails and flags is not MAP_FIXED, try again with NULL address. */
  if (start)
    base = MapViewOfFileEx(h, access_param, high, low, len, start);
  if (!base && !(flags & MAP_FIXED))
    base = MapViewOfFileEx(h, access_param, high, low, len, NULL);

  if (!base || ((flags & MAP_FIXED) && base != start))
  {
    if (!base)
      SetErrnoFromWinError(GetLastError());
    else
      errno = EINVAL;

    CloseHandle(h);
    return MAP_FAILED;
  }

  /* Save mapping handle */
  WaitForSingleObject(hMappingsLock, INFINITE);

  for(uiIndex = 0; uiIndex <= uiMappingsCount; uiIndex++)
  {
    if (pMappings[uiIndex].pStart == base)
    {
      bFound = 1;
      break;
    }
  }

  if (! bFound)
  {
    int inserted = 0;
    
    uiIndex = 0;

    while(!inserted)
    {
      if (pMappings[uiIndex].pStart == NULL)
      {
        pMappings[uiIndex].pStart = base;
        pMappings[uiIndex].hMapping = h;
        
        inserted = 1;
      }
      if (uiIndex == uiMappingsCount)
      {
        uiMappingsCount++;
        pMappings = (TMapping *) realloc(pMappings, (uiMappingsCount + 1) * sizeof(TMapping));
        pMappings[uiMappingsCount].pStart = NULL;
      }
      uiIndex++;
    }
  }
  ReleaseMutex(hMappingsLock);

  return base;
}

/**
 * @brief Unmap files from memory
 * @author Cygwin team
 * @author Nils Durner
 */
int _win_munmap(void *start, size_t length)
{
  unsigned uiIndex;

  if (UnmapViewOfFile(start))
  {
    BOOL success = TRUE;
    errno = 0;

    /* Release mapping handle */
    WaitForSingleObject(hMappingsLock, INFINITE);

    for(uiIndex = 0; uiIndex <= uiMappingsCount; uiIndex++)
    {
      if (pMappings[uiIndex].pStart == start)
      {
        success = CloseHandle(pMappings[uiIndex].hMapping);
        SetErrnoFromWinError(GetLastError());
        pMappings[uiIndex].pStart = NULL;
        pMappings[uiIndex].hMapping = NULL;

        break;
      }
    }

    ReleaseMutex(hMappingsLock);

    return success ? 0 : MAP_FAILED;
  }
  else
  {
    SetErrnoFromWinError(GetLastError());
    return MAP_FAILED;
  }
}

/* end of mmap.c */
