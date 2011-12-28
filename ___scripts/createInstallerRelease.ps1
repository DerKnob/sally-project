. ".\_setEnv.ps1"

echo "+++ creatInstallerRelease +++"

# empty install folder
echo "-- clearing install folder"
remove-item $startDir"sally\__setup\install" -recurse -ErrorAction SilentlyContinue

# ensure the folder exists
new-item $startDir"sally\__setup\install\" -type directory -verbose

# copy the beta to install
copy-item $startDir"sally\_beta\*" $startDir"sally\__setup\install" -recurse

# rename the Sally SallyConfig.exe.new/SallyAdminProcess.exe.new to SallyConfig.exe/SallyAdminProcess.exe
move-item $startDir"sally\__setup\install\SallyConfig.exe.new" $startDir"sally\__setup\install\SallyConfig.exe" -verbose -ErrorAction SilentlyContinue
move-item $startDir"sally\__setup\install\SallyAdminProcess.exe.new" $startDir"sally\__setup\install\SallyAdminProcess.exe" -verbose -ErrorAction SilentlyContinue

# delete not needed stuff
echo "-- deleting ext apps"
remove-item $startDir"sally\__setup\install\applications\de.der-knob.sally.app.cdvd" -recurse -ErrorAction SilentlyContinue
remove-item $startDir"sally\__setup\install\applications\de.der-knob.sally.app.skype" -recurse -ErrorAction SilentlyContinue
remove-item $startDir"sally\__setup\install\applications\de.der-knob.sally.app.eradio" -recurse -ErrorAction SilentlyContinue
remove-item $startDir"sally\__setup\install\applications\de.der-knob.sally.app.guitartoolbox" -recurse -ErrorAction SilentlyContinue
remove-item $startDir"sally\__setup\install\applications\de.der-knob.sally.app.rabbitisland" -recurse -ErrorAction SilentlyContinue
remove-item $startDir"sally\__setup\install\applications\de.der-knob.sally.app.servermonitor" -recurse -ErrorAction SilentlyContinue
remove-item $startDir"sally\__setup\install\applications\de.der-knob.sally.app.recipebook" -recurse -ErrorAction SilentlyContinue
remove-item $startDir"sally\__setup\install\applications\de.der-knob.sally.app.panic" -recurse -ErrorAction SilentlyContinue
remove-item $startDir"sally\__setup\install\applications\de.der-knob.sally.app.ruthe" -recurse -ErrorAction SilentlyContinue
remove-item $startDir"sally\__setup\install\applications\de.der-knob.sally.app.filebrowser" -recurse -ErrorAction SilentlyContinue
remove-item $startDir"sally\__setup\install\applications\de.der-knob.sally.app.playCD" -recurse -ErrorAction SilentlyContinue
remove-item $startDir"sally\__setup\install\applications\org.scummvm.sally.app" -recurse -ErrorAction SilentlyContinue

echo "-- deleting themes ext"
remove-item $startDir"sally\__setup\install\themes\de.der-knob.sally.theme.clean.old" -recurse -ErrorAction SilentlyContinue
remove-item $startDir"sally\__setup\install\themes\de.der-knob.sally.theme.ilovepuma" -recurse -ErrorAction SilentlyContinue
remove-item $startDir"sally\__setup\install\themes\de.der-knob.sally.theme.whiteui" -recurse -ErrorAction SilentlyContinue
remove-item $startDir"sally\__setup\install\themes\de.der-knob.sally.theme.mattblack" -recurse -ErrorAction SilentlyContinue
remove-item $startDir"sally\__setup\install\themes\de.der-knob.sally.theme.macosx.aqua" -recurse -ErrorAction SilentlyContinue
remove-item $startDir"sally\__setup\install\themes\de.der-knob.sally.theme.blue.noise" -recurse -ErrorAction SilentlyContinue
remove-item $startDir"sally\__setup\install\themes\de.der-knob.sally.theme.black.noise" -recurse -ErrorAction SilentlyContinue
remove-item $startDir"sally\__setup\install\themes\de.der-knob.sally.theme.noxes" -recurse -ErrorAction SilentlyContinue

echo "-- deleting lang files from ext apps"
remove-item $startDir"sally\__setup\install\languages\sally-project.language.de\de.der-knob.sally.app.cdvd.lang" -ErrorAction SilentlyContinue
remove-item $startDir"sally\__setup\install\languages\sally-project.language.de\de.der-knob.sally.app.skype.lang" -ErrorAction SilentlyContinue
remove-item $startDir"sally\__setup\install\languages\sally-project.language.de\de.der-knob.sally.app.eradio.lang" -ErrorAction SilentlyContinue
remove-item $startDir"sally\__setup\install\languages\sally-project.language.de\de.der-knob.sally.app.guitartoolbox.lang" -ErrorAction SilentlyContinue
remove-item $startDir"sally\__setup\install\languages\sally-project.language.de\de.der-knob.sally.app.partywebcam.lang" -ErrorAction SilentlyContinue
remove-item $startDir"sally\__setup\install\languages\sally-project.language.de\de.der-knob.sally.app.rabbitisland.lang" -ErrorAction SilentlyContinue
remove-item $startDir"sally\__setup\install\languages\sally-project.language.de\de.der-knob.sally.app.servermonitor.lang" -ErrorAction SilentlyContinue
remove-item $startDir"sally\__setup\install\languages\sally-project.language.de\de.der-knob.sally.app.recipebook.lang" -ErrorAction SilentlyContinue
remove-item $startDir"sally\__setup\install\languages\sally-project.language.de\de.der-knob.sally.app.cdvd.lang" -ErrorAction SilentlyContinue
remove-item $startDir"sally\__setup\install\languages\sally-project.language.de\org.scummvm.sally.app.lang" -ErrorAction SilentlyContinue

remove-item $startDir"sally\__setup\install\languages\sally-project.language.fr" -recurse -ErrorAction SilentlyContinue

# remove pdbs
echo "-- deleting pdb files plugins"
get-childitem $startDir"sally\__setup\" -include *.pdb -recurse -verbose | foreach ($_) { remove-item $_.fullname }

# copy the pdbs for the sally main apps
copy-item $startDir"sally\sally\Release\sally.pdb" $startDir"sally\__setup\install\" -verbose -force
copy-item $startDir"sally\sally\Release\sallyApi.pdb" $startDir"sally\__setup\install\" -verbose -force

echo "+++ creatInstallerRelease - DONE +++"