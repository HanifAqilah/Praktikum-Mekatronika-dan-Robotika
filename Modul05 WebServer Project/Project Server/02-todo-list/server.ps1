$ErrorActionPreference = "Stop"

$ProjectDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$DevPort = 3000
$PidFile = Join-Path $ProjectDir ".server.pid"
$LogFile = Join-Path $ProjectDir ".server.log"
$ErrorLogFile = Join-Path $ProjectDir ".server.error.log"
$CloudflarePidFile = Join-Path $ProjectDir ".cloudflared.pid"
$CloudflareLogFile = Join-Path $ProjectDir ".cloudflared.log"
$CloudflareErrorLogFile = Join-Path $ProjectDir ".cloudflared.error.log"
$CloudflareUrlFile = Join-Path $ProjectDir ".cloudflared.url"

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
    Write-Host "10. Start Cloudflare Tunnel"
    Write-Host "11. Stop Cloudflare Tunnel"
    Write-Host "12. Cloudflare Status"
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

function Get-ProcessByPidFile {
    param([string]$Path)

    if (-not (Test-Path -LiteralPath $Path)) { return $null }
    $pidValue = Get-Content -LiteralPath $Path -ErrorAction SilentlyContinue | Select-Object -First 1
    if (-not $pidValue) { return $null }
    try {
        return Get-Process -Id ([int]$pidValue) -ErrorAction Stop
    }
    catch {
        return $null
    }
}

function Refresh-ProcessPathFromMachineAndUser {
    $machinePath = [System.Environment]::GetEnvironmentVariable("Path", "Machine")
    $userPath = [System.Environment]::GetEnvironmentVariable("Path", "User")
    $env:Path = ($machinePath, $userPath -join ";")
}

function Ensure-WingetAvailable {
    if (Get-Command winget -ErrorAction SilentlyContinue) {
        return $true
    }

    try {
        Write-Host "winget tidak ditemukan. Mencoba install App Installer..."
        $tempFile = Join-Path $env:TEMP "Microsoft.DesktopAppInstaller.msixbundle"
        Invoke-WebRequest -Uri "https://aka.ms/getwinget" -OutFile $tempFile -UseBasicParsing -ErrorAction Stop
        Add-AppxPackage -Path $tempFile -ErrorAction Stop
        Remove-Item -Path $tempFile -ErrorAction SilentlyContinue
        Refresh-ProcessPathFromMachineAndUser
        return [bool](Get-Command winget -ErrorAction SilentlyContinue)
    }
    catch {
        return $false
    }
}

function Ensure-ChocoAvailable {
    if (Get-Command choco -ErrorAction SilentlyContinue) {
        return $true
    }

    try {
        Write-Host "chocolatey tidak ditemukan. Mencoba install chocolatey..."
        Set-ExecutionPolicy Bypass -Scope Process -Force
        [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072
        Invoke-Expression ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
        Refresh-ProcessPathFromMachineAndUser
        return [bool](Get-Command choco -ErrorAction SilentlyContinue)
    }
    catch {
        return $false
    }
}

function Get-PreferredPackageManager {
    if (Get-Command winget -ErrorAction SilentlyContinue) { return "winget" }
    if (Get-Command choco -ErrorAction SilentlyContinue) { return "choco" }
    if (Ensure-WingetAvailable) { return "winget" }
    if (Ensure-ChocoAvailable) { return "choco" }
    return $null
}

function Get-CloudflaredCommand {
    $cmd = Get-Command cloudflared -ErrorAction SilentlyContinue
    if ($cmd) {
        return $cmd.Source
    }

    $commonPaths = @(
        "C:\\Program Files\\Cloudflare\\Cloudflared\\cloudflared.exe",
        "C:\\Program Files\\cloudflared\\cloudflared.exe",
        "C:\\Program Files (x86)\\Cloudflare\\Cloudflared\\cloudflared.exe",
        "C:\\ProgramData\\chocolatey\\bin\\cloudflared.exe"
    )

    foreach ($path in $commonPaths) {
        if (Test-Path -Path $path) {
            return $path
        }
    }

    return $null
}

function Ensure-CloudflaredInstalled {
    $cloudflaredPath = Get-CloudflaredCommand
    if ($cloudflaredPath) {
        return $cloudflaredPath
    }

    $packageManager = Get-PreferredPackageManager
    if ($packageManager -eq "winget") {
        Write-Host "Installing cloudflared using winget..."
        winget install --id Cloudflare.cloudflared -e --accept-package-agreements --accept-source-agreements
    }
    elseif ($packageManager -eq "choco") {
        Write-Host "Installing cloudflared using chocolatey..."
        choco install cloudflared -y
    }
    else {
        Write-Host "winget/choco tidak tersedia, install cloudflared manual dulu."
        return $null
    }

    Refresh-ProcessPathFromMachineAndUser
    return (Get-CloudflaredCommand)
}

function Start-CloudflareTunnel {
    $cfProc = Get-ProcessByPidFile -Path $CloudflarePidFile
    if ($cfProc) {
        Write-Host "Cloudflare tunnel sudah berjalan (PID: $($cfProc.Id))"
        if (Test-Path -LiteralPath $CloudflareUrlFile) {
            $publicUrl = Get-Content -LiteralPath $CloudflareUrlFile -ErrorAction SilentlyContinue | Select-Object -First 1
            if ($publicUrl) {
                Write-Host "Public URL: $publicUrl"
            }
        }
        return
    }

    $cloudflaredPath = Ensure-CloudflaredInstalled
    if (-not $cloudflaredPath) {
        Write-Host "cloudflared tidak ditemukan. Tunnel tidak bisa dijalankan."
        return
    }

    $cmd = "Set-Location -LiteralPath '$ProjectDir'; & '$cloudflaredPath' tunnel --url http://127.0.0.1:$DevPort --no-autoupdate"
    $p = Start-Process -FilePath "powershell" -ArgumentList "-NoProfile", "-Command", $cmd -RedirectStandardOutput $CloudflareLogFile -RedirectStandardError $CloudflareErrorLogFile -PassThru -WindowStyle Hidden
    Set-Content -LiteralPath $CloudflarePidFile -Value $p.Id

    Start-Sleep -Seconds 4
    $publicUrl = $null
    if (Test-Path -LiteralPath $CloudflareLogFile) {
        $logRaw = Get-Content -LiteralPath $CloudflareLogFile -Raw -ErrorAction SilentlyContinue
        if ($logRaw -match 'https://[-a-zA-Z0-9]+\.trycloudflare\.com') {
            $publicUrl = $matches[0]
            Set-Content -LiteralPath $CloudflareUrlFile -Value $publicUrl
        }
    }

    Write-Host "Cloudflare tunnel started (PID: $($p.Id))"
    if ($publicUrl) {
        Write-Host "Public URL: $publicUrl"
    }
    else {
        Write-Host "Public URL belum terbaca. Cek menu log/status beberapa detik lagi."
    }
}

function Stop-CloudflareTunnel {
    $cfProc = Get-ProcessByPidFile -Path $CloudflarePidFile
    if ($cfProc) {
        Stop-Process -Id $cfProc.Id -Force -ErrorAction SilentlyContinue
        Write-Host "Cloudflare tunnel dihentikan"
    }
    else {
        Write-Host "Cloudflare tunnel tidak sedang berjalan"
    }

    Remove-Item -LiteralPath $CloudflarePidFile -Force -ErrorAction SilentlyContinue
    Remove-Item -LiteralPath $CloudflareUrlFile -Force -ErrorAction SilentlyContinue
}

function Show-CloudflareStatus {
    Write-Host ""
    Write-Host "Cloudflare Status"
    Write-Host "-----------------------------"

    $cfProc = Get-ProcessByPidFile -Path $CloudflarePidFile
    if ($cfProc) {
        Write-Host "Status : Running (PID: $($cfProc.Id))"
    }
    else {
        Write-Host "Status : Not running"
    }

    if (Test-Path -LiteralPath $CloudflareUrlFile) {
        $publicUrl = Get-Content -LiteralPath $CloudflareUrlFile -ErrorAction SilentlyContinue | Select-Object -First 1
        if ($publicUrl) {
            Write-Host "Public : $publicUrl"
        }
    }

    Write-Host "Local  : http://127.0.0.1:$DevPort"
    Write-Host ""
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
    Stop-CloudflareTunnel
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
    if (Test-Path -LiteralPath $CloudflareLogFile) {
        Write-Host ""
        Write-Host "Cloudflare Logs (last 30 lines):"
        Write-Host "-----------------------------"
        Get-Content -LiteralPath $CloudflareLogFile -Tail 30
    }
    if (Test-Path -LiteralPath $CloudflareErrorLogFile) {
        Write-Host ""
        Write-Host "Cloudflare Error Logs (last 30 lines):"
        Write-Host "-----------------------------"
        Get-Content -LiteralPath $CloudflareErrorLogFile -Tail 30
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
        "10" { Start-CloudflareTunnel }
        "11" { Stop-CloudflareTunnel }
        "12" { Show-CloudflareStatus }
        "0" { Write-Host "Goodbye!"; break }
        default { Write-Host "Invalid option. Please try again." }
    }
}