$ErrorActionPreference = "Stop"

$ProjectDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$BackendPort = 5000
$FrontendPort = 3000
$BackendPidFile = Join-Path $ProjectDir ".backend.pid"
$FrontendPidFile = Join-Path $ProjectDir ".frontend.pid"
$BackendLogFile = Join-Path $ProjectDir ".backend.log"
$FrontendLogFile = Join-Path $ProjectDir ".frontend.log"

function Show-Menu {
    Write-Host ""
    Write-Host "===================================================="
    Write-Host " Project 5: Expense Tracker"
    Write-Host " Stack: Express + React | SQLite"
    Write-Host "===================================================="
    Write-Host "1. Install Dependencies"
    Write-Host "2. Start All Servers"
    Write-Host "3. Stop All Servers"
    Write-Host "4. Restart All Servers"
    Write-Host "5. Status"
    Write-Host "6. View Backend Logs"
    Write-Host "7. View Frontend Logs"
    Write-Host "8. Run Tests"
    Write-Host "9. View Database"
    Write-Host "0. Exit"
    Write-Host ""
}

function Get-ProcFromPidFile([string]$pidFile) {
    if (-not (Test-Path -LiteralPath $pidFile)) { return $null }
    $pidValue = Get-Content -LiteralPath $pidFile -ErrorAction SilentlyContinue | Select-Object -First 1
    if (-not $pidValue) { return $null }
    try {
        return Get-Process -Id ([int]$pidValue) -ErrorAction Stop
    } catch {
        return $null
    }
}

function Install-Deps {
    Write-Host "Installing dependencies..."
    $rootPackage = Join-Path $ProjectDir "package.json"
    $backendPackage = Join-Path $ProjectDir "backend\package.json"
    $frontendPackage = Join-Path $ProjectDir "frontend\package.json"

    if (Test-Path -LiteralPath $rootPackage) {
        Push-Location $ProjectDir
        try { npm install } finally { Pop-Location }
    }
    if (Test-Path -LiteralPath $backendPackage) {
        Push-Location (Join-Path $ProjectDir "backend")
        try { npm install } finally { Pop-Location }
    }
    if (Test-Path -LiteralPath $frontendPackage) {
        Push-Location (Join-Path $ProjectDir "frontend")
        try { npm install } finally { Pop-Location }
    }

    if ((-not (Test-Path -LiteralPath $rootPackage)) -and (-not (Test-Path -LiteralPath $backendPackage))) {
        Write-Host "No package.json found. Create backend/frontend structure first."
        return
    }
    Write-Host "Dependencies installed"
}

function Start-Backend {
    if (Get-ProcFromPidFile $BackendPidFile) {
        Write-Host "Backend already running"
        return
    }

    $backendDir = Join-Path $ProjectDir "backend"
    if (-not (Test-Path -LiteralPath (Join-Path $backendDir "package.json"))) {
        $backendDir = $ProjectDir
    }
    if (-not (Test-Path -LiteralPath (Join-Path $backendDir "package.json"))) {
        Write-Host "Backend package.json not found."
        return
    }

    $packageJson = Join-Path $backendDir "package.json"
    $command = "node server.js"
    if (Select-String -Path $packageJson -Pattern '"dev"' -SimpleMatch -Quiet) {
        $command = "npm run dev"
    } elseif (Select-String -Path $packageJson -Pattern '"start"' -SimpleMatch -Quiet) {
        $command = "npm start"
    }

    Write-Host "Starting backend on port $BackendPort..."
    $cmd = "Set-Location -LiteralPath '$backendDir'; $command"
    $p = Start-Process -FilePath "powershell" -ArgumentList "-NoProfile", "-Command", $cmd -RedirectStandardOutput $BackendLogFile -RedirectStandardError $BackendLogFile -PassThru -WindowStyle Hidden
    Set-Content -LiteralPath $BackendPidFile -Value $p.Id
    Write-Host "Backend started (PID: $($p.Id))"
}

function Start-Frontend {
    if (Get-ProcFromPidFile $FrontendPidFile) {
        Write-Host "Frontend already running"
        return
    }
    $frontendDir = Join-Path $ProjectDir "frontend"
    if (-not (Test-Path -LiteralPath (Join-Path $frontendDir "package.json"))) {
        Write-Host "Frontend directory not found. Skipping."
        return
    }

    $packageJson = Join-Path $frontendDir "package.json"
    $command = if (Select-String -Path $packageJson -Pattern '"start"' -SimpleMatch -Quiet) { "npm start" } else { "npm run dev" }
    Write-Host "Starting frontend on port $FrontendPort..."
    $cmd = "Set-Location -LiteralPath '$frontendDir'; $command"
    $p = Start-Process -FilePath "powershell" -ArgumentList "-NoProfile", "-Command", $cmd -RedirectStandardOutput $FrontendLogFile -RedirectStandardError $FrontendLogFile -PassThru -WindowStyle Hidden
    Set-Content -LiteralPath $FrontendPidFile -Value $p.Id
    Write-Host "Frontend started (PID: $($p.Id))"
}

function Start-Servers {
    Start-Backend
    Start-Frontend
    Start-Sleep -Seconds 2
    Show-Status
}

function Stop-Servers {
    foreach ($pidFile in @($BackendPidFile, $FrontendPidFile)) {
        $proc = Get-ProcFromPidFile $pidFile
        if ($proc) {
            Stop-Process -Id $proc.Id -Force -ErrorAction SilentlyContinue
        }
        if (Test-Path -LiteralPath $pidFile) {
            Remove-Item -LiteralPath $pidFile -Force -ErrorAction SilentlyContinue
        }
    }
    Write-Host "All servers stopped"
}

function Show-Status {
    Write-Host ""
    Write-Host "Server Status - Expense Tracker"
    Write-Host "-------------------------------"

    $backend = Get-ProcFromPidFile $BackendPidFile
    $frontend = Get-ProcFromPidFile $FrontendPidFile
    if ($backend) {
        Write-Host "Backend  : Running (PID: $($backend.Id)) -> http://localhost:$BackendPort"
    } else {
        Write-Host "Backend  : Not running"
    }
    if ($frontend) {
        Write-Host "Frontend : Running (PID: $($frontend.Id)) -> http://localhost:$FrontendPort"
    } else {
        Write-Host "Frontend : Not running"
    }

    $dbFiles = Get-ChildItem -LiteralPath $ProjectDir -Recurse -File -Include *.db, *.sqlite -ErrorAction SilentlyContinue
    if ($dbFiles) {
        $db = $dbFiles | Select-Object -First 1
        Write-Host "Database : SQLite -> $($db.FullName)"
    } else {
        Write-Host "Database : No .db file yet (created on first run)"
    }
    Write-Host ""
    Write-Host "App : http://localhost:$FrontendPort"
    Write-Host "API : http://localhost:$BackendPort/api/expenses"
    Write-Host ""
}

function View-BackendLogs {
    Write-Host "Backend Logs (last 30 lines):"
    Write-Host "-----------------------------"
    if (Test-Path -LiteralPath $BackendLogFile) {
        Get-Content -LiteralPath $BackendLogFile -Tail 30
    } else {
        Write-Host "No backend logs found."
    }
    Write-Host ""
}

function View-FrontendLogs {
    Write-Host "Frontend Logs (last 30 lines):"
    Write-Host "------------------------------"
    if (Test-Path -LiteralPath $FrontendLogFile) {
        Get-Content -LiteralPath $FrontendLogFile -Tail 30
    } else {
        Write-Host "No frontend logs found."
    }
    Write-Host ""
}

function Run-Tests {
    $testDir = Join-Path $ProjectDir "backend"
    if (-not (Test-Path -LiteralPath (Join-Path $testDir "package.json"))) {
        $testDir = $ProjectDir
    }
    $packageJson = Join-Path $testDir "package.json"
    if ((Test-Path -LiteralPath $packageJson) -and (Select-String -Path $packageJson -Pattern '"test"' -SimpleMatch -Quiet)) {
        Push-Location $testDir
        try { npm test } finally { Pop-Location }
    } else {
        Write-Host "No test script found."
    }
}

function View-Database {
    Write-Host "SQLite Database Info:"
    Write-Host "---------------------"
    $db = Get-ChildItem -LiteralPath $ProjectDir -Recurse -File -Include *.db, *.sqlite -ErrorAction SilentlyContinue | Select-Object -First 1
    if (-not $db) {
        Write-Host "No database file found yet."
        return
    }
    Write-Host "File: $($db.FullName)"
    Write-Host "Size: $([Math]::Round($db.Length / 1KB, 2)) KB"

    if (Get-Command sqlite3 -ErrorAction SilentlyContinue) {
        Write-Host ""
        Write-Host "Tables:"
        sqlite3 $db.FullName ".tables"
    } else {
        Write-Host "sqlite3 CLI not found. Install sqlite3 to run queries from terminal."
    }
    Write-Host ""
}

function Restart-Servers {
    Stop-Servers
    Start-Sleep -Seconds 1
    Start-Servers
}

while ($true) {
    Show-Menu
    $choice = Read-Host "Choose an option"
    switch ($choice) {
        "1" { Install-Deps }
        "2" { Start-Servers }
        "3" { Stop-Servers }
        "4" { Restart-Servers }
        "5" { Show-Status }
        "6" { View-BackendLogs }
        "7" { View-FrontendLogs }
        "8" { Run-Tests }
        "9" { View-Database }
        "0" { Write-Host "Goodbye!"; break }
        default { Write-Host "Invalid option. Please try again." }
    }
}
