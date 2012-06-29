@echo off
echo Converting Volume Texture
set SRC=%1
set ENCRYPT=%2
set OUTPUT=%3
set BZIP2=%4

%ENCRYPT% %SRC% %OUTPUT% %BZIP2% nop