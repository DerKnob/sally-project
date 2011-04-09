////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	SallyConfig\DlgProcLanguages.h
///
/// \brief	Declares the dialog proc languages. 
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

void ItemSelectedLanguages(HWND hDelete)
{
	int selectedItem = ListView_GetSelectionMark(hLanguageView.GetHwnd());
	if (selectedItem == -1)
	{
		EnableWindow(hDelete, FALSE);
		return;
	}

	char temp[255];
	hLanguageView.GetListViewItemText(temp, 255, selectedItem, 3);

	if (strcmp(temp, "sally-project.language.en") == 0)
		EnableWindow(hDelete, FALSE);
	else
		EnableWindow(hDelete, TRUE);
}

void DeleteLanguage(HWND hDlg)
{
	if (MessageBox(hDlg, GetLocalisation(IDS_PLUGIN_Q_DELETE_LANGUAGE, hInstance).c_str(),
		GetLocalisation(IDS_PLUGIN_Q_DELETE_CAPTION, hInstance).c_str(), MB_YESNO | MB_ICONQUESTION) == IDNO)
	{
		return;
	}

	int selectedItem = ListView_GetSelectionMark(hLanguageView.GetHwnd());
	if (selectedItem == -1)
		return;

	int id = hLanguageView.GetListViewItemId(selectedItem);
	std::string languagePath = languageMap[id];

	std::vector<std::string> folderList = SallyAPI::String::StringHelper::TokenizeString(languagePath, "\\");

	std::string folderToDelete = SallyAPI::System::SystemHelper::GetModulePath();
	folderToDelete.append("languages\\");
	folderToDelete.append(folderList.at(folderList.size() - 2));

	if (UninstallExtensionAsAdmin(folderToDelete))
		hLanguageView.DeleteItem(selectedItem);
}

BOOL CALLBACK DlgProcLanguages(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static RECT					WindowRectSelf;
	static LPNMTTDISPINFO		lpnmtdi;
	static HWND					hList;
	static HWND					hDelete;
	static HWND					hGetMore;

	switch (message)
	{
	case WM_INITDIALOG:
		hList = GetDlgItem(hDlg, IDC_LIST_LANGUAGES);
		hDelete = GetDlgItem(hDlg, IDC_BUTTON_DELETE);
		hGetMore = GetDlgItem(hDlg, IDC_BUTTON_GET_MORE_ONLINE);

		hLanguageView.SetHWND(hList);
		ListView_SetExtendedListViewStyle(hLanguageView.GetHwnd(), LVS_EX_FULLROWSELECT | WS_EX_LTRREADING);

		hLanguageView.InitListViewColumns(0, GetLocalisation(IDS_PLUGIN_NAME, hInstance).c_str(), 200);
		hLanguageView.InitListViewColumns(1, GetLocalisation(IDS_PLUGIN_AUTHOR, hInstance).c_str(), 150);
		hLanguageView.InitListViewColumns(2, GetLocalisation(IDS_PLUGIN_VERSION, hInstance).c_str(), 70);
		hLanguageView.InitListViewColumns(3, GetLocalisation(IDS_PLUGIN_UID, hInstance).c_str(), 200);

		SendMessage(GetDlgItem(hDlg, IDC_BUTTON_DELETE), BCM_SETSHIELD, NULL, (LPARAM) TRUE);

		LoadLanguages();
		break;
	case WM_NOTIFY:
		lpnmtdi = (LPNMTTDISPINFO) lParam;

		if (lpnmtdi->hdr.hwndFrom == hLanguageView.GetHwnd())
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
				ItemSelectedLanguages(hDelete);
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
			DeleteLanguage(hDlg);
			break;
		}
		break;
	}
	return FALSE ;
}