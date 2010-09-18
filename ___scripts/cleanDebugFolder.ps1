. ".\_setEnv.ps1"

get-childitem $startDir"sally\Debug\" -include *.ilk -recurse | foreach ($_) { remove-item $_.fullname }
get-childitem $startDir"sally\Debug\" -include *.pdb -recurse | foreach ($_) { remove-item $_.fullname }
get-childitem $startDir"sally\Debug\" -include *.exp -recurse | foreach ($_) { remove-item $_.fullname }
get-childitem $startDir"sally\Debug\applications\" -include *.lib -recurse | foreach ($_) { remove-item $_.fullname }