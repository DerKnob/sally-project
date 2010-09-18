. ".\_setEnv.ps1"

echo "cleanProjects.ps1"
. ".\cleanProjects.ps1"

echo "update plugin resources for SVN checkin"
#copy-item $startDir"sallyPlugins\plugins\eRadio\Resources\*" -recurse
copy-item $startDir"sally\Debug\applications\de.der-knob.sally.app.eradio\*" $startDir"sallyPlugins\plugins\eRadio\Resources\" -recurse -force

#copy-item $startDir"sallyPlugins\plugins\GuitarToolbox\Resources\*" -recurse
copy-item $startDir"sally\Debug\applications\de.der-knob.sally.app.guitartoolbox\*" $startDir"sallyPlugins\plugins\GuitarToolbox\Resources\" -recurse -force

#copy-item $startDir"sallyPlugins\plugins\PartyWebCam\Resources\*" -recurse
copy-item $startDir"sally\Debug\applications\de.der-knob.sally.app.partywebcam\*" $startDir"sallyPlugins\plugins\PartyWebCam\Resources\" -recurse -force

#copy-item $startDir"sallyPlugins\plugins\ServerMonitor\Resources\*" -recurse
copy-item $startDir"sally\Debug\applications\de.der-knob.sally.app.servermonitor\*" $startDir"sallyPlugins\plugins\ServerMonitor\Resources\" -recurse -force

#copy-item $startDir"sallyPlugins\plugins\RabbitIsland\Resources\*" -recurse
copy-item $startDir"sally\Debug\applications\de.der-knob.sally.app.rabbitisland\*" $startDir"sallyPlugins\plugins\RabbitIsland\Resources\" -recurse -force

#copy-item $startDir"sallyPlugins\plugins\Skype\Resources\*" -recurse
copy-item $startDir"sally\Debug\applications\de.der-knob.sally.app.skype\*" $startDir"sallyPlugins\plugins\Skype\Resources\" -recurse -force

# build
echo "build SallyAPI Debug"
cmd /c start /wait $devenv $startDir"sally\sally.sln" /project sallyAPI /projectconfig Debug /build Debug

echo "build Sally Release"
cmd /c start /wait $devenv $startDir"sally\sally.sln" /build Release
echo "build Sally Plugins Release"
cmd /c start /wait $devenv $startDir"sallyPlugins\sally Plugins.sln" /build Release
echo "build ScummVM Release"
cmd /c start /wait $devenv $startDir"scummvm\release-1-1-1\dists\msvc9\scummvm.sln" /project sally /projectconfig Release /build Release