@echo off
echo Compiling Shader %COMPILE% %ENTRY%
%FXC% /T %SHADER_TYPE% /Fo %COMPILE% %SRC% /E %ENTRY%
%ENCRYPT% %COMPILE% %OUTPUT% %BZIP2% delete