. ".\_setEnv.ps1"

echo "+++ creatBetaRelease +++"

# empty beta folder
echo "-- clearing _beta folder"
remove-item $startDir"sally\_beta" -recurse -ErrorAction SilentlyContinue

# ensure the folder exists
new-item $startDir"sally\_beta\" -type directory -verbose

. $startDir"sally\___scripts\cleanDebugFolder.ps1"

#################
# prepare sally #
#################

# copy the resource folders to the _beta folder
echo "-- copy Debug to _beta folder"
copy-item $startDir"sally\sally\Debug\*" $startDir"sally\_beta\" -recurse

# rename the Sally SallyConfig.exe/SallyAdminProcess.exe to SallyConfig.exe.new/SallyAdminProcess.exe.new
move-item $startDir"sally\_beta\SallyConfig.exe" $startDir"sally\_beta\SallyConfig.exe.new" -verbose
move-item $startDir"sally\_beta\SallyAdminProcess.exe" $startDir"sally\_beta\SallyAdminProcess.exe.new" -verbose

remove-item $startDir"sally\_beta\sallyAPI.lib" -verbose

# copy the sally files
echo "-- copy Release files to _beta folder"
copy-item $startDir"sally\sally\Release\sally.exe" $startDir"sally\_beta\" -force -verbose
copy-item $startDir"sally\sally\Release\sally.pdb" $startDir"sally\_beta\" -force -verbose
copy-item $startDir"sally\sally\Release\sallyApi.dll" $startDir"sally\_beta\" -force -verbose
copy-item $startDir"sally\sally\Release\sallyApi.pdb" $startDir"sally\_beta\" -force -verbose

########################
# prepare applications #
########################

# copy new pdb files 2 dirs up
get-childitem $startDir"sally\sallyPlugins\Release\" -include *.pdb -recurse -ErrorAction SilentlyContinue | foreach ($_) { move-item $_.fullname $startDir"sally\sallyPlugins\Release\" }

# remove all files created from the build process
get-childitem $startDir"sally\sallyPlugins\Release\" -include *.lib -recurse -ErrorAction SilentlyContinue | foreach ($_) { remove-item $_.fullname }
get-childitem $startDir"sally\sallyPlugins\Release\" -include *.exp -recurse -ErrorAction SilentlyContinue | foreach ($_) { remove-item $_.fullname }


# copy new pdb files 2 dirs up
get-childitem $startDir"sallyPlugins\Release\" -include *.pdb -recurse -ErrorAction SilentlyContinue | foreach ($_) { move-item $_.fullname $startDir"sallyPlugins\Release\" -force -verbose -ErrorAction SilentlyContinue }

# remove all files created from the build process
get-childitem $startDir"sallyPlugins\Release\" -include *.lib -recurse -ErrorAction SilentlyContinue | foreach ($_) { remove-item $_.fullname -verbose -ErrorAction SilentlyContinue }
get-childitem $startDir"sallyPlugins\Release\" -include *.exp -recurse -ErrorAction SilentlyContinue | foreach ($_) { remove-item $_.fullname -verbose -ErrorAction SilentlyContinue }

# copy scummvm plugin
if(test-path $startDir"scummvm\trunk\dists\msvc9\Release32\sally.dll")
{
	new-item $startDir"sallyPlugins\Release\applications\org.scummvm.sally.app\" -type directory -verbose -ErrorAction SilentlyContinue
	copy-item $startDir"scummvm\trunk\dists\msvc9\Release32\sally.dll" $startDir"sallyPlugins\Release\applications\org.scummvm.sally.app\sally.dll" -verbose -ErrorAction SilentlyContinue
}

# copy the files to the _beta folder
copy-item $startDir"sally\sallyPlugins\Release\*" $startDir"sally\_beta\" -force -recurse

# copy the files to the _beta folder
copy-item $startDir"sallyPlugins\Release\*" $startDir"sally\_beta\" -force -recurse -ErrorAction SilentlyContinue

# remove all .svn folders
get-childItem $startDir"sally\_beta\" -recurse *.svn | where {$_.PSIsContainer} |  remove-item -recurse -force

echo "+++ creatBetaRelease - DONE +++"