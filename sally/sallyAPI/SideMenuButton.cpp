////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\SideMenuButton.cpp
///
/// \brief	Implements the side menu button class. 
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

#include "SideMenuButton.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CSideMenuButton::CSideMenuButton(SallyAPI::GUI::CGUIBaseObject* parent,
/// SallyAPI::GUI::SideMenuButtonType sideMenuType, int controlId) :SallyAPI::GUI::CButton(parent,
/// 0, 0, MENU_WIDTH, CONTROL_HEIGHT, controlId, SallyAPI::GUI::BUTTON_TYPE_NORMAL)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	parent	If non-null, the parent. 
/// \param	sideMenuType	Type of the side menu. 
/// \param	controlId		Identifier for the control. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CSideMenuButton::CSideMenuButton(SallyAPI::GUI::CGUIBaseObject* parent, SallyAPI::GUI::SideMenuButtonType sideMenuType, int controlId)
	:SallyAPI::GUI::CButton(parent, 0, 0, MENU_WIDTH, CONTROL_HEIGHT, controlId, SallyAPI::GUI::BUTTON_TYPE_NORMAL)
{
	m_eSideMenuType = sideMenuType;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CSideMenuButton::~CSideMenuButton()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CSideMenuButton::~CSideMenuButton()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CSideMenuButton::RenderControl()
///
/// \brief	Renders a control. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CSideMenuButton::RenderControl()
{
	int borderLeft = 4;
	int borderRight = 4;

	switch (m_eSideMenuType)
	{
	case SIDE_MENUE_BUTTON_TYPE_NORMAL:
		if ((m_bChecked) || (m_bPressed) || (m_bActive))
		{
			DrawButtonBackground(GUI_THEME_SIDE_MENU_BUTTON_SELECTED_LEFT, GUI_THEME_SIDE_MENU_BUTTON_SELECTED, GUI_THEME_SIDE_MENU_BUTTON_SELECTED_RIGHT);
		}
		else
		{
			DrawButtonBackground(GUI_THEME_SIDE_MENU_BUTTON_NORMAL_LEFT, GUI_THEME_SIDE_MENU_BUTTON_NORMAL, GUI_THEME_SIDE_MENU_BUTTON_NORMAL_RIGHT);
		}

		if (m_iImage)
		{
			if (m_bImageLeft)
			{
				borderLeft += 4 + m_iImageSize;
				int imageWidthLeft = 0;

				if (m_strText.length() > 0)
				{
					// Temp variables
					SallyAPI::GUI::CPicture* image = NULL;

					image = GetPicture(GUI_THEME_SIDE_MENU_BUTTON_SELECTED_LEFT);
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

					image = GetPicture(GUI_THEME_SIDE_MENU_BUTTON_SELECTED_RIGHT);
					if (image != NULL) {
						imageWidthRight = image->GetWidth();
					}
				}
				DrawImage(m_iImage, m_iWidth - (m_iImageSize + imageWidthRight), (m_iHeight - m_iImageSize) / 2, m_iImageSize, m_iImageSize);
			}
		}
		if ((m_bChecked) || (m_bPressed) || (m_bActive))
			DrawText(GUI_THEME_SIDE_MENU_BUTTON_SELECTED_LEFT, GUI_THEME_SIDE_MENU_BUTTON_SELECTED_RIGHT, borderLeft, borderRight, "sidebutton.active.font");
		else
			DrawText(GUI_THEME_SIDE_MENU_BUTTON_NORMAL_LEFT, GUI_THEME_SIDE_MENU_BUTTON_NORMAL_RIGHT, borderLeft, borderRight, "sidebutton.font");
		break;
	case SIDE_MENUE_BUTTON_TYPE_HEADING:
		DrawImage(GUI_THEME_SIDE_MENU_SEPERATOR, 0, 0);
		DrawText(0, 0, 6, 6, "sidebutton.heading.font");
		break;
	case SIDE_MENUE_BUTTON_TYPE_SEPERATOR:
		break;
	}
}