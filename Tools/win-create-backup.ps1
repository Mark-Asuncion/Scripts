$filepath = $args[0]
$datetime = Get-Date -Format "yyyy_MM_dd_HHmm"

$extension = [System.IO.Path]::GetExtension("$filepath")
$filename = [System.IO.Path]::GetFileNameWithoutExtension("$filepath")
$directory = [System.IO.Path]::GetDirectoryName("$filepath")

$backup_file = "${directory}\${filename}_${datetime}${extension}"

# Write-Host $filepath
# Write-Host $backup_file
Copy-Item -Path $filepath -Destination $backup_file
# Read-Host -Prompt "Press Enter to exit"