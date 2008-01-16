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
 * @file src/fwrite.c
 * @brief fwrite()
 */

#include "plibc_private.h"

/**
 * @brief Writes data to a stream
 */
size_t _win_fwrite(const void *buffer, size_t size, size_t count, FILE *stream)
{
  DWORD dwWritten;

  WriteFile((HANDLE) _get_osfhandle(fileno(stream)), buffer, size, &dwWritten,
            NULL);
  SetErrnoFromWinError(GetLastError());

  return dwWritten;
}

/* end of fwrite.c */
