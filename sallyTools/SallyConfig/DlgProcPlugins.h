////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	SallyConfig\DlgProcPlugins.h
///
/// \brief	Declares the dialog proc plugins. 
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

void ItemSelectedPlugins(HWND hEnable, HWND hDelete)
{
	int selectedItem = ListView_GetSelectionMark(hPluginView.GetHwnd());
	if (selectedItem == -1)
	{
		EnableWindow(hEnable, FALSE);
		EnableWindow(hDelete, FALSE);
		return;
	}
	EnableWindow(hEnable, TRUE);
	EnableWindow(hDelete, TRUE);

	char temp[255];
	hPluginView.GetListViewItemText(temp, 255, selectedItem, 1);

	if (strcmp(temp, GetLocalisation(IDS_PLUGIN_ON, hInstance).c_str()) == 0)
	{
		SetWindowText(hEnable, GetLocalisation(IDS_PLUGIN_ENABLE, hInstance).c_str());
	}
	else
	{
		SetWindowText(hEnable, GetLocalisation(IDS_PLUGIN_DISABLE, hInstance).c_str());
	}
}

void DeleteApp(HWND hDlg)
{
	if (MessageBox(hDlg, GetLocalisation(IDS_PLUGIN_Q_DELETE_APP, hInstance).c_str(),
		GetLocalisation(IDS_PLUGIN_Q_DELETE_CAPTION, hInstance).c_str(), MB_YESNO | MB_ICONQUESTION) == IDNO)
	{
		return;
	}

	int selectedItem = ListView_GetSelectionMark(hPluginView.GetHwnd());
	if (selectedItem == -1)
		return;

	int id = hPluginView.GetListViewItemId(selectedItem);
	std::string pluginPath = appMap[id];

	std::vector<std::string> folderList = SallyAPI::String::StringHelper::TokenizeString(pluginPath, "\\");

	std::string folderToDelete = SallyAPI::System::SystemHelper::GetModulePath();
	folderToDelete.append("applications\\");
	folderToDelete.append(folderList.at(folderList.size() - 2));

	if (UninstallExtensionAsAdmin(folderToDelete))
		hPluginView.DeleteItem(selectedItem);
}

void EnableApp(HWND hEnable, HWND hDelete)
{
	int selectedItem = ListView_GetSelectionMark(hPluginView.GetHwnd());
	if (selectedItem == -1)
		return;

	char temp[255];
	hPluginView.GetListViewItemText(temp, 255, selectedItem, 1);

	int id = hPluginView.GetListViewItemId(selectedItem);
	std::string pluginPath = appMap[id];
	pluginPath.erase(pluginPath.length() - 1, 1);

	std::string fileInfo = GetPluginFolder(SallyAPI::String::PathHelper::GetFileFromFolderString(pluginPath));;
	fileInfo.append("option.ini");

	if (strcmp(temp, GetLocalisation(IDS_PLUGIN_ON, hInstance).c_str()) == 0)
	{
		SetWindowText(hEnable, GetLocalisation(IDS_PLUGIN_DISABLE, hInstance).c_str());
		SallyAPI::System::COption::SetPropertyBoolStatic("info", "disabled", false, fileInfo);

		hPluginView.AddItem(selectedItem, 1, GetLocalisation(IDS_PLUGIN_OFF, hInstance).c_str(), 0, 0);
	}
	else
	{
		SetWindowText(hEnable, GetLocalisation(IDS_PLUGIN_ENABLE, hInstance).c_str());
		SallyAPI::System::COption::SetPropertyBoolStatic("info", "disabled", true, fileInfo);

		hPluginView.AddItem(selectedItem, 1, GetLocalisation(IDS_PLUGIN_ON, hInstance).c_str(), 0, 0);
	}
}

BOOL CALLBACK DlgProcPlugins(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static RECT					WindowRectSelf;
	static LPNMTTDISPINFO		lpnmtdi;
	static HWND					hList;
	static HWND					hEnable;
	static HWND					hDelete;
	static HWND					hGetMore;
	
	switch (message)
	{
	case WM_INITDIALOG:
		hList = GetDlgItem(hDlg, IDC_LIST_PLUGINS);
		hEnable = GetDlgItem(hDlg, IDC_BUTTON_ENABLE);
		hDelete = GetDlgItem(hDlg, IDC_BUTTON_DELETE);
		hGetMore = GetDlgItem(hDlg, IDC_BUTTON_GET_MORE_ONLINE);

		hPluginView.SetHWND(hList);
		ListView_SetExtendedListViewStyle(hPluginView.GetHwnd(), LVS_EX_FULLROWSELECT | WS_EX_LTRREADING);

		hPluginView.InitListViewColumns(0, GetLocalisation(IDS_PLUGIN_NAME, hInstance).c_str(), 200);
		hPluginView.InitListViewColumns(1, GetLocalisation(IDS_PLUGIN_DISABLE, hInstance).c_str(), 70);
		hPluginView.InitListViewColumns(2, GetLocalisation(IDS_PLUGIN_AUTHOR, hInstance).c_str(), 150);
		hPluginView.InitListViewColumns(3, GetLocalisation(IDS_PLUGIN_VERSION, hInstance).c_str(), 70);
		hPluginView.InitListViewColumns(4, GetLocalisation(IDS_PLUGIN_UID, hInstance).c_str(), 200);
		hPluginView.InitListViewColumns(5, GetLocalisation(IDS_PLUGIN_APIVERSION, hInstance).c_str(), 70);

		SendMessage(GetDlgItem(hDlg, IDC_BUTTON_DELETE), BCM_SETSHIELD, NULL, (LPARAM) TRUE);

		LoadPlugins();
		break;
	case WM_NOTIFY:
		lpnmtdi = (LPNMTTDISPINFO) lParam;

		if (lpnmtdi->hdr.hwndFrom == hPluginView.GetHwnd())
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
				ItemSelectedPlugins(hEnable, hDelete);
				break;
			}
		}
		break;
	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case WM_SIZE:
			RECT	windowRect;
			RECT	enableRect;
			RECT	deleteRect;
			RECT	getMoreRect;

			GetClientRect(hDlg, &windowRect);
			GetClientRect(hEnable, &enableRect);
			GetClientRect(hDelete, &deleteRect);
			GetClientRect(hGetMore, &getMoreRect);
			
			MoveWindow(hList, 7, 7, windowRect.right - 14, windowRect.bottom - 14 - 7 - enableRect.bottom, TRUE);

			MoveWindow(hEnable, windowRect.right - 7 - enableRect.right, windowRect.bottom - 7 - enableRect.bottom,
				enableRect.right, enableRect.bottom, TRUE);
			MoveWindow(hDelete, windowRect.right - 7 - enableRect.right - 7 - deleteRect.right, windowRect.bottom - 7 - deleteRect.bottom,
				deleteRect.right, deleteRect.bottom, TRUE);

			MoveWindow(hGetMore, 7, windowRect.bottom - 7 - getMoreRect.bottom,
				getMoreRect.right, getMoreRect.bottom, TRUE);
			return FALSE;
		case IDC_BUTTON_ENABLE:
			EnableApp(hEnable, hDelete);
			break;
		case IDC_BUTTON_GET_MORE_ONLINE:
			DownloadOnline();
			break;
		case IDC_BUTTON_DELETE:
			DeleteApp(hDlg);
			break;
		}
		break;
     }
     return FALSE ;
}