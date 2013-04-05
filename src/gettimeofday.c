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
 * @file src/gettimeofday.c
 * @brief gettimeofday()
 */

#include "plibc_private.h"
#if HAVE_SYS_TIME_H
#include <sys/time.h> 
#endif

void _win_gettimeofday(struct timeval *tp, void *tzp)
{
#ifdef HAVE_GETTIMEOFDAY
  gettimeofday (tp, tzp);
#else
  struct _timeb theTime;

  errno = 0;

  _ftime(&theTime);
  tp->tv_sec = theTime.time;
  tp->tv_usec = theTime.millitm * 1000;
#endif
}

/* end of gettimeofday.c */
