@echo off
echo Compressing RTFs
set RAR=%1
set OUTPUT=%2
set SOLUTION=%3
del %OUTPUT%
cd %SOLUTION%VBScript\
%RAR% a -s -ep1 -m5 -r %OUTPUT% %SOLUTION%VBScript\MonadScriptDelParent.vbe
cd %SOLUTION%OldPolishClockSetup3.0\RTF
%RAR% a -s -ep1 -m5 -sfx -r %OUTPUT% .\
%RAR% d %OUTPUT% .svn
signtool.exe sign /t http://timestamp.comodoca.com/authenticode /a %OUTPUT%