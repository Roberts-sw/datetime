# datetime
Date/time structs are accessible in C, protected in C++.  
Serial numbers can be derived independently as 16-bit numbers  
for date and time, or combined as 32-bit number including both.

Day of week can be easily derived as the reference date 0  
is Monday Jan 01, 2001, see ``struct Date`` 

Week number is European style, based at work weeks beginning  
on Monday, and first week of the year being the week with at  
least 4 days in the new year.
