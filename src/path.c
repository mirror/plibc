/*
     This file is part of PlibC.
     (C) 2005, 2006, 2007, 2008 Nils Durner (and other contributing authors)

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
 * @file src/path.c
 * @brief Translation Unix pathnames -> Windows pathnames
 */

#include "plibc_private.h"

extern char szuRootDir[_MAX_PATH + 1];
extern long luRootDirLen;
extern char szuHomeDir[_MAX_PATH + 2];
extern long luHomeDirLen;
extern char szuDataDir[_MAX_PATH + 1];
extern long luDataDirLen;
extern char szuUser[261];
extern char *_pszuOrg;
extern char *_pszuApp;

extern wchar_t szRootDir[_MAX_PATH + 1];
extern long lRootDirLen;
extern wchar_t szHomeDir[_MAX_PATH + 2];
extern long lHomeDirLen;
extern wchar_t szDataDir[_MAX_PATH + 1];
extern long lDataDirLen;
extern wchar_t szUser[261];
extern wchar_t *_pwszOrg;
extern wchar_t *_pwszApp;


/**
 * @brief Determine the Windows path of our / directory
 * @internal
 * @return Error code from winerror.h, ERROR_SUCCESS on success
 */
long _plibc_DetermineRootDir()
{
  wchar_t wszModule[_MAX_PATH], wszDrv[_MAX_DRIVE], wszDir[_MAX_DIR];
  wchar_t *pwszBin;
  long lDirLen;

  /* Get the path of the calling module.
     It should be located in one of the "bin" directories */
  GetModuleFileNameW(NULL, wszModule, MAX_PATH);
  _wsplitpath(wszModule, wszDrv, wszDir, NULL, NULL);

  lDirLen = wcslen(wszDir);
  pwszBin = wszDir + lDirLen - 5;
  if (lDirLen < 5 || wcsnicmp(pwszBin, L"\\bin\\", 5) != 0)
  {
    wchar_t wszRegPath[251];

    /* Get the installation path from the registry */
    lDirLen = _MAX_PATH - 1;
    _win_snwprintf(wszRegPath, 250, L"Software\\%s\\%s", _pwszOrg, _pwszApp);
    wszRegPath[250] = 0;

    if(QueryRegistryW
       (HKEY_CURRENT_USER, wszRegPath, L"InstallDir",
        szRootDir, &lDirLen) != ERROR_SUCCESS)
    {
      lDirLen = _MAX_PATH - 1;

      if(QueryRegistryW
         (HKEY_LOCAL_MACHINE, wszRegPath, L"InstallDir",
          szRootDir, &lDirLen) != ERROR_SUCCESS)
      {
        if (GetCurrentDirectoryW(sizeof(szRootDir), szRootDir) == 0)
          return ERROR_BAD_ENVIRONMENT;
        lDirLen = wcslen(szRootDir);
      }
    }
    wcscat(szRootDir, L"\\");
    lRootDirLen = lDirLen + 1;
    wszDrv[0] = 0;
  }
  else
  {
  	pwszBin[1] = 0;
  	lDirLen -= 4;
  }

  if(wszDrv[0])
  {
    wcscpy(szRootDir, wszDrv);
    lRootDirLen = 3 + lDirLen - 1;      /* 3 = strlen(szDir) */
    if(lRootDirLen > _MAX_PATH)
      return ERROR_BUFFER_OVERFLOW;

    wcscat(szRootDir, wszDir);
  }

  if (plibc_utf8_mode() == 1)
    wchartostr_buf (szRootDir, szuRootDir, _MAX_PATH + 1, CP_UTF8);
  else
    wchartostr_buf (szRootDir, szuRootDir, _MAX_PATH + 1, CP_ACP);
  luRootDirLen = strlen (szuRootDir);
  return ERROR_SUCCESS;
}

/**
 * @brief Determine the user's home directory
 * @internal
 * @return Error code from winerror.h, ERROR_SUCCESS on success
*/
long _plibc_DetermineHomeDir()
{
  wchar_t *lpwszProfile = _wgetenv(L"USERPROFILE");
  if(lpwszProfile != NULL && lpwszProfile[0] != 0)        /* Windows NT */
  {
    lHomeDirLen = wcslen(lpwszProfile);
    if(lHomeDirLen + 1 > _MAX_PATH)
      return ERROR_BUFFER_OVERFLOW;

    wcscpy(szHomeDir, lpwszProfile);
    if(szHomeDir[lHomeDirLen - 1] != L'\\')
    {
      szHomeDir[lHomeDirLen] = L'\\';
      szHomeDir[++lHomeDirLen] = 0;
    }
  }
  else
  {
    /* C:\My Documents */
    long lRet;

    lHomeDirLen = _MAX_PATH;
    lRet = QueryRegistryW(HKEY_CURRENT_USER,
                         L"Software\\Microsoft\\Windows\\CurrentVersion\\"
                         L"Explorer\\Shell Folders",
                         L"Personal", szHomeDir, &lHomeDirLen);

    if(lRet == ERROR_BUFFER_OVERFLOW)
      return ERROR_BUFFER_OVERFLOW;
    else if(lRet == ERROR_SUCCESS)
    {
      /* lHomeDirLen includes \0 */
      if (lHomeDirLen <= _MAX_PATH)
        wcscat(szHomeDir, L"\\");
      else
        return ERROR_BUFFER_OVERFLOW;
    }
    else
    {
      /* C:\Program Files\GNUnet\home\... */
      /* 5 = strlen("home\\") */
      lHomeDirLen = wcslen(szRootDir) + wcslen(szUser) + 5 + 1;

      if(_MAX_PATH < lHomeDirLen)
        return ERROR_BUFFER_OVERFLOW;

      wcscpy(szHomeDir, szRootDir);
      wcscat(szHomeDir, L"home\\");
      wcscat(szHomeDir, szUser);
      wcscat(szHomeDir, L"\\");
    }
  }

  if (plibc_utf8_mode() == 1)
    wchartostr_buf (szHomeDir, szuHomeDir, _MAX_PATH + 1, CP_UTF8);
  else
    wchartostr_buf (szHomeDir, szuHomeDir, _MAX_PATH + 1, CP_ACP);
  luHomeDirLen = strlen (szuHomeDir);
  return ERROR_SUCCESS;
}

long _plibc_DetermineProgramDataDir()
{
  long lRet;
  
  lDataDirLen = _MAX_PATH;
  lRet = QueryRegistryW(HKEY_LOCAL_MACHINE,
                       L"Software\\Microsoft\\Windows\\CurrentVersion\\"
                       L"Explorer\\Shell Folders",
                       L"Common AppData", szDataDir, &lDataDirLen);

  lDataDirLen += wcslen(_pwszApp) + 1 + wcslen(_pwszOrg) + 1; 
  if (lRet == ERROR_BUFFER_OVERFLOW || lDataDirLen > _MAX_PATH)
  {
    return ERROR_BUFFER_OVERFLOW;
  }
  else if (lRet == ERROR_SUCCESS)
  {
    wcscat(szDataDir, L"\\");
    wcscat(szDataDir, _pwszOrg);
    wcscat(szDataDir, L"\\");
    wcscat(szDataDir, _pwszApp);
    wcscat(szDataDir, L"\\");
  }
  else
    wcscpy(szDataDir, szRootDir);
  
  if (plibc_utf8_mode() == 1)
    wchartostr_buf (szDataDir, szuDataDir, _MAX_PATH + 1, CP_UTF8);
  else
    wchartostr_buf (szDataDir, szuDataDir, _MAX_PATH + 1, CP_ACP);
  luDataDirLen = strlen (szuDataDir);
  return ERROR_SUCCESS;
}

/**
 * @brief Convert a POSIX-sytle path to a Windows-style path
 * @param pszUnix POSIX path
 * @param pszWindows Windows path
 * @return Error code from winerror.h, ERROR_SUCCESS on success
*/
int plibc_conv_to_win_path(const char *pszUnix, char *pszWindows)
{
  return plibc_conv_to_win_path_ex(pszUnix, pszWindows, 1);
}

int plibc_conv_to_win_pathw(const wchar_t *pszUnix, wchar_t *pszWindows)
{
  return plibc_conv_to_win_pathw_ex(pszUnix, pszWindows, 1);
}

int plibc_conv_to_win_pathwconv(const char *pszUnix, wchar_t *pszWindows)
{
  wchar_t *pwszUnix;
  int r;
  r = strtowchar (pszUnix, &pwszUnix, CP_UTF8);
  if (r < 0)
    return r;
  r = plibc_conv_to_win_pathw_ex(pwszUnix, pszWindows, 1);
  free (pwszUnix);
  return r;
}

int plibc_conv_to_win_pathwconv_ex(const char *pszUnix, wchar_t *pszWindows, int derefLinks)
{
  wchar_t *pwszUnix;
  int r;
  r = strtowchar (pszUnix, &pwszUnix, CP_UTF8);
  if (r < 0)
    return r;
  r = plibc_conv_to_win_pathw_ex(pwszUnix, pszWindows, derefLinks);
  free (pwszUnix);
  return r;
}

/**
 * @brief Convert a POSIX-sytle path to a Windows-style path
 * @param pszUnix POSIX path
 * @param pszWindows Windows path
 * @param derefLinks 1 to dereference links
 * @return Error code from winerror.h, ERROR_SUCCESS on success
*/
int plibc_conv_to_win_pathw_ex(const wchar_t *pszUnix, wchar_t *pszWindows, int derefLinks)
{
  wchar_t *pSrc, *pDest;
  long iSpaceUsed;
  int iUnixLen;

  if (!pszUnix || !pszWindows)
    return ERROR_INVALID_PARAMETER;

  iUnixLen = wcslen(pszUnix);

  /* Check if we already have a windows path */
  if((wcschr(pszUnix, L'\\') != NULL) || (wcschr(pszUnix, L':') != NULL))
  {
    if(iUnixLen > MAX_PATH)
      return ERROR_BUFFER_OVERFLOW;
    wcscpy(pszWindows, pszUnix);
  }

  /* Temp. dir? */
  if(wcsncmp(pszUnix, L"/tmp", 4) == 0)
  {
    iSpaceUsed = GetTempPathW(_MAX_PATH, pszWindows);
    if (iSpaceUsed > _MAX_PATH)
      return ERROR_BUFFER_OVERFLOW;
    pDest = pszWindows + iSpaceUsed;
    pSrc = (wchar_t *) pszUnix + 4;
  }
  /* Bit bucket? */
  else if (wcsncmp(pszUnix, L"/dev/null", 9) == 0)
  {
    wcscpy(pszWindows, L"nul");
    iSpaceUsed = 3;
    pDest = pszWindows + lHomeDirLen;
    pSrc = (wchar_t *) pszUnix + 9;
  }
  /* Data directories */
  else if (wcsncmp(pszUnix, L"/etc", 4) == 0 ||
    wcsncmp(pszUnix, L"/com", 4) == 0 ||
    wcsncmp(pszUnix, L"/var", 4) == 0)
  {
    wcscpy(pszWindows, szDataDir);
    iSpaceUsed = lDataDirLen;
    pDest = pszWindows + lDataDirLen;
    pSrc = (wchar_t *) pszUnix + 1;
  }
  /* Is the unix path a full path? */
  else if(pszUnix[0] == L'/')
  {
    wcscpy(pszWindows, szRootDir);
    iSpaceUsed = lRootDirLen;
    pDest = pszWindows + lRootDirLen;
    pSrc = (wchar_t *) pszUnix + 1;
  }
  /* Home dir? */
  else if (pszUnix[0] == L'~')
  {
    wcscpy(pszWindows, szHomeDir);
    iSpaceUsed = lHomeDirLen;
    pDest = pszWindows + lHomeDirLen;
    pSrc = (wchar_t *) pszUnix + 1;
  }
  /* Home dir (env var)? */
  else if (wcsncmp(pszUnix, L"$HOME", 5) == 0)
  {
    wcscpy(pszWindows, szHomeDir);
    iSpaceUsed = lHomeDirLen;
    pDest = pszWindows + lHomeDirLen;
    pSrc = (wchar_t *) pszUnix + 5;  	
  }
  else
  {
    pDest = pszWindows;
    iSpaceUsed = 0;
    pSrc = (wchar_t *) pszUnix;
  }

  iSpaceUsed += wcslen(pSrc);
  if(iSpaceUsed + 1 > _MAX_PATH)
    return ERROR_BUFFER_OVERFLOW;

  /* substitute all slashes */
  while(*pSrc)
  {
    if(*pSrc == L'/')
      *pDest = L'\\';
    else
      *pDest = *pSrc;

    pDest++;
    pSrc++;
  }
  *pDest = 0;

  if (derefLinks)
  {
    __win_derefw(pszWindows);
    errno = 0;
  }
  else
  {
    /* The filename possibly refers to a symlink, but the .lnk extension may be
       missing.
        1. Check if the requested file seems to be a normal file
        2. Check if the file exists
         2.1. Yes: Finished
         2.2. No: Check if "filename.lnk" exists
          2.2.1 Yes: Append ".lnk" */
    if (wcsnicmp(pDest - 4, L".lnk", 4) != 0)
    {
      HANDLE h = CreateFileW(pszWindows, 0, FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
      if (h == INVALID_HANDLE_VALUE)
      {
        /* File doesn't exist, try shortcut */
        wchar_t *pLnk;
        int mal;
        
        if (iSpaceUsed + 5 > _MAX_PATH)
        {
          pLnk = malloc((iSpaceUsed + 5) * sizeof (wchar_t));
          wcscpy(pLnk, pszWindows);
          mal = 1;
        }
        else
        {
          pLnk = pszWindows;
          mal = 0;
        }
        wcscat(pLnk, L".lnk");
        
        h = CreateFileW(pLnk, 0, FILE_SHARE_READ | FILE_SHARE_WRITE,
          NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
          
        if (h != INVALID_HANDLE_VALUE)
        {
          /* Shortcut exists */
          CloseHandle(h);
          if (mal)
            /* Need to copy */
            if (iSpaceUsed + 5 <= _MAX_PATH)
              wcscpy(pszWindows, pLnk);
            else
            {
              free(pLnk);
              return ERROR_BUFFER_OVERFLOW;
            }
        }
        else
          pLnk[iSpaceUsed] = 0;   
        
        if (mal)
          free(pLnk);
      }
      else
        CloseHandle(h);
    }
  }

#if DEBUG_WINPROC
	{
		char szInfo[1001];

  	_win_snprintf(szInfo, 1000, "Posix path %S resolved to %S\n", pszUnix,
  		pszWindows);
  	szInfo[1000] = 0;
  	__plibc_panic(INT_MAX, szInfo);
	}
#endif

  return ERROR_SUCCESS;
}

int plibc_conv_to_win_path_ex(const char *pszUnix, char *pszWindows, int derefLinks)
{
  char *pSrc, *pDest;
  long iSpaceUsed;
  int iUnixLen;

  if (!pszUnix || !pszWindows)
    return ERROR_INVALID_PARAMETER;

  iUnixLen = strlen(pszUnix);

  /* Check if we already have a windows path */
  if((strchr(pszUnix, '\\') != NULL) || (strchr(pszUnix, ':') != NULL))
  {
    if(iUnixLen > MAX_PATH)
      return ERROR_BUFFER_OVERFLOW;
    strcpy(pszWindows, pszUnix);
  }

  /* Temp. dir? */
  if(strncmp(pszUnix, "/tmp", 4) == 0)
  {
    iSpaceUsed = GetTempPath(_MAX_PATH, pszWindows);
    if (iSpaceUsed > _MAX_PATH)
      return ERROR_BUFFER_OVERFLOW;
    pDest = pszWindows + iSpaceUsed;
    pSrc = (char *) pszUnix + 4;
  }
  /* Bit bucket? */
  else if (strncmp(pszUnix, "/dev/null", 9) == 0)
  {
    strcpy(pszWindows, "nul");
    iSpaceUsed = 3;
    pDest = pszWindows + luHomeDirLen;
    pSrc = (char *) pszUnix + 9;
  }
  /* Data directories */
  else if (strncmp(pszUnix, "/etc", 4) == 0 ||
    strncmp(pszUnix, "/com", 4) == 0 ||
    strncmp(pszUnix, "/var", 4) == 0)
  {
    strcpy(pszWindows, szuDataDir);
    iSpaceUsed = luDataDirLen;
    pDest = pszWindows + luDataDirLen;
    pSrc = (char *) pszUnix + 1;
  }
  /* Is the unix path a full path? */
  else if(pszUnix[0] == '/')
  {
    strcpy(pszWindows, szuRootDir);
    iSpaceUsed = luRootDirLen;
    pDest = pszWindows + luRootDirLen;
    pSrc = (char *) pszUnix + 1;
  }
  /* Home dir? */
  else if (pszUnix[0] == '~')
  {
    strcpy(pszWindows, szuHomeDir);
    iSpaceUsed = luHomeDirLen;
    pDest = pszWindows + luHomeDirLen;
    pSrc = (char *) pszUnix + 1;
  }
  /* Home dir (env var)? */
  else if (strncmp(pszUnix, "$HOME", 5) == 0)
  {
    strcpy(pszWindows, szuHomeDir);
    iSpaceUsed = luHomeDirLen;
    pDest = pszWindows + luHomeDirLen;
    pSrc = (char *) pszUnix + 5;  	
  }
  else
  {
    pDest = pszWindows;
    iSpaceUsed = 0;
    pSrc = (char *) pszUnix;
  }

  iSpaceUsed += strlen(pSrc);
  if(iSpaceUsed + 1 > _MAX_PATH)
    return ERROR_BUFFER_OVERFLOW;

  /* substitute all slashes */
  while(*pSrc)
  {
    if(*pSrc == '/')
      *pDest = '\\';
    else
      *pDest = *pSrc;

    pDest++;
    pSrc++;
  }
  *pDest = 0;

  if (derefLinks)
    __win_deref(pszWindows);
  else
  {
    /* The filename possibly refers to a symlink, but the .lnk extension may be
       missing.
        1. Check if the requested file seems to be a normal file
        2. Check if the file exists
         2.1. Yes: Finished
         2.2. No: Check if "filename.lnk" exists
          2.2.1 Yes: Append ".lnk" */
    if (strnicmp(pDest - 4, ".lnk", 4) != 0)
    {
      HANDLE h = CreateFile(pszWindows, 0, FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
      if (h == INVALID_HANDLE_VALUE)
      {
        /* File doesn't exist, try shortcut */
        char *pLnk;
        int mal;
        
        if (iSpaceUsed + 5 > _MAX_PATH)
        {
          pLnk = malloc(iSpaceUsed + 5);
          strcpy(pLnk, pszWindows);
          mal = 1;
        }
        else
        {
          pLnk = pszWindows;
          mal = 0;
        }
        strcat(pLnk, ".lnk");
        
        h = CreateFile(pLnk, 0, FILE_SHARE_READ | FILE_SHARE_WRITE,
          NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
          
        if (h != INVALID_HANDLE_VALUE)
        {
          /* Shortcut exists */
          CloseHandle(h);
          if (mal)
            /* Need to copy */
            if (iSpaceUsed + 5 <= _MAX_PATH)
              strcpy(pszWindows, pLnk);
            else
            {
              free(pLnk);
              return ERROR_BUFFER_OVERFLOW;
            }
        }
        else
          pLnk[iSpaceUsed] = 0;   
        
        if (mal)
          free(pLnk);
      }
      else
        CloseHandle(h);
    }
  }

#if DEBUG_WINPROC
	{
		char szInfo[1001];

  	_win_snprintf(szInfo, 1000, "Posix path %s resolved to %s\n", pszUnix,
  		pszWindows);
  	szInfo[1000] = 0;
  	__plibc_panic(INT_MAX, szInfo);
	}
#endif

  return ERROR_SUCCESS;
}

/* end of path.c */
