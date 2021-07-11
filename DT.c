//  RvL/DT.c (c) 2021,  daynr 0 is Mon 1-1-2001
//  C naming convention:
//    • module2rep = convert to rep
//    • module_rep = convert from rep
#include "DT.h"

u16 date2nr (Sdate dt)           //date to nr
{   u16 nr=365*(dt.Y-1)+(dt.Y-1)/4;
    switch(dt.M)
    {   case 2: nr += 31;
        case 1: break;
        default:nr += (dt.Y&3 ? 59 : 60);
            for(u08 m=dt.M; m-->3; )
                nr += 30 + (8&m ? !(1&m) : 1&m);
    }   return nr + dt.D-1-(nr>36218);  //!(29 feb 2100)
}
Sdate date_nr (u16 nr)           //date from nr
{   Sdate rc={.wd=(u08)(nr%7) };
    u16 s; u08 x=1;
    if(nr>=36218)               nr+=1;  //!(29 feb 2100)
    while(nr>=1461)             nr-=1461, x+=4;
    while(s=365+!(x&3),nr>=s)   nr-=s, x+=1;
    if(x&3 && nr>=59)           nr +=1; //!(29 Feb)
    rc.Y=x;
    switch(nr)
    {   case  0 ... 30:         rc.M=1; break;  //jan
        case 31 ... 59: nr-=31; rc.M=2; break;  //feb
        default:x=31;   nr-=60; rc.M=3; //maa..jul|aug..nov
            while(nr>=x) nr-=x, x=30+(++rc.M&8 ? !(1&rc.M) : 1&rc.M);
    }   return rc.D=nr+1, rc;
}
Sdate date_str (char const *str, u08 lang)  //ex: date_str("Jul  6 2021", 0);
{   Sdate rc =
    {   .Y=twodigits(str+9)
    ,   .M=txt2month(str+0, lang)
    ,   .D=twodigits(str+4)
    };
    if(!rc.Y || !rc.D)
        rc.Y=rc.M=rc.D=1, rc.wd=0;
    return rc;
}
Sdate date_ymd (u16 y, u08 m, u08 d)//from ymd
{   Sdate rc={.Y=(u08)(y%200), .M=m, .D=d};
    return rc;
}

u16 time2nr (Stime ti)          //get timenr from time
{   u16 nr=(60*ti.h+ti.m);
    nr=(u32)nr*93207UL>>10+1;   //0...1439 ==> 0 ... 0xffff - 45
    return nr + (ti.s+1)*3/4;
}
Stime time_nr (u16 nr)          //convert timenr to time
{   Stime rc;
    u32 x=nr;   x*=24;  rc.h=x>>16;
    x=(u16)x;   x*=60;  rc.m=x>>16;
    x=(u16)x;   x*=61;  rc.s=x>>16;
    return rc;
}
Stime time_str (char const *str)    //ex: txtGB2date("Jul 06 2021");
{   Stime rc = {.h=twodigits(str+0), .m=twodigits(str+3), .s=twodigits(str+6)};
    return rc;
}

u32 dt2nr (Sdt dt)               //get stamp from date/time
{   u32 res=date2nr(dt.date)<<16 | time2nr(dt.time);
    return res;
}
Sdt dt_nr (u32 nr)                   //convert stamp to date/time
{   Sdt res={.date=date_nr(nr>>16), .time=time_nr(nr) };
    return res;
}

    //utility
u08 B36dec (s08 c)
{   //BASE36-decoder:       //00-2F 30-39 3A-40 41-5A 5B-60 61-7A 7B-C0 C1-FF
    if((c-='A')>=0)         //BF-EE EF-F8 F9-FF 00-19 1A-1F 20-39 3A-7F 80-BE
        c|='a'-'A';         //                  20-39 3A-3F 20-39 3A-7F
    else if((c+='A'+~'9')<0)//C6-F5 F6-FF 00-06                         87-C5
        c+='a'-'A';         //E6-15 16-1F                               A7-E5
    c-='a'-'A'+~9;          //D0-FF 00-09 EA-F0 0A-23 24-29 0A-23 24-69 91-CF
    return (u08)c;          //       0-9        10-35       10-35
}
u08 bcd2bin (u08 bcd) { return bcd - bcd/16*6;  }//0x21 ==> 33-2*6 ==>   21
u08 bin2bcd (u08 bin) { return bin + bin/10*6;  }//  21 ==> 21+2*6 ==> 0x21
int search4 (char const *s, char ch)
{   char const *p=s; while(*p && *p!=ch) ++p;   return p-s; }
u08 twodigits (char const *str)
{   u08 c1=B36dec(str[0]);  if(c1>=10) c1 = 0;  //0..9
    u08 c2=B36dec(str[1]);  return c2<10 ? 10*c1+c2 : c1;
}
u08 txt2month (char const *str, u08 lang)
{   //Jan Feb Mar Apr May Jun Jul Aug Sep Okt Nov Dec (GB)
    //Jan Feb Mär Apr Mai Jun Jul Aug Sep Okt Nov Dez (D)
    //jan feb maa apr mei jun jul aug sep okt nov dec (NL)
    u08 rc=search4("jfmamjjasond", str[0]|0x20)+1;  //1..13
    switch(rc)
    {   case 3:
        if(!lang)
        {       if('i'==str[2]) rc = 5; //GB
        } else  if('r'!=str[2]) rc = 5; //D NL
        break;
        case 4: if('r'!=str[2]) rc = 8; break;
        case 1: if('a'!=str[1])
            rc = 'n' == str[2] ? 6 : 7;
                else
        case 13:    rc=1;
        default:break;  //2 9 10 11 12
    }   return rc;
}
