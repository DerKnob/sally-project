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
	SetBlendIn(SallyAPI::GUI::POPUP_BLEND_IN_CENTER);

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
		int newX = 0;
		int newY = y - (-imageHeightArrow + imageHeightBorderLeft);

		if (x < m_iWidth / 3)
		{
			m_pContextMenu->SetArrowPosition(SallyAPI::GUI::CONTEXT_MENU_ARROW_POSITION_LEFT_TOP);
			newX = x - imageWidthBorderLeft - (imageWidthArrow / 2);
		}
		else if ((x > m_iWidth / 3) && (x < (m_iWidth / 3) * 2))
		{
			m_pContextMenu->SetArrowPosition(SallyAPI::GUI::CONTEXT_MENU_ARROW_POSITION_CENTER_TOP);
			newX = x - (m_pContextMenu->GetWidth() / 2);
		}
		else
		{
			m_pContextMenu->SetArrowPosition(SallyAPI::GUI::CONTEXT_MENU_ARROW_POSITION_RIGHT_TOP);
			newX = x - m_pContextMenu->GetWidth() + imageWidthBorderRight + (imageWidthArrow / 2);
		}

		m_pContextMenu->Move(newX, newY - 50);
		m_pContextMenu->MoveAnimated(newX, newY, 400, true);
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
		int newX = 0;
		int newY = y - m_pContextMenu->GetHeight() - imageHeightArrow + imageHeightBorderLeft;

		if (x < m_iWidth / 3)
		{
			m_pContextMenu->SetArrowPosition(SallyAPI::GUI::CONTEXT_MENU_ARROW_POSITION_LEFT_BOTTOM);
			newX = x - imageWidthBorderLeft - (imageWidthArrow / 2);
		}
		else if ((x > m_iWidth / 3) && (x < (m_iWidth / 3) * 2))
		{
			m_pContextMenu->SetArrowPosition(SallyAPI::GUI::CONTEXT_MENU_ARROW_POSITION_CENTER_BOTTOM);
			newX = x - (m_pContextMenu->GetWidth() / 2);
		}
		else
		{
			m_pContextMenu->SetArrowPosition(SallyAPI::GUI::CONTEXT_MENU_ARROW_POSITION_RIGHT_BOTTOM);
			newX = x - m_pContextMenu->GetWidth() + imageWidthBorderRight + (imageWidthArrow / 2);
		}

		m_pContextMenu->Move(newX, newY + 50);
		m_pContextMenu->MoveAnimated(newX, newY, 400, true);
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

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CContextMenuPopUp::SetPopUpControlPoint(SallyAPI::GUI::CControl* control)
///
/// \brief	Sets a pop up point by the specified control. 
///
/// \author	Christian Knobloch
/// \date	01.04.2011
///
/// \param [in,out]	control	If non-null, the control. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CContextMenuPopUp::SetPopUpControlPoint(SallyAPI::GUI::CControl* control)
{
	int x = 0;
	int y = 0;
	
	control->GetAbsolutPosition(&x, &y);
	
	x = x + (control->GetWidth() / 2);

	y = y + (control->GetHeight() / 2);
	if (y > WINDOW_HEIGHT / 2)
		y = y - (control->GetHeight() / 2); // popup above the dropdown
	else
		y = y + (control->GetHeight() / 2); // popup below the dropdown

	SetPopUpPoint(x, y);
}