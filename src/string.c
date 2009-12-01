/*
     This file is part of PlibC.
     (C) 2005, 2009 Nils Durner (and other contributing authors)

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
 * @file src/string.c
 * @brief String functions
 */

#include "plibc_private.h"

#if !HAVE_STRNDUP
/**
 * return a malloc'd copy of at most the specified
 * number of bytes of a string
 * @author glibc-Team
 */
char *strndup (const char *s, size_t n)
{
  size_t len = strnlen (s, n);
  char *new = (char *) malloc (len + 1);

  if (new == NULL)
    return NULL;

  new[len] = '\0';
  return (char *) memcpy (new, s, len);
}
#endif

#if !HAVE_STRNLEN
/**
 * Determine the length of a fixed-size string
 * @author Jakub Jelinek (jakub at redhat dot com)
 */
size_t strnlen (const char *str, size_t maxlen)
{
  const char *char_ptr, *end_ptr = str + maxlen;
  const unsigned long int *longword_ptr;
  unsigned long int longword, magic_bits, himagic, lomagic;

  if (maxlen == 0)
    return 0;

  if (__builtin_expect (end_ptr < str, 0))
    end_ptr = (const char *) ~0UL;

  /* Handle the first few characters by reading one character at a time.
     Do this until CHAR_PTR is aligned on a longword boundary.  */
  for (char_ptr = str; ((unsigned long int) char_ptr
			& (sizeof (longword) - 1)) != 0;
       ++char_ptr)
    if (*char_ptr == '\0')
      {
	if (char_ptr > end_ptr)
	  char_ptr = end_ptr;
	return char_ptr - str;
      }

  /* All these elucidatory comments refer to 4-byte longwords,
     but the theory applies equally well to 8-byte longwords.  */

  longword_ptr = (unsigned long int *) char_ptr;

  /* Bits 31, 24, 16, and 8 of this number are zero.  Call these bits
     the "holes."  Note that there is a hole just to the left of
     each byte, with an extra at the end:

     bits:  01111110 11111110 11111110 11111111
     bytes: AAAAAAAA BBBBBBBB CCCCCCCC DDDDDDDD

     The 1-bits make sure that carries propagate to the next 0-bit.
     The 0-bits provide holes for carries to fall into.  */
  magic_bits = 0x7efefeffL;
  himagic = 0x80808080L;
  lomagic = 0x01010101L;
  if (sizeof (longword) > 4)
    {
      /* 64-bit version of the magic.  */
      /* Do the shift in two steps to avoid a warning if long has 32 bits.  */
      magic_bits = ((0x7efefefeL << 16) << 16) | 0xfefefeffL;
      himagic = ((himagic << 16) << 16) | himagic;
      lomagic = ((lomagic << 16) << 16) | lomagic;
    }
  if (sizeof (longword) > 8)
    abort ();

  /* Instead of the traditional loop which tests each character,
     we will test a longword at a time.  The tricky part is testing
     if *any of the four* bytes in the longword in question are zero.  */
  while (longword_ptr < (unsigned long int *) end_ptr)
    {
      /* We tentatively exit the loop if adding MAGIC_BITS to
	 LONGWORD fails to change any of the hole bits of LONGWORD. */

      longword = *longword_ptr++;

      if ((longword - lomagic) & himagic)
	{
	  /* Which of the bytes was the zero?  If none of them were, it was
	     a misfire; continue the search.  */

	  const char *cp = (const char *) (longword_ptr - 1);

	  char_ptr = cp;
	  if (cp[0] == 0)
	    break;
	  char_ptr = cp + 1;
	  if (cp[1] == 0)
	    break;
	  char_ptr = cp + 2;
	  if (cp[2] == 0)
	    break;
	  char_ptr = cp + 3;
	  if (cp[3] == 0)
	    break;
	  if (sizeof (longword) > 4)
	    {
	      char_ptr = cp + 4;
	      if (cp[4] == 0)
		break;
	      char_ptr = cp + 5;
	      if (cp[5] == 0)
		break;
	      char_ptr = cp + 6;
	      if (cp[6] == 0)
		break;
	      char_ptr = cp + 7;
	      if (cp[7] == 0)
		break;
	    }
	}
      char_ptr = end_ptr;
    }

  if (char_ptr > end_ptr)
    char_ptr = end_ptr;
  return char_ptr - str;
}
#endif

/**
 * @brief identical to strchr()
 */
char *index(const char *s, int c)
{
    return strchr(s, c);
}

/**
 * @brief copy a string returning a pointer to its end
 */
char *stpcpy(char *dest, const char *src)
{
  size_t len;

  len = strlen(src);
  return (char *) memcpy(dest, src, len + 1) + len;
}

/* end of string.c */
