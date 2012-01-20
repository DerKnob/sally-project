. ".\_setEnv.ps1"

echo "+++ buildRelease +++"

# clean the vc projects
. ".\cleanProjects.ps1"

# build
echo "-- build SallyAPI Debug"
cmd /c start /wait $devenv $startDir"sally\sally\sally.sln" /project sallyAPI /projectconfig Debug /build Debug

echo "-- build Sally Release"
cmd /c start /wait $devenv $startDir"sally\sally\sally.sln" /build Release
echo "-- build Sally Plugins Release"
cmd /c start /wait $devenv $startDir"sally\sallyPlugins\sally Plugins.sln" /build Release
echo "-- build Sally Plugins Ext Release"
cmd /c start /wait $devenv $startDir"sallyPlugins\sally Plugins.sln" /build Release
echo "-- build ScummVM Release"
cmd /c start /wait $devenv $startDir"scummvm\trunk\dists\msvc9\scummvm.sln" /project sally /projectconfig Release /build Release

echo "+++ buildRelease - DONE +++"