@echo off
echo Compressing D3DX
set RAR=%1
set OUTPUT=%2
set SOLUTION=%3
del %OUTPUT%
cd %SOLUTION%DX Runtime\
%RAR% a -av -s -ep1 -m5 -sfx %OUTPUT% Jun*D3D*x??.*
signtool.exe sign /t http://timestamp.comodoca.com/authenticode /a %OUTPUT%