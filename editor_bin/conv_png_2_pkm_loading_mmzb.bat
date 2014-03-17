@echo off

SET PATH="C:\Program Files (x86)\arm\mali developer tools\mali texture compression tool v4.0.1\bin";"D:\Program Files (x86)\arm\mali developer tools\mali texture compression tool v4.0.1\bin";%PATH%

rd /S/Q tmp1
mkdir tmp1
cd tmp1
start etcpack.exe ..\build_loading_mmzb\loading_mmzb1.png ..\build_loading_mmzb -s slow -e perceptual -c etc1 -ext PNG -progress -as
cd ..

