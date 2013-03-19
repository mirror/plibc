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
 * @file src/fopen.c
 * @brief fopen()
 */

#include "plibc_private.h"

FILE *_win_wfopen(const wchar_t *filename, const wchar_t *wmode)
{
  wchar_t szFile[_MAX_PATH + 1];

  FILE *hFile;
  int i;

  if ((i = plibc_conv_to_win_pathw(filename, szFile)) != ERROR_SUCCESS)
  {
    SetErrnoFromWinError(i);
    return NULL;
  }

  hFile = _wfopen(szFile, wmode);
  SetErrnoFromWinError(GetLastError());

  return hFile;
}

/**
 * Open a file
 */
FILE *
_win_fopen (const char *filename, const char *mode)
{
  wchar_t *wfilename = NULL;
  wchar_t *wmode = NULL;
  int r;
  int e;
  FILE *result;
  if (mode != NULL)
  {
    if (plibc_utf8_mode() == 1)
      r = strtowchar (mode, &wmode, CP_UTF8);
    else
      r = strtowchar (mode, &wmode, CP_ACP);
    if (r < 0)
      return NULL;
  }
  if (filename != NULL)
  {
    if (plibc_utf8_mode() == 1)
      r = strtowchar (filename, &wfilename, CP_UTF8);
    else
      r = strtowchar (filename, &wfilename, CP_ACP);
    if (r < 0)
    {
      if (wmode)
        free (wmode);
      return NULL;
    }
  }
  result = _win_wfopen (wfilename, wmode);
  e = errno;
  if (wmode)
    free (wmode);
  if (wfilename)
    free (wfilename);
  errno = e;
  return result;
}

/* end of fopen.c */
