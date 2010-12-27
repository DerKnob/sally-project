////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	SallyConfig\UIDefine.h
///
/// \brief	Declares the user interface defines. 
///
/// \author	Christian Knobloch
/// \date	13.09.2010
///
/// This file is part of the Sally Project
/// 
/// Copyright(c) 2008-2010 Sally Project
/// http://www.sally-project.org/
///
/// This program is free software: you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
/// 
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
/// GNU General Public License for more details.
/// 
/// You should have received a copy of the GNU General Public License
/// along with this program. If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////////////////////////

#define PBS_MARQUEE             0x08
#define PBM_SETMARQUEE          (WM_USER+10)

#define MENU_WIDTH			128
#define TOP_BORDER_HEIGHT	60

SallyAPI::System::COption*	option;
LPDIRECT3D9					m_lpD3D;

typedef struct _ADAPTERINFO {
	D3DADAPTER_IDENTIFIER9 adapter;            
	UINT                   dismode_count;		 
	D3DDISPLAYMODE         dismodes[255];   
}ADAPTERINFO;

_ADAPTERINFO		Adapterinfo[10];

#define EXTENSION_TYPE_APPLICATION	1
#define EXTENSION_TYPE_THEME		2
#define EXTENSION_TYPE_LANGUEAGE	3
#define EXTENSION_TYPE_SALLY		4
#define EXTENSION_TYPE_KEYBOARD		5

#define INSTALL_NONE				0
#define INSTALL_MIMETYPES			10
#define INSTALL_UPDATE				20
#define INSTALL_FILE_INSTALL		30

HWND						hMainWindow;

std::string					strExtensionFile;
int							extensionType = INSTALL_NONE;
HWND						hList;
HWND						hOk;
HWND						hCancel;
HWND						hProcessbar;
HWND						hWait;
ListView					hInstallPluginView;
HANDLE						hThread;
std::map<int, std::string>	installAppMap;
std::string					searchUpdates;

HINSTANCE	hInstance;
HICON		hIcon;
HICON		hIconConfig;
HICON		hIconApplication;
HICON		hIconLanguage;
HICON		hIconKeyboard;
HICON		hIconTheme;
HIMAGELIST	himl;

ListView	hMenuView;
ListView	hPluginView;
ListView	hThemeView;
ListView	hLanguageView;
ListView	hKeyboardView;

HWND		hWndTabConfig;
HWND		hWndTabPlugins;
HWND		hWndTabLanguages;
HWND		hWndTabKeyboards;
HWND		hWndTabThemes;

std::map<int, std::string>	appMap;
std::map<int, std::string>	themeMap;
std::map<int, std::string>	languageMap;
std::map<int, std::string>	keyboardMap;

std::map<std::string, int>	deviceList;

bool bInstallError = false;
bool bSkipUpdate = false;