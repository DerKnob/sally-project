////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\GroupBox.cpp
///
/// \brief	Implements the group box class. 
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

#include "GroupBox.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CGroupBox::CGroupBox(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width,
/// int height, int controlId) :SallyAPI::GUI::CForm(parent, x, y, width, height, controlId)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	parent	If non-null, the parent. 
/// \param	x				The x coordinate. 
/// \param	y				The y coordinate. 
/// \param	width			The width. 
/// \param	height			The height. 
/// \param	controlId		Identifier for the control. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CGroupBox::CGroupBox(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int height, int controlId)
	:SallyAPI::GUI::CForm(parent, x, y, width, height, controlId)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CGroupBox::~CGroupBox()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CGroupBox::~CGroupBox()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CGroupBox::RenderControl()
///
/// \brief	Renders a control. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CGroupBox::RenderControl()
{
	DrawButtonBackground(GUI_THEME_GROUPBOX_LEFT_TOP, GUI_THEME_GROUPBOX_TOP, GUI_THEME_GROUPBOX_RIGHT_TOP, 
		GUI_THEME_GROUPBOX_LEFT, GUI_THEME_GROUPBOX_CENTER, GUI_THEME_GROUPBOX_RIGHT, 
		GUI_THEME_GROUPBOX_LEFT_BOTTOM, GUI_THEME_GROUPBOX_BOTTOM, GUI_THEME_GROUPBOX_RIGHT_BOTTOM);

	// Render Childs
	SallyAPI::GUI::CForm::RenderControl();
}