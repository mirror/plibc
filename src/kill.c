/*
     This file is part of PlibC.
     (C) 2008 Nils Durner (and other contributing authors)

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
 * @file src/kill.c
 * @brief kill()
 */

#include "plibc_private.h"

/**
 * @brief send a signal to a process or a group of processes
 */
int _win_kill(pid_t pid, int sig)
{
  if (sig == SIGKILL || sig == SIGTERM)
  {
    HANDLE h;
    
    h = OpenProcess(PROCESS_TERMINATE, FALSE, (DWORD) pid);
    if (!h)
    {
      SetErrnoFromWinError(GetLastError());
      return -1;
    }
    
    if (!TerminateProcess(h, 0))
    {
      SetErrnoFromWinError(GetLastError());
      return -1;      
    }
    else
      return 0;
  }
  
  errno = EINVAL;
  return -1;
}
