////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\SideMenu.cpp
///
/// \brief	Implements the side menu class. 
///
/// \author	Christian Knobloch
/// \date	13.09.2010
///
/// This file is part of the Sally Project
/// 
/// Copyright(c) 2008-2010 Sally Project
/// http://www.sally-project.de/
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

#include "SideMenu.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CSideMenu::CSideMenu(SallyAPI::GUI::CGUIBaseObject* parent,
/// int controlId) :SallyAPI::GUI::CForm(parent, 0, 0, 0, 0, controlId)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	parent	If non-null, the parent. 
/// \param	controlId		Identifier for the control. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CSideMenu::CSideMenu(SallyAPI::GUI::CGUIBaseObject* parent, int controlId)
	:SallyAPI::GUI::CForm(parent, 0, 0, 0, 0, controlId)
{
	Move(WINDOW_WIDTH - MENU_WIDTH, 0);
	Resize(MENU_WIDTH, WINDOW_HEIGHT);

	m_iYPosition = MENU_HEIGHT;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CSideMenu::~CSideMenu()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CSideMenu::~CSideMenu()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CSideMenu::RenderControl()
///
/// \brief	Renders a control. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CSideMenu::RenderControl()
{
	DrawImage(GUI_THEME_SIDE_MENU_BACKGROUND, 0, 0, m_iWidth, MAX_HEIGHT);

	SallyAPI::GUI::CForm::RenderControl();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CSideMenu::AddChild(SallyAPI::GUI::CControl* control)
///
/// \brief	Adds a child. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	control	If non-null, the control. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CSideMenu::AddChild(SallyAPI::GUI::CControl* control)
{
	SallyAPI::GUI::CForm::AddChild(control);

	control->Resize(m_iWidth, CONTROL_HEIGHT);
	control->Move(0, m_iYPosition);
	m_iYPosition += CONTROL_HEIGHT;
}