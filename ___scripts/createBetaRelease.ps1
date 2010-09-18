. ".\_setEnv.ps1"

# empty beta folder
remove-item $startDir"sally\__beta\*" -recurse

. $startDir"sally\___scripts\cleanDebugFolder.ps1"

#################
# prepare sally #
#################

# copy the resource folders to the __beta folder
copy-item $startDir"sally\Debug\*" $startDir"sally\__beta\" -recurse

# rename the Sally SallyConfig.exe/SallyAdminProcess.exe to SallyConfig.exe.new/SallyAdminProcess.exe.new
move-item $startDir"sally\__beta\SallyConfig.exe" $startDir"sally\__beta\SallyConfig.exe.new"
move-item $startDir"sally\__beta\SallyAdminProcess.exe" $startDir"sally\__beta\SallyAdminProcess.exe.new"

remove-item $startDir"sally\__beta\sallyAPI.lib"

# copy the sally files
copy-item $startDir"sally\Release\sally.exe" $startDir"sally\__beta\" -force
copy-item $startDir"sally\Release\sally.pdb" $startDir"sally\__beta\" -force
copy-item $startDir"sally\Release\sallyApi.dll" $startDir"sally\__beta\" -force
copy-item $startDir"sally\Release\sallyApi.pdb" $startDir"sally\__beta\" -force

########################
# prepare applications #
########################

# remove old pdb files from the plugins
#remove-item $startDir"sallyPlugins\Release\*"  -include *.pdb

# copy new pdb files 2 dirs up
get-childitem $startDir"sallyPlugins\Release\" -include *.pdb -recurse | foreach ($_) { move-item $_.fullname $startDir"sallyPlugins\Release\" -force }

# remove all files created from the build process
get-childitem $startDir"sallyPlugins\Release\" -include *.lib -recurse | foreach ($_) { remove-item $_.fullname }
get-childitem $startDir"sallyPlugins\Release\" -include *.exp -recurse | foreach ($_) { remove-item $_.fullname }

# copy scummvm plugin
copy-item $startDir"scummvm\trunk\dists\msvc9\Release32\sally.dll" $startDir"\sallyPlugins\Release\applications\org.scummvm.sally.app\sally.dll"

# copy the files to the __beta folder
copy-item $startDir"sallyPlugins\Release\*" $startDir"sally\__beta\" -force -recurse

# create the zip file
#$winrar -afzip $startDir"sally\__beta.zip" $startDir"sally\__beta\*"