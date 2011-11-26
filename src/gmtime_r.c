/*
     This file is part of PlibC.
     (C) 2011 Nils Durner (and other contributing authors)

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

#include <stdint.h>
#include <time.h>

/* Copyright (C) 2002-2004   Manuel Novoa III    <mjn3@codepoet.org>
 *
 * GNU Library General Public License (LGPL) version 2 or later.
 *
 * Dedicated to Toni.  See uClibc/DEDICATION.mjn3 for details.
 */

static const uint16_t _vals[] = {
        60, 60, 24, 7 /* special */, 36524, 1461, 365, 0
};

static const unsigned char days[] = {
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, /* non-leap */
            29,
};

#ifndef __isleap
#define __isleap(y) ( !((y) % 4) && ( ((y) % 100) || !((y) % 400) ) )
#endif

#ifdef __UCLIBC_HAS_TM_EXTENSIONS__
static const char utc_string[] = "UTC";
#endif

/* Notes:
 * If time_t is 32 bits, then no overflow is possible.
 * It time_t is > 32 bits, this needs to be adjusted to deal with overflow.
 */

/* Note: offset is the correction in _days_ to *timer! */

struct tm *_time_t2tm(const time_t *__restrict timer,
                                          int offset, struct tm *__restrict result)
{
        register int *p;
        time_t t1, t, v;
        int wday = wday; /* ok to be uninitialized, shutting up warning */

        {
                register const uint16_t *vp;
                t = *timer;
                p = (int *) result;
                p[7] = 0;
                vp = _vals;
                do {
                        if ((v = *vp) == 7) {
                                /* Overflow checking, assuming time_t is long int... */
#if (LONG_MAX > INT_MAX) && (LONG_MAX > 2147483647L)
#if (INT_MAX == 2147483647L) && (LONG_MAX == 9223372036854775807L)
                                /* Valid range for t is [-784223472856L, 784223421720L].
                                 * Outside of this range, the tm_year field will overflow. */
                                if (((unsigned long)(t + offset- -784223472856L))
                                        > (784223421720L - -784223472856L)
                                        ) {
                                        return NULL;
                                }
#else
#error overflow conditions unknown
#endif
#endif

                                /* We have days since the epoch, so caluclate the weekday. */
#if defined(__BCC__) && TIME_T_IS_UNSIGNED
                                wday = (t + 4) % (*vp); /* t is unsigned */
#else
                                wday = ((int)((t % (*vp)) + 11)) % ((int)(*vp)); /* help bcc */
#endif
                                /* Set divisor to days in 400 years.  Be kind to bcc... */
                                v = ((time_t)(vp[1])) << 2;
                                ++v;
                                /* Change to days since 1/1/1601 so that for 32 bit time_t
                                 * values, we'll have t >= 0.  This should be changed for
                                 * archs with larger time_t types.
                                 * Also, correct for offset since a multiple of 7. */

                                /* TODO: Does this still work on archs with time_t > 32 bits? */
                                t += (135140L - 366) + offset; /* 146097 - (365*30 + 7) -366 */
                        }
#if defined(__BCC__) && TIME_T_IS_UNSIGNED
                        t -= ((t1 = t / v) * v);
#else
                        if ((t -= ((t1 = t / v) * v)) < 0) {
                                t += v;
                                --t1;
                        }
#endif

                        if ((*vp == 7) && (t == v-1)) {
                                --t;                    /* Correct for 400th year leap case */
                                ++p[4];                 /* Stash the extra day... */
                        }

#if defined(__BCC__) && 0
                        *p = t1;
                        if (v <= 60) {
                                *p = t;
                                t = t1;
                        }
                        ++p;
#else
                        if (v <= 60) {
                                *p++ = t;
                                t = t1;
                        } else {
                                *p++ = t1;
                        }
#endif
                } while (*++vp);
        }

        if (p[-1] == 4) {
                --p[-1];
                t = 365;
        }

        *p += ((int) t);                        /* result[7] .. tm_yday */

        p -= 2;                                         /* at result[5] */

#if (LONG_MAX > INT_MAX) && (LONG_MAX > 2147483647L)
        /* Protect against overflow.  TODO: Unecessary if int arith wraps? */
        *p = ((((p[-2]<<2) + p[-1])*25 + p[0])<< 2) + (p[1] - 299); /* tm_year */
#else
        *p = ((((p[-2]<<2) + p[-1])*25 + p[0])<< 2) + p[1] - 299; /* tm_year */
#endif

        p[1] = wday;                            /* result[6] .. tm_wday */

        {
                register const unsigned char *d = days;

                wday = 1900 + *p;
                if (__isleap(wday)) {
                        d += 11;
                }

                wday = p[2] + 1;                /* result[7] .. tm_yday */
                *--p = 0;                               /* at result[4] .. tm_mon */
                while (wday > *d) {
                        wday -= *d;
                        if (*d == 29) {
                                d -= 11;                /* Backup to non-leap Feb. */
                        }
                        ++d;
                        ++*p;                           /* Increment tm_mon. */
                }
                p[-1] = wday;                   /* result[3] .. tm_mday */
        }
        /* TODO -- should this be 0? */
        p[4] = 0;                                       /* result[8] .. tm_isdst */
#ifdef __UCLIBC_HAS_TM_EXTENSIONS__
# ifdef __USE_BSD
        result->tm_gmtoff = 0;
        result->tm_zone = utc_string;
# else
        result->__tm_gmtoff = 0;
        result->__tm_zone = utc_string;
# endif
#endif /* __UCLIBC_HAS_TM_EXTENSIONS__ */

        return result;
}


struct tm *gmtime_r(const time_t *clock, struct tm *result)
{
  return _time_t2tm(clock, 0, result);
}
