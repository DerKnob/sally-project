. ".\_setEnv.ps1"

# build the beta
#. $startDir"sally\___scripts\buildRelease.ps1"

# rename the Sally SallyConfig.exe/SallyAdminProcess.exe to SallyConfig.exe.new/SallyAdminProcess.exe.new
move-item $startDir"sally\__setup\install\SallyConfig.exe.new" $startDir"sally\__setup\install\SallyConfig.exe"
move-item $startDir"sally\__setup\install\SallyAdminProcess.exe.new" $startDir"sally\__setup\install\SallyAdminProcess.exe"