////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Breadcrumb.cpp
///
/// \brief	Implements the breadcrumb class. 
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

#include "Breadcrumb.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CBreadcrumb::CBreadcrumb(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width,
/// 	int controlId) :SallyAPI::GUI::CControl(parent, x, y, width, CONTROL_HEIGHT, controlId),
/// 	m_iImage(NULL), m_iImageSize(ICON_SIZE)
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
/// \param	controlId		Identifier for the control. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CBreadcrumb::CBreadcrumb(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int controlId)
	:SallyAPI::GUI::CControl(parent, x, y, width, CONTROL_HEIGHT, controlId), m_iImage(NULL), m_iImageSize(ICON_SIZE)
{
	m_bLocalised = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CBreadcrumb::~CBreadcrumb()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CBreadcrumb::~CBreadcrumb()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CBreadcrumb::RenderControl()
///
/// \brief	Renders a control. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CBreadcrumb::RenderControl()
{
	DrawButtonBackground(GUI_THEME_BREADCRUMB_LEFT, GUI_THEME_BREADCRUMB, GUI_THEME_BREADCRUMB_RIGHT);

	int borderLeft = 4;
	int borderRight = 4;
	int imageWidthLeft = 0;
	int imageWidthSep = 0;
	if (m_iImage)
	{
		// Temp variables
		SallyAPI::GUI::CPicture* image = NULL;

		image = GetPicture(GUI_THEME_BREADCRUMB_LEFT);
		if (image != NULL) {
			imageWidthLeft = image->GetWidth();
		}

		DrawImage(m_iImage, imageWidthLeft, (m_iHeight - m_iImageSize) / 2, m_iImageSize, m_iImageSize);
	}
	SallyAPI::GUI::CPicture* imageSep = GetPicture(GUI_THEME_BREADCRUMB_SEPERATOR);
	if (imageSep != NULL) {
		imageWidthSep = imageSep->GetWidth();
	}
	DrawImage(GUI_THEME_BREADCRUMB_SEPERATOR, imageWidthLeft + m_iImageSize, 0);

	DrawText(GUI_THEME_BREADCRUMB_LEFT, GUI_THEME_BREADCRUMB_RIGHT, borderLeft + imageWidthLeft + m_iImageSize + imageWidthSep, borderRight, "breadcrumb.font");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CBreadcrumb::SetImageId(int image)
///
/// \brief	Sets an image identifier. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	image	The image. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CBreadcrumb::SetImageId(int image)
{
	m_iImage = image;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CBreadcrumb::ProcessMouseUp(int x, int y)
///
/// \brief	Process a mouse up. 
///
/// \author	Christian Knobloch
/// \date	09.04.2011
///
/// \param	x	The x coordinate. 
/// \param	y	The y coordinate. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CBreadcrumb::ProcessMouseUp(int x, int y)
{
	if (m_bMouseDown)
	{
		int imageWidthLeft = 35;
		int xControl = 0;
		int yControl = 0;

		GetAbsolutPosition(&xControl, &yControl);

		// Temp variables
		SallyAPI::GUI::CPicture* image = NULL;

		image = GetPicture(GUI_THEME_BREADCRUMB_LEFT);
		if (image != NULL) {
			imageWidthLeft += image->GetWidth();
		}

		if (x - xControl< imageWidthLeft)
		{
			m_pParent->SendMessageToParent(this, m_iControlId, GUI_BREADCRUMB_START_CLICKED);
		}
	}
	SallyAPI::GUI::CControl::ProcessMouseUp(x, y);
	return true;
}