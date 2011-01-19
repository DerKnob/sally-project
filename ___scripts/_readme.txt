These scripts will help you to create a sally release.
All scripts are PowerShell scripts. More information here:

http://en.wikipedia.org/wiki/Windows_PowerShell
http://www.microsoft.com/powershell

You should make your enviroment settings in the _setEnv.ps1 file.

buildDebug.ps1					creates all projects as a debug build
buildRelease.ps1				creates all projects as a release build
cleanAll.ps1					deletes all not necessary files in these folders _beta, __apiInstall, _setup. .ncb, .user files. Also calls the cleanDebugFolder.ps1 and cleanProjects.ps1
cleanDebugFolder.ps1			cleans the debug output folder
cleanProjects.ps1				cleans the visual studio projects - the debug and the release build
createApiInstaller.ps1			creates the API install - take a look at the __apiInstall when done
createBetaRelease.ps1			create a beta release. Can be found in the _beta folder. call buildRelease.ps1 and all dependencies before!
createInstallerRelease.ps1		create a installer release. Can be found in the __setup/install folder. call buildRelease.ps1 and all dependencies before!
makeRelease.ps1					rolls the makeUpdate.ps1 changes back
makeUpdate.ps1					renames files in the __setup/install folder to make a sally update version. call createInstallerRelease.ps1 and all dependencies before!

To make a complete build:
1.	buildRelease.ps1
2.	createBetaRelease.ps1
3.	zip the _beta content to get the beta update (only the content of this folder)
4.	createInstallerRelease.ps1
5.	execute the __setup/setup.iss to get an new installer (use Inno Setup: http://www.innosetup.com/)
6.	execute makeUpdate.p1
7.	zip the __setup/install folder to get the update (only the content of this folder)