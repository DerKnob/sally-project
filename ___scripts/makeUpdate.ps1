. ".\_setEnv.ps1"

# build the beta
#. $startDir"sally\___scripts\buildRelease.ps1"

# rename the Sally SallyConfig.exe/SallyAdminProcess.exe to SallyConfig.exe.new/SallyAdminProcess.exe.new
move-item $startDir"sally\__setup\install\SallyConfig.exe" $startDir"sally\__setup\install\SallyConfig.exe.new"
move-item $startDir"sally\__setup\install\SallyAdminProcess.exe" $startDir"sally\__setup\install\SallyAdminProcess.exe.new"