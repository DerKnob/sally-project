. ".\_setEnv.ps1"

echo "+++ cleanDebugFolder +++"

get-childitem $startDir"sally\sally\Debug\" -include *.ilk -recurse | foreach ($_) { remove-item $_.fullname -verbose -ErrorAction SilentlyContinue }
get-childitem $startDir"sally\sally\Debug\" -include *.pdb -recurse | foreach ($_) { remove-item $_.fullname -verbose -ErrorAction SilentlyContinue }
get-childitem $startDir"sally\sally\Debug\" -include *.exp -recurse | foreach ($_) { remove-item $_.fullname -verbose -ErrorAction SilentlyContinue }
get-childitem $startDir"sally\sally\Debug\applications\" -include *.lib -recurse | foreach ($_) { remove-item $_.fullname -verbose -ErrorAction SilentlyContinue }

echo "+++ cleanDebugFolder - DONE +++"