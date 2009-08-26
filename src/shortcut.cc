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
 * @file src/shortcut.cc
 * @brief symlink implementation for Windows
 */

#ifdef __MINGW32__
//define windows UUID manually because -luuid has no shared library (.DLL)
#define INITGUID
#include <basetyps.h>
DEFINE_OLEGUID(IID_IPersistFile, 0x0000010BL, 0, 0);
#endif

#include "plibc_private.h"

#include <shlobj.h>
#include <objbase.h>

extern "C" {

BOOL _plibc_CreateShortcut(const char *pszSrc, const char *pszDest)
{
    /* Create shortcut */
    IShellLink *pLink;
    IPersistFile *pFile;
    WCHAR *pwszDest;
    char *pszFileLnk;
    HRESULT hRes;
    
    CoInitialize(NULL);
    
    if ((strlen(pszSrc) > _MAX_PATH) || (strlen(pszDest) + 4 > _MAX_PATH))
    {
      CoUninitialize();
      errno = ENAMETOOLONG;
      
      return FALSE;
    }
    
    /* Create Shortcut-Object */
    if (CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,
        IID_IShellLink, (void **) &pLink) != S_OK)
    {
      CoUninitialize();
      errno = ESTALE;
      
      return FALSE;
    }
  
    /* Set target path */
    pLink->SetPath(pszSrc);
  
    /* Get File-Object */
    if (pLink->QueryInterface(IID_IPersistFile, (void **) &pFile) != S_OK)
    {
      free(pwszDest);
      pLink->Release();
      CoUninitialize();
      errno = ESTALE;
     
      return FALSE;
    }

    /* shortcuts have the extension .lnk */
    pszFileLnk = (char *) malloc(strlen(pszDest) + 5);
    sprintf(pszFileLnk, "%s.lnk", pszDest);
  
    /* Turn filename into widechars */
    pwszDest = (WCHAR *) malloc((_MAX_PATH + 5) * sizeof(WCHAR));
    MultiByteToWideChar(CP_ACP, 0, pszFileLnk, -1, pwszDest, _MAX_PATH);
    
    free(pszFileLnk);
    
    /* Save shortcut */
    if (FAILED(hRes = pFile->Save((LPCOLESTR) pwszDest, TRUE)))
    {
      free(pwszDest);
      pLink->Release();
      pFile->Release();
      CoUninitialize();
      SetErrnoFromHRESULT(hRes);
  
      return FALSE;
    }
  
    free(pwszDest);
    
    pFile->Release();
    pLink->Release();
    CoUninitialize();
    errno = 0;
      
    return TRUE;
}

BOOL _plibc_DereferenceShortcut(char *pszShortcut)
{
  IShellLink *pLink;
  IPersistFile *pFile;
  WCHAR *pwszShortcut;
  char *pszLnk;
  int iErr, iLen;
  HRESULT hRes;
  HANDLE hLink;
  char szTarget[_MAX_PATH + 1];

  if (! *pszShortcut)
    return TRUE;

  if (GetFileAttributes (pszShortcut) & (FILE_ATTRIBUTE_DEVICE | FILE_ATTRIBUTE_DIRECTORY))
    {
      errno = EINVAL;
      return FALSE;
    }

  CoInitialize(NULL);
  szTarget[0] = 0;
  
  /* Create Shortcut-Object */
  if (CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,
      IID_IShellLink, (void **) &pLink) != S_OK)
  {
    CoUninitialize();
    errno = ESTALE;
    
    return FALSE;
  }

  /* Get File-Object */
  if (pLink->QueryInterface(IID_IPersistFile, (void **) &pFile) != S_OK)
  {
    pLink->Release();
    CoUninitialize();
    errno = ESTALE;
    
    return FALSE;
  }

  pwszShortcut = (WCHAR *) malloc((_MAX_PATH + 1) * sizeof(WCHAR));

  /* Shortcuts have the extension .lnk
     If it isn't there, append it */
  iLen = strlen(pszShortcut);
  if (iLen > 4 && (strcmp(pszShortcut + iLen - 4, ".lnk") != 0))
  {
    HANDLE hLink;
    
    pszLnk = (char *) malloc(iLen + 5);
    sprintf(pszLnk, "%s.lnk", pszShortcut);
  }
  else
    pszLnk = strdup(pszShortcut);

  /* Make sure the path refers to a file */
  hLink = CreateFile(pszLnk, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
                   NULL, OPEN_EXISTING, 0, NULL);
  if (hLink == INVALID_HANDLE_VALUE)
  {
    free(pszLnk);
    SetErrnoFromWinError(GetLastError());
    
    if (errno == ENOENT)
    {
      /* There's no path with the ".lnk" extension.
         We don't quit here, because we have to decide whether the path doesn't
         exist or the path isn't a link. */

      /* Is it a directory? */
      if (GetFileAttributes(pszShortcut) & FILE_ATTRIBUTE_DIRECTORY)
      {
        errno = EINVAL;
        
        pLink->Release();
        pFile->Release();
        free(pwszShortcut);
        CoUninitialize();
        
        return FALSE;
      }

      pszLnk = strdup(pszShortcut);
      
      hLink = CreateFile(pszLnk, GENERIC_READ, FILE_SHARE_READ |
                FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
      SetErrnoFromWinError(GetLastError());      
    }
    else
    {
      pLink->Release();
      pFile->Release();
      free(pwszShortcut);
      CoUninitialize();
      
      return FALSE; /* File/link is there but unaccessible */
    }
  }
    
  MultiByteToWideChar(CP_ACP, 0, pszLnk, -1, pwszShortcut, _MAX_PATH);
  
  /* Open shortcut */
  if (FAILED(hRes = pFile->Load((LPCOLESTR) pwszShortcut, STGM_READ)))
  {
    pLink->Release();
    pFile->Release();
    free(pwszShortcut);
    CoUninitialize();
    
    /* For some reason, opening an invalid link sometimes fails with ACCESSDENIED.
       Since we have opened the file previously, insufficient priviledges
       are rather not the problem. */
    if (hRes == E_FAIL || hRes == E_ACCESSDENIED)
    {
      /* Check file magic */
      if (hLink != INVALID_HANDLE_VALUE)
      {
        DWORD dwRead;
        char pMagic[4] = {0, 0, 0, 0};
        
        ReadFile(hLink, pMagic, 4, &dwRead, NULL);
        if (memcmp(pMagic, "L\0\0\0", 4) == 0)
          SetErrnoFromHRESULT(hRes);
        else
          errno = EINVAL; /* No link */
      }
      /* else: errno was set above! */
    }
    else
      SetErrnoFromHRESULT(hRes);

    free(pszLnk);
          
    CloseHandle(hLink);
    return FALSE;
  }
  
  CloseHandle(hLink);
  free(pszLnk);
  free(pwszShortcut);
  
  /* Get target file */
  if (FAILED(hRes = pLink->GetPath(szTarget, _MAX_PATH, NULL, 0)))
  {
    pLink->Release();
    pFile->Release();
    CoUninitialize();
    
    if (hRes == E_FAIL)
      errno = EINVAL; /* Not a symlink */
    else
      SetErrnoFromHRESULT(hRes);
    
    return FALSE;
  }

  pFile->Release();
  pLink->Release();
  CoUninitialize();
  errno = 0;
  
  if (szTarget[0] != 0)
  {
  	strcpy(pszShortcut, szTarget);
  	return TRUE;
  }
  else
  {
    /* GetPath() did not return a valid path */
    errno = EINVAL;
    return FALSE;
  }
}

/**
 * @brief Dereference a symlink recursively
 */
int __win_deref(char *path)
{
  int iDepth = 0;

  errno = 0;

  while (_plibc_DereferenceShortcut(path))
  {
    if (iDepth++ > 10)
    {
      errno = ELOOP;
      return -1;
    }
  }

  if (iDepth != 0 && errno == EINVAL)
    errno = 0;

  return errno ? -1 : 0;
}

} // extern "C"

/* end of shortcut.cc */
