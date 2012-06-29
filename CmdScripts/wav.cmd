@echo off
echo Sound file
if "%1" == "1" copy /b %2 %3
if "%1" == "2" %4 -b 192000 %2 %3
%5 %3 %6 %7 delete