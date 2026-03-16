$ErrorActionPreference = "Stop"

$ProjectDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$Port = 8080
$PidFile = Join-Path $ProjectDir ".server.pid"
$LogFile = Join-Path $ProjectDir ".server.log"
$ErrorLogFile = Join-Path $ProjectDir ".server.error.log"

function Show-Menu {
    Write-Host ""
    Write-Host "===================================================="
    Write-Host " Project 1: Portfolio Website Statis"
    Write-Host " Stack: HTML5 + CSS3 + JavaScript (Static)"
    Write-Host "===================================================="
    Write-Host "1. Start Server"
    Write-Host "2. Stop Server"
    Write-Host "3. Status"
    Write-Host "4. View Logs"
    Write-Host "5. Restart Server"
    Write-Host "6. Open in Browser"
    Write-Host "7. Build / Validate HTML"
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

function Start-Server {
    $proc = Get-SavedProcess
    if ($proc) {
        Write-Host "Server is already running (PID: $($proc.Id))"
        return
    }

    Write-Host "Starting static file server on port $Port..."
    $workDir = $ProjectDir
    $started = $false

    if (Get-Command py -ErrorAction SilentlyContinue) {
        $cmd = "Set-Location -LiteralPath '$workDir'; py -3 -m http.server $Port"
        $p = Start-Process -FilePath "powershell" -ArgumentList "-NoProfile", "-Command", $cmd -RedirectStandardOutput $LogFile -RedirectStandardError $ErrorLogFile -PassThru -WindowStyle Hidden
        $started = $true
    } elseif (Get-Command python -ErrorAction SilentlyContinue) {
        $cmd = "Set-Location -LiteralPath '$workDir'; python -m http.server $Port"
        $p = Start-Process -FilePath "powershell" -ArgumentList "-NoProfile", "-Command", $cmd -RedirectStandardOutput $LogFile -RedirectStandardError $ErrorLogFile -PassThru -WindowStyle Hidden
        $started = $true
    } elseif (Get-Command npx -ErrorAction SilentlyContinue) {
        $cmd = "Set-Location -LiteralPath '$workDir'; npx serve -p $Port ."
        $p = Start-Process -FilePath "powershell" -ArgumentList "-NoProfile", "-Command", $cmd -RedirectStandardOutput $LogFile -RedirectStandardError $ErrorLogFile -PassThru -WindowStyle Hidden
        $started = $true
    }

    if (-not $started) {
        Write-Host "No HTTP server found. Install Python 3 or Node.js."
        return
    }

    Set-Content -LiteralPath $PidFile -Value $p.Id
    Start-Sleep -Seconds 1
    Write-Host "Server started (PID: $($p.Id))"
    Write-Host "Access: http://localhost:$Port"
}

function Stop-Server {
    Write-Host "Stopping server..."
    $proc = Get-SavedProcess
    if ($proc) {
        Stop-Process -Id $proc.Id -Force -ErrorAction SilentlyContinue
    }
    if (Test-Path -LiteralPath $PidFile) {
        Remove-Item -LiteralPath $PidFile -Force -ErrorAction SilentlyContinue
    }
    Write-Host "Server stopped"
}

function Show-Status {
    Write-Host ""
    Write-Host "Server Status - Portfolio Website"
    Write-Host "---------------------------------"
    $proc = Get-SavedProcess
    if ($proc) {
        Write-Host "Status : Running (PID: $($proc.Id))"
        Write-Host "Access : http://localhost:$Port"
    } else {
        Write-Host "Status : Not running"
    }
    Write-Host ""
    Write-Host "Project files:"
    foreach ($file in @("index.html", "style.css", "script.js")) {
        if (Test-Path -LiteralPath (Join-Path $ProjectDir $file)) {
            Write-Host "  OK  $file"
        } else {
            Write-Host "  MISS $file"
        }
    }
    Write-Host ""
}

function View-Logs {
    Write-Host "Server Logs (last 30 lines):"
    Write-Host "---------------------------------"
    if (Test-Path -LiteralPath $LogFile) {
        Get-Content -LiteralPath $LogFile -Tail 30
    } else {
        Write-Host "No logs found."
    }
    if (Test-Path -LiteralPath $ErrorLogFile) {
        Write-Host ""
        Write-Host "Server Error Logs (last 30 lines):"
        Write-Host "---------------------------------"
        Get-Content -LiteralPath $ErrorLogFile -Tail 30
    }
    Write-Host ""
}

function Open-Browser {
    Start-Process "http://localhost:$Port"
}

function Validate-Html {
    Write-Host "Validating project files..."
    $missing = 0
    foreach ($file in @("index.html", "style.css", "script.js")) {
        $path = Join-Path $ProjectDir $file
        if (Test-Path -LiteralPath $path) {
            $lines = (Get-Content -LiteralPath $path | Measure-Object -Line).Lines
            Write-Host "  Found: $file ($lines lines)"
        } else {
            Write-Host "  Missing: $file"
            $missing++
        }
    }
    if ($missing -eq 0) {
        Write-Host "All required files present. Ready to serve!"
    } else {
        Write-Host "$missing file(s) missing. Create them before starting the server."
    }
}

function Restart-Server {
    Stop-Server
    Start-Sleep -Seconds 1
    Start-Server
}

while ($true) {
    Show-Menu
    $choice = Read-Host "Choose an option"
    switch ($choice) {
        "1" { Start-Server }
        "2" { Stop-Server }
        "3" { Show-Status }
        "4" { View-Logs }
        "5" { Restart-Server }
        "6" { Open-Browser }
        "7" { Validate-Html }
        "0" { Write-Host "Goodbye!"; break }
        default { Write-Host "Invalid option. Please try again." }
    }
}