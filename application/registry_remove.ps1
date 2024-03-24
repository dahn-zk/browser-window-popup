param ([string]$root = "HKCU:")

$path = "$root\SOFTWARE\Mozilla\NativeMessagingHosts\browser_window_popup"

Remove-Item -Path $path -Recurse -Force

