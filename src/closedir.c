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
 * @file src/closedir.c
 * @brief closedir()
 */

#include "plibc_private.h"

/**
 * @brief Close a directory
 */
int _win_closedir(DIR *dirp)
{
  /* closedir sets errno */
  if (plibc_utf8_mode() == 1)
  {
    struct plibc_WDIR *pwd;
    int result;
    pwd = (struct plibc_WDIR *) dirp;
    if (pwd->self != pwd)
    {
      errno = EINVAL;
      return -1;
    }
    result = _wclosedir(pwd->mingw_wdir);
    free (pwd);
    return result;
  }
  else
    return closedir(dirp);
}


/* end of closedir.c */
