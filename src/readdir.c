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
 * @file src/readdir.c
 * @brief readdir()
 */

#include "plibc_private.h"

/**
 * @brief Read a directory
 */
struct dirent *_win_readdir(DIR *dirp)
{
  /* readdir sets errno */
  if (plibc_utf8_mode() == 1)
  {
    struct _wdirent *w;
    struct plibc_WDIR *pwd;
    errno = 0;
    pwd = (struct plibc_WDIR *) dirp;
    if (pwd->self != pwd)
    {
      errno = EINVAL;
      return NULL;
    }

    w = _wreaddir(pwd->mingw_wdir);
    if (w == NULL)
      return NULL;

    pwd->udirent.d_ino = w->d_ino;
    pwd->udirent.d_reclen = w->d_reclen;
    pwd->udirent.d_namlen = w->d_namlen;

    if (wchartostr_buf (w->d_name, pwd->udirent.d_name, FILENAME_MAX, CP_UTF8) < 0)
    {
      errno = EOVERFLOW;
      return NULL;
    }
    pwd->udirent.d_namlen = strlen (pwd->udirent.d_name);
    return &pwd->udirent;
  }
  else
    return readdir(dirp);
}

/* end of readdir.c */
