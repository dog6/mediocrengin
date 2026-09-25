@echo off
echo.
type "./banner.txt"
echo Rebuilding..
timeout /t 3
set build_dir="D:\Projects\CPP\smallengine\build"
cd %build_dir%
for /F "delims=" %%i in ('dir /b') do (rmdir "%%i" /s/q || del "%%i" /s/q)
echo ..deleted previous build
cd ..
cmake -B build
echo ..created CMake build files
cmake --build build
echo.
echo Finished building smallengine! :D
echo.

