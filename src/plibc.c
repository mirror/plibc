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
 * @file src/plibc.c
 * @brief Core of PlibC
 */

#include "plibc_private.h"
#include "plibc_strconv.h"

#define DEBUG_WINPROC 0

wchar_t szRootDir[_MAX_PATH + 1] = L"";
long lRootDirLen;
char szuRootDir[_MAX_PATH + 1] = "";
long luRootDirLen;
wchar_t szDataDir[_MAX_PATH + 1] = L"";
long lDataDirLen;
char szuDataDir[_MAX_PATH + 1] = "";
long luDataDirLen;
wchar_t szHomeDir[_MAX_PATH + 2] = L"";
long lHomeDirLen;
char szuHomeDir[_MAX_PATH + 2] = "";
long luHomeDirLen;
wchar_t szUser[261] = L"";
char szuUser[261] = "";
wchar_t *_pwszOrg = NULL, *_pwszApp = NULL;
char *_pszuOrg = NULL, *_pszuApp = NULL;
OSVERSIONINFO theWinVersion;
unsigned int uiSockCount = 0;
Winsock *pSocks = NULL;
HANDLE hSocksLock, hHandlesLock;
unsigned int uiMappingsCount = 0;
unsigned int uiHandlesCount = 0;
TMapping *pMappings = NULL;
THandleInfo *pHandles = NULL;
HANDLE hMappingsLock;
TPanicProc __plibc_panic = NULL;
int iInit = 0;
HMODULE hMsvcrt = NULL;
TStat64 _plibc_stat64 = NULL;
TWStat64 _plibc_wstat64 = NULL;
static int _plibc_utf8_mode = 0;
int plibc_utf8_mode() { return _plibc_utf8_mode; }

static HINSTANCE hIphlpapi, hAdvapi;

unsigned plibc_get_handle_count()
{
  return uiHandlesCount;
}

BOOL __win_IsHandleMarkedAsBlocking(int hHandle)
{
  BOOL bBlocking;
  unsigned int uiIndex;

  bBlocking = TRUE;
  WaitForSingleObject(hSocksLock, INFINITE);
  for(uiIndex = 0; uiIndex <= uiSockCount; uiIndex++)
  {
    if (pSocks[uiIndex].s == hHandle)
    {
      bBlocking = pSocks[uiIndex].bBlocking;
      break;
    }
  }
  ReleaseMutex(hSocksLock);

  return bBlocking;
}

void __win_SetHandleBlockingMode(int s, BOOL bBlocking)
{
  unsigned int uiIndex = 0;
  int bFound = 0;

  WaitForSingleObject(hSocksLock, INFINITE);

  for(uiIndex = 0; uiIndex <= uiSockCount; uiIndex++)
  {
    if (pSocks[uiIndex].s == s)
    {
      bFound = 1;
      break;
    }
  }

  if (bFound)
    pSocks[uiIndex].bBlocking = bBlocking;
  else
  {
    uiIndex = 0;

    while(TRUE)
    {
      int iSet = 0;

      if (pSocks[uiIndex].s == -1)
      {
        pSocks[uiIndex].s = s;
        pSocks[uiIndex].bBlocking = bBlocking;
        iSet = 1;
      }
      if (uiIndex == uiSockCount)
      {
        uiSockCount++;
        pSocks = (Winsock *) realloc(pSocks, (uiSockCount + 1) * sizeof(Winsock));
        pSocks[uiSockCount].s = -1;

        break;
      }

      if (iSet)
        break;

      uiIndex++;
    }
  }
  ReleaseMutex(hSocksLock);
}

void __win_DiscardHandleBlockingMode(int s)
{
  unsigned int uiIndex;

  WaitForSingleObject(hSocksLock, INFINITE);
  for(uiIndex = 0; uiIndex <= uiSockCount; uiIndex++)
    if (pSocks[uiIndex].s == s)
      pSocks[uiIndex].s = -1;
  ReleaseMutex(hSocksLock);
}

THandleType __win_GetHandleType(DWORD dwHandle)
{
  THandleType eType;
  unsigned int uiIndex;

  eType = UNKNOWN_HANDLE;
  WaitForSingleObject(hHandlesLock, INFINITE);
  for(uiIndex = 0; uiIndex <= uiHandlesCount; uiIndex++)
  {
    if (pHandles[uiIndex].dwHandle == dwHandle)
    {
      eType = pHandles[uiIndex].eType;
      break;
    }
  }
  ReleaseMutex(hHandlesLock);

  return eType;
}

void __win_SetHandleType(DWORD dwHandle, THandleType eType)
{
  unsigned int uiIndex = 0;
  int bFound = 0;

  WaitForSingleObject(hHandlesLock, INFINITE);

  for(uiIndex = 0; uiIndex <= uiHandlesCount; uiIndex++)
  {
    if (pHandles[uiIndex].dwHandle == dwHandle)
    {
      bFound = 1;
      break;
    }
  }

  if (bFound)
    pHandles[uiIndex].eType = eType;
  else
  {
    uiIndex = 0;

    while(TRUE)
    {
      int iSet = 0;

      if (pHandles[uiIndex].dwHandle == 0)
      {
        pHandles[uiIndex].dwHandle = dwHandle;
        pHandles[uiIndex].eType = eType;
        iSet = 1;
      }
      if (uiIndex == uiHandlesCount)
      {
        uiHandlesCount++;
        pHandles = (THandleInfo *) realloc(pHandles, (uiHandlesCount + 1) * sizeof(THandleInfo));
        pHandles[uiHandlesCount].dwHandle = 0;

        break;
      }

      if (iSet)
        break;

      uiIndex++;
    }
  }
  ReleaseMutex(hHandlesLock);
}

void __win_DiscardHandleType(DWORD dwHandle)
{
  unsigned int uiIndex;

  WaitForSingleObject(hHandlesLock, INFINITE);
  for(uiIndex = 0; uiIndex <= uiHandlesCount; uiIndex++)
    if (pHandles[uiIndex].dwHandle == dwHandle)
      pHandles[uiIndex].dwHandle = 0;
  ReleaseMutex(hHandlesLock);
}

/**
 * Check if socket is valid
 * @return 1 if valid, 0 otherwise
 */
int _win_isSocketValid(int s)
{
  long l;
  return ioctlsocket((SOCKET) s, FIONREAD, &l) != SOCKET_ERROR && _get_osfhandle(s) == -1;
}

/**
 * @brief Default panic proc
 * @internal
 */
void __plibc_panic_default(int err, char *szMsg)
{
#if DEBUG_WINPROC
	if(err == INT_MAX)
		fputs(stderr, szMsg);
#endif
}

/**
 * @brief Checks whether PlibC is already initialized
 * @note This is useful if you have to do additional initializations on Win32
 *       in independent modules
 * @returns 1 if initialized, 0 otherwise
 */
int plibc_initialized()
{
  return iInit > 0;
}

/**
 * @brief Initialize POSIX emulation and set up Windows environment
 * @param pszOrg Organisation ("GNU" for GNU projects)
 * @param pszApp Application title
 * @return Error code from winerror.h, ERROR_SUCCESS on success
 * @note Example: plibc_init("My Company", "My Application");
*/
int plibc_init(char *pszOrg, char *pszApp)
{
  return plibc_init_utf8 (pszOrg, pszApp, 0);
}

/**
 * @brief Initialize POSIX emulation and set up Windows environment
 * @param pszOrg Organisation ("GNU" for GNU projects)
 * @param pszApp Application title
 * @param utf8_mode 1 to enable automatic UTF-8 conversion, 0 to use system CP
 * @return Error code from winerror.h, ERROR_SUCCESS on success
 * @note Example: plibc_init("My Company", "My Application", 1);
*/
int plibc_init_utf8(char *pszOrg, char *pszApp, int utf8_mode)
{
  long lRet;
  WSADATA wsaData;
  enum {ROOT, USER, HOME, DATA} eAction = ROOT;
  UINT uiCP;
  char szLang[11] = "LANG=";
  wchar_t *ini;
  struct _stat inistat;
  LCID locale;
  wchar_t *binpath, *binpath_idx;

  _plibc_utf8_mode = utf8_mode;

  if (iInit > 0)
  {
    iInit++;

    return ERROR_SUCCESS;
  }

  __plibc_panic = __plibc_panic_default;

  /* Since different modules may initialize to *their* org/app, we need a mechanism to force this
   * information to a global "product name" */
  binpath = malloc (4200);
  GetModuleFileNameW (NULL, binpath, 4096);
  binpath_idx = binpath + wcslen (binpath);
  while ((binpath_idx > binpath) && (*binpath_idx != L'\\') && (*binpath_idx != L'/'))
    binpath_idx--;
  *binpath_idx = L'\0';

  wcscat(binpath, L"\\");
  binpath_idx++;

  ini = L"plibc.ini";
  wcscat(binpath, ini);
  if (_wstat(binpath, &inistat) != 0)
  {
    ini = L"..\\share\\plibc.ini";
    memcpy(binpath_idx, ini, 19 * sizeof (wchar_t));
    if (_wstat(binpath, &inistat) != 0)
    {
      ini = L"..\\share\\plibc\\plibc.ini";
      memcpy(binpath_idx, ini, 25 * sizeof (wchar_t));
      if (_wstat(binpath, &inistat) != 0)
      {
        ini = L"..\\etc\\plibc.ini";
        memcpy(binpath_idx, ini, 17 * sizeof (wchar_t));
        if (_wstat(binpath, &inistat) != 0)
        {
          ini = L"..\\etc\\plibc\\plibc.ini";
          memcpy(binpath_idx, ini, 23 * sizeof (wchar_t));
          if (_wstat(binpath, &inistat) != 0)
            ini = NULL;
        }
      }
    }
  }

  if (ini)
  {
    GetPrivateProfileStringW(L"init", L"organisation", NULL, szUser, sizeof(szUser), binpath);
    _pwszOrg = wcsdup(szUser);
    GetPrivateProfileStringW(L"init", L"application", NULL, szUser, sizeof(szUser), binpath);
    _pwszApp = wcsdup(szUser);

    if (plibc_utf8_mode() == 1)
    {
      if (wchartostr (_pwszOrg, &_pszuOrg, CP_UTF8) < 0)
        _pszuOrg = NULL;
      if (wchartostr (_pwszApp, &_pszuApp, CP_UTF8) < 0)
        _pszuApp = NULL;
    }
    else
    {
      if (wchartostr (_pwszOrg, &_pszuOrg, CP_ACP) < 0)
        _pszuOrg = NULL;
      if (wchartostr (_pwszApp, &_pszuApp, CP_ACP) < 0)
        _pszuApp = NULL;
    }
  }
  else
  {
    strtowchar (pszOrg, &_pwszOrg, CP_UTF8);
    strtowchar (pszApp, &_pwszApp, CP_UTF8);
    _pszuOrg = strdup(pszOrg);
    _pszuApp = strdup(pszApp);
  }

  /* Init path translation */
  if((lRet = _plibc_DetermineRootDir()) == ERROR_SUCCESS)
  {
    DWORD dwSize = 261;
    char *t;
    int r;

    eAction = USER;
    GetUserNameW(szUser, &dwSize);
    if (plibc_utf8_mode() == 1)
      r = wchartostr (szUser, &t, CP_UTF8);
    else
      r = wchartostr (szUser, &t, CP_ACP);
    if (r < 0)
      szuUser[0] = '\0';
    else
    {
      strncpy (szuUser, t, 261);
      free (t);
    }

    eAction = HOME;
    lRet = _plibc_DetermineHomeDir();
  }

  if (lRet == ERROR_SUCCESS)
  {
    eAction = DATA;
    lRet = _plibc_DetermineProgramDataDir();
  }

  if(lRet != ERROR_SUCCESS)
  {
    char *pszMsg, *pszMsg2;
	  char szPanic[1001];
	  long lMem;

    lMem =
      FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                    FORMAT_MESSAGE_FROM_SYSTEM |
                    FORMAT_MESSAGE_IGNORE_INSERTS, NULL, lRet, 0,
                    (LPTSTR) & pszMsg, 0, NULL);

    pszMsg2 = (char *) malloc(lMem + 1);
    strcpy(pszMsg2, pszMsg);
    if(pszMsg2[lMem - 2] == '\r')
      pszMsg2[lMem - 2] = 0;

    snprintf(szPanic, 1000, "Cannot determine %s directory (%s)\n",
            eAction == ROOT ? "root" :
            eAction == HOME ? "home" : "data", pszMsg2);
    szPanic[1000] = 0;
    __plibc_panic(1, szPanic);

    LocalFree(pszMsg);
    free(pszMsg2);

    return lRet;
  }

  /* Init Winsock */
  if (WSAStartup(257, &wsaData) != 0)
  {
    __plibc_panic(2, "Cannot initialize Winsock\n");

    return GetLastError();
  }

  /* To keep track of blocking/non-blocking sockets */
  pSocks = (Winsock *) malloc(sizeof(Winsock) + (uiSockCount + 1));
  pSocks[0].s = -1;
  hSocksLock = CreateMutex(NULL, FALSE, NULL);

  /* To keep track of mapped files */
  pMappings = (TMapping *) malloc(sizeof(TMapping));
  pMappings[0].pStart = NULL;
  hMappingsLock = CreateMutex(NULL, FALSE, NULL);

  /* To keep track of handle types */
  pHandles = (THandleInfo *) malloc(sizeof(THandleInfo));
  pHandles[0].dwHandle = 0;
  hHandlesLock = CreateMutex(NULL, FALSE, NULL);

  /* Open files in binary mode */
  _fmode = _O_BINARY;

  /* Get Windows version */
  theWinVersion.dwOSVersionInfoSize = sizeof(theWinVersion);
  GetVersionEx(&theWinVersion);

  /* Use ANSI codepage for console IO */
  uiCP = GetACP();
  SetConsoleCP(uiCP);
  SetConsoleOutputCP(uiCP);
  setlocale( LC_ALL, ".OCP" );

  /* Set LANG environment variable */
  locale = GetThreadLocale();
  GetLocaleInfo(locale, LOCALE_SISO3166CTRYNAME, szLang + 5, 3);
  szLang[7] = '_';
  GetLocaleInfo(locale, LOCALE_SISO639LANGNAME, szLang + 8, 3);
  putenv(szLang);

  /* Initialize COM library */
  CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

  /* stat64 isn't available under Windows 9x */
  hMsvcrt = LoadLibrary("msvcrt.dll");
  _plibc_stat64 = (TStat64) GetProcAddress(hMsvcrt, "_stat64");
  _plibc_wstat64 = (TWStat64) GetProcAddress(hMsvcrt, "_wstat64");

  srand((unsigned int) time(NULL));

  iInit++;

  return ERROR_SUCCESS;
}

/**
 * @brief Clean up Windows environment
 */
void plibc_shutdown()
{
	if (iInit != 1)
  {
    if (iInit > 1)
      iInit--;

		return;
  }

  WSACleanup();
  free(pSocks);
  CloseHandle(hSocksLock);

  free(pMappings);
  CloseHandle(hMappingsLock);

  free(pHandles);
  CloseHandle(hHandlesLock);

  FreeLibrary(hIphlpapi);
  FreeLibrary(hAdvapi);

  CoUninitialize();

  if (hMsvcrt)
    FreeModule(hMsvcrt);

  free(_pwszOrg);
  free(_pwszApp);
  free(_pszuOrg);
  free(_pszuApp);

  iInit--;
}

/**
 * @brief Register a function which is called when plibc
 *        encounters an interal error
 * @param proc void my_proc(int, char *)
 */
void plibc_set_panic_proc(TPanicProc proc)
{
	__plibc_panic = proc;
}

int IsWinNT()
{
  return theWinVersion.dwPlatformId == VER_PLATFORM_WIN32_NT;
}

/* end of plibc.c */
