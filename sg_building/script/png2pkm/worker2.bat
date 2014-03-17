@echo off

SET WORKER_NAME=worker2
SET SRC_DIR=..\..\..\data\texpack_tex
SET OUT_DIR=..\..\..\data\texpack_tex

SET PATH="C:\Program Files (x86)\arm\mali developer tools\mali texture compression tool v4.0.1\bin";"D:\Program Files (x86)\arm\mali developer tools\mali texture compression tool v4.0.1\bin";%PATH%

rd /S/Q %WORKER_NAME%
mkdir %WORKER_NAME%
cd %WORKER_NAME%
etcpack.exe %SRC_DIR%\buildings23.png %OUT_DIR% -s slow -e perceptual -c etc1 -ext PNG -progress -as
etcpack.exe %SRC_DIR%\buildings24.png %OUT_DIR% -s slow -e perceptual -c etc1 -ext PNG -progress -as
cd ..
rd /S/Q %WORKER_NAME%

echo %WORKER_NAME% done
pause

