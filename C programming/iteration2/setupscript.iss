; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
AppName=CEDAR Logic Simulator
AppVerName=CEDARLS 1.1
AppPublisher=Cedarville University
AppPublisherURL=http://www.cedarville.edu
AppSupportURL=http://www.cedarville.edu
AppUpdatesURL=http://www.cedarville.edu
DefaultDirName={pf}\CEDAR Logic
DefaultGroupName=CEDAR Logic
AllowNoIcons=yes
OutputDir=C:\proj\projects\wxWidgetsBasePrj\setup
OutputBaseFilename=cedarlssetup
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "C:\proj\projects\wxWidgetsBasePrj\Release\cedarls.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\proj\projects\wxWidgetsBasePrj\GUI\TestGates.lib"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\proj\projects\wxWidgetsBasePrj\GUI\times_new1.glf"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\proj\projects\wxWidgetsBasePrj\GUI\license.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\proj\projects\wxWidgetsBasePrj\KLS_Logic.chm"; DestDir: "{app}"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\CEDAR Logic"; Filename: "{app}\cedarls.exe"; WorkingDir: "{app}"
Name: "{group}\License"; Filename: "{app}\license.txt"
Name: "{group}\Uninstall CEDAR Logic"; Filename: "{uninstallexe}"
Name: "{userdesktop}\CEDAR Logic"; Filename: "{app}\cedarls.exe"; Tasks: desktopicon ; WorkingDir: "{app}"
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\CEDAR Logic"; Filename: "{app}\cedarls.exe"; Tasks: quicklaunchicon ; WorkingDir: "{app}"

[Run]
Filename: "{app}\cedarls.exe"; Description: "{cm:LaunchProgram,CEDAR Logic}"; Flags: nowait postinstall skipifsilent

