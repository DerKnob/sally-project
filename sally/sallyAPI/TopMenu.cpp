////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\TopMenu.cpp
///
/// \brief	Implements the top menu class. 
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

#include "TopMenu.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CTopMenu::CTopMenu(SallyAPI::GUI::CGUIBaseObject* parent,
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

CTopMenu::CTopMenu(SallyAPI::GUI::CGUIBaseObject* parent, int controlId)
	:SallyAPI::GUI::CForm(parent, 0, 0, 0, 0, controlId)
{
	Move(0, 0);
	Resize(WINDOW_WIDTH, MENU_HEIGHT);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CTopMenu::~CTopMenu()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CTopMenu::~CTopMenu()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CTopMenu::RenderControl()
///
/// \brief	Renders a control. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CTopMenu::RenderControl()
{
	DrawImage(GUI_THEME_SALLY_SETTINGS_TOP_BORDER, 0, 0, MAX_WIDTH, m_iHeight);

	SallyAPI::GUI::CForm::RenderControl();
}
