@echo off
%1 %2 %3
signtool.exe sign /t http://timestamp.comodoca.com/authenticode /a %3