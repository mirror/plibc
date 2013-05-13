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
 * @file src/shortcut.c
 * @brief symlink implementation for Windows
 */

#include <objidl.h>

#include "plibc_private.h"

#include <shlguid.h>
#include <shlobj.h>
#include <objbase.h>

BOOL
_plibc_CreateShortcutW(const wchar_t *pwszSrc, const wchar_t *pwszDest)
{
    /* Create shortcut */
    IShellLinkW *pLink;
    IPersistFile *pFile;
    wchar_t *pwszFileLnk;
    HRESULT hRes;

    CoInitialize(NULL);
    
    if ((wcslen(pwszSrc) > _MAX_PATH) || (wcslen(pwszDest) + 4 > _MAX_PATH))
    {
      CoUninitialize();
      errno = ENAMETOOLONG;
      
      return FALSE;
    }
    
    /* Create Shortcut-Object */
    if (CoCreateInstance(&CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,
        &IID_IShellLink, (void **) &pLink) != S_OK)
    {
      CoUninitialize();
      errno = ESTALE;
      
      return FALSE;
    }
  
    /* Set target path */
    pLink->lpVtbl->SetPath(pLink, pwszSrc);
  
    /* Get File-Object */
    if (pLink->lpVtbl->QueryInterface(pLink, &IID_IPersistFile, (void **) &pFile) != S_OK)
    {
      pLink->lpVtbl->Release(pLink);
      CoUninitialize();
      errno = ESTALE;
     
      return FALSE;
    }

    /* shortcuts have the extension .lnk */
    pwszFileLnk = (wchar_t *) malloc((wcslen(pwszDest) + 5) * sizeof (wchar_t));
    swprintf(pwszFileLnk, L"%s.lnk", pwszDest);
  
    /* Save shortcut */
    hRes = pFile->lpVtbl->Save(pFile, (LPCOLESTR) pwszFileLnk, TRUE);
    free(pwszFileLnk);
    pLink->lpVtbl->Release(pLink);
    pFile->lpVtbl->Release(pFile);
    CoUninitialize();
    if (FAILED(hRes))
    {
      SetErrnoFromHRESULT(hRes);
      return FALSE;
    }

    errno = 0;
    return TRUE;
}

BOOL
_plibc_CreateShortcut(const char *pszSrc, const char *pszDest)
{
  wchar_t *pwszSrc = NULL, *pwszDest = NULL;
  int r;
  BOOL result;
  int e;
  if (pszSrc != NULL)
  {
    if (plibc_utf8_mode() == 1)
      r = strtowchar (pszSrc, &pwszSrc, CP_UTF8);
    else
      r = strtowchar (pszSrc, &pwszSrc, CP_ACP);
    if (r < 0)
      return FALSE;
  }
  if (pszDest != NULL)
  {
    if (plibc_utf8_mode() == 1)
      r = strtowchar (pszDest, &pwszDest, CP_UTF8);
    else
      r = strtowchar (pszDest, &pwszDest, CP_ACP);
    if (r < 0)
    {
      if (pwszSrc)
        free (pwszSrc);
      return FALSE;
    }
  }
  result = _plibc_CreateShortcutW (pwszSrc, pwszDest);
  e = errno;
  if (pwszSrc)
    free (pwszSrc);
  if (pwszDest)
    free (pwszDest);
  errno = e;
  return result;
}

BOOL
_plibc_DereferenceShortcutW(wchar_t *pwszShortcut)
{
  IShellLinkW *pLink;
  wchar_t *pwszLnk;
  wchar_t szTarget[_MAX_PATH + 1];
  IPersistFile *pFile;
  int iLen;
  HRESULT hRes;
  HANDLE hLink;

  if (! *pwszShortcut)
    return TRUE;

  if (GetFileAttributesW (pwszShortcut) & (FILE_ATTRIBUTE_DEVICE | FILE_ATTRIBUTE_DIRECTORY))
  {
    errno = EINVAL;
    return FALSE;
  }

  CoInitialize(NULL);
  szTarget[0] = 0;
  
  /* Create Shortcut-Object */
  if (CoCreateInstance(&CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,
      &IID_IShellLink, (void **) &pLink) != S_OK)
  {
    CoUninitialize();
    errno = ESTALE;
    
    return FALSE;
  }

  /* Get File-Object */
  if (pLink->lpVtbl->QueryInterface(pLink, &IID_IPersistFile, (void **) &pFile) != S_OK)
  {
    pLink->lpVtbl->Release(pLink);
    CoUninitialize();
    errno = ESTALE;
    
    return FALSE;
  }

  /* Shortcuts have the extension .lnk
     If it isn't there, append it */
  iLen = wcslen(pwszShortcut);
  if (iLen > 4 && (wcscmp(pwszShortcut + iLen - 4, L".lnk") != 0))
  {
    pwszLnk = (wchar_t *) malloc((iLen + 5) * sizeof (wchar_t));
    swprintf(pwszLnk, L"%s.lnk", pwszShortcut);
  }
  else
    pwszLnk = wcsdup(pwszShortcut);

  /* Make sure the path refers to a file */
  hLink = CreateFileW(pwszLnk, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
                   NULL, OPEN_EXISTING, 0, NULL);
  if (hLink == INVALID_HANDLE_VALUE)
  {
    free (pwszLnk);
    SetErrnoFromWinError(GetLastError());
    
    if (errno == ENOENT)
    {
      /* There's no path with the ".lnk" extension.
         We don't quit here, because we have to decide whether the path doesn't
         exist or the path isn't a link. */

      /* Is it a directory? */
      if (GetFileAttributesW(pwszShortcut) & FILE_ATTRIBUTE_DIRECTORY)
      {
        errno = EINVAL;
        
        pLink->lpVtbl->Release(pLink);
        pFile->lpVtbl->Release(pFile);
        CoUninitialize();
        
        return FALSE;
      }

      pwszLnk = wcsdup(pwszShortcut);
      
      hLink = CreateFileW(pwszLnk, GENERIC_READ, FILE_SHARE_READ |
                FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
      SetErrnoFromWinError(GetLastError());
    }
    else
    {
      pLink->lpVtbl->Release(pLink);
      pFile->lpVtbl->Release(pFile);
      CoUninitialize();
      
      return FALSE; /* File/link is there but unaccessible */
    }
  }
  
  /* Open shortcut */
  hRes = pFile->lpVtbl->Load(pFile, (LPCOLESTR) pwszLnk, STGM_READ);
  free (pwszLnk);
  if (FAILED(hRes))
  {
    pLink->lpVtbl->Release(pLink);
    pFile->lpVtbl->Release(pFile);
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
          
    CloseHandle(hLink);
    return FALSE;
  }
  
  CloseHandle(hLink);
  
  /* Get target file */
  if (FAILED(hRes = pLink->lpVtbl->GetPath(pLink, szTarget, _MAX_PATH, NULL, 0)))
  {
    pLink->lpVtbl->Release(pLink);
    pFile->lpVtbl->Release(pFile);
    CoUninitialize();
    
    if (hRes == E_FAIL)
      errno = EINVAL; /* Not a symlink */
    else
      SetErrnoFromHRESULT(hRes);
    
    return FALSE;
  }

  pFile->lpVtbl->Release(pFile);
  pLink->lpVtbl->Release(pLink);
  CoUninitialize();
  errno = 0;
  
  if (szTarget[0] != 0)
  {
  	wcscpy(pwszShortcut, szTarget);
  	return TRUE;
  }
  else
  {
    /* GetPath() did not return a valid path */
    errno = EINVAL;
    return FALSE;
  }
}

BOOL _plibc_DereferenceShortcut(char *pszShortcut)
{
  WCHAR pwszShortcut[_MAX_PATH + 1];
  int r;
  int e;
  BOOL result;
  if (plibc_utf8_mode() == 1)
    r = strtowchar_buf (pszShortcut, pwszShortcut, _MAX_PATH, CP_UTF8);
  else
    r = strtowchar_buf (pszShortcut, pwszShortcut, _MAX_PATH, CP_ACP);
  if (r < 0)
    return FALSE;
  result = _plibc_DereferenceShortcutW(pwszShortcut);
  if (result == FALSE)
    return FALSE;
  e = errno;
  if (plibc_utf8_mode() == 1)
    r = wchartostr_buf (pwszShortcut, pszShortcut, _MAX_PATH, CP_UTF8);
  else
    r = wchartostr_buf (pwszShortcut, pszShortcut, _MAX_PATH, CP_ACP);
  if (r < 0)
    return FALSE;
  errno = e;
  return TRUE;
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

int __win_derefw(wchar_t *path)
{
  int iDepth = 0;

  errno = 0;

  while (_plibc_DereferenceShortcutW(path))
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

/* end of shortcut.c */
