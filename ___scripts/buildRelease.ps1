. ".\_setEnv.ps1"

echo "+++ buildRelease +++"

# clean the vc projects
. ".\cleanProjects.ps1"

# updating plugin ext resources
echo "-- update plugin resources for SVN checkin"
copy-item $startDir"sally\sally\Debug\applications\de.der-knob.sally.app.eradio\*" $startDir"sallyPlugins\plugins\eRadio\Resources\" -recurse -force -verbose -ErrorAction SilentlyContinue
copy-item $startDir"sally\sally\Debug\applications\de.der-knob.sally.app.guitartoolbox\*" $startDir"sallyPlugins\plugins\GuitarToolbox\Resources\" -recurse -force -verbose -ErrorAction SilentlyContinue
copy-item $startDir"sally\sally\Debug\applications\de.der-knob.sally.app.servermonitor\*" $startDir"sallyPlugins\plugins\ServerMonitor\Resources\" -recurse -force -verbose -ErrorAction SilentlyContinue
copy-item $startDir"sally\sally\Debug\applications\de.der-knob.sally.app.recipebook\*" $startDir"sallyPlugins\plugins\RecipeBook\Resources\" -recurse -force -verbose -ErrorAction SilentlyContinue
copy-item $startDir"sally\sally\Debug\applications\de.der-knob.sally.app.rabbitisland\*" $startDir"sallyPlugins\plugins\RabbitIsland\Resources\" -recurse -force -verbose -ErrorAction SilentlyContinue
copy-item $startDir"sally\sally\Debug\applications\de.der-knob.sally.app.skype\*" $startDir"sallyPlugins\plugins\Skype\Resources\" -recurse -force -verbose -ErrorAction SilentlyContinue
copy-item $startDir"sally\sally\Debug\applications\de.der-knob.sally.app.cdvd\*" $startDir"sallyPlugins\plugins\cDVD\Resources\" -recurse -force -verbose -ErrorAction SilentlyContinue
copy-item $startDir"sally\sally\Debug\applications\de.der-knob.sally.app.panic\*" $startDir"sallyPlugins\plugins\Panic!\Resources\" -recurse -force -verbose -ErrorAction SilentlyContinue

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