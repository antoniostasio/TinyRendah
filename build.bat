@echo off

mkdir .\build
pushd .\build
cl 
cl -Zi -O2 ..\tgaimage.cpp ..\model.cpp ..\main.cpp
popd