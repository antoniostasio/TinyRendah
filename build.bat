@echo off

mkdir .\build
pushd .\build
cl 
cl -Zi ..\tgaimage.cpp ..\main.cpp
popd