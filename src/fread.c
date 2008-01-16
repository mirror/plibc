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
 * @file src/fread.c
 * @brief fread()
 */

#include "plibc_private.h"

/**
 * @brief Reads data from a stream
 */
size_t _win_fread( void *buffer, size_t size, size_t count, FILE *stream )
{
  DWORD dwRead;
  int iItemsRead;
  void *pDest = buffer;

  for(iItemsRead = 0; iItemsRead < count; iItemsRead++)
  {
    if (!ReadFile((HANDLE) _get_osfhandle(fileno(stream)), pDest, size,
                  &dwRead, NULL))
      break;
    pDest += size;
  }

  SetErrnoFromWinError(GetLastError());

  return iItemsRead;
}

/* end of fread.c */
