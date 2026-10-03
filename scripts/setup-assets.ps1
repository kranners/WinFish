<#
.SYNOPSIS
  Copies game assets from your own Insaniquarium Deluxe install into the repo root.

.DESCRIPTION
  The decompilation ships no assets. This finds the Steam install (or uses -GameDir),
  then copies bass.dll and the data folders next to WinFish.sln, where the build's
  post-build step expects them. Everything copied is gitignored.

.EXAMPLE
  .\scripts\setup-assets.ps1
  .\scripts\setup-assets.ps1 -GameDir "D:\Games\Insaniquarium Deluxe"
#>
param(
    [string]$GameDir
)

$ErrorActionPreference = 'Stop'
$RepoRoot = Split-Path -Parent $PSScriptRoot
$Items = @('bass.dll', 'data', 'fishsongs', 'images', 'music', 'properties', 'sounds')

function Find-SteamGameDir {
    $steamRoots = @()
    foreach ($key in 'HKCU:\Software\Valve\Steam', 'HKLM:\SOFTWARE\WOW6432Node\Valve\Steam') {
        $props = Get-ItemProperty $key -ErrorAction SilentlyContinue
        if ($props.SteamPath) { $steamRoots += $props.SteamPath }
        if ($props.InstallPath) { $steamRoots += $props.InstallPath }
    }
    $steamRoots += "${env:ProgramFiles(x86)}\Steam"

    # Every Steam library is listed in libraryfolders.vdf as "path"  "<dir>"
    $libraries = @()
    foreach ($root in ($steamRoots | Select-Object -Unique)) {
        $vdf = Join-Path $root 'steamapps\libraryfolders.vdf'
        if (Test-Path $vdf) {
            $libraries += Select-String -Path $vdf -Pattern '"path"\s+"([^"]+)"' |
                ForEach-Object { $_.Matches[0].Groups[1].Value -replace '\\\\', '\' }
        }
        $libraries += $root
    }

    foreach ($lib in ($libraries | Select-Object -Unique)) {
        $candidate = Join-Path $lib 'steamapps\common\Insaniquarium Deluxe'
        if (Test-Path (Join-Path $candidate 'images')) { return $candidate }
    }
    return $null
}

if (-not $GameDir) { $GameDir = Find-SteamGameDir }
if (-not $GameDir -or -not (Test-Path (Join-Path $GameDir 'images'))) {
    throw "Couldn't find Insaniquarium Deluxe. Pass -GameDir with the folder that contains images\ and bass.dll."
}

Write-Host "Copying assets from $GameDir"
foreach ($item in $Items) {
    $src = Join-Path $GameDir $item
    if (-not (Test-Path $src)) { throw "Missing $item in $GameDir" }
    Copy-Item $src -Destination $RepoRoot -Recurse -Force
    $count = if (Test-Path $src -PathType Container) { (Get-ChildItem $src -Recurse -File).Count } else { 1 }
    Write-Host ("  {0,-11} {1,4} file(s)" -f $item, $count)
}
Write-Host "Done. Assets are in $RepoRoot (gitignored)."
