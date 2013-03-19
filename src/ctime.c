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
 * @file src/ctime.c
 * @brief ctime(), ctime_r()
 */

#include "plibc_private.h"

/**
 * @brief convert a time value to date and time string
 */
char *_win_ctime(const time_t *clock)
{
  if (plibc_utf8_mode() == 1)
  {
    char *timeu = NULL;
    wchar_t *timew = _wasctime (localtime (clock));
    if (!timew)
      return NULL;
    wchartostr (timew, &timeu, CP_UTF8);
    free (timew);
    /* in case of error timeu stays NULL */
    return timeu;
  }
  else
    return asctime(localtime(clock));
}

/**
 * @brief convert a time value to date and time string
 */
char *_win_ctime_r(const time_t *clock, char *buf)
{
  if (plibc_utf8_mode() == 1)
  {
    wchar_t *ret;
    char *retu = NULL;
    int r;
    ret = _wasctime(localtime(clock));
    if (!ret)
      return ret;
    r = wchartostr (ret, &retu, CP_UTF8);
    free (ret);
    if (r < 0)
      return NULL;
    if (retu)
    {
      strcpy (buf, retu);
      retu = buf;
    }
    return retu;
  }
  else
  {
    char *ret;
    ret = asctime(localtime(clock));
    if (ret != NULL)
    {
      strcpy(buf, ret);
      ret = buf;
    }
    return ret;
  }
}

/* end of ctime.c */
