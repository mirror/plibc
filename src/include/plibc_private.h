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
 * @file include/plibc_private.h
 * @brief Private declarations
 * @internal
 */

#ifndef _PLIBC_PRIVATE_H_
#define _PLIBC_PRIVATE_H_

#include "config.h"

#include "plibc.h"

#ifndef ENABLE_NLS
  #ifdef HAVE_INTL
    #define ENABLE_NLS 1
  #endif
#endif

#include "langinfo.h"
#include <sys/timeb.h>
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <locale.h>
#include <objbase.h>

#include "plibc_strconv.h"

typedef struct {
  char *pStart;
  HANDLE hMapping;
  HANDLE hFile;
} TMapping;

typedef struct
{
  SOCKET s;
  BOOL bBlocking;
} Winsock;
extern Winsock *pSocks;
extern unsigned int uiSockCount;
extern HANDLE hSocksLock;
extern TPanicProc __plibc_panic;

typedef struct
{
  int fildes;
  void *buf;
  size_t nbyte;
} TReadWriteInfo;

typedef int (*TStat64) (const char *path, struct stat64 *buffer);
typedef int (*TWStat64) (const wchar_t *path, struct stat64 *buffer);

typedef enum {UNKNOWN_HANDLE, SOCKET_HANDLE, PIPE_HANDLE, FD_HANDLE} THandleType;
typedef struct
{
  DWORD dwHandle;
  THandleType eType;
} THandleInfo;

extern TStat64 _plibc_stat64;
extern TWStat64 _plibc_wstat64;

struct plibc_WDIR
{
  struct plibc_WDIR *self;
  _WDIR *mingw_wdir;
  struct dirent udirent;
};

int plibc_utf8_mode();

THandleType __win_GetHandleType (DWORD dwHandle);
void __win_SetHandleType (DWORD dwHandle, THandleType eType);
void __win_DiscardHandleType (DWORD dwHandle);

int __win_deref (char *path);
int __win_derefw (wchar_t *path);

long _plibc_DetermineRootDir (void);
long _plibc_DetermineProgramDataDir (void);
long _plibc_DetermineHomeDir (void);
int plibc_conv_to_win_path_ex (const char *pszUnix, char *pszWindows, int derefLinks);

#endif //_PLIBC_PRIVATE_H_

/* end of plibc_private.h */
