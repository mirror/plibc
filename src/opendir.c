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
 * @file src/opendir.c
 * @brief opendir()
 */

#include "plibc_private.h"

/**
 * @brief Open a directory
 */
DIR *_win_opendir(const char *dirname)
{
  _WDIR *mingw_wdir = NULL;
  struct plibc_WDIR *pwd;
  wchar_t szDir[_MAX_PATH + 1];
  long lRet;
  if (plibc_utf8_mode() == 1)
    lRet = plibc_conv_to_win_pathwconv(dirname, szDir);
  else
    lRet = plibc_conv_to_win_path(dirname, (char *) szDir);
  if (lRet != ERROR_SUCCESS)
  {
    SetErrnoFromWinError(lRet);
    return NULL;
  }

  /* opendir sets errno */
  if (plibc_utf8_mode() == 1)
  {
    mingw_wdir = _wopendir(szDir);
    if (mingw_wdir == NULL)
      return mingw_wdir;
    /* Now, we need to store some extra info, so return
     * a wrapped pointer of our own.
     */
    pwd = malloc (sizeof (struct plibc_WDIR));
    pwd->mingw_wdir = mingw_wdir;
    pwd->self = pwd;
    memset (&pwd->udirent, 0, sizeof (struct dirent));
    return (DIR *) pwd;
  }
  else
    return opendir((char *) szDir);
}


/* end of opendir.c */
