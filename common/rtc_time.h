#ifndef __RTC_TIME_H
#define __RTC_TIME_H

#include <stdint.h>

typedef long rtc_time_t;

//struct timeval
struct rtc_timeval
{
    long tv_sec;
    long tv_usec;
};

// timespec
struct rtc_timespec
{
    rtc_time_t tv_sec;
    long tv_nsec;
};

//timezone
struct rtc_timezone
{
    int tz_minuteswest;
    int tz_dsttime;
};

//tm
struct rtc_tm
{
  int tm_sec;			/* Seconds.	[0-60] (1 leap second) */
  int tm_min;			/* Minutes.	[0-59] */
  int tm_hour;			/* Hours.	[0-23] */
  int tm_mday;			/* Day.		[1-31] */
  int tm_mon;			/* Month.	[0-11] */
  int tm_year;			/* Year - 1900. */
  int tm_wday;			/* Day of week.	[0-6] */
  int tm_yday;			/* Days in year.[0-365]	*/
  int tm_isdst;			/* DST.		[-1/0/1]*/

  long int tm_gmtoff;		/* Seconds east of UTC.  */
  const char *tm_zone;		/* Timezone abbreviation.  */
};

//获取从1970-1-1到现在的时间
extern int rtc_gettimeofday(struct rtc_timeval *tp,void *ignore);
//返回1970-1-1到现在的秒数
extern rtc_time_t rtc_time(rtc_time_t *t);
//设置时间
extern int rtc_settimeofday(struct rtc_timeval *tp,void *ignore);
//将时间转换为1970-1-1到现在的秒数
rtc_time_t rtc_mktime(struct rtc_tm *const t);
//格式化输出时间
char *rtc_asctime(const struct rtc_tm *timeptr);
//格式化输出
char *rtc_ctime(const rtc_time_t *timep);
//获取当地时间
struct rtc_tm *rtc_localtime(const rtc_time_t *t);

char *rtc_asctime_r(const struct rtc_tm *t,char *buf);
//转换为现实使用的格式
struct rtc_tm *rtc_gmtime_r(const rtc_time_t *timep,struct rtc_tm *r);

struct rtc_tm *rtc_localtime_r(const rtc_time_t *t,struct rtc_tm *r);

void rtc_set_time(int year,int month,int day,int hour,int minute,int second);


struct rtc_date
{
    uint8_t week;
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t sec;
};
extern uint8_t rtc_get_date(struct rtc_date *date);
extern uint8_t rtc_set_sec(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t sec);
/*
	rtc_set_time(2019,12,6,11,55,50);

	rtc_time_t timer;
	struct rtc_tm* t_tm;
	rtc_time(&timer);
	t_tm = rtc_localtime(&timer);
	log_info("today is %4d-%02d-%02d %02d:%02d:%02d \n", t_tm->tm_year+1900,
	t_tm->tm_mon+1, t_tm->tm_mday, t_tm->tm_hour, t_tm->tm_min, t_tm->tm_sec);
*/

#endif // __RTC_TIME_H

