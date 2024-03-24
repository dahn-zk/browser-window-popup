param ([string]$root = "HKCU:")

$path = "$root\SOFTWARE\Mozilla\NativeMessagingHosts\browser_window_popup"
$value = Get-ChildItem -Path manifest.json -File

New-Item -Path $path -Force
Set-Item -Path $path -Value $value -Force

