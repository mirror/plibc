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
 * @file src/access.c
 * @brief access()
 */

#include "plibc_private.h"

/**
 * @brief Determine file-access permission.
 */
int _win_access( const char *path, int mode )
{
  wchar_t szFile[_MAX_PATH + 1];
  long lRet;

  mode &= 6;
  if (plibc_utf8_mode() == 1)
    lRet = plibc_conv_to_win_pathwconv(path, szFile);
  else
    lRet = plibc_conv_to_win_path(path, (char *) szFile);
  if (lRet != ERROR_SUCCESS)
  {
    SetErrnoFromWinError(lRet);
    return -1;
  }

  /* access sets errno */
  if (plibc_utf8_mode() == 1)
    return _waccess(szFile, mode);
  else
    return access((char *) szFile, mode);
}

/* end of access.c */
