//  RvL/DT.h (c) 2021,  daynr 0 is Mon 1-1-2001
//  C naming convention:
//    • module2rep = convert to rep
//    • module_rep = convert from rep
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

#   ifdef __cplusplus
extern "C" {
#   endif

u16 date2nr(Sdate dt);  //to nr
u08 date2weeknr(Sdate dt);
Sdate date_nr(u16 nr);  //from nr
Sdate date_str(char const *str, u08 lang);//from str
Sdate date_ymd(u16 y, u08 m, u08 d);//from ymd

u16 time2nr(Stime ti);  //to nr
Stime time_nr(u16 nr);  //from nr
Stime time_str(char const *str);//from str

u32 dt2nr(Sdt dt);      //to nr
Sdt dt_nr(u32 nr);      //from nr

    //utility
u08 B36dec(s08 c);
u08 bcd2bin(u08 bcd);
u08 bin2bcd(u08 bin);
int search4(char const *s, char ch);
u08 twodigits(char const *str);
u08 txt2month(char const *str, u08 lang);
#   ifdef __cplusplus
}

struct Date
{
protected:
    Sdate date;
public:
    Date () : Date(__DATE__, 0) {}      //compile date
    Date (u16 daynr) {                  date=date_nr(daynr);        }
    Date (char const *str, u08 lang=0) {date=date_str(str, lang);   }
    Date (u16 Y, u08 M, u08 D) {date.Y=Y%200, date.M=M, date.D=D;   }

    u16 nr ()           {return date2nr(date);}
    u08 year() const    {return date.Y;}
    u08 month() const   {return date.M;}
    u08 day() const     {return date.D;}
    u08 weekday()       {return date.wd=nr()%7;}    //0..6 - Monday..Sunday
    u08 weeknr()		{return date2weeknr(date);}
};

struct Time
{
protected:
    Stime time;
public:
    Time () : Time(__TIME__) {}         //compile time
    Time (u16 timenr) {                 time=time_nr(timenr);       }
    Time (char const *str) {            time=time_str(str);         }
    Time (u08 h, u08 m, u08 s=0) {  time.h=h, time.m=m, time.s=s;   }

    u16 nr ()           {return time2nr(time); }
    u08 hour() const    {return time.h;}
    u08 minute() const  {return time.m;}
    u08 second() const  {return time.s;}
};

class Dt : public Date, public Time
{
public:
    Dt () : Date(), Time() {}           //compile date/time
    Dt (u32 stamp) : Date(stamp>>16), Time(stamp) {}
    Dt (u16 Y, u08 M, u08 D, u08 h=12, u08 m=0, u08 s=0)
        : Date(Y,M,D), Time(h,m,s) {}
    ~Dt () {}

    u32 nr () { return (u32)Date::nr()<<16 | Time::nr();}
    void setdate_nr (u16 nr) {Sdate dat=date_nr(nr>>16);date=dat;   }
    void settime_nr (u16 nr) {Stime tim=time_nr(nr>> 0);time=tim;   }
    void _nr(u32 nr) {setdate_nr(nr>>16);   settime_nr(nr>> 0);     }
};
#   endif

/*  C++ example:
#include "DT.h"
int main (void)
{   Dt dt=Dt();                 //compile date/time
    u16 day=dt.Date::nr();
    u16 tim=dt.Time::nr();
    dt.setdate_nr(++day);       //next day from daynr
    dt.weekday();
    u32 timestamp=dt.nr();
    dt=Dt(timestamp+0x10000);   //next day from time stamp
    u08 weeknr=dt.weeknr();
    ...
}
//*/

#endif//DT_H_
