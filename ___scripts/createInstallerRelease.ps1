. ".\_setEnv.ps1"

# build the beta
# . $startDir"sally\___scripts\createBetaRelease.ps1"

# empty install folder
remove-item $startDir"sally\__setup\install\*" -recurse

# copy the beta to install
copy-item $startDir"sally\_beta\*" $startDir"sally\__setup\install" -recurse

# rename the Sally SallyConfig.exe.new/SallyAdminProcess.exe.new to SallyConfig.exe/SallyAdminProcess.exe
move-item $startDir"sally\__setup\install\SallyConfig.exe.new" $startDir"sally\__setup\install\SallyConfig.exe"
move-item $startDir"sally\__setup\install\SallyAdminProcess.exe.new" $startDir"sally\__setup\install\SallyAdminProcess.exe"

# delete not needed stuff
remove-item $startDir"sally\__setup\install\applications\de.der-knob.sally.app.cdvd" -recurse
remove-item $startDir"sally\__setup\install\applications\de.der-knob.sally.app.skype" -recurse
remove-item $startDir"sally\__setup\install\applications\de.der-knob.sally.app.eradio" -recurse
remove-item $startDir"sally\__setup\install\applications\de.der-knob.sally.app.guitartoolbox" -recurse
remove-item $startDir"sally\__setup\install\applications\de.der-knob.sally.app.partywebcam" -recurse
remove-item $startDir"sally\__setup\install\applications\de.der-knob.sally.app.rabbitisland" -recurse
remove-item $startDir"sally\__setup\install\applications\de.der-knob.sally.app.servermonitor" -recurse
remove-item $startDir"sally\__setup\install\applications\org.scummvm.sally.app" -recurse

remove-item $startDir"sally\__setup\install\themes\de.der-knob.sally.theme.clean.old" -recurse
remove-item $startDir"sally\__setup\install\themes\de.der-knob.sally.theme.ilovepuma" -recurse
remove-item $startDir"sally\__setup\install\themes\de.der-knob.sally.theme.whiteui" -recurse
remove-item $startDir"sally\__setup\install\themes\de.der-knob.sally.theme.mattblack" -recurse

remove-item $startDir"sally\__setup\install\languages\sally-project.language.de\de.der-knob.sally.app.skype.lang"
remove-item $startDir"sally\__setup\install\languages\sally-project.language.de\de.der-knob.sally.app.eradio.lang"
remove-item $startDir"sally\__setup\install\languages\sally-project.language.de\de.der-knob.sally.app.guitartoolbox.lang"
remove-item $startDir"sally\__setup\install\languages\sally-project.language.de\de.der-knob.sally.app.partywebcam.lang"
remove-item $startDir"sally\__setup\install\languages\sally-project.language.de\de.der-knob.sally.app.rabbitisland.lang"
remove-item $startDir"sally\__setup\install\languages\sally-project.language.de\de.der-knob.sally.app.servermonitor.lang"
remove-item $startDir"sally\__setup\install\languages\sally-project.language.de\org.scummvm.sally.app.lang"

# remove pdbs
get-childitem $startDir"sally\__setup\" -include *.pdb -recurse | foreach ($_) { remove-item $_.fullname }

# copy the pdbs for the sally main apps
copy-item $startDir"sally\sally\Release\sally.pdb" $startDir"sally\__setup\install\" -force
copy-item $startDir"sally\sally\Release\sallyApi.pdb" $startDir"sally\__setup\install\" -force