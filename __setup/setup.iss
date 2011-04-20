; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
AppId={{44047E88-E028-4C15-88D7-67D5B542C094}
AppName=Sally Project
AppVerName=Sally Project 0.7.0.2
AppPublisher=Sally Project
AppPublisherURL=http://www.sally-project.org
AppSupportURL=http://www.sally-project.org
AppUpdatesURL=http://www.sally-project.org
DefaultDirName={pf}\Sally Project
DefaultGroupName=Sally Project
AllowNoIcons=yes
LicenseFile=text\lizenz.txt
InfoBeforeFile=Install\changelog.txt
OutputBaseFilename=sally
Compression=lzma
SolidCompression=yes
WizardImageFile=WizModernImage-IS.bmp
WizardSmallImageFile=WizModernSmallImage-IS.bmp
UninstallDisplayIcon={app}\sally.exe
ChangesAssociations=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "german"; MessagesFile: "compiler:Languages\German.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}";
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[InstallDelete]
Type: filesandordirs; Name: "{app}\themes\Clean"
Type: filesandordirs; Name: "{app}\themes\Clean (old)"
Type: filesandordirs; Name: "{app}\themes\I Love Puma"
Type: filesandordirs; Name: "{app}\keyboard"
Type: filesandordirs; Name: "{app}\lang"
Type: filesandordirs; Name: "{app}\plugins"
Type: files; Name: "{app}\SallyExtInstaller.exe"

[Files]
Source: "dxwebsetup.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "vcredist_x86.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "install\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

[INI]
Filename: "{app}\sally.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://www.sally-project.org"

[Icons]
Name: "{group}\Sally Project"; Filename: "{app}\sally.exe"
Name: "{group}\Sally Project Config"; Filename: "{app}\SallyConfig.exe"
Name: "{group}\{cm:ProgramOnTheWeb,Sally Project}"; Filename: "{app}\sally.url"
Name: "{group}\{cm:UninstallProgram,Sally Project}"; Filename: "{uninstallexe}"
Name: "{userdesktop}\Sally Project"; Filename: "{app}\sally.exe"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\Sally Project"; Filename: "{app}\sally.exe"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\dxwebsetup.exe"; StatusMsg: "Installing DirectX..."; Flags: hidewizard
Filename: "{app}\vcredist_x86.exe"; Parameters: "/q"; StatusMsg: "Installing VC++ Runtime...";
Filename: "{app}\SallyAdminProcess.exe"; Parameters: "mimetypes"; StatusMsg: "Installing Mimetypes..."; Flags: hidewizard
Filename: "{app}\SallyConfig.exe"; Flags: hidewizard
