# Build script for Polaris Engine (Windows PowerShell)

Write-Host "🔨 Building Polaris Engine..." -ForegroundColor Cyan

# Create build directory if it doesn't exist
if (-not (Test-Path "build")) {
    New-Item -ItemType Directory -Path "build" | Out-Null
}

Set-Location build

# Run CMake
Write-Host "📋 Running CMake..." -ForegroundColor Yellow
cmake ..

if ($LASTEXITCODE -ne 0) {
    Write-Host "❌ CMake configuration failed!" -ForegroundColor Red
    exit 1
}

# Build
Write-Host "⚙️  Compiling..." -ForegroundColor Yellow
cmake --build . --config Release

if ($LASTEXITCODE -eq 0) {
    Write-Host "✅ Build successful!" -ForegroundColor Green
    Write-Host ""
    Write-Host "🚀 Run with: .\build\polaris.exe" -ForegroundColor Cyan
    Write-Host "📊 Results will be saved to stats.csv" -ForegroundColor Cyan
} else {
    Write-Host "❌ Build failed!" -ForegroundColor Red
    exit 1
}

Set-Location ..
