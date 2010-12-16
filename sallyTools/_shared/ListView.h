////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	_shared\ListView.h
///
/// \brief	Declares the list view class. 
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

#include <windows.h>
#include <string>
#include <windows.h>
#include <commctrl.h>
#include <string>

#define NO_HWND				1
#define LW_ERROR			2
#define LISTVIEW_OK			100

#ifndef LISTVIEW_H
#define LISTVIEW_H

class ListView
{
private:
	HWND hwnd;
	int bSort;

public:
	ListView();
	~ListView();

	int CreateLV(HWND hMainWindow, HINSTANCE hInstanceMain, HMENU ID);
	void SetHWND(HWND hwnd);
	int InitListViewColumns(int index, const std::string szText, int width);

	int SetImageList(HIMAGELIST himl, int iImageList);
	int ChangeIcon(int Item, int Icon);
	int ChangeIconByIndefierID(int indefierID, int Icon);

	int AddItem(int Item, int SubItem, const std::string Text, int IndefierID, int Icon);
	int DeleteColumn(int iColumn);
	int DeleteItem(int iItem);
	int SelectItem(int Item);

	int ClearListView();

	int GetListViewItemText(char*Text, int iLength, int iItem, int iSubItem = 0);
	int GetListViewItemId(int iItem);
	int GetSelectionId();

	int SetColumnWidth(int x, int y);
	int MoveLV(int x, int y, int nWidth, int nHeight, BOOL bRepaint);
	int ShowLV(int iShow);
	HWND GetHwnd();
};

#endif
