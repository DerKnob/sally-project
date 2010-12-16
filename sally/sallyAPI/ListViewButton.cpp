////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ListViewButton.cpp
///
/// \brief	Implements the list view button class. 
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
/// it under the terms of the GNU Lesser General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
/// 
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
/// GNU Lesser General Public License for more details.
/// 
/// You should have received a copy of the GNU Lesser General Public License
/// along with this program. If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "ListViewButton.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CListViewButton::CListViewButton(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y,
/// int width, int height, int controlId) :SallyAPI::GUI::CButton(parent, x, y, width, height,
/// controlId, SallyAPI::GUI::BUTTON_TYPE_NORMAL), m_iNumber(0), m_bFirst(false), m_bLast(false),
/// m_bSmallFont(false)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	13.09.2010
///
/// \param [in,out]	parent	If non-null, the parent. 
/// \param	x				The x coordinate. 
/// \param	y				The y coordinate. 
/// \param	width			The width. 
/// \param	height			The height. 
/// \param	controlId		Identifier for the control. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CListViewButton::CListViewButton(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int height, int controlId)
	:SallyAPI::GUI::CButton(parent, x, y, width, height, controlId, SallyAPI::GUI::BUTTON_TYPE_NORMAL),
	m_iNumber(0), m_bFirst(false), m_bLast(false), m_bSmallFont(false)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CListViewButton::~CListViewButton()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	13.09.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CListViewButton::~CListViewButton()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CListViewButton::SetFirst(bool first)
///
/// \brief	Sets a first. 
///
/// \author	Christian Knobloch
/// \date	13.09.2010
///
/// \param	first	true to first. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListViewButton::SetFirst(bool first)
{
	m_bFirst = first;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CListViewButton::SetLast(bool last)
///
/// \brief	Sets a last. 
///
/// \author	Christian Knobloch
/// \date	13.09.2010
///
/// \param	last	true to last. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListViewButton::SetLast(bool last)
{
	m_bLast = last;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CListViewButton::SetNumber(int number)
///
/// \brief	Sets a number. 
///
/// \author	Christian Knobloch
/// \date	13.09.2010
///
/// \param	number	Number of. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListViewButton::SetNumber(int number)
{
	m_iNumber = number;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CListViewButton::GetNumber()
///
/// \brief	Gets the number. 
///
/// \author	Christian Knobloch
/// \date	13.09.2010
///
/// \return	The number. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CListViewButton::GetNumber()
{
	return m_iNumber;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void ClistViewButton::SetSmallFont(bool value)
///
/// \brief	Sets a small font. 
///
/// \author	Christian Knobloch
/// \date	30.11.2010
///
/// \param	value	true to value. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListViewButton::SetSmallFont(bool value)
{
	m_bSmallFont = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CListViewButton::ProcessMouseDoubleClick(int x, int y)
///
/// \brief	Process a mouse double click. 
///
/// \author	Christian Knobloch
/// \date	13.09.2010
///
/// \param	x	The x coordinate. 
/// \param	y	The y coordinate. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CListViewButton::ProcessMouseDoubleClick(int x, int y)
{
	SallyAPI::GUI::CButton::ProcessMouseDoubleClick(x, y);
	m_pParent->SendMessageToParent(this, GetControlId(), GUI_LISTVIEW_ITEM_DOUBLECLICKED);
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CListViewButton::RenderControl()
///
/// \brief	Renders a control. 
///
/// \author	Christian Knobloch
/// \date	13.09.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListViewButton::RenderControl()
{
	int borderLeft = 4;
	int borderRight = 4;

	int leftButton;
	int rightButton;

	if ((m_bChecked) || (m_bActive) || (m_bPressed))
	{
		leftButton = GUI_THEME_LISTVIEWBUTTON_SELECTED_SEPERATOR_LEFT;
		rightButton = GUI_THEME_LISTVIEWBUTTON_SELECTED_SEPERATOR_RIGHT;

		if (m_bFirst)
			leftButton = GUI_THEME_LISTVIEWBUTTON_SELECTED_LEFT;
		if (m_bLast)
			rightButton = GUI_THEME_LISTVIEWBUTTON_SELECTED_RIGHT;
		DrawButtonBackground(leftButton, GUI_THEME_LISTVIEWBUTTON_SELECTED, rightButton);
	}
	else
	{
		if (m_iNumber % 2)
		{
			leftButton = GUI_THEME_LISTVIEWBUTTON_NORMAL2_SEPERATOR_LEFT;
			rightButton = GUI_THEME_LISTVIEWBUTTON_NORMAL2_SEPERATOR_RIGHT;

			if (m_bFirst)
				leftButton = GUI_THEME_LISTVIEWBUTTON_NORMAL2_LEFT;
			if (m_bLast)
				rightButton = GUI_THEME_LISTVIEWBUTTON_NORMAL2_RIGHT;
			DrawButtonBackground(leftButton, GUI_THEME_LISTVIEWBUTTON_NORMAL2, rightButton);
		}
		else
		{
			leftButton = GUI_THEME_LISTVIEWBUTTON_NORMAL1_SEPERATOR_LEFT;
			rightButton = GUI_THEME_LISTVIEWBUTTON_NORMAL1_SEPERATOR_RIGHT;

			if (m_bFirst)
				leftButton = GUI_THEME_LISTVIEWBUTTON_NORMAL1_LEFT;
			if (m_bLast)
				rightButton = GUI_THEME_LISTVIEWBUTTON_NORMAL1_RIGHT;
			DrawButtonBackground(leftButton, GUI_THEME_LISTVIEWBUTTON_NORMAL1, rightButton);
		}
	}

	if ((m_iImage != GUI_NO_IMAGE) && (m_iImage != 0))
	{
		if (m_bImageLeft)
		{
			borderLeft += 4 + m_iImageSize;
			int imageWidthLeft = 0;

			if (m_strText.length() > 0)
			{
				// Temp variables
				SallyAPI::GUI::CPicture* image = NULL;

				image = GetPicture(leftButton);
				if (image != NULL) {
					imageWidthLeft = image->GetWidth();
				}
			}
			else
			{
				imageWidthLeft = (m_iWidth - m_iImageSize) / 2;
			}

			DrawImage(m_iImage, imageWidthLeft, (m_iHeight - m_iImageSize) / 2, m_iImageSize, m_iImageSize);
		}
		else
		{
			borderRight += 4 + m_iImageSize;
			int imageWidthRight = 0;

			// Temp variables
			SallyAPI::GUI::CPicture* image = NULL;

			if (m_strText.length() > 0)
			{
				// Temp variables
				SallyAPI::GUI::CPicture* image = NULL;

				image = GetPicture(rightButton);
				if (image != NULL) {
					imageWidthRight = image->GetWidth();
				}
			}
			DrawImage(m_iImage, m_iWidth - (m_iImageSize + imageWidthRight), (m_iHeight - m_iImageSize) / 2, m_iImageSize, m_iImageSize);
		}
	}
	if (m_bSmallFont)
	{
		if ((m_bChecked) || (m_bActive) || (m_bPressed))
			DrawText(leftButton, rightButton, borderLeft, borderRight, "listview.small.active.font");
		else
			DrawText(leftButton, rightButton, borderLeft, borderRight, "listview.small.font");
	}
	else
	{
		if ((m_bChecked) || (m_bActive) || (m_bPressed))
			DrawText(leftButton, rightButton, borderLeft, borderRight, "listview.active.font");
		else
			DrawText(leftButton, rightButton, borderLeft, borderRight, "listview.font");
	}
}