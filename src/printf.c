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
 * @file src/printf.c
 * @brief Unix compatible printf for Windows
 * @author Nils Durner
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

int _win_vsnprintf(char* str, size_t size, const char *format, va_list arg_ptr)
{
  return vsnprintf(str, size, format, arg_ptr);
}

int _win_vsprintf(char *dest, const char *format, va_list arg_ptr)
{
  return vsprintf(dest, format, arg_ptr);
}

int _win_vfprintf(FILE *stream, const char *format, va_list arg_ptr)
{
  return vfprintf(stream, format, arg_ptr);
}

int _win_vprintf(const char *format, va_list ap)
{
  return vprintf(format, ap);
}

int _win_fprintf(FILE *f,const char *format, ...)
{
  int ret;
  va_list ap;

  va_start(ap, format);
  ret = vfprintf(f, format, ap);
  va_end(ap);

  return ret;
}

int _win_printf(const char *format, ...)
{
  int n;
  va_list arg_ptr;

  va_start(arg_ptr, format);
  n = vprintf(format, arg_ptr);
  va_end(arg_ptr);
  return n;
}

int _win_snprintf(char *str, size_t size, const char *format, ...)
{
  int n;
  va_list arg_ptr;

  va_start(arg_ptr, format);
  n = vsnprintf(str, size, format, arg_ptr);
  va_end(arg_ptr);
  return n;
}

int _win_sprintf(char *dest, const char *format, ...)
{
  int n;
  va_list arg_ptr;

  va_start(arg_ptr, format);
  n = vsprintf(dest, format, arg_ptr);
  va_end(arg_ptr);

  return n;
}

int _win_vsscanf(const char* str, const char* format, va_list arg_ptr)
{
  return vsscanf(str, format, arg_ptr);
}

int _win_sscanf(const char *str, const char *format, ...)
{
  int n;
  va_list arg_ptr;

  va_start(arg_ptr, format);
  n = vsscanf(str, format, arg_ptr);
  va_end (arg_ptr);
  return n;
}

int _win_vfscanf(FILE *stream, const char *format, va_list arg_ptr)
{
  return vfscanf(stream, format, arg_ptr);
}

int _win_vscanf(const char *format, va_list arg_ptr)
{
  return vscanf(format, arg_ptr);
}

int _win_scanf(const char *format, ...)
{
  int n;
  va_list arg_ptr;

  va_start(arg_ptr, format);
  n = fscanf(stdin, format, arg_ptr);
  va_end(arg_ptr);

  return n;
}

int _win_fscanf(FILE *stream, const char *format, ...)
{
  int n;
  va_list arg_ptr;

  va_start(arg_ptr, format);
  n = vfscanf(stream, format, arg_ptr);
  va_end(arg_ptr);

  return n;
}

/* end of printf.c */
