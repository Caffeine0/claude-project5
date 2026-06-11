# ============================================================
# Keil STM32F103ZET6 Project Build Script (PowerShell)
# ============================================================

$KeilPath = "D:\studySoft\KEIL\UV4\UV4.exe"
$ProjectPath = "F:\StudyMaterial\SCUJJC\course\graduationproject\QRS\project\project5\Project\C8T6 Proj.uvprojx"
$OutputDir = "F:\StudyMaterial\SCUJJC\course\graduationproject\QRS\project\project5\Output"
$LogFile = Join-Path $OutputDir "build_log.txt"

Write-Host "============================================================" -ForegroundColor Cyan
Write-Host " STM32F103ZET6 Project Build Script" -ForegroundColor Cyan
Write-Host "============================================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "Keil Path: $KeilPath" -ForegroundColor Yellow
Write-Host "Project:   $ProjectPath" -ForegroundColor Yellow
Write-Host "Log File:  $LogFile" -ForegroundColor Yellow
Write-Host ""

# Check if Keil exists
if (-not (Test-Path $KeilPath)) {
    Write-Host "ERROR: Keil UV4.exe not found at $KeilPath" -ForegroundColor Red
    Write-Host "Please update KeilPath in this script" -ForegroundColor Red
    Read-Host "Press Enter to exit"
    exit 1
}

# Check if project exists
if (-not (Test-Path $ProjectPath)) {
    Write-Host "ERROR: Project file not found at $ProjectPath" -ForegroundColor Red
    Read-Host "Press Enter to exit"
    exit 1
}

# Create output directory if not exists
if (-not (Test-Path $OutputDir)) {
    New-Item -ItemType Directory -Path $OutputDir -Force | Out-Null
}

Write-Host "Starting build..." -ForegroundColor Green
Write-Host ""

# Build the project (batch mode)
# -b: build project
# -j0: batch mode (no interactive)
# -o: output log file
# -t: target name
$BuildArgs = @(
    "-b", $ProjectPath,
    "-j0",
    "-o", $LogFile,
    "-t", "Target 1"
)

$Process = Start-Process -FilePath $KeilPath -ArgumentList $BuildArgs -Wait -PassThru -NoNewWindow

Write-Host ""
Write-Host "Build completed. Exit code: $($Process.ExitCode)" -ForegroundColor $(if ($Process.ExitCode -eq 0) { "Green" } else { "Red" })
Write-Host ""

# Display build log
if (Test-Path $LogFile) {
    Write-Host "=== Build Log ===" -ForegroundColor Cyan
    Get-Content $LogFile | ForEach-Object {
        if ($_ -match "error|Error|ERROR") {
            Write-Host $_ -ForegroundColor Red
        } elseif ($_ -match "warning|Warning|WARNING") {
            Write-Host $_ -ForegroundColor Yellow
        } elseif ($_ -match "Build Time|Program Size|creating") {
            Write-Host $_ -ForegroundColor Green
        } else {
            Write-Host $_
        }
    }
} else {
    Write-Host "WARNING: Log file not created" -ForegroundColor Yellow
}

Write-Host ""
Write-Host "============================================================" -ForegroundColor Cyan
Write-Host " Build finished. Check the log above for details." -ForegroundColor Cyan
Write-Host "============================================================" -ForegroundColor Cyan

# Check for errors in log
if (Test-Path $LogFile) {
    $LogContent = Get-Content $LogFile -Raw
    if ($LogContent -match "error|Error") {
        Write-Host ""
        Write-Host "!!! BUILD FAILED - Errors detected !!!" -ForegroundColor Red
        exit 1
    } elseif ($LogContent -match "0 Error") {
        Write-Host ""
        Write-Host "BUILD SUCCESS - No errors" -ForegroundColor Green
        exit 0
    }
}

Read-Host "Press Enter to exit"