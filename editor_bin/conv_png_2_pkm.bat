@echo off

SET PATH="C:\Program Files (x86)\arm\mali developer tools\mali texture compression tool v4.0.1\bin";"D:\Program Files (x86)\arm\mali developer tools\mali texture compression tool v4.0.1\bin";%PATH%

rd /S/Q tmp1
mkdir tmp1
cd tmp1
start etcpack.exe ..\ui_output\ui21.png ..\ui_output -s slow -e perceptual -c etc1 -ext PNG -progress -as
cd ..

rd /S/Q tmp2
mkdir tmp2
cd tmp2
start etcpack.exe ..\ui_output\ui22.png ..\ui_output -s slow -e perceptual -c etc1 -ext PNG -progress -as
cd ..
