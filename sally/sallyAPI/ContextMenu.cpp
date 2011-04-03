////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ContextMenu.cpp
///
/// \brief	Implements the context menu class. 
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

#include "ContextMenu.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CContextMenu::CContextMenu(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width,
/// int height, int controlId) :SallyAPI::GUI::CForm(parent, x, y, width, height, controlId)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	30.03.2011
///
/// \param [in,out]	parent	If non-null, the parent. 
/// \param	x				The x coordinate. 
/// \param	y				The y coordinate. 
/// \param	width			The width. 
/// \param	height			The height. 
/// \param	controlId		Identifier for the control. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CContextMenu::CContextMenu(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int height, int controlId)
	:SallyAPI::GUI::CForm(parent, x, y, width, height, controlId), m_ePosition(CONTEXT_MENU_ARROW_POSITION_LEFT_TOP)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CContextMenu::~CContextMenu()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	30.03.2011
////////////////////////////////////////////////////////////////////////////////////////////////////

CContextMenu::~CContextMenu()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CContextMenu::RenderControl()
///
/// \brief	Renders a control. 
///
/// \author	Christian Knobloch
/// \date	30.03.2011
////////////////////////////////////////////////////////////////////////////////////////////////////

void CContextMenu::RenderControl()
{
	SallyAPI::GUI::CPicture* image = NULL;
	int imageHeightLeft = 0;
	int imageWidthLeft = 0;
	int imageHeightArrow = 0;
	int imageWidthArrow = 0;
	int imageWidthRight = 0;

	switch (m_ePosition)
	{
	case CONTEXT_MENU_ARROW_POSITION_LEFT_TOP:
	case CONTEXT_MENU_ARROW_POSITION_RIGHT_TOP:
	case CONTEXT_MENU_ARROW_POSITION_CENTER_TOP:
		DrawButtonBackground(GUI_THEME_GROUPBOX_LEFT_TOP, -1, GUI_THEME_GROUPBOX_RIGHT_TOP, 
			GUI_THEME_GROUPBOX_LEFT, GUI_THEME_GROUPBOX_CENTER, GUI_THEME_GROUPBOX_RIGHT, 
			GUI_THEME_GROUPBOX_LEFT_BOTTOM, GUI_THEME_GROUPBOX_BOTTOM, GUI_THEME_GROUPBOX_RIGHT_BOTTOM);

		image = GetPicture(GUI_THEME_GROUPBOX_ARROW_TOP);
		if (image != NULL) {
			imageHeightArrow = image->GetHeight();
			imageWidthArrow = image->GetWidth();
		}

		image = GetPicture(GUI_THEME_GROUPBOX_LEFT_TOP);
		if (image != NULL) {
			imageHeightLeft = image->GetHeight();
			imageWidthLeft = image->GetWidth();
		}

		image = GetPicture(GUI_THEME_GROUPBOX_RIGHT_TOP);
		if (image != NULL) {
			imageWidthRight = image->GetWidth();
		}

		switch (m_ePosition)
		{
		case CONTEXT_MENU_ARROW_POSITION_LEFT_TOP:
			DrawImage(GUI_THEME_GROUPBOX_ARROW_TOP, imageWidthLeft, -imageHeightArrow + imageHeightLeft);
			DrawImage(GUI_THEME_GROUPBOX_TOP, imageWidthLeft + imageWidthArrow, 0,
				m_iWidth - (imageWidthLeft + imageWidthRight + imageWidthArrow), imageHeightLeft);
			break;
		case CONTEXT_MENU_ARROW_POSITION_RIGHT_TOP:
			DrawImage(GUI_THEME_GROUPBOX_TOP, imageWidthLeft, 0,
				m_iWidth - (imageWidthLeft + imageWidthRight + imageWidthArrow), imageHeightLeft);
			DrawImage(GUI_THEME_GROUPBOX_ARROW_TOP, m_iWidth - (imageWidthRight + imageWidthArrow), -imageHeightArrow + imageHeightLeft);
			break;
		case CONTEXT_MENU_ARROW_POSITION_CENTER_TOP:
			DrawImage(GUI_THEME_GROUPBOX_TOP, imageWidthLeft, 0,
				((m_iWidth - imageWidthArrow) / 2) - imageWidthLeft, imageHeightLeft);
			DrawImage(GUI_THEME_GROUPBOX_ARROW_TOP, (m_iWidth - imageWidthArrow) / 2, -imageHeightArrow + imageHeightLeft);
			DrawImage(GUI_THEME_GROUPBOX_TOP, ((m_iWidth - imageWidthArrow) / 2) + imageWidthArrow, 0,
				m_iWidth - (((m_iWidth - imageWidthArrow) / 2) + imageWidthArrow + imageWidthRight), imageHeightLeft);
			break;
		}
		break;
	case CONTEXT_MENU_ARROW_POSITION_LEFT_BOTTOM:
	case CONTEXT_MENU_ARROW_POSITION_RIGHT_BOTTOM:
	case CONTEXT_MENU_ARROW_POSITION_CENTER_BOTTOM:
		DrawButtonBackground(GUI_THEME_GROUPBOX_LEFT_TOP, GUI_THEME_GROUPBOX_TOP, GUI_THEME_GROUPBOX_RIGHT_TOP, 
			GUI_THEME_GROUPBOX_LEFT, GUI_THEME_GROUPBOX_CENTER, GUI_THEME_GROUPBOX_RIGHT, 
			GUI_THEME_GROUPBOX_LEFT_BOTTOM, -1, GUI_THEME_GROUPBOX_RIGHT_BOTTOM);

		image = GetPicture(GUI_THEME_GROUPBOX_ARROW_BOTTOM);
		if (image != NULL) {
			imageHeightArrow = image->GetHeight();
			imageWidthArrow = image->GetWidth();
		}

		image = GetPicture(GUI_THEME_GROUPBOX_LEFT_BOTTOM);
		if (image != NULL) {
			imageHeightLeft = image->GetHeight();
			imageWidthLeft = image->GetWidth();
		}

		image = GetPicture(GUI_THEME_GROUPBOX_RIGHT_BOTTOM);
		if (image != NULL) {
			imageWidthRight = image->GetWidth();
		}

		switch (m_ePosition)
		{
		case CONTEXT_MENU_ARROW_POSITION_LEFT_BOTTOM:
			DrawImage(GUI_THEME_GROUPBOX_ARROW_BOTTOM, imageWidthLeft, m_iHeight - imageHeightLeft);
			DrawImage(GUI_THEME_GROUPBOX_BOTTOM, imageWidthLeft + imageWidthArrow, m_iHeight - imageHeightLeft,
				m_iWidth - (imageWidthLeft + imageWidthRight + imageWidthArrow), imageHeightLeft);
			break;
		case CONTEXT_MENU_ARROW_POSITION_RIGHT_BOTTOM:
			DrawImage(GUI_THEME_GROUPBOX_BOTTOM, imageWidthLeft, m_iHeight - imageHeightLeft,
				m_iWidth - (imageWidthLeft + imageWidthRight + imageWidthArrow), imageHeightLeft);
			DrawImage(GUI_THEME_GROUPBOX_ARROW_BOTTOM, m_iWidth - (imageWidthRight + imageWidthArrow), m_iHeight - imageHeightLeft);
			break;
		case CONTEXT_MENU_ARROW_POSITION_CENTER_BOTTOM:
			DrawImage(GUI_THEME_GROUPBOX_BOTTOM, imageWidthLeft, m_iHeight - imageHeightLeft,
				((m_iWidth - imageWidthArrow) / 2) - imageWidthLeft, imageHeightLeft);
			DrawImage(GUI_THEME_GROUPBOX_ARROW_BOTTOM, (m_iWidth - imageWidthArrow) / 2, m_iHeight - imageHeightLeft);
			DrawImage(GUI_THEME_GROUPBOX_BOTTOM, ((m_iWidth - imageWidthArrow) / 2) + imageWidthArrow, m_iHeight - imageHeightLeft,
				m_iWidth - (((m_iWidth - imageWidthArrow) / 2) + imageWidthArrow + imageWidthRight), imageHeightLeft);
			break;
		}
		break;
	}
	

	// Render Childs
	SallyAPI::GUI::CForm::RenderControl();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CContextMenu::SetArrowPosition(CONTEXT_MENU_ARROW_POSITION position)
///
/// \brief	Sets an arrow position. 
///
/// \author	Christian Knobloch
/// \date	30.03.2011
///
/// \param	position	The position. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CContextMenu::SetArrowPosition(CONTEXT_MENU_ARROW_POSITION position)
{
	m_ePosition = position;
}