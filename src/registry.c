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
 * @file src/registry.c
 * @brief Helper functions for the Windows registry
 */

#include "plibc_private.h"

/**
 * @brief Get information from the Windows registry
 * @param hMainKey handle to (main-)key to query (HKEY_LOCAL_MACHINE etc.)
 * @param pszKey name of key to query
 * @param pszSubKey name of subkey to query
 * @param pszBuffer buffer for returned string
 * @param pdLength receives size of returned string
 * @return Error code from winerror.h, ERROR_SUCCESS on success
 */
long QueryRegistry(HKEY hMainKey, char *pszKey, char *pszSubKey,
              char *pszBuffer, long *pdLength)
{
  HKEY hKey;
  long lRet;
  long lBufferSize;

  lBufferSize = *pdLength;
  if((lRet = RegOpenKeyEx(hMainKey, pszKey, 0, KEY_EXECUTE, &hKey)) ==
     ERROR_SUCCESS)
  {
    lRet = RegQueryValueEx(hKey, pszSubKey, 0, NULL, pszBuffer, pdLength);
    if (lRet == ERROR_SUCCESS)
    {
      if (pszBuffer[*pdLength - 1] != 0)
      {
        if (*pdLength < lBufferSize)
          pszBuffer[*pdLength] = 0;
        else
          lRet = ERROR_MORE_DATA;
      }
      else
        *pdLength = strlen(pszBuffer);
    }

    RegCloseKey(hKey);
  }

  return lRet;
}

/* end of registry.c */
