#include "rtc_time.h"
#include <stdio.h>

/* days per month -- nonleap! */
const short __rtc_spm[13] =
  { 0,
    (31),
    (31+28),
    (31+28+31),
    (31+28+31+30),
    (31+28+31+30+31),
    (31+28+31+30+31+30),
    (31+28+31+30+31+30+31),
    (31+28+31+30+31+30+31+31),
    (31+28+31+30+31+30+31+31+30),
    (31+28+31+30+31+30+31+31+30+31),
    (31+28+31+30+31+30+31+31+30+31+30),
    (31+28+31+30+31+30+31+31+30+31+30+31),
  };
static long int timezone;
static const char days[] = "Sun Mon Tue Wed Thu Fri Sat ";
static const char months[] = "Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec ";

/* seconds per day */
#define SPD 24*60*60

//ÅÐ¶ÏÊÇ·ñÎªÈòÄê
int __isleap(int year)
{
	return (!(year % 4) && ((year % 100) || !(year % 400)));
}

struct rtc_tm *rtc_gmtime_r(const rtc_time_t *timep, struct rtc_tm *r)
{
	rtc_time_t i;
	register rtc_time_t work = *timep % (SPD);
	r->tm_sec = work % 60;
	work /= 60;
	r->tm_min = work % 60;
	r->tm_hour = work / 60;
	work = *timep / (SPD);
	r->tm_wday = (4 + work) % 7;
	for (i = 1970;; ++i)
	{
		register rtc_time_t k = __isleap(i) ? 366 : 365;
		if (work >= k)
			work -= k;
		else
			break;
	}
	r->tm_year = i - 1900;
	r->tm_yday = work;

	r->tm_mday = 1;
	if (__isleap(i) && (work > 58))
	{
		if (work == 59)
			r->tm_mday = 2; /* 29.2. */
		work -= 1;
	}

	for (i = 11; i && (__rtc_spm[i] > work); --i)
		;
	r->tm_mon = i;
	r->tm_mday += work - __rtc_spm[i];
	return r;
}

struct rtc_tm* rtc_localtime_r(const rtc_time_t* t, struct rtc_tm* r)
{
	rtc_time_t tmp;
	struct rtc_timezone tz = {0};
	rtc_gettimeofday(0, &tz);
	timezone = tz.tz_minuteswest * 60L;
	tmp = *t + timezone;
	return rtc_gmtime_r(&tmp, r);
}

struct rtc_tm* rtc_localtime(const rtc_time_t* t)
{
	static struct rtc_tm tmp;
	return rtc_localtime_r(t, &tmp);
}

rtc_time_t rtc_mktime(struct rtc_tm * const t)
{
	register rtc_time_t day;
	register rtc_time_t i;
	register rtc_time_t years = t->tm_year - 70;

	if (t->tm_sec > 60)
	{
		t->tm_min += t->tm_sec / 60;
		t->tm_sec %= 60;
	}
	if (t->tm_min > 60)
	{
		t->tm_hour += t->tm_min / 60;
		t->tm_min %= 60;
	}
	if (t->tm_hour > 24)
	{
		t->tm_mday += t->tm_hour / 24;
		t->tm_hour %= 24;
	}
	if (t->tm_mon > 12)
	{
		t->tm_year += t->tm_mon / 12;
		t->tm_mon %= 12;
	}
	while (t->tm_mday > __rtc_spm[1 + t->tm_mon])
	{
		if (t->tm_mon == 1 && __isleap(t->tm_year + 1900))
		{
			--t->tm_mday;
		}
		t->tm_mday -= __rtc_spm[t->tm_mon];
		++t->tm_mon;
		if (t->tm_mon > 11)
		{
			t->tm_mon = 0;
			++t->tm_year;
		}
	}

	if (t->tm_year < 70)
		return (rtc_time_t) -1;

	/* Days since 1970 is 365 * number of years + number of leap years since 1970 */
	day = years * 365 + (years + 1) / 4;

	/* After 2100 we have to substract 3 leap years for every 400 years
	 This is not intuitive. Most mktime implementations do not support
	 dates after 2059, anyway, so we might leave this out for it's
	 bloat. */
	if (years >= 131)
	{
		years -= 131;
		years /= 100;
		day -= (years >> 2) * 3 + 1;
		if ((years &= 3) == 3)
			years--;
		day -= years;
	}

	day += t->tm_yday = __rtc_spm[t->tm_mon] + t->tm_mday - 1 +
			(__isleap(t->tm_year + 1900) & (t->tm_mon > 1));

	/* day is now the number of days since 'Jan 1 1970' */
	i = 7;
	t->tm_wday = (day + 4) % i; /* Sunday=0, Monday=1, ..., Saturday=6 */

	i = 24;
	day *= i;
	i = 60;
	return ((day + t->tm_hour) * i + t->tm_min) * i + t->tm_sec;
}

static void num2str(char *c, int i)
{
	c[0] = i / 10 + '0';
	c[1] = i % 10 + '0';
}

char *rtc_asctime_r(const struct rtc_tm *t, char *buf)
{
	/* "Wed Jun 30 21:49:08 1993\n" */
	*(int*) buf = *(int*) (days + (t->tm_wday << 2));
	*(int*) (buf + 4) = *(int*) (months + (t->tm_mon << 2));
	num2str(buf + 8, t->tm_mday);
	if (buf[8] == '0')
		buf[8] = ' ';
	buf[10] = ' ';
	num2str(buf + 11, t->tm_hour);
	buf[13] = ':';
	num2str(buf + 14, t->tm_min);
	buf[16] = ':';
	num2str(buf + 17, t->tm_sec);
	buf[19] = ' ';
	num2str(buf + 20, (t->tm_year + 1900) / 100);
	num2str(buf + 22, (t->tm_year + 1900) % 100);
	buf[24] = '\n';
	return buf;
}

char *rtc_asctime(const struct rtc_tm *timeptr)
{
	static char buf[25];
	return rtc_asctime_r(timeptr, buf);
}

char *rtc_ctime(const rtc_time_t *timep)
{
	return rtc_asctime(rtc_localtime(timep));
}

void rtc_set_time(int year,int month,int day,int hour,int minute,int second)
{
    struct rtc_tm tptr;
    struct rtc_timeval tv;

    tptr.tm_year = year - 1900;
    tptr.tm_mon = month - 1;
    tptr.tm_mday = day;
    tptr.tm_hour = hour;
    tptr.tm_min = minute;
    tptr.tm_sec = second;

    tv.tv_sec = rtc_mktime(&tptr);
    tv.tv_usec = 0;

    rtc_settimeofday(&tv,NULL);
}


