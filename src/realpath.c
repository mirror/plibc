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
 * @file src/realpath.c
 * @brief realpath()
 */

#include "plibc_private.h"

char *realpath(const char *file_name, char *resolved_name)
{
  char szFile[_MAX_PATH + 1];
  long lRet;
  char *pszRet;

  if ((lRet = plibc_conv_to_win_path(file_name, szFile)) != ERROR_SUCCESS)
  {
    SetErrnoFromWinError(lRet);
    return NULL;
  }

  pszRet = _fullpath(szFile, resolved_name, MAX_PATH);
  SetErrnoFromWinError(GetLastError());

  return pszRet;
}

/* end of realpath.c */
