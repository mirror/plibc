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

#include <windows.h>
#include <string.h>
#include <mbstring.h>
#include <wchar.h>

/**
 * strtowchar:
 * @str: a string to convert
 * @wretstr: a pointer to variable (pointer to wchar_t) to receive the result
 * @cp: codepage to convert from
 *
 * Allocates new wchar_t string, fills it with converted @str
 * and writes the pointer to it into @wretstr.
 * If the function fails, *@wretstr remains unmodified.
 * Converts from user-provided codepage @cp to UTF-16.
 * See http://msdn.microsoft.com/en-us/library/dd319072%28VS.85%29.aspx
 * MultiByteToWideChar() documentation for values of cp.
 * CP_ACP (to convert from current codepage) and CP_UTF8 (to convert from UTF-8)
 * are recommended.
 * Free the string returned in @wretstr with free() when it is no longer needed
 *
 * Returns:
 *  0 - conversion is successful
 * -1 - conversion failed at length-counting phase
 * -2 - conversion failed at memory allocation phase
 * -3 - conversion failed at string conversion phase
 */
int
strtowchar (const char *str, wchar_t **wretstr, UINT cp)
{
  wchar_t *wstr;
  int len, lenc;
  len = MultiByteToWideChar (cp, 0, str, -1, NULL, 0);
  if (len <= 0)
  {
    return -1;
  }
  
  wstr = malloc (sizeof (wchar_t) * len);
  if (wstr == NULL)
  {
    return -2;
  }
  
  lenc = MultiByteToWideChar (cp, 0, str, -1, wstr, len);
  if (lenc != len)
  {
    free (wstr);
    return -3;
  }
  *wretstr = wstr;
  return 0;
}

int
strtowchar_buf (const char *str, wchar_t *wstr, long wstr_size, UINT cp)
{
  int len, lenc;
  len = MultiByteToWideChar (cp, 0, str, -1, NULL, 0);
  if (len <= 0)
  {
    return -1;
  }
  
  if (wstr_size < len)
  {
    return -2;
  }
  
  lenc = MultiByteToWideChar (cp, 0, str, -1, wstr, len);
  if (lenc != len)
  {
    return -3;
  }
  return 0;
}

/**
 * wchartostr:
 * @wstr: a string (UTF-16-encoded) to convert
 * @wretstr: a pointer to variable (pointer to char) to receive the result
 * @cp: codepage to convert to
 *
 * Allocates new wchar_t string, fills it with converted @wstr
 * and writes the pointer to it into @retstr.
 * If the function fails, *@retstr remains unmodified.
 * Converts from UTF-16 to user-provided codepage @cp.
 * See http://msdn.microsoft.com/en-us/library/dd319072%28VS.85%29.aspx
 * WideCharToMultiByte() documentation for values of cp.
 * CP_ACP (to convert to current codepage) and CP_UTF8 (to covert to UTF-8)
 * are recommended.
 * Free the string returned in @retstr with free() when it is no longer needed
 *
 * Returns:
 *  1 - conversion is successful, but some characters were replaced by placeholders
 *  0 - conversion is successful
 * -1 - conversion failed at length-counting phase
 * -2 - conversion failed at memory allocation phase
 * -3 - conversion failed at string conversion phase
 */
int
wchartostr (const wchar_t *wstr, char **retstr, UINT cp)
{
  char *str;
  int len, lenc;
  BOOL lossy = FALSE;
  len = WideCharToMultiByte (cp, 0, wstr, -1, NULL, 0, NULL, cp == CP_UTF8 ? NULL : &lossy);
  if (len <= 0)
  {
    return -1;
  }
  
  str = malloc (sizeof (char) * len);
  if (wstr == NULL)
  {
    return -2;
  }
  
  lenc = WideCharToMultiByte (cp, 0, wstr, -1, str, len, NULL, cp == CP_UTF8 ? NULL : &lossy);
  if (lenc != len)
  {
    free (str);
    return -3;
  }
  *retstr = str;
  if (lossy)
    return 1;
  return 0;
}

int
wchartostr_buf (const wchar_t *wstr, char *str, long str_len, UINT cp)
{
  int len, lenc;
  BOOL lossy = FALSE;
  len = WideCharToMultiByte (cp, 0, wstr, -1, NULL, 0, NULL, cp == CP_UTF8 ? NULL : &lossy);
  if (len <= 0)
  {
    return -1;
  }
  
  if (str_len < len)
  {
    return -2;
  }
  
  lenc = WideCharToMultiByte (cp, 0, wstr, -1, str, len, NULL, cp == CP_UTF8 ? NULL : &lossy);
  if (lenc != len)
  {
    return -3;
  }
  if (lossy)
    return 1;
  return 0;
}
