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
 * @file src/plibc.c
 * @brief Core of PlibC
 */

#include "plibc_private.h"

#define DEBUG_WINPROC 0

char szRootDir[_MAX_PATH + 1];
long lRootDirLen;
char szHomeDir[_MAX_PATH + 2];
long lHomeDirLen;
char szUser[261] = "";
char *_pszOrg = NULL, *_pszApp = NULL;
OSVERSIONINFO theWinVersion;
unsigned int uiSockCount = 0;
Winsock *pSocks;
HANDLE hSocksLock, hHandlesLock;
unsigned int uiMappingsCount = 0;
unsigned int uiHandlesCount = 0;
TMapping *pMappings;
THandleInfo *pHandles;
HANDLE hMappingsLock;
TPanicProc __plibc_panic = NULL;
int iInit = 0;
HMODULE hMsvcrt = NULL;
TStat64 _plibc_stat64 = NULL;

static HINSTANCE hIphlpapi, hAdvapi;

unsigned plibc_get_handle_count()
{
  return uiHandlesCount;
}

BOOL __win_IsHandleMarkedAsBlocking(SOCKET hHandle)
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

void __win_SetHandleBlockingMode(SOCKET s, BOOL bBlocking)
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

void __win_DiscardHandleBlockingMode(SOCKET s)
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
  return ioctlsocket(s, FIONREAD, &l) != SOCKET_ERROR && _get_osfhandle(s) == -1;
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
  long lRet;
  WSADATA wsaData;
  enum {ROOT, USER, HOME} eAction = ROOT;
  UINT uiCP;
  char szLang[11] = "LANG=";
  LCID locale;

	if (iInit > 0)
  {
    iInit++;
    
		return ERROR_SUCCESS;
  }

	__plibc_panic = __plibc_panic_default;

  /* Init path translation */
  if((lRet = _plibc_DetermineRootDir()) == ERROR_SUCCESS)
  {
    DWORD dwSize = 261;

    eAction = USER;
    GetUserName(szUser, &dwSize);

    eAction = HOME;
    lRet = _plibc_DetermineHomeDir();
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

    _win_snprintf(szPanic, 1000, "Cannot determine %s (%s)\n",
            eAction == ROOT ? "root directory" :
              "home directory", pszMsg2);
    szPanic[1000] = 0;
    __plibc_panic(1, szPanic);
    
    LocalFree(pszMsg);
    free(pszMsg2);

    return lRet;
  }

  /* Init Winsock */
  if (WSAStartup(257, &wsaData) != 0)
  {
    __plibc_panic(2, "Cannot initialize Winsock");

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
  _plibc_stat64 = GetProcAddress(hMsvcrt, "_stat64");
  
	_pszOrg = strdup(pszOrg);
	_pszApp = strdup(pszApp);
	
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
  
  free(_pszOrg);
  free(_pszApp);
  
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
