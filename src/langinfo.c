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

wchar_t __wlanginfo[251];
char __langinfo[251];

#define gli(li) \
    if (plibc_utf8_mode() == 1) \
    { \
      __langinfo[0] = '\0'; \
      if (GetLocaleInfoW(loc, li, __wlanginfo, 251) > 0) \
        wchartostr_buf (__wlanginfo, __langinfo, 251, CP_UTF8); \
    } \
    else \
      GetLocaleInfo(loc, li, __langinfo, 251)


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
        if (plibc_utf8_mode() == 1)
        {
          /* FIXME: Not sure about this */
          strcpy(__langinfo, "UTF-8"); /* ? */
        }
        else
        {
          unsigned int cp = GetACP();
          if (cp)
            sprintf(__langinfo, "CP%u", cp);
          else
            strcpy(__langinfo, "UTF-8"); /* ? */
        }
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
      gli (LOCALE_S1159);
      return __langinfo;
    case PM_STR:
      gli (LOCALE_S2359);
      return __langinfo;
    case DAY_1:
      gli (LOCALE_SDAYNAME1);
      return __langinfo;
    case DAY_2:
      gli (LOCALE_SDAYNAME2);
      return __langinfo;
    case DAY_3:
      gli (LOCALE_SDAYNAME3);
      return __langinfo;
    case DAY_4:
      gli (LOCALE_SDAYNAME4);
      return __langinfo;
    case DAY_5:
      gli (LOCALE_SDAYNAME5);
      return __langinfo;
    case DAY_6:
      gli (LOCALE_SDAYNAME6);
      return __langinfo;
    case DAY_7:
      gli (LOCALE_SDAYNAME7);
      return __langinfo;
    case ABDAY_1:
      gli (LOCALE_SABBREVDAYNAME1);
      return __langinfo;
    case ABDAY_2:
      gli (LOCALE_SABBREVDAYNAME2);
      return __langinfo;
    case ABDAY_3:
      gli (LOCALE_SABBREVDAYNAME3);
      return __langinfo;
    case ABDAY_4:
      gli (LOCALE_SABBREVDAYNAME4);
      return __langinfo;
    case ABDAY_5:
      gli (LOCALE_SABBREVDAYNAME5);
      return __langinfo;
    case ABDAY_6:
      gli (LOCALE_SABBREVDAYNAME6);
      return __langinfo;
    case ABDAY_7:
      gli (LOCALE_SABBREVDAYNAME7);
      return __langinfo;
    case MON_1:
      gli (LOCALE_SMONTHNAME1);
      return __langinfo;
    case MON_2:
      gli (LOCALE_SMONTHNAME2);
      return __langinfo;
    case MON_3:
      gli (LOCALE_SMONTHNAME3);
      return __langinfo;
    case MON_4:
      gli (LOCALE_SMONTHNAME4);
      return __langinfo;
    case MON_5:
      gli (LOCALE_SMONTHNAME5);
      return __langinfo;
    case MON_6:
      gli (LOCALE_SMONTHNAME6);
      return __langinfo;
    case MON_7:
      gli (LOCALE_SMONTHNAME7);
      return __langinfo;
    case MON_8:
      gli (LOCALE_SMONTHNAME8);
      return __langinfo;
    case MON_9:
      gli (LOCALE_SMONTHNAME9);
      return __langinfo;
    case MON_10:
      gli (LOCALE_SMONTHNAME10);
      return __langinfo;
    case MON_11:
      gli (LOCALE_SMONTHNAME11);
      return __langinfo;
    case MON_12:
      gli (LOCALE_SMONTHNAME12);
      return __langinfo;
    case ABMON_1:
      gli (LOCALE_SABBREVMONTHNAME1);
      return __langinfo;
    case ABMON_2:
      gli (LOCALE_SABBREVMONTHNAME2);
      return __langinfo;
    case ABMON_3:
      gli (LOCALE_SABBREVMONTHNAME3);
      return __langinfo;
    case ABMON_4:
      gli (LOCALE_SABBREVMONTHNAME4);
      return __langinfo;
    case ABMON_5:
      gli (LOCALE_SABBREVMONTHNAME5);
      return __langinfo;
    case ABMON_6:
      gli (LOCALE_SABBREVMONTHNAME6);
      return __langinfo;
    case ABMON_7:
      gli (LOCALE_SABBREVMONTHNAME7);
      return __langinfo;
    case ABMON_8:
      gli (LOCALE_SABBREVMONTHNAME8);
      return __langinfo;
    case ABMON_9:
      gli (LOCALE_SABBREVMONTHNAME9);
      return __langinfo;
    case ABMON_10:
      gli (LOCALE_SABBREVMONTHNAME10);
      return __langinfo;
    case ABMON_11:
      gli (LOCALE_SABBREVMONTHNAME11);
      return __langinfo;
    case ABMON_12:
      gli (LOCALE_SABBREVMONTHNAME12);
      return __langinfo;
    case ERA:
      /* Not implemented */
      __langinfo[0] = 0;
      return __langinfo;
    case ALT_DIGITS:
      gli (LOCALE_SNATIVEDIGITS);
      return __langinfo;
    case RADIXCHAR:
      gli (LOCALE_SDECIMAL);
      return __langinfo;
    case THOUSEP:
      gli (LOCALE_STHOUSAND);
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
      gli (LOCALE_STHOUSAND);
      if (__langinfo[0] == '0' || __langinfo[0] == '2')
        __langinfo[0] = '-';
      else
        __langinfo[0] = '+';
      if (plibc_utf8_mode() == 1)
      {
        if (GetLocaleInfoW(loc, LOCALE_SCURRENCY, __wlanginfo, 251) > 0)
          wchartostr_buf (__wlanginfo, __langinfo + 1, 250, CP_UTF8);
      }
      else
        GetLocaleInfo(loc, LOCALE_SCURRENCY, __langinfo + 1, 250);
      return __langinfo;
    default:
      __langinfo[0] = 0;
      return __langinfo;
  }
}

/* end of langinfo.c */
