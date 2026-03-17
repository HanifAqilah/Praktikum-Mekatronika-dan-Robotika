$ErrorActionPreference = "Stop"

$ProjectDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$DevPort = 3000
$PidFile = Join-Path $ProjectDir ".server.pid"
$LogFile = Join-Path $ProjectDir ".server.log"
$ErrorLogFile = Join-Path $ProjectDir ".server.error.log"

function Show-Menu {
    Write-Host ""
    Write-Host "===================================================="
    Write-Host " Project 3: Weather App"
    Write-Host " Stack: React + Axios | OpenWeatherMap API"
    Write-Host "===================================================="
    Write-Host "1. Install Dependencies"
    Write-Host "2. Start Dev Server"
    Write-Host "3. Stop Dev Server"
    Write-Host "4. Restart Dev Server"
    Write-Host "5. Status"
    Write-Host "6. View Logs"
    Write-Host "7. Build for Production"
    Write-Host "8. Preview Production Build"
    Write-Host "9. Test OpenWeatherMap API Key"
    Write-Host "10. Run Tests"
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
    $packageJson = Join-Path $ProjectDir "package.json"
    if (Test-Path -LiteralPath $packageJson) {
        Push-Location $ProjectDir
        try { npm install } finally { Pop-Location }
    } else {
        Write-Host "package.json not found. Initialize project first."
        Write-Host "API key required: https://openweathermap.org/api"
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

    $envFile = Join-Path $ProjectDir ".env"
    $envLocal = Join-Path $ProjectDir ".env.local"
    if ((-not (Test-Path -LiteralPath $envFile)) -and (-not (Test-Path -LiteralPath $envLocal))) {
        Write-Host "No .env or .env.local found. You may need VITE_WEATHER_API_KEY."
    }

    $useStart = Select-String -Path $packageJson -Pattern '"start"' -SimpleMatch -Quiet
    $command = if ($useStart) { "npm start" } else { "npm run dev" }
    $cmd = "Set-Location -LiteralPath '$ProjectDir'; $command"
    $p = Start-Process -FilePath "powershell" -ArgumentList "-NoProfile", "-Command", $cmd -RedirectStandardOutput $LogFile -RedirectStandardError $ErrorLogFile -PassThru -WindowStyle Hidden
    Set-Content -LiteralPath $PidFile -Value $p.Id
    Start-Sleep -Seconds 2
    Write-Host "Dev server started (PID: $($p.Id))"
    Write-Host "Access: http://localhost:$DevPort"
}

function Stop-Server {
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
    Write-Host "Server Status - Weather App"
    Write-Host "---------------------------"
    $proc = Get-SavedProcess
    if ($proc) {
        Write-Host "Status : Running (PID: $($proc.Id))"
        Write-Host "Access : http://localhost:$DevPort"
    } else {
        Write-Host "Status : Not running"
    }

    $envCandidates = @(
        Join-Path $ProjectDir ".env",
        Join-Path $ProjectDir ".env.local"
    )
    $found = $false
    foreach ($file in $envCandidates) {
        if (Test-Path -LiteralPath $file) {
            $raw = Get-Content -LiteralPath $file -Raw
            if ($raw -match "WEATHER_API_KEY|OPENWEATHER") {
                $found = $true
                break
            }
        }
    }
    if ($found) {
        Write-Host "API Key : Found in .env/.env.local"
    } else {
        Write-Host "API Key : Not found. Set VITE_WEATHER_API_KEY"
    }
    Write-Host ""
}

function View-Logs {
    Write-Host "Dev Server Logs (last 30 lines):"
    Write-Host "---------------------------"
    if (Test-Path -LiteralPath $LogFile) {
        Get-Content -LiteralPath $LogFile -Tail 30
    } else {
        Write-Host "No logs found."
    }
    if (Test-Path -LiteralPath $ErrorLogFile) {
        Write-Host ""
        Write-Host "Dev Server Error Logs (last 30 lines):"
        Write-Host "---------------------------"
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
    if (Test-Path -LiteralPath $distDir) {
        Start-Process -FilePath "powershell" -ArgumentList "-NoProfile", "-Command", "Set-Location -LiteralPath '$distDir'; py -3 -m http.server 4173"
        Write-Host "Preview at http://localhost:4173"
    } else {
        Write-Host "No build directory found. Run option 7 first."
    }
}

function Get-ApiKey {
    $patterns = @(
        '^\s*VITE_WEATHER_API_KEY\s*=\s*(.+)$',
        '^\s*REACT_APP_WEATHER_API_KEY\s*=\s*(.+)$',
        '^\s*OPENWEATHER_API_KEY\s*=\s*(.+)$'
    )
    foreach ($file in @((Join-Path $ProjectDir ".env"), (Join-Path $ProjectDir ".env.local"))) {
        if (-not (Test-Path -LiteralPath $file)) { continue }
        foreach ($line in Get-Content -LiteralPath $file) {
            foreach ($pattern in $patterns) {
                if ($line -match $pattern) {
                    return $Matches[1].Trim()
                }
            }
        }
    }
    return ""
}

function Test-ApiKey {
    $apiKey = Get-ApiKey
    if ([string]::IsNullOrWhiteSpace($apiKey)) {
        Write-Host "No API key found in .env/.env.local"
        return
    }
    $uri = "https://api.openweathermap.org/data/2.5/weather?q=London&appid=$apiKey&units=metric"
    try {
        $res = Invoke-RestMethod -Uri $uri -Method Get -TimeoutSec 20
        if ($null -ne $res.main) {
            Write-Host "API key is valid"
            Write-Host "London: $($res.main.temp) C, $($res.weather[0].description)"
        } else {
            Write-Host "API key test failed"
        }
    } catch {
        Write-Host "API key test failed: $($_.Exception.Message)"
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
        "9" { Test-ApiKey }
        "10" { Run-Tests }
        "0" { Write-Host "Goodbye!"; break }
        default { Write-Host "Invalid option. Please try again." }
    }
}
