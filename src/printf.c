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

static char *wfmt(const char *fmt)
{
  char *ret;
  char *idx;

  ret = strdup(fmt);
  while(idx = strstr(fmt, "%llu"))
    memcpy(idx + 1, "I64", 3);

  return ret;
}

int _win_vsnprintf(char* str, size_t size, const char *format, va_list arg_ptr)
{
  int ret;
  char *fmt;

  fmt = wfmt(str);
  ret = vsnprintf(str, size, fmt, arg_ptr);
  free(fmt);

  return ret;
}

int _win_vsprintf(char *dest, const char *format, va_list arg_ptr)
{
  char *fmt;
  int ret;

  fmt = wfmt(format);
  ret = vsprintf(dest, fmt, arg_ptr);
  free(fmt);

  return ret;
}

int _win_vfprintf(FILE *stream, const char *format, va_list arg_ptr)
{
  char *fmt;
  int ret;

  fmt = wfmt(format);
  ret = vfprintf(stream, fmt, arg_ptr);
  free(fmt);

  return ret;
}

int _win_vprintf(const char *format, va_list ap)
{
  char *fmt;
  int ret;

  fmt = wfmt(format);
  ret = vprintf(fmt, ap);
  free(fmt);

  return ret;
}

int _win_fprintf(FILE *f,const char *format, ...)
{
  char *fmt;
  int ret;
  va_list ap;

  fmt = wfmt(format);
  va_start(ap, format);
  ret = vfprintf(f, fmt, ap);
  free(fmt);
  va_end(ap);

  return ret;
}

int _win_printf(const char *format, ...)
{
  char *fmt;
  int n;
  va_list arg_ptr;

  va_start(arg_ptr, format);
  fmt = wfmt(format);
  n = vprintf(fmt, arg_ptr);
  free(fmt);
  va_end(arg_ptr);
  return n;
}

int _win_snprintf(char *str, size_t size, const char *format, ...)
{
  char *fmt;
  int n;
  va_list arg_ptr;

  va_start(arg_ptr, format);
  fmt = wfmt(format);
  n = vsnprintf(str, size, fmt, arg_ptr);
  free(fmt);
  va_end(arg_ptr);
  return n;
}

int _win_sprintf(char *dest, const char *format, ...)
{
  char *fmt;
  int n;
  va_list arg_ptr;

  fmt = wfmt(format);
  va_start(arg_ptr, format);
  n = vsprintf(dest, fmt, arg_ptr);
  free(fmt);
  va_end(arg_ptr);

  return n;
}

int _win_vsscanf(const char* str, const char* format, va_list arg_ptr)
{
  int ret;
  char *fmt;

  fmt = wfmt(format);
  ret = vsscanf(str, fmt, arg_ptr);
  free(fmt);

  return ret;
}

int _win_sscanf(const char *str, const char *format, ...)
{
  char *fmt;
  int n;
  va_list arg_ptr;

  va_start(arg_ptr, format);
  fmt = wfmt(format);
  n = vsscanf(str,fmt,arg_ptr);
  va_end (arg_ptr);
  return n;
}

int _win_vfscanf(FILE *stream, const char *format, va_list arg_ptr)
{
  char *fmt;
  int ret;

  fmt = wfmt(format);
  ret = vfscanf(stream, fmt, arg_ptr);
  free(fmt);

  return ret;
}

int _win_vscanf(const char *format, va_list arg_ptr)
{
  char *fmt;
  int ret;

  fmt = wfmt(format);
  ret = vscanf(fmt, arg_ptr);
  free(fmt);

  return ret;
}

int _win_scanf(const char *format, ...)
{
  char *fmt;
  int n;
  va_list arg_ptr;

  va_start(arg_ptr, format);
  fmt = wfmt(format);
  n = fscanf(stdin, fmt, arg_ptr);
  free(fmt);
  va_end(arg_ptr);

  return n;
}

int _win_fscanf(FILE *stream, const char *format, ...)
{
  char *fmt;
  int n;
  va_list arg_ptr;

  va_start(arg_ptr, format);
  fmt = wfmt(format);
  n = vfscanf(stream, fmt, arg_ptr);
  free(fmt);
  va_end(arg_ptr);

  return n;
}

/* end of printf.c */
