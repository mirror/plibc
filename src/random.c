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
 * @file src/random.c
 * @brief random()
 */

#include "plibc_private.h"

static unsigned int _plibc_weak_rand32_state = 1;

/**
 * @brief pseudo-random number function
 */
long _win_random(void)
{
  _plibc_weak_rand32_state = ((_plibc_weak_rand32_state * 1103515245) + 12345) & 0x7fffffff;
  return _plibc_weak_rand32_state;
}

/**
 * @brief Initialize the PRNG
 */
void _win_srandom(unsigned int seed)
{
  glibc_weak_rand32_state = seed;
}

/* end of random.c */
