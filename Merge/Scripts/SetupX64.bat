@echo off
echo Compressing Resources and X64
set RAR=%1
set OUTPUT=%2
set SOLUTION=%3
del %OUTPUT%
cd %SOLUTION%
signtool.exe sign /t http://timestamp.comodoca.com/authenticode /sha1 2d44312eda21015cbf75426a78915fb1d54501da SetupX64\Release\OPC-Setup.msi
%RAR% a -s -ep1 -m5 -r %OUTPUT% SetupX64\Release\Win
%RAR% a -s -ep1 -m5 -r %OUTPUT% SetupX64\Release\Bin
%RAR% a -s -ep1 -m5 -r %OUTPUT% "SetupX64\Release\System (64-bit) Folder"
%RAR% a -s -ep1 -m5 -sfx %OUTPUT% SetupX64\Release\OPC-Setup.msi
%RAR% d %OUTPUT% .svn
signtool.exe sign /t http://timestamp.comodoca.com/authenticode /a %OUTPUT%