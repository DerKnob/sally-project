////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	SallyConfig\DlgProcThemes.h
///
/// \brief	Declares the dialog proc themes. 
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

void ItemSelectedThemes(HWND hDelete)
{
	int selectedItem = ListView_GetSelectionMark(hThemeView.GetHwnd());
	if (selectedItem == -1)
	{
		EnableWindow(hDelete, FALSE);
		return;
	}

	char temp[255];
	hThemeView.GetListViewItemText(temp, 255, selectedItem, 3);

	if (strcmp(temp, "de.der-knob.sally.theme.clean") == 0)
		EnableWindow(hDelete, FALSE);
	else
		EnableWindow(hDelete, TRUE);
}

void DeleteTheme(HWND hDlg)
{
	if (MessageBox(hDlg, GetLocalisation(IDS_PLUGIN_Q_DELETE_THEME, hInstance).c_str(),
		GetLocalisation(IDS_PLUGIN_Q_DELETE_CAPTION, hInstance).c_str(), MB_YESNO | MB_ICONQUESTION) == IDNO)
	{
		return;
	}

	int selectedItem = ListView_GetSelectionMark(hThemeView.GetHwnd());
	if (selectedItem == -1)
		return;

	int id = hThemeView.GetListViewItemId(selectedItem);
	std::string themePath = themeMap[id];

	std::vector<std::string> folderList = SallyAPI::String::StringHelper::TokenizeString(themePath, "\\");

	std::string folderToDelete = SallyAPI::System::SystemHelper::GetModulePath();
	folderToDelete.append("themes\\");
	folderToDelete.append(folderList.at(folderList.size() - 2));

	if (UninstallExtensionAsAdmin(folderToDelete))
		hThemeView.DeleteItem(selectedItem);
}

BOOL CALLBACK DlgProcThemes(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static RECT					WindowRectSelf;
	static LPNMTTDISPINFO		lpnmtdi;
	static HWND					hList;
	static HWND					hDelete;
	static HWND					hGetMore;

	switch (message)
	{
	case WM_INITDIALOG:
		hList = GetDlgItem(hDlg, IDC_LIST_THEMES);
		hDelete = GetDlgItem(hDlg, IDC_BUTTON_DELETE);
		hGetMore = GetDlgItem(hDlg, IDC_BUTTON_GET_MORE_ONLINE);

		hThemeView.SetHWND(hList);
		ListView_SetExtendedListViewStyle(hThemeView.GetHwnd(), LVS_EX_FULLROWSELECT | WS_EX_LTRREADING);

		hThemeView.InitListViewColumns(0, GetLocalisation(IDS_PLUGIN_NAME, hInstance).c_str(), 200);
		hThemeView.InitListViewColumns(1, GetLocalisation(IDS_PLUGIN_AUTHOR, hInstance).c_str(), 150);
		hThemeView.InitListViewColumns(2, GetLocalisation(IDS_PLUGIN_VERSION, hInstance).c_str(), 70);
		hThemeView.InitListViewColumns(3, GetLocalisation(IDS_PLUGIN_UID, hInstance).c_str(), 200);

		SendMessage(GetDlgItem(hDlg, IDC_BUTTON_DELETE), BCM_SETSHIELD, NULL, (LPARAM) TRUE);

		LoadThemes();
		break;
	case WM_NOTIFY:
		lpnmtdi = (LPNMTTDISPINFO) lParam;

		if (lpnmtdi->hdr.hwndFrom == hThemeView.GetHwnd())
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
				ItemSelectedThemes(hDelete);
				break;
			}
		}
		break;
	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case WM_SIZE:
			RECT	windowRect;
			RECT	deleteRect;
			RECT	getMoreRect;

			GetClientRect(hDlg, &windowRect);
			GetClientRect(hDelete, &deleteRect);
			GetClientRect(hGetMore, &getMoreRect);

			MoveWindow(hList, 7, 7, windowRect.right - 14, windowRect.bottom - 14 - 7 - deleteRect.bottom, TRUE);

			MoveWindow(hDelete, windowRect.right - 7 - deleteRect.right, windowRect.bottom - 7 - deleteRect.bottom,
				deleteRect.right, deleteRect.bottom, TRUE);

			MoveWindow(hGetMore, 7, windowRect.bottom - 7 - getMoreRect.bottom,
				getMoreRect.right, getMoreRect.bottom, TRUE);
			return FALSE;
		case IDC_BUTTON_GET_MORE_ONLINE:
			DownloadOnline();
			break;
		case IDC_BUTTON_DELETE:
			DeleteTheme(hDlg);
			break;
		}
		break;
	}
	return FALSE ;
}