. ".\_setEnv.ps1"

remove-item $startDir"sally\_beta\*" -recurse
remove-item $startDir"sally\__setup\install\*" -recurse

. $startDir"sally\___scripts\cleanDebugFolder.ps1"
. $startDir"sally\___scripts\cleanProjects.ps1"