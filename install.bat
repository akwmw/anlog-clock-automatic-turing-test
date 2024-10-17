@echo off
setlocal enabledelayedexpansion

:: Check for MSYS2 installation
if not exist "C:\msys64" (
    echo MSYS2 is not installed. Downloading and installing MSYS2...
    
    :: Download MSYS2
    powershell -Command "Invoke-WebRequest -Uri 'https://github.com/msys2/msys2-installer/releases/download/2023-05-26/msys2-x86_64-20230526.exe' -OutFile 'msys2-installer.exe'"
    
    :: Run MSYS2 installer
    start /wait msys2-installer.exe --install "C:\msys64"
    
    :: Clean up installer
    del msys2-installer.exe
)

:: Add MSYS2 to PATH
set "PATH=C:\msys64\mingw64\bin;C:\msys64\usr\bin;%PATH%"

:: Check for GCC installation
gcc --version > nul 2>&1
if %errorlevel% neq 0 (
    echo GCC is not installed. Installing GCC...
    C:\msys64\usr\bin\bash.exe -lc "pacman -S mingw-w64-x86_64-gcc --noconfirm"
)

:: Check for GTK3 installation
pkg-config --modversion gtk+-3.0 > nul 2>&1
if %errorlevel% neq 0 (
    echo GTK3 is not installed. Installing GTK3...
    C:\msys64\usr\bin\bash.exe -lc "pacman -S mingw-w64-x86_64-gtk3 --noconfirm"
)

:: Run GCC command
echo Compiling clock.c...
gcc -o clock clock.c `pkg-config --cflags --libs gtk+-3.0` -lm

if %errorlevel% neq 0 (
    echo Compilation failed.
    exit /b 1
) else (
    echo Compilation successful.
    echo Executable created: clock.exe
)

endlocal