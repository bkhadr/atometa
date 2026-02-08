@echo off
setlocal enabledelayedexpansion

echo ============================================================
echo  Atometa - Ninja Build System
echo  Fast C++ Build with Ninja Generator
echo ============================================================
echo.

REM Check if vcpkg is installed
where vcpkg >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] vcpkg not found in PATH!
    echo.
    echo Please install vcpkg first:
    echo   1. git clone https://github.com/Microsoft/vcpkg.git C:\vcpkg
    echo   2. cd C:\vcpkg
    echo   3. bootstrap-vcpkg.bat
    echo   4. vcpkg integrate install
    echo.
    pause
    exit /b 1
)

echo [1/7] Checking for Ninja...
vcpkg list ninja:x64-windows >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo Ninja not found. Installing...
    vcpkg install ninja:x64-windows
) else (
    echo Ninja already installed
)

echo.
echo [2/7] Installing dependencies...
vcpkg install glfw3:x64-windows glad:x64-windows glm:x64-windows imgui[glfw-binding,opengl3-binding]:x64-windows eigen3:x64-windows stb:x64-windows gtest:x64-windows

if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Failed to install dependencies!
    pause
    exit /b 1
)

REM Find vcpkg toolchain file
for /f "delims=" %%i in ('where vcpkg') do set VCPKG_EXE=%%i
for %%i in ("%VCPKG_EXE%") do set VCPKG_DIR=%%~dpi
set VCPKG_TOOLCHAIN=C:\vcpkg\scripts\buildsystems\vcpkg.cmake

echo.
echo [3/7] Configuring CMake with Ninja...
cmake -G Ninja -B build -S . -DCMAKE_TOOLCHAIN_FILE="%VCPKG_TOOLCHAIN%" -DCMAKE_BUILD_TYPE=Release

if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] CMake configuration failed!
    pause
    exit /b 1
)

echo.
echo [4/7] Building with Ninja (Release)...
cmake --build build --config Release

if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Build failed!
    pause
    exit /b 1
)

echo.
echo [5/7] Building Debug configuration...
cmake -G Ninja -B build-debug -S . -DCMAKE_TOOLCHAIN_FILE="%VCPKG_TOOLCHAIN%" -DCMAKE_BUILD_TYPE=Debug
cmake --build build-debug --config Debug

echo.
echo [6/7] Creating Visual Studio solution (for editing only)...
cmake -B build-vs -S . -DCMAKE_TOOLCHAIN_FILE="%VCPKG_TOOLCHAIN%"

echo.
echo [7/7] Copying assets...
xcopy /E /I /Y assets build\bin\assets >nul
xcopy /E /I /Y assets build-debug\bin\assets >nul

echo.
echo ============================================================
echo  BUILD COMPLETE!
echo ============================================================
echo.
echo Ninja builds are 30-50%% faster than MSBuild!
echo.
echo Executables:
echo   Release: build\bin\Atometa.exe
echo   Debug:   build-debug\bin\Atometa.exe
echo.
echo For development in Visual Studio:
echo   Open: build-vs\Atometa.sln
echo.
echo To rebuild quickly:
echo   ninja -C build
echo   ninja -C build-debug
echo.
pause