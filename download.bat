@echo off
REM ============================================================
REM ST-Link STM32 Download Script
REM ============================================================
REM 
REM Usage:
REM   Double-click this script to download and run the program
REM
REM Requirements:
REM   - ST-Link is properly connected
REM   - Program has been compiled (Output\C8T6 Proj.hex exists)
REM
REM ============================================================

setlocal

set KEIL_PATH=D:\studySoft\KEIL\UV4\UV4.exe
set PROJECT_PATH=F:\StudyMaterial\SCUJJC\course\graduationproject\QRS\project\project5\Project\C8T6 Proj.uvprojx
set OUTPUT_DIR=F:\StudyMaterial\SCUJJC\course\graduationproject\QRS\project\project5\Output
set LOG_FILE=%OUTPUT_DIR%\download_log.txt

echo ============================================================
echo  ST-Link Download Script
echo ============================================================
echo.
echo This script will:
echo   1. Rebuild the project
echo   2. Download to STM32F103ZET6 via ST-Link
echo   3. Reset and Run automatically
echo.

REM Check if Keil exists
if not exist "%KEIL_PATH%" (
    echo ERROR: Keil UV4.exe not found at %KEIL_PATH%
    pause
    exit /b 1
)

REM Check if project exists
if not exist "%PROJECT_PATH%" (
    echo ERROR: Project file not found
    pause
    exit /b 1
)

REM Create output directory if not exists
if not exist "%OUTPUT_DIR%" mkdir "%OUTPUT_DIR%"

echo Step 1: Building project...
echo.
"%KEIL_PATH%" -b "%PROJECT_PATH%" -j0 -o "%LOG_FILE%" -t "Target 1"

REM Check if build was successful
if %ERRORLEVEL% NEQ 0 (
    echo Build failed! Check %LOG_FILE%
    type "%LOG_FILE%"
    pause
    exit /b 1
)

echo Build completed successfully.
echo.

REM Check if hex file exists
set HEX_FILE=%OUTPUT_DIR%\C8T6 Proj.hex
if not exist "%HEX_FILE%" (
    echo ERROR: Hex file not found. Build may have failed.
    pause
    exit /b 1
)

echo Step 2: Downloading to device...
echo.

REM Download using Keil with ST-Link
REM -d: Download to flash
REM -j0: No UI
REM -t: Target name
"%KEIL_PATH%" -d "%PROJECT_PATH%" -j0 -t "Target 1"

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ============================================================
    echo  Download SUCCESS!
    echo ============================================================
    echo.
    echo The program is now running on the STM32F103ZET6 board.
    echo.
    echo If serial output shows "0. AT" messages, the ESP8266
    echo initialization is in progress.
    echo.
) else (
    echo.
    echo ============================================================
    echo  Download FAILED!
    echo ============================================================
    echo.
    echo Please check:
    echo   1. ST-Link is connected to PC
    echo   2. ST-Link is connected to board (SWCLK, SWDIO, GND)
    echo   3. Board is powered
    echo   4. ST-Link drivers are installed
)

echo.
pause
endlocal