<#
.SYNOPSIS
  Builds WinFish.sln (Debug|Win32) with MSBuild from Visual Studio / Build Tools 2022.

.EXAMPLE
  .\scripts\build.ps1
  .\scripts\build.ps1 -Rebuild
#>
param(
    [switch]$Rebuild
)

$ErrorActionPreference = 'Stop'
$RepoRoot = Split-Path -Parent $PSScriptRoot

$vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
if (-not (Test-Path $vswhere)) {
    throw "Visual Studio 2022 or Build Tools 2022 with the C++ workload is required (vswhere.exe not found)."
}
$msbuild = & $vswhere -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 `
    -find 'MSBuild\**\Bin\MSBuild.exe' | Select-Object -First 1
if (-not $msbuild) {
    throw "MSBuild with the MSVC v143 toolset wasn't found. Install the 'Desktop development with C++' workload."
}

# Only Debug|Win32 is fully configured upstream (the framework has no x64 configs,
# and the solution maps Release|Win32 back to Debug).
$target = if ($Rebuild) { 'Rebuild' } else { 'Build' }
$msbuildArgs = @((Join-Path $RepoRoot 'WinFish.sln'), "-t:$target",
    '-p:Configuration=Debug', '-p:Platform=Win32', '-m', '-nologo', '-v:minimal')

# The post-build step copies assets from the repo root next to the exe. Without them
# (e.g. in CI) compile and link only.
if (-not (Test-Path (Join-Path $RepoRoot 'images'))) {
    Write-Warning "No game assets in the repo root, so the post-build copy is skipped. Run .\scripts\setup-assets.ps1 to play."
    $msbuildArgs += '-p:PostBuildEventUseInBuild=false'
}

& $msbuild @msbuildArgs
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

Write-Host "Built $(Join-Path $RepoRoot 'Win32\Debug\Insaniquarium.exe')"
