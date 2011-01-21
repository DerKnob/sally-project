. ".\_setEnv.ps1"

echo "+++ cleanAll +++"

# cleanup the install dirs
remove-item $startDir"sally\_beta" -recurse -verbose -ErrorAction SilentlyContinue
remove-item $startDir"sally\__setup\install\*" -recurse -verbose -ErrorAction SilentlyContinue
remove-item $startDir"sally\__setup\Output\*" -recurse -verbose -ErrorAction SilentlyContinue

# cleanup the api
remove-item $startDir"sally\__apiInstall\include" -recurse -verbose -ErrorAction SilentlyContinue
remove-item $startDir"sally\__apiInstall\lib" -recurse -verbose -ErrorAction SilentlyContinue

. $startDir"sally\___scripts\cleanDebugFolder.ps1"
. $startDir"sally\___scripts\cleanProjects.ps1"

remove-item $startDir"sally\sally\sally.ncb" -verbose -force -ErrorAction SilentlyContinue
remove-item $startDir"sally\sally\sally.suo" -verbose -force -ErrorAction SilentlyContinue
remove-item $startDir"sally\sally\Release" -recurse -force -verbose -ErrorAction SilentlyContinue

remove-item $startDir"sally\sallyPlugins\sally Plugins.ncb" -verbose -force -ErrorAction SilentlyContinue
remove-item $startDir"sally\sallyPlugins\sally Plugins.suo" -verbose -force -ErrorAction SilentlyContinue
remove-item $startDir"sally\sallyPlugins\Release" -recurse -force -verbose -ErrorAction SilentlyContinue

remove-item $startDir"sally\sallyTools\sally Tools.ncb" -verbose -force -ErrorAction SilentlyContinue
remove-item $startDir"sally\sallyTools\sally Tools.suo" -verbose -force -ErrorAction SilentlyContinue
remove-item $startDir"sally\sallyTools\Release" -recurse -force -verbose -ErrorAction SilentlyContinue
remove-item $startDir"sally\sallyTools\Debug" -recurse -force -verbose -ErrorAction SilentlyContinue

remove-item $startDir"sallyPlugins\sally.ncb" -verbose -force -ErrorAction SilentlyContinue
remove-item $startDir"sallyPlugins\sally.suo" -verbose -force -ErrorAction SilentlyContinue
remove-item $startDir"sallyPlugins\Release" -recurse -force -verbose -ErrorAction SilentlyContinue

remove-item $startDir"sally\*.user" -recurse -verbose -force -ErrorAction SilentlyContinue
remove-item $startDir"sallyPlugins\*.user" -recurse -verbose -force -ErrorAction SilentlyContinue

echo "+++ cleanAll - DONE +++"