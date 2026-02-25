param(
    [string]$BuildType = "Release",
    [string]$Query = "line5s",
    [string]$InputFile = ""
)

$ErrorActionPreference = "Stop"

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $scriptDir

if (-not (Get-Command cmake -ErrorAction SilentlyContinue)) {
    Write-Error "CMake not found in PATH. Install CMake 3.23+ and ensure it is in PATH."
}

if ([string]::IsNullOrWhiteSpace($InputFile)) {
    $InputFile = Join-Path $scriptDir "test_simple.txt"
}

if (-not (Test-Path $InputFile)) {
    Write-Error "Input file not found: $InputFile"
}

$buildDir = Join-Path $scriptDir "build"
if (-not (Test-Path $buildDir)) {
    New-Item -ItemType Directory -Path $buildDir | Out-Null
}

Set-Location $buildDir

if (-not (Test-Path (Join-Path $buildDir "CMakeCache.txt"))) {
    cmake .. -DCMAKE_BUILD_TYPE=$BuildType
} else {
    cmake .. | Out-Null
}

cmake --build . --target crown_exec --config $BuildType

$exeCandidates = @(
    (Join-Path $buildDir "experiments\crown_exec.exe"),
    (Join-Path $buildDir ("experiments\" + $BuildType + "\crown_exec.exe"))
)

$exePath = $exeCandidates | Where-Object { Test-Path $_ } | Select-Object -First 1
if (-not $exePath) {
    Write-Error "crown_exec.exe not found in build output."
}

Write-Host "Running: $exePath -q $Query -i $InputFile -p"
& $exePath -q $Query -i $InputFile -p
# Build and Run Script for CROWN Algorithm

Write-Host "Building CROWN algorithm..." -ForegroundColor Green

# Navigate to implement directory
$scriptPath = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $scriptPath

# Create build directory if it doesn't exist
if (-not (Test-Path "build")) {
    New-Item -ItemType Directory -Path "build" | Out-Null
    Write-Host "Created build directory" -ForegroundColor Yellow
}

Set-Location build

# Configure CMake
Write-Host "`nConfiguring CMake..." -ForegroundColor Cyan
cmake ..
if ($LASTEXITCODE -ne 0) {
    Write-Host "CMake configuration failed!" -ForegroundColor Red
    Write-Host "Please ensure CMake 3.23+ is installed and in PATH" -ForegroundColor Red
    exit 1
}

# Build crown_exec
Write-Host "`nBuilding crown_exec..." -ForegroundColor Cyan
cmake --build . --target crown_exec --config Release
if ($LASTEXITCODE -ne 0) {
    Write-Host "Build failed!" -ForegroundColor Red
    exit 1
}

# Find the executable
$exePath = $null
$possiblePaths = @(
    "experiments\Release\crown_exec.exe",
    "experiments\Debug\crown_exec.exe",
    "experiments\crown_exec.exe"
)

foreach ($path in $possiblePaths) {
    if (Test-Path $path) {
        $exePath = $path
        break
    }
}

if (-not $exePath) {
    Write-Host "Executable not found! Searched in:" -ForegroundColor Red
    $possiblePaths | ForEach-Object { Write-Host "  $_" }
    exit 1
}

Write-Host "`nBuild successful! Executable: $exePath" -ForegroundColor Green

# Run the test
Write-Host "`nRunning test with test_simple.txt..." -ForegroundColor Cyan
Write-Host "=" * 60 -ForegroundColor Gray

& $exePath -q line5s -i ..\test_simple.txt -p

Write-Host "`n" + ("=" * 60) -ForegroundColor Gray
Write-Host "Test completed!" -ForegroundColor Green

