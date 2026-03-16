$ErrorActionPreference = "Stop"

$ProjectDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$DevPort = 3000
$PidFile = Join-Path $ProjectDir ".server.pid"
$LogFile = Join-Path $ProjectDir ".server.log"
$ErrorLogFile = Join-Path $ProjectDir ".server.error.log"

function Show-Menu {
    Write-Host ""
    Write-Host "===================================================="
    Write-Host " Project 2: Todo List App"
    Write-Host " Stack: React + Hooks | localStorage (No DB)"
    Write-Host "===================================================="
    Write-Host "1. Install Dependencies"
    Write-Host "2. Start Dev Server"
    Write-Host "3. Stop Dev Server"
    Write-Host "4. Restart Dev Server"
    Write-Host "5. Status"
    Write-Host "6. View Logs"
    Write-Host "7. Build for Production"
    Write-Host "8. Preview Production Build"
    Write-Host "9. Run Tests"
    Write-Host "0. Exit"
    Write-Host ""
}

function Get-SavedProcess {
    if (-not (Test-Path -LiteralPath $PidFile)) { return $null }
    $pidValue = Get-Content -LiteralPath $PidFile -ErrorAction SilentlyContinue | Select-Object -First 1
    if (-not $pidValue) { return $null }
    try {
        return Get-Process -Id ([int]$pidValue) -ErrorAction Stop
    } catch {
        return $null
    }
}

function Install-Deps {
    Write-Host "Installing dependencies..."
    $packageJson = Join-Path $ProjectDir "package.json"
    if (Test-Path -LiteralPath $packageJson) {
        Push-Location $ProjectDir
        try { npm install } finally { Pop-Location }
        Write-Host "Dependencies installed"
    } else {
        Write-Host "package.json not found. Initialize project first."
    }
}

function Start-Server {
    $proc = Get-SavedProcess
    if ($proc) {
        Write-Host "Dev server is already running (PID: $($proc.Id))"
        return
    }

    $packageJson = Join-Path $ProjectDir "package.json"
    if (-not (Test-Path -LiteralPath $packageJson)) {
        Write-Host "package.json not found. Run option 1 first."
        return
    }

    $useStart = Select-String -Path $packageJson -Pattern '"start"' -SimpleMatch -Quiet
    $command = if ($useStart) { "npm start" } else { "npm run dev" }

    Write-Host "Starting dev server on port $DevPort..."
    $cmd = "Set-Location -LiteralPath '$ProjectDir'; $command"
    $p = Start-Process -FilePath "powershell" -ArgumentList "-NoProfile", "-Command", $cmd -RedirectStandardOutput $LogFile -RedirectStandardError $ErrorLogFile -PassThru -WindowStyle Hidden
    Set-Content -LiteralPath $PidFile -Value $p.Id
    Start-Sleep -Seconds 2
    Write-Host "Dev server started (PID: $($p.Id))"
    Write-Host "Access: http://localhost:$DevPort"
}

function Stop-Server {
    Write-Host "Stopping dev server..."
    $proc = Get-SavedProcess
    if ($proc) {
        Stop-Process -Id $proc.Id -Force -ErrorAction SilentlyContinue
    }
    if (Test-Path -LiteralPath $PidFile) {
        Remove-Item -LiteralPath $PidFile -Force -ErrorAction SilentlyContinue
    }
    Write-Host "Dev server stopped"
}

function Restart-Server {
    Stop-Server
    Start-Sleep -Seconds 1
    Start-Server
}

function Show-Status {
    Write-Host ""
    Write-Host "Server Status - Todo List App"
    Write-Host "-----------------------------"
    $proc = Get-SavedProcess
    if ($proc) {
        Write-Host "Status : Running (PID: $($proc.Id))"
        Write-Host "Access : http://localhost:$DevPort"
    } else {
        Write-Host "Status : Not running"
    }
    Write-Host ""
    Write-Host "Storage: Browser localStorage"
    if (Test-Path -LiteralPath (Join-Path $ProjectDir "package.json")) {
        Write-Host "package.json found"
    } else {
        Write-Host "package.json not found"
    }
    Write-Host ""
}

function View-Logs {
    Write-Host "Dev Server Logs (last 30 lines):"
    Write-Host "-----------------------------"
    if (Test-Path -LiteralPath $LogFile) {
        Get-Content -LiteralPath $LogFile -Tail 30
    } else {
        Write-Host "No logs found."
    }
    if (Test-Path -LiteralPath $ErrorLogFile) {
        Write-Host ""
        Write-Host "Dev Server Error Logs (last 30 lines):"
        Write-Host "-----------------------------"
        Get-Content -LiteralPath $ErrorLogFile -Tail 30
    }
    Write-Host ""
}

function Build-Production {
    if (-not (Test-Path -LiteralPath (Join-Path $ProjectDir "package.json"))) {
        Write-Host "package.json not found."
        return
    }
    Push-Location $ProjectDir
    try { npm run build } finally { Pop-Location }
}

function Preview-Build {
    $packageJson = Join-Path $ProjectDir "package.json"
    if (Test-Path -LiteralPath $packageJson) {
        $hasPreview = Select-String -Path $packageJson -Pattern '"preview"' -SimpleMatch -Quiet
        if ($hasPreview) {
            Push-Location $ProjectDir
            try { npm run preview } finally { Pop-Location }
            return
        }
    }

    $distDir = Join-Path $ProjectDir "dist"
    $buildDir = Join-Path $ProjectDir "build"
    if (Test-Path -LiteralPath $distDir) {
        Start-Process -FilePath "powershell" -ArgumentList "-NoProfile", "-Command", "Set-Location -LiteralPath '$distDir'; py -3 -m http.server 4173"
        Write-Host "Preview at http://localhost:4173"
    } elseif (Test-Path -LiteralPath $buildDir) {
        Start-Process -FilePath "powershell" -ArgumentList "-NoProfile", "-Command", "Set-Location -LiteralPath '$buildDir'; py -3 -m http.server 4173"
        Write-Host "Preview at http://localhost:4173"
    } else {
        Write-Host "No build directory found. Run option 7 first."
    }
}

function Run-Tests {
    $packageJson = Join-Path $ProjectDir "package.json"
    if ((Test-Path -LiteralPath $packageJson) -and (Select-String -Path $packageJson -Pattern '"test"' -SimpleMatch -Quiet)) {
        Push-Location $ProjectDir
        try { npm test } finally { Pop-Location }
    } else {
        Write-Host "No test script found in package.json."
    }
}

while ($true) {
    Show-Menu
    $choice = Read-Host "Choose an option"
    switch ($choice) {
        "1" { Install-Deps }
        "2" { Start-Server }
        "3" { Stop-Server }
        "4" { Restart-Server }
        "5" { Show-Status }
        "6" { View-Logs }
        "7" { Build-Production }
        "8" { Preview-Build }
        "9" { Run-Tests }
        "0" { Write-Host "Goodbye!"; break }
        default { Write-Host "Invalid option. Please try again." }
    }
}