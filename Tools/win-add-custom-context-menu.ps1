Write-Host "You didn't change the `$SCRIPT_PATH. Change it to correct path and remove the exit line"
Write-Host "Add -Del to remove the registry"
exit 1

$REG_PARENT_PATH = 'Registry::HKEY_CLASSES_ROOT\*\shell'
$REG_NAME = "BackupFile"
$SCRIPT_PATH = "D:\Projects\Scripts\Tools\win-create-backup.ps1"
$COMMAND_VALUE = "powershell.exe -WindowStyle Hidden -NoProfile -File `"${SCRIPT_PATH}`" `"%1`""

if ($args.Count -gt 0 -and $args[0] -eq "-Del") {
    if (Test-Path -LiteralPath "${REG_PARENT_PATH}\${REG_NAME}") {
        Remove-Item -LiteralPath "${REG_PARENT_PATH}\${REG_NAME}" -Recurse -Force
        Write-Host "Registry key removed successfully."
    } else {
        Write-Host "Registry key does not exist."
    }
    exit 0
}

New-Item -Path "${REG_PARENT_PATH}\${REG_NAME}" -Force
Set-ItemProperty -LiteralPath "${REG_PARENT_PATH}\${REG_NAME}" -Name "(default)" -Value "Backup File" -Force

New-Item -Path "${REG_PARENT_PATH}\${REG_NAME}\command" -Force
Set-ItemProperty -LiteralPath "${REG_PARENT_PATH}\${REG_NAME}\command" -Name "(default)" -Value "$COMMAND_VALUE"
