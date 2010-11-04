. ".\_setEnv.ps1"

# empty beta folder
remove-item $startDir"sally\_beta\*" -recurse -Force

. $startDir"sally\___scripts\cleanDebugFolder.ps1"

#################
# prepare sally #
#################

# copy the resource folders to the _beta folder
copy-item $startDir"sally\sally\Debug\*" $startDir"sally\_beta\" -recurse

# rename the Sally SallyConfig.exe/SallyAdminProcess.exe to SallyConfig.exe.new/SallyAdminProcess.exe.new
move-item $startDir"sally\_beta\SallyConfig.exe" $startDir"sally\_beta\SallyConfig.exe.new"
move-item $startDir"sally\_beta\SallyAdminProcess.exe" $startDir"sally\_beta\SallyAdminProcess.exe.new"

remove-item $startDir"sally\_beta\sallyAPI.lib"

# copy the sally files
copy-item $startDir"sally\sally\Release\sally.exe" $startDir"sally\_beta\" -force
copy-item $startDir"sally\sally\Release\sally.pdb" $startDir"sally\_beta\" -force
copy-item $startDir"sally\sally\Release\sallyApi.dll" $startDir"sally\_beta\" -force
copy-item $startDir"sally\sally\Release\sallyApi.pdb" $startDir"sally\_beta\" -force

########################
# prepare applications #
########################

# remove old pdb files from the plugins
#remove-item $startDir"sallyPlugins\Release\*"  -include *.pdb

# copy new pdb files 2 dirs up
get-childitem $startDir"sally\sallyPlugins\Release\" -include *.pdb -recurse | foreach ($_) { move-item $_.fullname $startDir"sally\sallyPlugins\Release\" -force }

# remove all files created from the build process
get-childitem $startDir"sally\sallyPlugins\Release\" -include *.lib -recurse | foreach ($_) { remove-item $_.fullname }
get-childitem $startDir"sally\sallyPlugins\Release\" -include *.exp -recurse | foreach ($_) { remove-item $_.fullname }


# copy new pdb files 2 dirs up
get-childitem $startDir"sallyPlugins\Release\" -include *.pdb -recurse | foreach ($_) { move-item $_.fullname $startDir"sallyPlugins\Release\" -force }

# remove all files created from the build process
get-childitem $startDir"sallyPlugins\Release\" -include *.lib -recurse | foreach ($_) { remove-item $_.fullname }
get-childitem $startDir"sallyPlugins\Release\" -include *.exp -recurse | foreach ($_) { remove-item $_.fullname }

# copy scummvm plugin
copy-item $startDir"scummvm\trunk\dists\msvc9\Release32\sally.dll" $startDir"sallyPlugins\Release\applications\org.scummvm.sally.app\sally.dll"

# copy the files to the _beta folder
copy-item $startDir"sally\sallyPlugins\Release\*" $startDir"sally\_beta\" -force -recurse

# copy the files to the _beta folder
copy-item $startDir"sallyPlugins\Release\*" $startDir"sally\_beta\" -force -recurse

# create the zip file
#$winrar -afzip $startDir"sally\_beta.zip" $startDir"sally\_beta\*"

# remove all .svn folders
get-childItem $startDir"sally\_beta\" -recurse *.svn | where {$_.PSIsContainer} |  remove-item -recurse -force