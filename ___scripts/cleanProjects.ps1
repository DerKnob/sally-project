. ".\_setEnv.ps1"

echo "+++ cleanProjects +++"

# cleanup
cmd /c start /wait $devenv $startDir"sally\sally\sally.sln" /clean Debug
cmd /c start /wait $devenv $startDir"sally\sally\sally.sln" /clean Release
cmd /c start /wait $devenv $startDir"sally\sallyPlugins\sally Plugins.sln" /clean Debug
cmd /c start /wait $devenv $startDir"sally\sallyPlugins\sally Plugins.sln" /clean Release
cmd /c start /wait $devenv $startDir"sallyPlugins\sally Plugins.sln" /clean Debug
cmd /c start /wait $devenv $startDir"sallyPlugins\sally Plugins.sln" /clean Release
cmd /c start /wait $devenv $startDir"scummvm\trunk\dists\msvc9\scummvm.sln" /project sally /projectconfig Debug /clean Debug
cmd /c start /wait $devenv $startDir"scummvm\trunk\dists\msvc9\scummvm.sln" /project sally /projectconfig Release /clean Release

get-childitem $startDir"sallyPlugins\Release\" -include *.pdb -recurse -ErrorAction SilentlyContinue | foreach ($_) { remove-item $_.fullname -verbose -ErrorAction SilentlyContinue }
get-childitem $startDir"sallyPlugins\Release\" -include *.dll -recurse -ErrorAction SilentlyContinue | foreach ($_) { remove-item $_.fullname -verbose -ErrorAction SilentlyContinue }

echo "+++ cleanProjects - DONE +++"