<#
.SYNOPSIS
  Builds (unless -NoBuild) and launches the game from Win32\Debug.

.EXAMPLE
  .\scripts\run.ps1
  .\scripts\run.ps1 -NoBuild
#>
param(
    [switch]$NoBuild
)

$ErrorActionPreference = 'Stop'
$RepoRoot = Split-Path -Parent $PSScriptRoot
$exe = Join-Path $RepoRoot 'Win32\Debug\Insaniquarium.exe'

if (-not (Test-Path (Join-Path $RepoRoot 'images'))) {
    throw "Game assets are missing. Run .\scripts\setup-assets.ps1 first."
}
if (-not $NoBuild) {
    & (Join-Path $PSScriptRoot 'build.ps1')
    if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
}
if (-not (Test-Path $exe)) { throw "$exe doesn't exist. Build first." }

# The game changes into its own folder at startup; the post-build step puts the assets there.
Start-Process -FilePath $exe -WorkingDirectory (Split-Path $exe)
