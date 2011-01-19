. ".\_setEnv.ps1"

echo "+++ buildDebug +++"

# clean the vc projects
. ".\cleanProjects.ps1"

# build
echo "-- build Sally Debug"
cmd /c start /wait $devenv $startDir"sally\sally\sally.sln" /build Debug
echo "-- build Sally Plugins Debug"
cmd /c start /wait $devenv $startDir"sally\sallyPlugins\sally Plugins.sln" /build Debug
echo "-- build Sally Plugins Ext Debug"
cmd /c start /wait $devenv $startDir"sallyPlugins\sally Plugins.sln" /build Debug
echo "-- build ScummVM Debug"
cmd /c start /wait $devenv $startDir"scummvm\trunk\dists\msvc9\scummvm.sln" /project sally /projectconfig Debug /build Debug

echo "+++ buildDebug - DONE +++"