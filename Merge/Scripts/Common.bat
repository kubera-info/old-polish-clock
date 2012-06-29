@echo off
echo Compressing DirectX
set RAR=%1
set OUTPUT=%2
set SOLUTION=%3
del %OUTPUT%
cd %SOLUTION%
signtool.exe sign /t http://timestamp.comodoca.com/authenticode /sha1 2d44312eda21015cbf75426a78915fb1d54501da Setup\Release\OPC-Setup.msi
cd %SOLUTION%DX Runtime\
%RAR% a -s -ep1 -m5 %OUTPUT% D*.*
%RAR% a -s -ep1 -m5 %OUTPUT% *XAudio*x??.*
cd %SOLUTION%SetupRes\
%RAR% a -s -ep1 -m5 -sfx -r %OUTPUT% .\*.monad-*
signtool.exe sign /t http://timestamp.comodoca.com/authenticode /a %OUTPUT%
