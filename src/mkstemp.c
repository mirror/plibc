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
 * @file src/mkstemp.c
 * @brief mkstemp()
 */

#include "plibc_private.h"

int mkstemp(char *tmplate)
{
  static const char letters[]
    = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  int iLen, iRnd, bExists, iRet;
  char *pChr;

  errno = 0;
  bExists = 1;

  iLen = strlen(tmplate);
  pChr = tmplate + iLen - 6;
  
  if (iLen < 6 || strncmp(pChr, "XXXXXX", 6) != 0)
  {
    errno = EINVAL;
    return -1;
  }

  while (bExists)
  {
    int iChr;

    for(iChr = 0; iChr < 6; iChr++)
    {
      /* 528.5 = RAND_MAX / letters */
      iRnd = rand() / 528.5;
      *(pChr++) = letters[iRnd > 0 ? iRnd - 1 : 0];
    }
 
    iRet = _win_open(tmplate, _O_CREAT | _O_EXCL | _O_RDWR, _S_IREAD | _S_IWRITE);
    
    if (iRet == -1 && errno == EEXIST)
      pChr = tmplate + iLen - 6;
    else
      bExists = 0;
  }
  
  return iRet;
}

/* end of mkstemp.c */
