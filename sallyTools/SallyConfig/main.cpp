////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	SallyConfig\main.cpp
///
/// \brief	Implements the main function. 
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

#if defined _M_IX86 
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"") 
#elif defined _M_IA64 
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"") 
#elif defined _M_X64 
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"") 
#else 
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"") 
#endif

#include <windows.h>
#include <shlobj.h>
#include <commctrl.h>
#include <shellapi.h>
#include <string>
#include <map>

// Direct3D includes
#include <d3d9.h>
#include <d3dx9.h>

#include "../_shared/SystemHelper.h"
#include "../_shared/StringHelper.h"
#include "../_shared/FileHelper.h"
#include "../_shared/Option.h"
#include "../_shared/ListView.h"
#include "resource.h"
#include "UIDefine.h"
#include "../_shared/UIHelper.h"
#include "../_shared/FunctionHelper.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "comctl32.lib")

#include "ConfigHelper.h"
#include "PluginsHelper.h"
#include "ThemesHelper.h"
#include "LanguagesHelper.h"
#include "KeyboardsHelper.h"

#include "DlgProcConfig.h"
#include "DlgProcPlugins.h"
#include "DlgProcThemes.h"
#include "DlgProcLanguages.h"
#include "DlgProcKeyboards.h"
#include "DlgInstallerProc.h"

void SelectMenu(int selectedItem = -1)
{
	if (selectedItem == -1)
	{
		selectedItem = ListView_GetSelectionMark(hMenuView.GetHwnd());
		if (selectedItem == -1)
			return;
	}

	switch (selectedItem)
	{
	case 0:
		ShowWindow(hWndTabConfig, SW_SHOW);
		ShowWindow(hWndTabPlugins, SW_HIDE);
		ShowWindow(hWndTabThemes, SW_HIDE);
		ShowWindow(hWndTabLanguages, SW_HIDE);
		ShowWindow(hWndTabKeyboards, SW_HIDE);
		break;
	case 1:
		ShowWindow(hWndTabConfig, SW_HIDE);
		ShowWindow(hWndTabPlugins, SW_SHOW);
		ShowWindow(hWndTabThemes, SW_HIDE);
		ShowWindow(hWndTabLanguages, SW_HIDE);
		ShowWindow(hWndTabKeyboards, SW_HIDE);
		break;
	case 2:
		ShowWindow(hWndTabConfig, SW_HIDE);
		ShowWindow(hWndTabPlugins, SW_HIDE);
		ShowWindow(hWndTabThemes, SW_SHOW);
		ShowWindow(hWndTabLanguages, SW_HIDE);
		ShowWindow(hWndTabKeyboards, SW_HIDE);
		break;
	case 3:
		ShowWindow(hWndTabConfig, SW_HIDE);
		ShowWindow(hWndTabPlugins, SW_HIDE);
		ShowWindow(hWndTabThemes, SW_HIDE);
		ShowWindow(hWndTabLanguages, SW_SHOW);
		ShowWindow(hWndTabKeyboards, SW_HIDE);
		break;
	case 4:
		ShowWindow(hWndTabConfig, SW_HIDE);
		ShowWindow(hWndTabPlugins, SW_HIDE);
		ShowWindow(hWndTabThemes, SW_HIDE);
		ShowWindow(hWndTabLanguages, SW_HIDE);
		ShowWindow(hWndTabKeyboards, SW_SHOW);
		break;
	}
}

BOOL CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static LPNMTTDISPINFO		lpnmtdi;
	static PAINTSTRUCT			ps;
	static RECT					windowRect;
	static HDC					hdc;

	switch (message)
	{
	case WM_DWMCOMPOSITIONCHANGED:
		ActivateGlass(hDlg);
		InvalidateRect(hDlg, NULL, true);
		break;
	case WM_ACTIVATE:
		ActivateGlass(hDlg);
		break;
	case WM_INITDIALOG:
		hInstance = (HINSTANCE) GetWindowLong (hDlg, GWL_HINSTANCE);

		// Set Icon
		hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
		hIconWindow = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_MENU_WINDOW));
		hIconConfig = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_MENU_CONFIG));
		hIconApplication = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_PLUGIN_APPLICATION));
		hIconLanguage = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_PLUGIN_LANGUAGE));
		hIconTheme = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_PLUGIN_THEME));
		hIconKeyboard = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_PLUGIN_KEYBORD));

		SendMessage(hDlg, WM_SETICON, ICON_SMALL, (LPARAM) hIcon);
		SendMessage(hDlg, WM_SETICON, ICON_BIG, (LPARAM) hIcon);
		
		// Menu
		himl = ImageList_Create(16, 16, ILC_COLOR32, 6, 0);
		ImageList_SetBkColor(himl, CLR_NONE);
		ImageList_AddIcon(himl, hIconWindow);
		ImageList_AddIcon(himl, hIconConfig);
		ImageList_AddIcon(himl, hIconApplication);
		ImageList_AddIcon(himl, hIconTheme);
		ImageList_AddIcon(himl, hIconLanguage);
		ImageList_AddIcon(himl, hIconKeyboard);

		hMenuView.SetHWND(GetDlgItem (hDlg, IDC_LIST_MENU));
		hMenuView.SetImageList(himl, LVSIL_SMALL);
		ListView_SetExtendedListViewStyle(hMenuView.GetHwnd(), LVS_EX_FULLROWSELECT | WS_EX_LTRREADING);

		hMenuView.InitListViewColumns(0, "Settings", 190);

		hMenuView.AddItem(0, 0, GetLocalisation(IDS_MENU_CONFIG, hInstance), 1, 0);
		hMenuView.AddItem(1, 0, GetLocalisation(IDS_MENU_APPLICATIONS, hInstance), 2, 2);
		hMenuView.AddItem(2, 0, GetLocalisation(IDS_MENU_THEMES, hInstance), 3, 3);
		hMenuView.AddItem(3, 0, GetLocalisation(IDS_MENU_LANGUAGES, hInstance), 4, 4);
		hMenuView.AddItem(4, 0, GetLocalisation(IDS_MENU_KEYBOARDS, hInstance), 5, 5);

		// Tabs
		hWndTabConfig = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG_CONFIG), hDlg, DlgProcConfig);
		ShowWindow(hWndTabConfig, SW_HIDE);

		hWndTabPlugins = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG_PLUGINS), hDlg, DlgProcPlugins);
		ShowWindow(hWndTabPlugins, SW_HIDE);

		hWndTabThemes = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG_THEMES), hDlg, DlgProcThemes);
		ShowWindow(hWndTabThemes, SW_HIDE);

		hWndTabLanguages = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG_LANGUAGES), hDlg, DlgProcLanguages);
		ShowWindow(hWndTabLanguages, SW_HIDE);

		hWndTabKeyboards = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG_KEYBOARDS), hDlg, DlgProcKeyboards);
		ShowWindow(hWndTabKeyboards, SW_HIDE);

		SelectMenu(0);
		hMenuView.SelectItem(0);
		break;
	case WM_NOTIFY:
		lpnmtdi = (LPNMTTDISPINFO) lParam;

		if (lpnmtdi->hdr.hwndFrom == hMenuView.GetHwnd())
		{
			switch(lpnmtdi->hdr.code)
			{
			case LVN_ITEMCHANGED:
				LPNMLISTVIEW li;
				li = (LPNMLISTVIEW)lParam;
				if (li->uNewState != 3)
					break;
			case NM_CLICK:
			case NM_RCLICK:
			case NM_DBLCLK:
			case NM_RDBLCLK:
				SelectMenu();
				break;
			}
		}
		break;
	case WM_SIZE:
		RECT	windowRect;
		RECT	menuViewRect;
		RECT	tempViewRect;

		GetClientRect(hDlg, &windowRect);
		GetClientRect(hMenuView.GetHwnd(), &menuViewRect);

		MoveWindow(hMenuView.GetHwnd(), 7, TOP_BORDER_HEIGHT + 7, CONFIG_MENU_WIDTH - 7, windowRect.bottom - TOP_BORDER_HEIGHT - 14, TRUE);

		GetClientRect(hWndTabConfig, &tempViewRect);
		MoveWindow(hWndTabConfig,
			((windowRect.right - CONFIG_MENU_WIDTH - tempViewRect.right) / 2) + CONFIG_MENU_WIDTH,
			((windowRect.bottom - TOP_BORDER_HEIGHT - tempViewRect.bottom) / 2) + TOP_BORDER_HEIGHT,
			tempViewRect.right, tempViewRect.bottom, TRUE);

		MoveWindow(hWndTabPlugins, CONFIG_MENU_WIDTH, TOP_BORDER_HEIGHT,
			windowRect.right - CONFIG_MENU_WIDTH, windowRect.bottom - TOP_BORDER_HEIGHT, TRUE);

		MoveWindow(hWndTabThemes, CONFIG_MENU_WIDTH, TOP_BORDER_HEIGHT,
			windowRect.right - CONFIG_MENU_WIDTH, windowRect.bottom - TOP_BORDER_HEIGHT, TRUE);

		MoveWindow(hWndTabLanguages, CONFIG_MENU_WIDTH, TOP_BORDER_HEIGHT,
			windowRect.right - CONFIG_MENU_WIDTH, windowRect.bottom - TOP_BORDER_HEIGHT, TRUE);

		MoveWindow(hWndTabKeyboards, CONFIG_MENU_WIDTH, TOP_BORDER_HEIGHT,
			windowRect.right - CONFIG_MENU_WIDTH, windowRect.bottom - TOP_BORDER_HEIGHT, TRUE);

		SendMessage(hWndTabPlugins, WM_COMMAND, WM_SIZE, NULL);
		SendMessage(hWndTabThemes, WM_COMMAND, WM_SIZE, NULL);
		SendMessage(hWndTabLanguages, WM_COMMAND, WM_SIZE, NULL);
		SendMessage(hWndTabKeyboards, WM_COMMAND, WM_SIZE, NULL);
		break;
	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDCANCEL:
			EndDialog (hDlg, 0);
			return TRUE;
		}
		break;
	case WM_GETMINMAXINFO:
		LPMINMAXINFO(lParam)->ptMinTrackSize.x = 750; // kleinste Breite
		LPMINMAXINFO(lParam)->ptMinTrackSize.y = 520; // kleinste Höhe
		return 0;
	case WM_NCHITTEST:
		LRESULT lresult;

		lresult = DefWindowProc(hDlg, WM_NCHITTEST, wParam, lParam); 
		if (lresult == HTCLIENT)
		{ 
			GetWindowRect(hDlg, &windowRect);

			int yPos;
			yPos = GET_Y_LPARAM(lParam);
			yPos = yPos - windowRect.top - GetSystemMetrics(SM_CYSIZE) - GetSystemMetrics(SM_CYBORDER);

			if (yPos <= TOP_BORDER_HEIGHT)
				lresult = HTCAPTION;
		} 
		SetWindowLong( hDlg, DWL_MSGRESULT, lresult ); 
		return (TRUE);
	case WM_PAINT:
		hdc = OnPaint(hDlg, windowRect, hdc, ps, IDS_APP_TITLE);

		break;
	}
	return FALSE ;
}

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	strExtensionFile.append(szCmdLine);

	#ifdef _DEBUG
	//strExtensionFile = "D:\\Source\\c++\\Programme\\sally\\____ext\\Applications\\eRadio.sallyapplication";
	//strExtensionFile = "D:\\Source\\c++\\Programme\\sally\\____ext\\Keyboards\\fr.sallykeyboard";
	//strExtensionFile = "D:\\Source\\c++\\Programme\\sally\\____ext\\Languages\\fr.sallylanguage";
	//strExtensionFile = "C:\\Users\\christian.knobloch\\Downloads\\ScummVM_1.1.1.3.sallyapplication";
	//strExtensionFile = "update";
	//strExtensionFile = "applications\\de.der-knob.sally.app.eRadio";
	#endif

	if (SallyAPI::String::StringHelper::StringEndsWith(strExtensionFile, ".sallyplugin"))
		extensionType = INSTALL_FILE_INSTALL;
	else if (SallyAPI::String::StringHelper::StringEndsWith(strExtensionFile, ".sallyapplication"))
		extensionType = INSTALL_FILE_INSTALL;
	else if (SallyAPI::String::StringHelper::StringEndsWith(strExtensionFile, ".sallytheme"))
		extensionType = INSTALL_FILE_INSTALL;
	else if (SallyAPI::String::StringHelper::StringEndsWith(strExtensionFile, ".sallylanguage"))
		extensionType = INSTALL_FILE_INSTALL;
	else if (SallyAPI::String::StringHelper::StringEndsWith(strExtensionFile, ".sallykeyboard"))
		extensionType = INSTALL_FILE_INSTALL;
	else if (strExtensionFile.compare("update") == 0)
		extensionType = INSTALL_UPDATE;

	// get path and config files
	searchUpdates = GetUpdateFolder();

	// install ext or self update?
	if (extensionType != INSTALL_NONE)
	{
		HWND hwndWait = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG_WAIT), NULL, NULL);
		ShowWindow(hwndWait, SW_SHOW);
		UpdateWindow(hwndWait);

		bool running = true;

		// go on and show the Dialog
		HANDLE hSem = NULL;
		while (running) 
		{ 
			hSem = CreateSemaphore(NULL, 0, 1, "SallyProject"); 
			if (hSem)
			{
				if (GetLastError() == ERROR_ALREADY_EXISTS) 
				{
					running = true;
					::CloseHandle(hSem);
				}
				else
					running = false;
			}
			else
			{
				running = false;
			}
			Sleep(100);
		}

		// wait for all instances to end
		HANDLE hSemSelfUpdat = NULL;
		while (running) 
		{ 
			hSemSelfUpdat = CreateSemaphore(NULL, 0, 1, "SallyProjectUpdater");
			if (hSemSelfUpdat)
			{
				if (GetLastError() == ERROR_ALREADY_EXISTS)
				{
					running = true;
					::CloseHandle(hSemSelfUpdat);
				}
				else
					running = false;
			}
			else
			{
				running = false;
			}
			Sleep(100);
		}
		// close the window
		DestroyWindow(hwndWait);

		DialogBox (hInstance, MAKEINTRESOURCE(IDD_DIALOG_INSTALL), 0, DlgInstallerProc);

		if ((extensionType == INSTALL_UPDATE) && (bInstallError))
		{
			if (MessageBox(0, GetLocalisation(IDS_DELETE_UPDATE_ERRORS, hInstance).c_str(),
				GetLocalisation(IDS_DELETE_UPDATE_ERRORS_CAPTION, hInstance).c_str(), MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				DeleteDirectory(searchUpdates.c_str());
				CreateDirectory(searchUpdates.c_str(), NULL);
			}
		}

		::CloseHandle(hSem);
		::CloseHandle(hSemSelfUpdat);

		if (bSkipUpdate)
		{
			Sleep(1000);
			// start sally
			std::string modulePath = SallyAPI::System::SystemHelper::GetModulePath();
			modulePath.append("sally.exe");

			ShellExecute(NULL, "open", modulePath.c_str(), "skipUpdate", NULL, SW_SHOWNORMAL);
			return 0;
		}

		if ((extensionType == INSTALL_UPDATE) && (!bInstallError))
		{
			Sleep(1000);
			// start sally
			std::string modulePath = SallyAPI::System::SystemHelper::GetModulePath();
			modulePath.append("sally.exe");

			ShellExecute(NULL, "open", modulePath.c_str(), NULL, NULL, SW_SHOWNORMAL);
			return 0;
		}
	}
	else
	{
		// go on and show the Dialog
		HANDLE hSem = CreateSemaphore(NULL, 0, 1, "SallyProject"); 
		if (hSem) 
		{ 
			if (GetLastError() == ERROR_ALREADY_EXISTS) 
			{
				MessageBox(0, GetLocalisation(IDS_APP_SALLY_RUNNING, hInstance).c_str(),
					GetLocalisation(IDS_APP_SALLY_RUNNING_CAPTION, hInstance).c_str(), MB_OK | MB_ICONINFORMATION);

				//Die Semaphore existiert bereits, also beenden 
				return 0; 
			} 
		}

		// show the config dialog
		DialogBox (hInstance, MAKEINTRESOURCE(IDD_DIALOG), 0, DlgProc);

		::CloseHandle(hSem);
	}
	return 0;
}
