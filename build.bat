@echo off
REM ============================================================
REM Keil STM32F103ZET6 Project Build Script
REM ============================================================

set KEIL_PATH=D:\studySoft\KEIL\UV4\UV4.exe
set PROJECT_PATH=F:\StudyMaterial\SCUJJC\course\graduationproject\QRS\project\project5\Project\C8T6 Proj.uvprojx
set OUTPUT_DIR=F:\StudyMaterial\SCUJJC\course\graduationproject\QRS\project\project5\Output
set LOG_FILE=%OUTPUT_DIR%\build_log.txt

echo ============================================================
echo  STM32F103ZET6 Project Build Script
echo ============================================================
echo.
echo Keil Path: %KEIL_PATH%
echo Project:   %PROJECT_PATH%
echo Log File:  %LOG_FILE%
echo.

REM Check if Keil exists
if not exist "%KEIL_PATH%" (
    echo ERROR: Keil UV4.exe not found at %KEIL_PATH%
    echo Please update KEIL_PATH in this script
    pause
    exit /b 1
)

REM Check if project exists
if not exist "%PROJECT_PATH%" (
    echo ERROR: Project file not found at %PROJECT_PATH%
    pause
    exit /b 1
)

REM Create output directory if not exists
if not exist "%OUTPUT_DIR%" mkdir "%OUTPUT_DIR%"

echo Starting build...
echo.

REM Build the project (batch mode)
"%KEIL_PATH%" -b "%PROJECT_PATH%" -j0 -o "%LOG_FILE%" -t "Target 1"

echo.
echo Build completed. Checking results...
echo.

REM Display build log
type "%LOG_FILE%"

echo.
echo ============================================================
echo  Build finished. Check the log above for details.
echo ============================================================
pause