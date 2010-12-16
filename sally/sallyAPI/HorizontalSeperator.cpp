////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\HorizontalSeperator.cpp
///
/// \brief	Implements the horizontal seperator class. 
///
/// \author	Christian Knobloch
/// \date	19.09.2010
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

#include "HorizontalSeperator.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CHorizontalSeperator::CHorizontalSeperator(SallyAPI::GUI::CGUIBaseObject* parent, int x,
/// int y, int width, int controlId) :SallyAPI::GUI::CControl(parent, x, y, width, 4, controlId)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.09.2010
///
/// \param [in,out]	parent	If non-null, the parent. 
/// \param	x				The x coordinate. 
/// \param	y				The y coordinate. 
/// \param	width			The width. 
/// \param	controlId		Identifier for the control. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CHorizontalSeperator::CHorizontalSeperator(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int controlId)
	:SallyAPI::GUI::CControl(parent, x, y, width, 4, controlId)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CHorizontalSeperator::~CHorizontalSeperator()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.09.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CHorizontalSeperator::~CHorizontalSeperator()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CHorizontalSeperator::RenderControl()
///
/// \brief	Renders a control. 
///
/// \author	Christian Knobloch
/// \date	19.09.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CHorizontalSeperator::RenderControl()
{
	// Temp variables
	SallyAPI::GUI::CPicture* image = NULL;
	int imageWidthLeft = 0;
	int imageWidthRight = 0;
	int imageWidthMiddle = 1;

	image = GetPicture(GUI_THEME_SEPERATOR_H_LEFT);
	if (image != NULL) {
		imageWidthLeft = image->GetWidth();
	}
	image = GetPicture(GUI_THEME_SEPERATOR_H_RIGHT);
	if (image != NULL) {
		imageWidthRight = image->GetWidth();
	}
	image = GetPicture(GUI_THEME_SEPERATOR_H_CENTER);
	if (image != NULL) {
		imageWidthMiddle = image->GetWidth();
	}

	// Draw Left
	DrawImage(GUI_THEME_SEPERATOR_H_LEFT, 0, 0);

	DrawImage(GUI_THEME_SEPERATOR_H_CENTER, imageWidthLeft, 0, m_iWidth - (imageWidthLeft + imageWidthRight), m_iHeight);

	// Draw Right
	DrawImage(GUI_THEME_SEPERATOR_H_RIGHT, m_iWidth - imageWidthRight, 0);
}