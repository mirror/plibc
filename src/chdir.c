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
 * @file src/chdir.c
 * @brief chdir()
 */

#include "plibc_private.h"
/* for _wchdir */
#include <direct.h> 
/**
 * Change directory
 */
int _win_chdir(const char *path)
{
  wchar_t szDir[_MAX_PATH + 1];
          
  long lRet;

  if (plibc_utf8_mode() == 1)
    lRet = plibc_conv_to_win_pathwconv(path, szDir);
  else
    lRet = plibc_conv_to_win_path(path, (char *) szDir);
  if (lRet != ERROR_SUCCESS)
  {
    SetErrnoFromWinError(lRet);
    return -1;
  }

  /* chdir sets errno */
  if (plibc_utf8_mode() == 1)
    return _wchdir(szDir);
  else
    return chdir((char *) szDir);
}

/* end of chdir.c */
