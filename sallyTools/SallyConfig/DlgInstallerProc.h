////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	SallyConfig\DlgInstallerProc.h
///
/// \brief	Declares the dialog installer proc. 
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

DWORD WINAPI InstallThread(LPVOID lpParam)
{
	int count = SendMessage(hInstallPluginView.GetHwnd(), LVM_GETITEMCOUNT, 0 ,0);

	for (int i = 0; i < count; ++i)
	{
		int id = hInstallPluginView.GetListViewItemId(i);

		std::string temp = appMap[id];

		if (InstallExtensionAsAdmin(temp))
		{
			hInstallPluginView.AddItem(i, 1, GetLocalisation(IDS_MENU_INSTALL_DONE, hInstance), 1, 0);

			if (extensionType == INSTALL_UPDATE)
				DeleteFile(temp.c_str());
		}
		else
		{
			bInstallError = true;
			hInstallPluginView.AddItem(i, 1, GetLocalisation(IDS_MENU_INSTALL_ERROR, hInstance), 1, 0);
		}
	}
	EnableWindow(hCancel, TRUE);

	ShowWindow(hProcessbar, SW_HIDE);

	// if we are done without errors, end the installer and start sally
	if (!bInstallError)
	{
		EndDialog (hMainWindow, 0);
	}
	else
	{
		if (MessageBox(0, GetLocalisation(IDS_INSTALLATION_ERROR, hInstance).c_str(),
					GetLocalisation(IDS_INSTALLATION_ERROR_CAPTION, hInstance).c_str(), MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			std::string url = "url.dll,FileProtocolHandler http://www.sally-project.org/";

			// don't call direct ShellExecute with the URL
			// see sbug ticket 469
			ShellExecute(NULL, "open", "rundll32.exe", url.c_str(), NULL, SW_SHOWNORMAL);
		}
	}

	// set the name of the exit button to "Exit"
	SetWindowText(hCancel, GetLocalisation(IDS_EXIT, hInstance).c_str());

	ExitThread(0);
}

void Install()
{
	EnableWindow(hOk, FALSE);
	EnableWindow(hCancel, FALSE);

	ShowWindow(hProcessbar, SW_SHOW);

	hThread = ::CreateThread(NULL, NULL, InstallThread, 0, 0, 0);
}

void AddExt()
{
	// Add Plugins
	int					id = 0;
	HANDLE				hFolder;
	WIN32_FIND_DATA		pluginFolderInformation;
	std::string			searchQuery = searchUpdates;
	searchQuery.append("*");

	hFolder = FindFirstFile(searchQuery.c_str(), &pluginFolderInformation);

	if(hFolder != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (!(pluginFolderInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				&& (strcmp(pluginFolderInformation.cFileName, "..") != 0)
				&& (strcmp(pluginFolderInformation.cFileName, ".") != 0))
			{
				std::string file = searchUpdates;
				file.append(pluginFolderInformation.cFileName);

				appMap[id] = file;

				++id;
			}
		} while(FindNextFile(hFolder, &pluginFolderInformation) == TRUE);
	}
	FindClose(hFolder);	
}

void FillUpdateMap()
{
	if (extensionType == INSTALL_FILE_INSTALL)
	{
		appMap[0] = strExtensionFile;
	}
	else
	{
		AddExt();
	}

	std::map<int, std::string>::iterator iter = appMap.begin();

	while (iter != appMap.end())
	{
		std::string temp = iter->second;
		hInstallPluginView.AddItem(0, 0, SallyAPI::String::PathHelper::GetFileFromFolderString(temp), iter->first, 0);

		++iter;
	}
}

BOOL CALLBACK DlgInstallerProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
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
		hMainWindow = hDlg;
		break;
	case WM_INITDIALOG:
		hInstance = (HINSTANCE) GetWindowLong (hDlg, GWL_HINSTANCE);

		// Set Icon
		hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));

		SendMessage(hDlg, WM_SETICON, ICON_SMALL, (LPARAM) hIcon);
		SendMessage(hDlg, WM_SETICON, ICON_BIG, (LPARAM) hIcon);

		// init listview
		hList = GetDlgItem(hDlg, IDC_LIST_INSTALL);
		hOk = GetDlgItem(hDlg, IDOK);
		hCancel = GetDlgItem(hDlg, IDCANCEL);
		hProcessbar = GetDlgItem(hDlg, IDC_PROGRESS);

		SetWindowLongPtr(hProcessbar,GWL_STYLE,GetWindowLong(hProcessbar, GWL_STYLE) | PBS_MARQUEE); 
		SendMessage(hProcessbar,(UINT) PBM_SETMARQUEE,(WPARAM) TRUE,(LPARAM)50 );  

		hInstallPluginView.SetHWND(hList);
		ListView_SetExtendedListViewStyle(hInstallPluginView.GetHwnd(), LVS_EX_FULLROWSELECT | WS_EX_LTRREADING);

		hInstallPluginView.InitListViewColumns(0, GetLocalisation(IDS_MENU_NAME, hInstance), 200);
		hInstallPluginView.InitListViewColumns(1, GetLocalisation(IDS_MENU_STATUS, hInstance), 80);

		SendMessage(GetDlgItem(hDlg, IDOK), BCM_SETSHIELD, NULL, (LPARAM) TRUE);

		FillUpdateMap();

		// if we are running in the update mode, than set the name of the exit button
		// to "install later"
		if (extensionType == INSTALL_UPDATE)
			SetWindowText(hCancel, GetLocalisation(IDS_SKIP_UPDATE, hInstance).c_str());
		break;
	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDOK:
			Install();
			return FALSE;
		case IDCANCEL:
			if ((IsWindowEnabled(hOk) == TRUE) && (extensionType == INSTALL_UPDATE))
				bSkipUpdate = true;
			EndDialog (hDlg, 0);
			return TRUE;
		}
		break;
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
		hdc = OnPaint(hDlg, windowRect, hdc, ps, IDS_APP_TITLE2);

		break;
	}
	return FALSE ;
}