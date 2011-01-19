. ".\_setEnv.ps1"

move-item $startDir"sally\__setup\install\SallyConfig.exe" $startDir"sally\__setup\install\SallyConfig.exe.new" -verbose
move-item $startDir"sally\__setup\install\SallyAdminProcess.exe" $startDir"sally\__setup\install\SallyAdminProcess.exe.new" -verbose