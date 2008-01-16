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
char *ctime(const time_t *clock)
{
	return asctime(localtime(clock));
}

/**
 * @brief convert a time value to date and time string
 */
char *ctime_r(const time_t *clock, char *buf)
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

/* end of ctime.c */
