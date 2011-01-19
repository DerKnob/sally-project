. ".\_setEnv.ps1"

move-item $startDir"sally\__setup\install\SallyConfig.exe.new" $startDir"sally\__setup\install\SallyConfig.exe" -verbose
move-item $startDir"sally\__setup\install\SallyAdminProcess.exe.new" $startDir"sally\__setup\install\SallyAdminProcess.exe" -verbose