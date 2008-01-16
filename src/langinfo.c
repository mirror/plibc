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
 * @file src/langinfo.c
 * @brief langinfo()
 */

#include "plibc_private.h"

char __langinfo[251];

/**
 * @brief language information
 */
char *nl_langinfo(int item)
{
  unsigned int loc;

  loc = GetThreadLocale();

  switch(item)
  {
    case CODESET:
      {
        unsigned int cp = GetACP();

        if (cp)
          sprintf(__langinfo, "CP%u", cp);
        else
          strcpy(__langinfo, "UTF-8"); /* ? */
        return __langinfo;
      }
    case D_T_FMT:
    case T_FMT_AMPM:
    case ERA_D_T_FMT:
      strcpy(__langinfo, "%c");
      return __langinfo;
    case D_FMT:
    case ERA_D_FMT:
      strcpy(__langinfo, "%x");
      return __langinfo;
    case T_FMT:
    case ERA_T_FMT:
      strcpy(__langinfo, "%X");
      return __langinfo;
    case AM_STR:
      GetLocaleInfo(loc, LOCALE_S1159, __langinfo, 251);
      return __langinfo;
    case PM_STR:
      GetLocaleInfo(loc, LOCALE_S2359, __langinfo, 251);
      return __langinfo;
    case DAY_1:
      GetLocaleInfo(loc, LOCALE_SDAYNAME1, __langinfo, 251);
      return __langinfo;
    case DAY_2:
      GetLocaleInfo(loc, LOCALE_SDAYNAME2, __langinfo, 251);
      return __langinfo;
    case DAY_3:
      GetLocaleInfo(loc, LOCALE_SDAYNAME3, __langinfo, 251);
      return __langinfo;
    case DAY_4:
      GetLocaleInfo(loc, LOCALE_SDAYNAME4, __langinfo, 251);
      return __langinfo;
    case DAY_5:
      GetLocaleInfo(loc, LOCALE_SDAYNAME5, __langinfo, 251);
      return __langinfo;
    case DAY_6:
      GetLocaleInfo(loc, LOCALE_SDAYNAME6, __langinfo, 251);
      return __langinfo;
    case DAY_7:
      GetLocaleInfo(loc, LOCALE_SDAYNAME7, __langinfo, 251);
      return __langinfo;
    case ABDAY_1:
      GetLocaleInfo(loc, LOCALE_SABBREVDAYNAME1, __langinfo, 251);
      return __langinfo;
    case ABDAY_2:
      GetLocaleInfo(loc, LOCALE_SABBREVDAYNAME2, __langinfo, 251);
      return __langinfo;
    case ABDAY_3:
      GetLocaleInfo(loc, LOCALE_SABBREVDAYNAME3, __langinfo, 251);
      return __langinfo;
    case ABDAY_4:
      GetLocaleInfo(loc, LOCALE_SABBREVDAYNAME4, __langinfo, 251);
      return __langinfo;
    case ABDAY_5:
      GetLocaleInfo(loc, LOCALE_SABBREVDAYNAME5, __langinfo, 251);
      return __langinfo;
    case ABDAY_6:
      GetLocaleInfo(loc, LOCALE_SABBREVDAYNAME6, __langinfo, 251);
      return __langinfo;
    case ABDAY_7:
      GetLocaleInfo(loc, LOCALE_SABBREVDAYNAME7, __langinfo, 251);
      return __langinfo;
    case MON_1:
      GetLocaleInfo(loc, LOCALE_SMONTHNAME1, __langinfo, 251);
      return __langinfo;
    case MON_2:
      GetLocaleInfo(loc, LOCALE_SMONTHNAME2, __langinfo, 251);
      return __langinfo;
    case MON_3:
      GetLocaleInfo(loc, LOCALE_SMONTHNAME3, __langinfo, 251);
      return __langinfo;
    case MON_4:
      GetLocaleInfo(loc, LOCALE_SMONTHNAME4, __langinfo, 251);
      return __langinfo;
    case MON_5:
      GetLocaleInfo(loc, LOCALE_SMONTHNAME5, __langinfo, 251);
      return __langinfo;
    case MON_6:
      GetLocaleInfo(loc, LOCALE_SMONTHNAME6, __langinfo, 251);
      return __langinfo;
    case MON_7:
      GetLocaleInfo(loc, LOCALE_SMONTHNAME7, __langinfo, 251);
      return __langinfo;
    case MON_8:
      GetLocaleInfo(loc, LOCALE_SMONTHNAME8, __langinfo, 251);
      return __langinfo;
    case MON_9:
      GetLocaleInfo(loc, LOCALE_SMONTHNAME9, __langinfo, 251);
      return __langinfo;
    case MON_10:
      GetLocaleInfo(loc, LOCALE_SMONTHNAME10, __langinfo, 251);
      return __langinfo;
    case MON_11:
      GetLocaleInfo(loc, LOCALE_SMONTHNAME11, __langinfo, 251);
      return __langinfo;
    case MON_12:
      GetLocaleInfo(loc, LOCALE_SMONTHNAME12, __langinfo, 251);
      return __langinfo;
    case ABMON_1:
      GetLocaleInfo(loc, LOCALE_SABBREVMONTHNAME1, __langinfo, 251);
      return __langinfo;
    case ABMON_2:
      GetLocaleInfo(loc, LOCALE_SABBREVMONTHNAME2, __langinfo, 251);
      return __langinfo;
    case ABMON_3:
      GetLocaleInfo(loc, LOCALE_SABBREVMONTHNAME3, __langinfo, 251);
      return __langinfo;
    case ABMON_4:
      GetLocaleInfo(loc, LOCALE_SABBREVMONTHNAME4, __langinfo, 251);
      return __langinfo;
    case ABMON_5:
      GetLocaleInfo(loc, LOCALE_SABBREVMONTHNAME5, __langinfo, 251);
      return __langinfo;
    case ABMON_6:
      GetLocaleInfo(loc, LOCALE_SABBREVMONTHNAME6, __langinfo, 251);
      return __langinfo;
    case ABMON_7:
      GetLocaleInfo(loc, LOCALE_SABBREVMONTHNAME7, __langinfo, 251);
      return __langinfo;
    case ABMON_8:
      GetLocaleInfo(loc, LOCALE_SABBREVMONTHNAME8, __langinfo, 251);
      return __langinfo;
    case ABMON_9:
      GetLocaleInfo(loc, LOCALE_SABBREVMONTHNAME9, __langinfo, 251);
      return __langinfo;
    case ABMON_10:
      GetLocaleInfo(loc, LOCALE_SABBREVMONTHNAME10, __langinfo, 251);
      return __langinfo;
    case ABMON_11:
      GetLocaleInfo(loc, LOCALE_SABBREVMONTHNAME11, __langinfo, 251);
      return __langinfo;
    case ABMON_12:
      GetLocaleInfo(loc, LOCALE_SABBREVMONTHNAME12, __langinfo, 251);
      return __langinfo;
    case ERA:
      /* Not implemented */
      __langinfo[0] = 0;
      return __langinfo;
    case ALT_DIGITS:
      GetLocaleInfo(loc, LOCALE_SNATIVEDIGITS, __langinfo, 251);
      return __langinfo;
    case RADIXCHAR:
      GetLocaleInfo(loc, LOCALE_SDECIMAL, __langinfo, 251);
      return __langinfo;
    case THOUSEP:
      GetLocaleInfo(loc, LOCALE_STHOUSAND, __langinfo, 251);
      return __langinfo;
    case YESEXPR:
      /* Not localized */
      strcpy(__langinfo, "^[yY]");
      return __langinfo;
    case NOEXPR:
      /* Not localized */
      strcpy(__langinfo, "^[nN]");
      return __langinfo;
    case CRNCYSTR:
      GetLocaleInfo(loc, LOCALE_STHOUSAND, __langinfo, 251);
      if (__langinfo[0] == '0' || __langinfo[0] == '2')
        __langinfo[0] = '-';
      else
        __langinfo[0] = '+';
      GetLocaleInfo(loc, LOCALE_SCURRENCY, __langinfo + 1, 251);
    default:
      __langinfo[0] = 0;
      return __langinfo;
  }
}

/* end of langinfo.c */
