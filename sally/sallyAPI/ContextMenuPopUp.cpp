////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ContextMenuPopUp.cpp
///
/// \brief	Implements the context menu pop up class. 
///
/// \author	Christian Knobloch
/// \date	30.03.2011
///
/// This file is part of the Sally Project
/// 
/// Copyright(c) 2008-2011 Sally Project
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

#include "ContextMenuPopUp.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CContextMenuPopUp::CContextMenuPopUp(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId,
/// const std::string &explicidAppName) :SallyAPI::GUI::CPopUpWindow(parent, graphicId,
/// explicidAppName)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	30.03.2011
///
/// \param [in,out]	parent	If non-null, the parent. 
/// \param	graphicId		Identifier for the graphic. 
/// \param	explicidAppName	Name of the explicid application. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CContextMenuPopUp::CContextMenuPopUp(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId,
									 const std::string &explicidAppName, int width, int height)
	:SallyAPI::GUI::CPopUpWindow(parent, graphicId, explicidAppName)
{
	SetCloseOnClick(true);

	m_pContextMenu = new SallyAPI::GUI::CContextMenu(this, 0, 0, width, height);
	m_pContextMenu->SetAlwaysHandleInput(true);
	this->AddChild(m_pContextMenu);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CContextMenuPopUp::~CContextMenuPopUp()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	30.03.2011
////////////////////////////////////////////////////////////////////////////////////////////////////

CContextMenuPopUp::~CContextMenuPopUp()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CContextMenuPopUp::SetPopUpPoint(int x, int y)
///
/// \brief	Sets a pop up point. 
///
/// \author	Christian Knobloch
/// \date	30.03.2011
///
/// \param	x	The x coordinate. 
/// \param	y	The y coordinate. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CContextMenuPopUp::SetPopUpPoint(int x, int y)
{
	SallyAPI::GUI::CPicture* image = NULL;
	int imageHeightArrow = 0;
	int imageWidthArrow = 0;
	int imageHeightBorderLeft = 0;
	int imageWidthBorderLeft = 0;
	int imageWidthBorderRight = 0;

	// paint it with a top arrow
	if (y < m_iHeight / 2)
	{
		// get the arrow height
		image = GetPicture(GUI_THEME_GROUPBOX_ARROW_TOP);
		if (image != NULL)
		{
			imageHeightArrow = image->GetHeight();
			imageWidthArrow = image->GetWidth();
		}

		image = GetPicture(GUI_THEME_GROUPBOX_LEFT_TOP);
		if (image != NULL) 
		{
			imageHeightBorderLeft = image->GetHeight();
			imageWidthBorderLeft = image->GetWidth();
		}

		image = GetPicture(GUI_THEME_GROUPBOX_RIGHT_BOTTOM);
		if (image != NULL) 
		{
			imageWidthBorderRight = image->GetWidth();
		}

		// set the arrow
		if (x < m_iWidth / 3)
		{
			m_pContextMenu->SetArrowPosition(SallyAPI::GUI::CONTEXT_MENU_ARROW_POSITION_LEFT_TOP);
			m_pContextMenu->Move(x - imageWidthBorderLeft - (imageWidthArrow / 2),
				y - (-imageHeightArrow + imageHeightBorderLeft));
		}
		else if ((x > m_iWidth / 3) && (x < (m_iWidth / 3) * 2))
		{
			m_pContextMenu->SetArrowPosition(SallyAPI::GUI::CONTEXT_MENU_ARROW_POSITION_CENTER_TOP);
			m_pContextMenu->Move(x - (m_pContextMenu->GetWidth() / 2),
				y - (-imageHeightArrow + imageHeightBorderLeft));
		}
		else
		{
			m_pContextMenu->SetArrowPosition(SallyAPI::GUI::CONTEXT_MENU_ARROW_POSITION_RIGHT_TOP);
			m_pContextMenu->Move(x - m_pContextMenu->GetWidth() + imageWidthBorderRight + (imageWidthArrow / 2),
				y - (-imageHeightArrow + imageHeightBorderLeft));
		}
	}
	else
	{
		// get the arrow height
		image = GetPicture(GUI_THEME_GROUPBOX_ARROW_BOTTOM);
		if (image != NULL)
		{
			imageHeightArrow = image->GetHeight();
			imageWidthArrow = image->GetWidth();
		}

		image = GetPicture(GUI_THEME_GROUPBOX_LEFT_BOTTOM);
		if (image != NULL) 
		{
			imageHeightBorderLeft = image->GetHeight();
			imageWidthBorderLeft = image->GetWidth();
		}

		image = GetPicture(GUI_THEME_GROUPBOX_RIGHT_BOTTOM);
		if (image != NULL) 
		{
			imageWidthBorderRight = image->GetWidth();
		}

		// set the arrow
		if (x < m_iWidth / 3)
		{
			m_pContextMenu->SetArrowPosition(SallyAPI::GUI::CONTEXT_MENU_ARROW_POSITION_LEFT_BOTTOM);
			m_pContextMenu->Move(x - imageWidthBorderLeft - (imageWidthArrow / 2),
				y - m_pContextMenu->GetHeight() - imageHeightArrow + imageHeightBorderLeft);
		}
		else if ((x > m_iWidth / 3) && (x < (m_iWidth / 3) * 2))
		{
			m_pContextMenu->SetArrowPosition(SallyAPI::GUI::CONTEXT_MENU_ARROW_POSITION_CENTER_BOTTOM);
			m_pContextMenu->Move(x - (m_pContextMenu->GetWidth() / 2),
				y - m_pContextMenu->GetHeight() - imageHeightArrow + imageHeightBorderLeft);
		}
		else
		{
			m_pContextMenu->SetArrowPosition(SallyAPI::GUI::CONTEXT_MENU_ARROW_POSITION_RIGHT_BOTTOM);
			m_pContextMenu->Move(x - m_pContextMenu->GetWidth() + imageWidthBorderRight + (imageWidthArrow / 2),
				y - m_pContextMenu->GetHeight() - imageHeightArrow + imageHeightBorderLeft);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CContextMenuPopUp::SetPosition(int x, int y,
/// CONTEXT_MENU_ARROW_POSITION arrowPosition)
///
/// \brief	Sets a position. 
///
/// \author	Christian Knobloch
/// \date	30.03.2011
///
/// \param	x				The x coordinate. 
/// \param	y				The y coordinate. 
/// \param	arrowPosition	The arrow position. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CContextMenuPopUp::SetPosition(int x, int y, CONTEXT_MENU_ARROW_POSITION arrowPosition)
{
	m_pContextMenu->Move(x, y);
	m_pContextMenu->SetArrowPosition(arrowPosition);
}