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
 * @file src/truncate.c
 * @brief truncate()
 */

#include "plibc_private.h"

/**
 * @author Prof. A Olowofoyeku (The African Chief)
 * @author Frank Heckenbach
 * @author Nils Durner
 * @see http://gd.tuwien.ac.at/gnu/mingw/os-hacks.h
 */

#if !defined(HAVE_FTRUNCATE)
int truncate(const char *fname, int distance)
{
  int i;
  HANDLE hFile;
  wchar_t pszFile[_MAX_PATH + 1];
  long lRet;

  errno = 0;

  if (plibc_utf8_mode() == 1)
    lRet = plibc_conv_to_win_pathwconv(fname, pszFile);
  else
    lRet = plibc_conv_to_win_path(fname, (char *) pszFile);
  if (lRet != ERROR_SUCCESS)
  {
    SetErrnoFromWinError(lRet);
    return -1;
  }

  i = -1;
  if (plibc_utf8_mode() == 1)
    hFile = CreateFileW(pszFile, GENERIC_READ | GENERIC_WRITE,
                     FILE_SHARE_READ | FILE_SHARE_WRITE,
                     NULL, OPEN_EXISTING,
                     FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, NULL);
  else
    hFile = CreateFile((char *) pszFile, GENERIC_READ | GENERIC_WRITE,
                     FILE_SHARE_READ | FILE_SHARE_WRITE,
                     NULL, OPEN_EXISTING,
                     FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, NULL);

  if(hFile != INVALID_HANDLE_VALUE)
  {
    if(SetFilePointer(hFile, distance, NULL, FILE_BEGIN) != 0xFFFFFFFF)
    {
      if(SetEndOfFile(hFile))
        i = 0;
    }
    CloseHandle(hFile);
  }

  return i;
}
#endif

int _win_ftruncate(int fildes, off_t length)
{
  return chsize(fildes, length);
}

/* end of truncate.c */
