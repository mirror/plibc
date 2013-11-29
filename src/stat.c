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

typedef int (*statptr)(void *name, struct stat *s);

struct stat_desc
{
  uint8_t bWide;
  uint8_t iTimeSize;
  uint8_t iFileSize;
  statptr ptr;
};

/**
 * @brief Get status information on a file
 */
int __win_stat(const char *path, struct stat *buffer, int iDeref)
{
  wchar_t szFile[_MAX_PATH + 1];
  long lRet;
  uint8_t bWideChar;
  struct stat_desc stats[] =
       {
           {0, 4, 4, (statptr) _stat32},
           {1, 4, 4, (statptr) _wstat32},
           {0, 8, 8, (statptr) _stat64},
           {1, 8, 8, (statptr) _wstat64}
#if HAVE_DECL__WSTAT32I64
           ,
           {0, 4, 8, (statptr) _stat32i64},
           {1, 4, 8, (statptr) _wstat32i64},
           {0, 8, 4, (statptr) _stat64i32},
           {1, 8, 4, (statptr) _wstat64i32}
#endif
       };
  struct stat_desc *pIdx, *pEnd;
  statptr pStat;

  bWideChar = plibc_utf8_mode();
  if (bWideChar == 1)
    lRet = plibc_conv_to_win_pathwconv(path, szFile);
  else
    lRet = plibc_conv_to_win_path(path, (char *) szFile);
  if (lRet != ERROR_SUCCESS)
  {
    SetErrnoFromWinError(lRet);
    return -1;
  }

  /* Remove trailing slash */
  if (bWideChar == 1)
  {
    lRet = wcslen(szFile) - 1;
    if (szFile[lRet] == L'\\')
      szFile[lRet] = L'\0';
  }
  else
  {
    lRet = strlen(((char *) szFile)) - 1;
    if (((char *) szFile)[lRet] == '\\')
      ((char *) szFile)[lRet] = 0;
  }

  /* Dereference symlinks */
  if (iDeref)
  {
    if (bWideChar)
    {
      if (__win_derefw(szFile) == -1 && errno != EINVAL)
        return -1;
    }
    else
    {
      if (__win_deref((char *) szFile) == -1 && errno != EINVAL)
        return -1;
    }
  }

  /* choose the right stat */
  if (_plibc_stat_lengthSize != 0 && _plibc_stat_timeSize != 0)
  {
    pStat = NULL;
    for (pIdx = stats, pEnd = stats + (sizeof(stats) / sizeof(struct stat_desc)); pIdx < pEnd; pIdx++)
    {
      if (pIdx->bWide == bWideChar && pIdx->iFileSize == _plibc_stat_lengthSize &&
          pIdx->iTimeSize == _plibc_stat_timeSize)
      {
        pStat = pIdx->ptr;
        break;
      }
    }

    if (!pStat)
    {
      errno = EINVAL;
      return -1;
    }
  }
  else
  {
    if (bWideChar)
      pStat = (statptr) _wstat;
    else
      pStat = (statptr) stat;
  }

  /* stat sets errno */
  return pStat((void *) szFile, buffer);
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
int __win_stati64(const char *path, struct _stati64 *buffer, int iDeref)
{
  wchar_t szFile[_MAX_PATH + 1];
  long lRet;

  if (plibc_utf8_mode() == 1)
    lRet = plibc_conv_to_win_pathwconv(path, szFile);
  else
    lRet = plibc_conv_to_win_path(path, (char *) szFile);
  if (lRet != ERROR_SUCCESS)
  {
    SetErrnoFromWinError(lRet);
    return -1;
  }

  /* Remove trailing slash */
  if (plibc_utf8_mode() == 1)
  {
    lRet = wcslen(szFile) - 1;
    if (szFile[lRet] == L'\\')
      szFile[lRet] = L'\0';
  }
  else
  {
    lRet = strlen((char *) szFile) - 1;
    if (((char *) szFile)[lRet] == '\\')
      ((char *) szFile)[lRet] = 0;
  }

  /* Dereference symlinks */
  if (iDeref)
  {
    if (plibc_utf8_mode() == 1)
    {
      if (__win_derefw(szFile) == -1 && errno != EINVAL)
        return -1;
    }
    else
    {
      if (__win_deref((char *) szFile) == -1 && errno != EINVAL)
        return -1;
    }
  }

  if (plibc_utf8_mode () == 1 ? !_plibc_wstati64 : !_plibc_stati64)
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
  {
    /* stat sets errno */
    if (plibc_utf8_mode() == 1)
      return _plibc_wstati64(szFile, buffer);
    else
      return _plibc_stati64((char *) szFile, buffer);
  }
}

/**
 * @brief Get status information on a file
 */
int _win_stati64(const char *path, struct _stati64 *buffer)
{
  return __win_stati64(path, buffer, 1);
}

/**
 * @brief Get symbolic link status
 */
int _win_lstati64(const char *path, struct _stati64 *buf)
{
  return __win_stati64(path, buf, 0);
}

/* end of stat.c */
