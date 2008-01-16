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
 * @file src/creat.c
 * @brief creat()
 */

#include "plibc_private.h"

/**
 * create a new file or rewrite an existing one
 */
int _win_creat(const char *path, mode_t mode)
{
  char szFile[_MAX_PATH + 1];
  long lRet;
  int iFD;

  if ((lRet = plibc_conv_to_win_path(path, szFile)) != ERROR_SUCCESS)
  {
    SetErrnoFromWinError(lRet);
    return -1;
  }

  /* _creat sets errno */
  iFD = _creat(szFile, mode);
  __win_SetHandleType((DWORD) iFD, FD_HANDLE);
  
  return iFD;
}

/* end of creat.c */
