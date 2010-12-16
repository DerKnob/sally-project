////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	_shared\ListView.cpp
///
/// \brief	Implements the list view class. 
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

#include "ListView.h"

ListView::ListView()
{
	hwnd = NULL;
	bSort = FALSE;
}

ListView::~ListView()
{
}

int ListView::GetListViewItemText(char* Text, int iLength, int iItem, int iSubItem)
{
	if (hwnd != NULL)
	{
		LVITEM lvi;

		lvi.iItem = iItem;
		lvi.iSubItem = iSubItem;
		lvi.mask = LVIF_TEXT;
		lvi.pszText = Text;
		lvi.cchTextMax = iLength;

		if (SendMessage(hwnd,LVM_GETITEMTEXT,iItem,(LPARAM) &lvi) == -1)
			return LW_ERROR;
		return LISTVIEW_OK;
	}
	return NO_HWND;
}

int ListView::GetListViewItemId(int iItem)
{
	if (hwnd != NULL)
	{
		LVITEM lvi;

		lvi.iItem = iItem;
		lvi.iSubItem = 0;
		lvi.mask = LVIF_PARAM;

		ListView_GetItem(hwnd, &lvi);
		return (int) lvi.lParam;
	}
	return NO_HWND;
}

int ListView::InitListViewColumns(int index, const std::string szText, int width)
{
	if (hwnd != NULL)
	{
		LVCOLUMN lvc;

		lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

		lvc.iSubItem = index;
		lvc.pszText = (LPSTR) szText.c_str();
		lvc.cx = width;
		lvc.fmt = LVCFMT_LEFT;
		if (ListView_InsertColumn(hwnd, index, &lvc) == -1)
			return LW_ERROR;
		return LISTVIEW_OK;
	}
	return NO_HWND;
}


int ListView::GetSelectionId()
{
	int selection;
	
	selection = ListView_GetSelectionMark(hwnd);

	if (selection > -1)
	{
		LVITEM lvi;
		lvi.iItem = selection;
		lvi.iSubItem = 0;

		ListView_GetItem(hwnd, &lvi);
		return (int) lvi.lParam;
	}
	return -1;
}

int ListView::AddItem(int Item, int SubItem, const std::string Text, int indefierID, int Icon = -1)
{
	if (hwnd != NULL)
	{
		char pText[256];
		LVITEM lvi;

		strcpy(pText, Text.c_str());
		int ItemIndex;

		lvi.mask = LVIF_TEXT;
		lvi.pszText = pText;
		lvi.stateMask = 0;
		
		
		if (Icon != -1)
		{
			lvi.iImage = Icon;
			lvi.mask |= LVIF_IMAGE;
		}


		lvi.iItem = Item;
		lvi.iSubItem = SubItem;

		if (SubItem > 0)
		{
			//Für SubItems
			ItemIndex = (int) SendMessage(hwnd,LVM_SETITEM, 0, (LPARAM) &lvi);
		}
		else
		{
		
			lvi.lParam = indefierID;
			lvi.mask |= LVIF_PARAM;



			//Für das ErsteItem
			ItemIndex = (int) SendMessage(hwnd,LVM_INSERTITEM, 0, (LPARAM) &lvi);
		}
		int i;
		i = GetLastError();
		return ItemIndex; //Gibt den Index des neuen Elemets zurück
	}
	return -1;
}


int ListView::SelectItem(int Item)
{
	ListView_SetSelectionMark(hwnd, Item);
	ListView_SetItemState(hwnd, Item, LVIS_SELECTED, LVIS_SELECTED);
	return 0;
}

int ListView::ClearListView()
{
	if (hwnd != NULL)
	{
		ListView_DeleteAllItems(hwnd);
	}
	return NO_HWND;
}

int ListView::ShowLV(int iShow)
{
	if (hwnd != NULL)
	{
		ShowWindow(hwnd, iShow);
	}
	return NO_HWND;
}


int ListView::MoveLV(int x, int y, int nWidth, int nHeight, BOOL bRepaint)
{
	if (hwnd != NULL)
	{
		MoveWindow(hwnd, x, y, nWidth, nHeight, bRepaint);
		return LISTVIEW_OK;
	}
	return NO_HWND;
}

int ListView::SetColumnWidth(int x, int y)
{
	if (hwnd != NULL)
	{
		ListView_SetColumnWidth(hwnd, x, y);
		return LISTVIEW_OK;
	}
	return NO_HWND;
}

void ListView::SetHWND(HWND hwnd)
{
	this->hwnd = hwnd;
}


int ListView::DeleteColumn(int iColumn)
{
	if (hwnd != NULL)
	{
		SendMessage(hwnd, LVM_DELETECOLUMN, iColumn ,0);
		return LISTVIEW_OK;
	}
	return NO_HWND;
}

int ListView::DeleteItem(int iItem)
{
	if (hwnd != NULL)
	{
		SendMessage(hwnd, LVM_DELETEITEM, iItem ,0);
		return LISTVIEW_OK;
	}
	return NO_HWND;
}

int ListView::ChangeIconByIndefierID(int indefierID, int Icon)
{
	if (hwnd != NULL)
	{
		ChangeIcon(indefierID - 1, Icon);
	}
	return -1;
}

int ListView::ChangeIcon(int Item, int Icon)
{
	if (hwnd != NULL)
	{
		LVITEM lvi;

		lvi.iItem = Item;
		lvi.iSubItem = 0;
		lvi.iImage = Icon;
		lvi.mask = LVIF_IMAGE;

		SendMessage(hwnd,LVM_SETITEM, 0, (LPARAM) &lvi);
	}
	return -1;
}

int ListView::SetImageList(HIMAGELIST himl, int iImageList)
{
	if (hwnd != NULL)
	{
		ListView_SetImageList(hwnd, himl, iImageList);
	}
	return 0;
}

HWND ListView::GetHwnd()
{
	return this->hwnd;
}
