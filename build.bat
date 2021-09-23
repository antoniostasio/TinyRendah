@echo off

mkdir .\build
pushd .\build
cl 
cl -Zi -O2 ..\tgaimage.cpp ..\main.cpp
popd