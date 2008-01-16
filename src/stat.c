/*
     This file is part of PlibC.
     (C) 2005, 2006 Nils Durner (and other contributing authors)

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
 * @file src/stat.c
 * @brief stat()
 */

#include "plibc_private.h"

/**
 * @brief Get status information on a file
 */
int __win_stat(const char *path, struct stat *buffer, int iDeref)
{
  char szFile[_MAX_PATH + 1];
  long lRet;

  if ((lRet = plibc_conv_to_win_path(path, szFile)) != ERROR_SUCCESS)
  {
    SetErrnoFromWinError(lRet);
    return -1;
  }

  /* Remove trailing slash */
  lRet = strlen(szFile) - 1;
  if (szFile[lRet] == '\\')
  {
    szFile[lRet] = 0;
  }

  /* Dereference symlinks */
  if (iDeref)
  {
    if (__win_deref(szFile) == -1 && errno != EINVAL)
      return -1;
  }

  /* stat sets errno */
  return stat(szFile, buffer);
}

/**
 * @brief Get status information on a file
 */
int _win_stat(const char *path, struct stat *buffer)
{
  return __win_stat(path, buffer, 1);
}

/**
 * @brief Get symbolic link status
 */
int _win_lstat(const char *path, struct stat *buf)
{
  return __win_stat(path, buf, 0);
}

/**
 * @brief Get status information on a file
 */
int __win_stat64(const char *path, struct stat64 *buffer, int iDeref)
{
  char szFile[_MAX_PATH + 1];
  long lRet;

  if ((lRet = plibc_conv_to_win_path(path, szFile)) != ERROR_SUCCESS)
  {
    SetErrnoFromWinError(lRet);
    return -1;
  }

  /* Remove trailing slash */
  lRet = strlen(szFile) - 1;
  if (szFile[lRet] == '\\')
  {
    szFile[lRet] = 0;
  }

  /* Dereference symlinks */
  if (iDeref)
  {
    if (__win_deref(szFile) == -1 && errno != EINVAL)
      return -1;
  }

  if (!_plibc_stat64)
  {
    /* not supported under Windows 9x */
    struct stat theStat;
    int iRet;
    
    iRet = __win_stat(path, &theStat, iDeref);
    
    buffer->st_dev = theStat.st_dev;
    buffer->st_ino = theStat.st_ino;
    buffer->st_mode = theStat.st_mode;
    buffer->st_nlink = theStat.st_nlink;
    buffer->st_uid = theStat.st_uid;
    buffer->st_gid = theStat.st_gid;
    buffer->st_rdev = theStat.st_rdev;
    buffer->st_size = (theStat.st_size > LONG_MAX) ? LONG_MAX : theStat.st_size;
    buffer->st_atime = (theStat.st_atime > LONG_MAX) ? LONG_MAX : theStat.st_atime;
    buffer->st_mtime = (theStat.st_mtime > LONG_MAX) ? LONG_MAX : theStat.st_mtime;
    buffer->st_ctime = (theStat.st_ctime > LONG_MAX) ? LONG_MAX : theStat.st_ctime;
    
    return iRet;
  }
  else
    /* stat sets errno */
    return _plibc_stat64(szFile, buffer);
}

/**
 * @brief Get status information on a file
 */
int _win_stat64(const char *path, struct stat64 *buffer)
{
  return __win_stat64(path, buffer, 1);
}

/**
 * @brief Get symbolic link status
 */
int _win_lstat64(const char *path, struct stat64 *buf)
{
  return __win_stat64(path, buf, 0);
}

/* end of stat.c */
