////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\LabelBox.cpp
///
/// \brief	Implements the label box class. 
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

#include "LabelBox.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CLabelBox::CLabelBox(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width,
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

CLabelBox::CLabelBox(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int height, int controlId)
	:SallyAPI::GUI::CForm(parent, x, y, width, height, controlId)
{
	m_iAlign = DT_WORDBREAK;
	SetLocalised(false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CLabelBox::~CLabelBox()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CLabelBox::~CLabelBox()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CLabelBox::SetFont(const std::string& fontName)
///
/// \brief	Sets a font. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	fontName	Name of the font. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CLabelBox::SetFont(const std::string& fontName)
{
	m_strFontName = fontName;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CLabelBox::RenderControl()
///
/// \brief	Renders a control. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CLabelBox::RenderControl()
{
	SallyAPI::GUI::CPicture* image = NULL;

	int imageWidthLeftTop = 0;
	int imageWidthRightTop = 0;
	int imageWidthTop = 1;
	int imageHeightTop = 0;

	int imageWidthLeft = 0;
	int imageWidthCenter = 0;
	int imageWidthRight = 1;
	int imageHeightCenter = 0;

	int imageWidthLeftBottom = 0;
	int imageWidthRightBottom = 0;
	int imageWidthBottom = 1;
	int imageHeightBottom = 0;

	// Top
	image = GetPicture(GUI_THEME_LABELBOX_LEFT_TOP);
	if (image != NULL) {
		imageWidthLeftTop = image->GetWidth();
		if (imageHeightTop == 0) {
			imageHeightTop = image->GetHeight();
		}
	}
	image = GetPicture(GUI_THEME_LABELBOX_RIGHT_TOP);
	if (image != NULL) {
		imageWidthRightTop = image->GetWidth();
		if (imageHeightTop == 0) {
			imageHeightTop = image->GetHeight();
		}
	}
	image = GetPicture(GUI_THEME_LABELBOX_TOP);
	if (image != NULL) {
		imageWidthTop = image->GetWidth();
		if (imageHeightTop == 0) {
			imageHeightTop = image->GetHeight();
		}
	}

	// Center
	image = GetPicture(GUI_THEME_LABELBOX_LEFT);
	if (image != NULL) {
		imageWidthLeft = image->GetWidth();
	}
	image = GetPicture(GUI_THEME_LABELBOX_RIGHT);
	if (image != NULL) {
		imageWidthRight = image->GetWidth();
	}
	image = GetPicture(GUI_THEME_LABELBOX_CENTER);
	if (image != NULL) {
		imageWidthCenter = image->GetWidth();
	}

	// Bottom
	image = GetPicture(GUI_THEME_LABELBOX_LEFT_BOTTOM);
	if (image != NULL) {
		imageWidthLeftBottom = image->GetWidth();
		if (imageHeightBottom == 0) {
			imageHeightBottom = image->GetHeight();
		}
	}
	image = GetPicture(GUI_THEME_LABELBOX_RIGHT_BOTTOM);
	if (image != NULL) {
		imageWidthRightBottom = image->GetWidth();
		if (imageHeightBottom == 0) {
			imageHeightBottom = image->GetHeight();
		}
	}
	image = GetPicture(GUI_THEME_LABELBOX_BOTTOM);
	if (image != NULL) {
		imageWidthBottom = image->GetWidth();
		if (imageHeightBottom == 0) {
			imageHeightBottom = image->GetHeight();
		}
	}

	imageHeightCenter = m_iHeight - (imageHeightBottom + imageHeightTop);

	// Top
	DrawImage(GUI_THEME_LABELBOX_LEFT_TOP, 0, 0);
	DrawImage(GUI_THEME_LABELBOX_TOP, imageWidthLeftTop, 0, m_iWidth - (imageWidthLeftTop + imageWidthRightTop), imageHeightTop);
	DrawImage(GUI_THEME_LABELBOX_RIGHT_TOP, m_iWidth - imageWidthRightTop, 0);

	// Bottom
	DrawImage(GUI_THEME_LABELBOX_LEFT, 0, imageHeightTop, imageWidthLeft, imageHeightCenter);
	DrawImage(GUI_THEME_LABELBOX_CENTER, imageWidthLeft, imageHeightTop, m_iWidth - (imageWidthLeftBottom + imageWidthRightBottom), imageHeightCenter);
	DrawImage(GUI_THEME_LABELBOX_RIGHT, m_iWidth - imageWidthRight, imageHeightTop, imageWidthRight, imageHeightCenter);

	// Bottom
	DrawImage(GUI_THEME_LABELBOX_LEFT_BOTTOM, 0, imageHeightCenter + imageHeightTop);
	DrawImage(GUI_THEME_LABELBOX_BOTTOM, imageWidthLeftBottom, imageHeightCenter + imageHeightTop, m_iWidth - (imageWidthLeftBottom + imageWidthRightBottom), imageHeightBottom);
	DrawImage(GUI_THEME_LABELBOX_RIGHT_BOTTOM, m_iWidth - imageWidthRightBottom, imageHeightCenter + imageHeightBottom);

	// Draw the Text
	if (m_strFontName.length() != 0)
	{
		DrawText(GUI_THEME_LABELBOX_LEFT, GUI_THEME_LABELBOX_RIGHT, 4, 4, GUI_THEME_LABELBOX_TOP, GUI_THEME_LABELBOX_BOTTOM, 4, 4, m_strFontName);
	}
	else
	{
		DrawText(GUI_THEME_LABELBOX_LEFT, GUI_THEME_LABELBOX_RIGHT, 4, 4, GUI_THEME_LABELBOX_TOP, GUI_THEME_LABELBOX_BOTTOM, 4, 4, "labelbox.font");
	}
}
