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
 * @file include/langinfo.h
 * @brief Modified version of glibc's langinfo.h
 */

#ifndef _LANGINFO_H_
#define _LANGINFO_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef int nl_item;

/* Enumeration of locale items that can be queried with `nl_langinfo'. */
enum
{
  /* LC_TIME category: date and time formatting.  */

  /* Abbreviated days of the week. */
  ABDAY_1, /* Sun */
#define ABDAY_1     ABDAY_1
  ABDAY_2,
#define ABDAY_2     ABDAY_2
  ABDAY_3,
#define ABDAY_3     ABDAY_3
  ABDAY_4,
#define ABDAY_4     ABDAY_4
  ABDAY_5,
#define ABDAY_5     ABDAY_5
  ABDAY_6,
#define ABDAY_6     ABDAY_6
  ABDAY_7,
#define ABDAY_7     ABDAY_7

  /* Long-named days of the week. */
  DAY_1,      /* Sunday */
#define DAY_1     DAY_1
  DAY_2,      /* Monday */
#define DAY_2     DAY_2
  DAY_3,      /* Tuesday */
#define DAY_3     DAY_3
  DAY_4,      /* Wednesday */
#define DAY_4     DAY_4
  DAY_5,      /* Thursday */
#define DAY_5     DAY_5
  DAY_6,      /* Friday */
#define DAY_6     DAY_6
  DAY_7,      /* Saturday */
#define DAY_7     DAY_7

  /* Abbreviated month names.  */
  ABMON_1,      /* Jan */
#define ABMON_1     ABMON_1
  ABMON_2,
#define ABMON_2     ABMON_2
  ABMON_3,
#define ABMON_3     ABMON_3
  ABMON_4,
#define ABMON_4     ABMON_4
  ABMON_5,
#define ABMON_5     ABMON_5
  ABMON_6,
#define ABMON_6     ABMON_6
  ABMON_7,
#define ABMON_7     ABMON_7
  ABMON_8,
#define ABMON_8     ABMON_8
  ABMON_9,
#define ABMON_9     ABMON_9
  ABMON_10,
#define ABMON_10    ABMON_10
  ABMON_11,
#define ABMON_11    ABMON_11
  ABMON_12,
#define ABMON_12    ABMON_12

  /* Long month names.  */
  MON_1,      /* January */
#define MON_1     MON_1
  MON_2,
#define MON_2     MON_2
  MON_3,
#define MON_3     MON_3
  MON_4,
#define MON_4     MON_4
  MON_5,
#define MON_5     MON_5
  MON_6,
#define MON_6     MON_6
  MON_7,
#define MON_7     MON_7
  MON_8,
#define MON_8     MON_8
  MON_9,
#define MON_9     MON_9
  MON_10,
#define MON_10      MON_10
  MON_11,
#define MON_11      MON_11
  MON_12,
#define MON_12      MON_12

  AM_STR,     /* Ante meridian string.  */
#define AM_STR      AM_STR
  PM_STR,     /* Post meridian string.  */
#define PM_STR      PM_STR

  D_T_FMT,      /* Date and time format for strftime.  */
#define D_T_FMT     D_T_FMT
  D_FMT,      /* Date format for strftime.  */
#define D_FMT     D_FMT
  T_FMT,      /* Time format for strftime.  */
#define T_FMT     T_FMT
  T_FMT_AMPM,     /* 12-hour time format for strftime.  */
#define T_FMT_AMPM    T_FMT_AMPM

  ERA,        /* Alternate era.  */
#define ERA     ERA
  ERA_D_FMT,      /* Date in alternate era format.  */
#define ERA_D_FMT   ERA_D_FMT
  ALT_DIGITS,     /* Alternate symbols for digits.  */
#define ALT_DIGITS    ALT_DIGITS
  ERA_D_T_FMT,      /* Date and time in alternate era format.  */
#define ERA_D_T_FMT   ERA_D_T_FMT
  ERA_T_FMT,      /* Time in alternate era format.  */
#define ERA_T_FMT   ERA_T_FMT
  _DATE_FMT,    /* strftime format for date.  */
#define _DATE_FMT _DATE_FMT
  CODESET,
#define CODESET     CODESET
  CRNCYSTR,
#define CRNCYSTR     CRNCYSTR
  RADIXCHAR,
#define RADIXCHAR   RADIXCHAR
  THOUSEP,
#define THOUSEP     THOUSEP
  YESEXPR,
#define YESEXPR     YESEXPR
  NOEXPR,     /* Regex matching ``no'' input.  */
#define NOEXPR      NOEXPR
  /* This marks the highest value used.  */
  _NL_NUM
};

char *nl_langinfo(int item);

#ifdef __cplusplus
}
#endif

#endif //_LANGINFO_H_
