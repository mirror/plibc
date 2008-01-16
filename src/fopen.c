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
 * @file src/fopen.c
 * @brief fopen()
 */

#include "plibc_private.h"

/**
 * Open a file
 */
FILE *_win_fopen(const char *filename, const char *mode)
{
  char szFile[_MAX_PATH + 1];
  FILE *hFile;
  int i;

  if ((i = plibc_conv_to_win_path(filename, szFile)) != ERROR_SUCCESS)
  {
    SetErrnoFromWinError(i);

    return NULL;
  }

  hFile = fopen(szFile, mode);
  SetErrnoFromWinError(GetLastError());

  return hFile;
}

/* end of fopen.c */
