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
 * @file src/readlink.c
 * @brief readlink()
 */

#include "plibc_private.h"

/**
 * @brief Read the contents of a symbolic link
 */
int _win_readlink(const char *path, char *buf, size_t bufsize)
{
  char szDeref[_MAX_PATH + 1];
  int iLen;

  if(strlen(path) > _MAX_PATH)
  {
    errno = ENAMETOOLONG;
    return -1;
  }

  strcpy(szDeref, path);

  if (__win_deref(szDeref) == -1)
    return -1;

  if ((iLen = strlen(szDeref)) > bufsize)
  {
    errno = ENAMETOOLONG;
    return -1;
  }
  
  strcpy(buf, szDeref);

  errno = 0;
  return iLen;
}

/* end of readlink.c */
