//	RvL/DT.h (c) 2021,	daynr 0 is Mon 1-1-2001
#ifndef DT_H_
#define DT_H_
#include <stdint.h>
typedef int8_t   s08;
typedef uint8_t  u08;
typedef uint16_t u16;
typedef uint32_t u32;

typedef struct {u08 Y, M, D, wd;} Sdate;
typedef struct {u08 h, m, s;} Stime; 
typedef struct {Sdate date; Stime time;} Sdt;

#	ifdef __cplusplus
extern "C" {
#	endif

u16 date2daynr(Sdate dt);			//get daynr from date
Sdate daynr2date(u16 nr);			//convert daynr to date
Sdate txtGB2date(char const *str);	//==> txtGB2date(__DATE__);

u16 time2timenr(Stime ti);			//get timenr from time
Stime timenr2time(u16 nr);			//convert timenr to time
Stime txt2time(char const *str);	//==> txt2time(__TIME__);

u32 dt2stamp(Sdt dt);		//get stamp from date/time
Sdt stamp2dt(u32 nr);		//convert stamp to date/time

	//utility
u08 B36dec(s08 c);
u08 bcd2bin(u08 bcd);
u08 bin2bcd(u08 bin);
u08 txt2month(char const *str);
u08 twodigits(char const *str);
#	ifdef __cplusplus
}
class Dt
{
protected:
	Sdt dt;
public:
	Dt (u16 Y, u08 M, u08 D, u08 h=12, u08 m=0, u08 s=0)
	{					date(Y,M,D);	time(h,m,s);	}
	Dt (u16 daynr) {	dt.date=daynr2date(daynr);		}
	Dt (u32 dtstamp) {	dt=stamp2dt(dtstamp);			}
	~Dt () {}

	u16 daynr (void) {	return date2daynr(dt.date);		}
	u32 get (void) {	return dt2stamp(dt);			}
	u16 timenr (void) {	return time2timenr(dt.time);	}
	
	void date (u16 Y, u08 M, u08 D)
	{	dt.date.Y=Y%200, dt.date.M=M, dt.date.D=D;		}
	void time (u08 h, u08 m, u08 s)
	{	dt.time.h=h, dt.time.m=m, dt.time.s=s;			}

	u08 year() const	{return dt.date.Y;}
	u08 month() const	{return dt.date.M;}
	u08 day() const		{return dt.date.D;}
	u08 weekday() 		{return dt.date.wd=daynr()%7;}	//0..6 - Monday..Sunday
	u08 hour() const	{return dt.time.h;}
	u08 minute() const	{return dt.time.m;}
	u08 second() const	{return dt.time.s;}
};
#	endif

/*	C++ example:
#include "DT.h"
int main(void)
{	
	u16 daynr=date2daynr( txtGB2date(__DATE__) );
	u16 timnr=0x8000;	//noon
	Dt datetime((u32)daynr<<16|timnr);
	u08 dayofWeek = datetime.weekday();
	...
}
//*/

#endif//DT_H_
