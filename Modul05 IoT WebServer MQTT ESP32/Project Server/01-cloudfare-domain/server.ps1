#requires -Version 5.1

# Server Management Script (Windows)

$WebsiteServiceName = "website"
$CloudflareServiceName = "cloudflared"

# Optional log file paths. If files do not exist, the script falls back to service events.
$WebsiteLogPath = ".\logs\website.log"
$CloudflareLogPath = ".\logs\cloudflared.log"

function Show-Menu {
    Write-Host ""
    Write-Host "========================================="
    Write-Host "   Server Management - sirobo.codes"
    Write-Host "========================================="
    Write-Host ""
    Write-Host "1. Start Server"
    Write-Host "2. Stop Server"
    Write-Host "3. Status"
    Write-Host "4. View Logs (Website)"
    Write-Host "5. View Logs (Cloudflare)"
    Write-Host "6. Restart Server"
    Write-Host "7. Install Cloudflare (cloudflared)"
    Write-Host "0. Exit"
    Write-Host ""
}

function Test-IsAdministrator {
    $currentIdentity = [Security.Principal.WindowsIdentity]::GetCurrent()
    $principal = New-Object Security.Principal.WindowsPrincipal($currentIdentity)
    return $principal.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)
}

function Get-CloudflaredCommand {
    $cmd = Get-Command cloudflared -ErrorAction SilentlyContinue
    if ($cmd) {
        return $cmd.Source
    }

    $commonPaths = @(
        "C:\\Program Files\\cloudflared\\cloudflared.exe",
        "C:\\ProgramData\\chocolatey\\bin\\cloudflared.exe"
    )

    foreach ($path in $commonPaths) {
        if (Test-Path -Path $path) {
            return $path
        }
    }

    return $null
}

function Install-Cloudflare {
    Write-Host "Installing and configuring cloudflared..."

    if (-not (Test-IsAdministrator)) {
        Write-Host "Please run PowerShell as Administrator for install/setup service cloudflared."
        return
    }

    $cloudflaredPath = Get-CloudflaredCommand
    if (-not $cloudflaredPath) {
        if (Get-Command winget -ErrorAction SilentlyContinue) {
            Write-Host "Installing cloudflared using winget..."
            winget install --id Cloudflare.cloudflared -e --accept-package-agreements --accept-source-agreements
        }
        elseif (Get-Command choco -ErrorAction SilentlyContinue) {
            Write-Host "Installing cloudflared using chocolatey..."
            choco install cloudflared -y
        }
        else {
            Write-Host "winget/choco not found. Install cloudflared manually first."
            return
        }

        $cloudflaredPath = Get-CloudflaredCommand
        if (-not $cloudflaredPath) {
            Write-Host "cloudflared binary still not found after install."
            return
        }
    }

    $existingSvc = Get-Service -Name $CloudflareServiceName -ErrorAction SilentlyContinue
    if (-not $existingSvc) {
        Write-Host ""
        Write-Host "Paste Cloudflare Tunnel token (from dashboard):"
        $token = Read-Host "Tunnel token"

        if ([string]::IsNullOrWhiteSpace($token)) {
            Write-Host "Token is empty. Setup cancelled."
            return
        }

        & $cloudflaredPath service install $token
    }
    else {
        Write-Host "cloudflared service already exists, skip service install."
    }

    Start-Service -Name $CloudflareServiceName -ErrorAction SilentlyContinue
    Start-Sleep -Seconds 2

    Write-Host "cloudflared setup complete."
    Show-ServiceBrief -ServiceName $CloudflareServiceName
}

function Get-ServiceSafe {
    param(
        [Parameter(Mandatory = $true)]
        [string]$Name
    )

    try {
        return Get-Service -Name $Name -ErrorAction Stop
    }
    catch {
        Write-Host "Service '$Name' tidak ditemukan. Pastikan service sudah dibuat di Windows."
        return $null
    }
}

function Start-ManagedServer {
    Write-Host "Starting server..."

    $website = Get-ServiceSafe -Name $WebsiteServiceName
    $cloudflare = Get-ServiceSafe -Name $CloudflareServiceName
    if (-not $website -or -not $cloudflare) { return }

    Start-Service -Name $WebsiteServiceName -ErrorAction SilentlyContinue
    Start-Service -Name $CloudflareServiceName -ErrorAction SilentlyContinue
    Start-Sleep -Seconds 2

    Write-Host "Server started"
    Show-Status
}

function Stop-ManagedServer {
    Write-Host "Stopping server..."

    $website = Get-ServiceSafe -Name $WebsiteServiceName
    $cloudflare = Get-ServiceSafe -Name $CloudflareServiceName
    if (-not $website -or -not $cloudflare) { return }

    Stop-Service -Name $WebsiteServiceName -ErrorAction SilentlyContinue
    Stop-Service -Name $CloudflareServiceName -ErrorAction SilentlyContinue
    Start-Sleep -Seconds 1

    Write-Host "Server stopped"
}

function Show-ServiceBrief {
    param(
        [Parameter(Mandatory = $true)]
        [string]$ServiceName
    )

    $svc = Get-ServiceSafe -Name $ServiceName
    if (-not $svc) { return }

    $pid = "-"
    $wmi = Get-CimInstance -ClassName Win32_Service -Filter "Name='$ServiceName'" -ErrorAction SilentlyContinue
    if ($wmi -and $wmi.ProcessId -gt 0) {
        $pid = $wmi.ProcessId
    }

    Write-Host ("  Name    : {0}" -f $svc.Name)
    Write-Host ("  Status  : {0}" -f $svc.Status)
    Write-Host ("  StartType: {0}" -f $svc.StartType)
    Write-Host ("  PID     : {0}" -f $pid)
}

function Show-Status {
    Write-Host ""
    Write-Host "Server Status:"
    Write-Host "-----------------------------------------"
    Show-ServiceBrief -ServiceName $WebsiteServiceName
    Write-Host ""
    Show-ServiceBrief -ServiceName $CloudflareServiceName
    Write-Host ""
    Write-Host "Access:"
    Write-Host "  Local : http://localhost:8080"
    Write-Host "  Public: https://sirobo.codes"
    Write-Host ""
}

function Show-RecentServiceEvents {
    param(
        [Parameter(Mandatory = $true)]
        [string]$ServiceName
    )

    # Service state changes are usually recorded by Service Control Manager in System log.
    $events = Get-WinEvent -FilterHashtable @{ LogName = 'System'; ProviderName = 'Service Control Manager' } -MaxEvents 300 -ErrorAction SilentlyContinue |
        Where-Object { $_.Message -match [regex]::Escape($ServiceName) } |
        Select-Object -First 20

    if (-not $events) {
        Write-Host "Tidak ada event service terbaru untuk '$ServiceName'."
        return
    }

    $events |
        Select-Object TimeCreated, Id, LevelDisplayName, Message |
        Format-Table -Wrap -AutoSize
}

function View-WebsiteLogs {
    Write-Host "Website Logs (last 20 lines):"
    Write-Host "-----------------------------------------"

    if (Test-Path -Path $WebsiteLogPath) {
        Get-Content -Path $WebsiteLogPath -Tail 20
    }
    else {
        Show-RecentServiceEvents -ServiceName $WebsiteServiceName
    }

    Write-Host ""
}

function View-CloudflareLogs {
    Write-Host "Cloudflare Tunnel Logs (last 20 lines):"
    Write-Host "-----------------------------------------"

    if (Test-Path -Path $CloudflareLogPath) {
        Get-Content -Path $CloudflareLogPath -Tail 20
    }
    else {
        Show-RecentServiceEvents -ServiceName $CloudflareServiceName
    }

    Write-Host ""
}

function Restart-ManagedServer {
    Write-Host "Restarting server..."

    $website = Get-ServiceSafe -Name $WebsiteServiceName
    $cloudflare = Get-ServiceSafe -Name $CloudflareServiceName
    if (-not $website -or -not $cloudflare) { return }

    Restart-Service -Name $WebsiteServiceName -Force -ErrorAction SilentlyContinue
    Restart-Service -Name $CloudflareServiceName -Force -ErrorAction SilentlyContinue
    Start-Sleep -Seconds 2

    Write-Host "Server restarted"
    Show-Status
}

while ($true) {
    Show-Menu
    $choice = Read-Host "Choose an option"

    switch ($choice) {
        "1" { Start-ManagedServer }
        "2" { Stop-ManagedServer }
        "3" { Show-Status }
        "4" { View-WebsiteLogs }
        "5" { View-CloudflareLogs }
        "6" { Restart-ManagedServer }
        "7" { Install-Cloudflare }
        "0" {
            Write-Host "Goodbye!"
            break
        }
        default {
            Write-Host "Invalid option. Please try again."
        }
    }
}
