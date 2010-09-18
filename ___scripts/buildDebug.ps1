. ".\_setEnv.ps1"

echo "cleanProjects.ps1"
. ".\cleanProjects.ps1"

# build the debug
echo "build Sally Debug"
cmd /c start /wait $devenv $startDir"sally\sally.sln" /build Debug
echo "build Sally Plugins Debug"
cmd /c start /wait $devenv $startDir"sallyPlugins\sally Plugins.sln" /build Debug
echo "build ScummVM Debug"
cmd /c start /wait $devenv $startDir"scummvm\release-1-1-1\dists\msvc9\scummvm.sln" /project sally /projectconfig Debug /build Debug