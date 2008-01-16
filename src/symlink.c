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
 * @file src/symlink.c
 * @brief symlink()
 */

#include "plibc_private.h"

/***
 * @brief Make a link to a file
 **/
int _win_symlink(const char *path1, const char *path2)
{
  char szFile1[_MAX_PATH + 1], szFile2[_MAX_PATH + 1];
  long lRet;

  if ((lRet = plibc_conv_to_win_path(path1, szFile1)) != ERROR_SUCCESS)
  {
    SetErrnoFromWinError(lRet);
    return -1;
  }

  if ((lRet = plibc_conv_to_win_path_ex(path2, szFile2, 0)) != ERROR_SUCCESS)
  {
    SetErrnoFromWinError(lRet);
    return -1;
  }

  /* CreateShortcut sets errno */
  lRet = _plibc_CreateShortcut(szFile1, szFile2);

  return lRet ? 0 : -1;
}

/* end of symlink.c */
