. ".\_setEnv.ps1"

# empty include folder
remove-item $startDir"sally\__apiInstall\include\include\sallyAPI\*"

copy-item $startDir"sally\sallyAPI\*.h" $startDir"sally\__apiInstall\include\include\sallyAPI\"

copy-item $startDir"sally\Release\sallyAPI.lib" $startDir"sally\__apiInstall\lib\lib\"
copy-item $startDir"sally\Release\sallyAPI.pdb" $startDir"sally\__apiInstall\lib\lib\"