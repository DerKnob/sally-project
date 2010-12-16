////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\VerticalSeperator.cpp
///
/// \brief	Implements the vertical seperator class. 
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

#include "VerticalSeperator.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CVerticalSeperator::CVerticalSeperator(SallyAPI::GUI::CGUIBaseObject* parent, int x,
/// int y, int height, int controlId) :SallyAPI::GUI::CControl(parent, x, y, 4, height, controlId)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.09.2010
///
/// \param [in,out]	parent	If non-null, the parent. 
/// \param	x				The x coordinate. 
/// \param	y				The y coordinate. 
/// \param	height			The height. 
/// \param	controlId		Identifier for the control. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CVerticalSeperator::CVerticalSeperator(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int height, int controlId)
	:SallyAPI::GUI::CControl(parent, x, y, 4, height, controlId)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CVerticalSeperator::~CVerticalSeperator()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.09.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CVerticalSeperator::~CVerticalSeperator()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CVerticalSeperator::RenderControl()
///
/// \brief	Renders a control. 
///
/// \author	Christian Knobloch
/// \date	19.09.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CVerticalSeperator::RenderControl()
{
	// Temp variables
	SallyAPI::GUI::CPicture* image = NULL;
	int imageHeightTop = 0;
	int imageHeightBottom = 0;
	int imageHeightMiddle = 1;

	image = GetPicture(GUI_THEME_SEPERATOR_V_TOP);
	if (image != NULL) {
		imageHeightTop = image->GetHeight();
	}
	image = GetPicture(GUI_THEME_SEPERATOR_V_CENTER);
	if (image != NULL) {
		imageHeightBottom = image->GetHeight();
	}
	image = GetPicture(GUI_THEME_SEPERATOR_V_BOTTOM);
	if (image != NULL) {
		imageHeightMiddle = image->GetHeight();
	}

	// Draw Left
	DrawImage(GUI_THEME_SEPERATOR_V_TOP, 0, 0);

	DrawImage(GUI_THEME_SEPERATOR_V_CENTER, 0, imageHeightTop, m_iWidth, m_iHeight - (imageHeightTop + imageHeightBottom));

	// Draw Right
	DrawImage(GUI_THEME_SEPERATOR_V_BOTTOM, 0, m_iHeight - imageHeightBottom);
}