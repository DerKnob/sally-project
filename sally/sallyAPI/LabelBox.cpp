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

#include "LabelBox.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CLabelBox::CLabelBox(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width,
/// int height, bool showScrollbar, int controlId) :SallyAPI::GUI::CForm(parent, x, y, width,
/// height, controlId)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	06.07.2011
///
/// \param [in,out]	parent	If non-null, the parent. 
/// \param	x				The x coordinate. 
/// \param	y				The y coordinate. 
/// \param	width			The width. 
/// \param	height			The height. 
/// \param	showScrollbar	true to show, false to hide the scrollbar. 
/// \param	controlId		Identifier for the control. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CLabelBox::CLabelBox(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int height, bool showScrollbar, int controlId)
	:SallyAPI::GUI::CForm(parent, x, y, width, height, controlId), m_bAutoResize(false)
{
	m_iAlign = DT_WORDBREAK;

	if (showScrollbar)
	{
		m_pScrollbar = new SallyAPI::GUI::CScrollbar(this, width - CONTROL_HEIGHT, 0, CONTROL_HEIGHT, height, SallyAPI::GUI::SCROLLBAR_ALIGNMENT_VERTICAL);
		m_pScrollbar->ShowScrollbarIfNotScrollable(true);
		this->AddChild(m_pScrollbar);
	}
	else
	{
		m_pScrollbar = NULL;
	}

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
/// \fn	void CLabelBox::SetText(const std::string& text)
///
/// \brief	Sets a text. 
///
/// \author	Christian Knobloch
/// \date	06.07.2011
///
/// \param	text	The text. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CLabelBox::SetText(const std::string& text)
{
	EnterRenderLock();

	SallyAPI::GUI::CControl::SetText(text);

	AutoResize();
	UpdateScrollbar();

	LeaveRenderLock();
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

	int widthTemp = m_iWidth;
	if (m_pScrollbar != NULL)
		widthTemp -= CONTROL_HEIGHT;

	// Top
	DrawImage(GUI_THEME_LABELBOX_LEFT_TOP, 0, 0);
	DrawImage(GUI_THEME_LABELBOX_TOP, imageWidthLeftTop, 0, widthTemp - (imageWidthLeftTop + imageWidthRightTop), imageHeightTop);
	DrawImage(GUI_THEME_LABELBOX_RIGHT_TOP, widthTemp - imageWidthRightTop, 0);

	// Bottom
	DrawImage(GUI_THEME_LABELBOX_LEFT, 0, imageHeightTop, imageWidthLeft, imageHeightCenter);
	DrawImage(GUI_THEME_LABELBOX_CENTER, imageWidthLeft, imageHeightTop, widthTemp - (imageWidthLeftBottom + imageWidthRightBottom), imageHeightCenter);
	DrawImage(GUI_THEME_LABELBOX_RIGHT, widthTemp - imageWidthRight, imageHeightTop, imageWidthRight, imageHeightCenter);

	// Bottom
	DrawImage(GUI_THEME_LABELBOX_LEFT_BOTTOM, 0, imageHeightCenter + imageHeightTop);
	DrawImage(GUI_THEME_LABELBOX_BOTTOM, imageWidthLeftBottom, imageHeightCenter + imageHeightTop, widthTemp - (imageWidthLeftBottom + imageWidthRightBottom), imageHeightBottom);
	DrawImage(GUI_THEME_LABELBOX_RIGHT_BOTTOM, widthTemp - imageWidthRightBottom, imageHeightCenter + imageHeightBottom);

	// calculate the scrolling
	int position = 0;

	if (m_pScrollbar != NULL)
		position = m_pScrollbar->GetPosition();

	int borderTop = 4 - position;
	int borderRight = 4;
	if (m_pScrollbar != NULL)
		borderRight += CONTROL_HEIGHT;

	SallyAPI::Core::CCamera* camera = SallyAPI::Core::CGame::GetCamera();

	int x = 0;
	int y = 0;
	GetAbsolutPosition(&x, &y);

	RECT rect;
	rect.left = x + 4;
	rect.right = x + 4 + m_iWidth - borderRight - 4;
	rect.top = y + 4;
	rect.bottom = y + 4 + m_iHeight - 4;

	// correct Scissor Rect
	RECT scissorRect = camera->GetScissorRect();

	if (rect.left < scissorRect.left)
		rect.left = scissorRect.left;
	if (rect.top < scissorRect.top)
		rect.top = scissorRect.top;
	if (rect.bottom > scissorRect.bottom)
		rect.bottom = scissorRect.bottom;
	if (rect.right > scissorRect.right)
		rect.right= scissorRect.right;
	
	RECT rectOld = camera->SetupScissorRect(rect);

	// Draw the Text
	if (m_strFontName.length() != 0)
		DrawText(GUI_THEME_LABELBOX_LEFT, GUI_THEME_LABELBOX_RIGHT, 4, borderRight, GUI_THEME_LABELBOX_TOP, GUI_THEME_LABELBOX_BOTTOM, borderTop, 4, m_strFontName);
	else
		DrawText(GUI_THEME_LABELBOX_LEFT, GUI_THEME_LABELBOX_RIGHT, 4, borderRight, GUI_THEME_LABELBOX_TOP, GUI_THEME_LABELBOX_BOTTOM, borderTop, 4, "labelbox.font");

	camera->DisableScissorRect();

	// restore old SetupScissorRect
	camera->SetupScissorRect(rectOld);

	SallyAPI::GUI::CForm::RenderControl();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CLabelBox::UpdateControl()
///
/// \brief	Updates a control. 
///
/// \author	Christian Knobloch
/// \date	06.07.2011
////////////////////////////////////////////////////////////////////////////////////////////////////

void CLabelBox::UpdateControl()
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

	AutoResize();
	UpdateScrollbar();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CLabelBox::Resize(int width, int height)
///
/// \brief	Resizes. 
///
/// \author	Christian Knobloch
/// \date	06.07.2011
///
/// \param	width	The width. 
/// \param	height	The height. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CLabelBox::Resize(int width, int height)
{
	SallyAPI::GUI::CForm::Resize(width, height);

	if (m_pScrollbar != NULL)
	{
		m_pScrollbar->Move(m_iWidth - CONTROL_HEIGHT, 0);
		m_pScrollbar->Resize(m_pScrollbar->GetWidth(), m_iHeight);
	}

	UpdateScrollbar();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CLabelBox::UpdateScrollbar()
///
/// \brief	Updates a scrollbar. 
///
/// \author	Christian Knobloch
/// \date	06.07.2011
////////////////////////////////////////////////////////////////////////////////////////////////////

void CLabelBox::UpdateScrollbar()
{
	InvalidateControl();

	if (m_pScrollbar == NULL)
		return;

	// only if we have the scrollbar enabled
	SallyAPI::Core::CFontManager* fontManager = SallyAPI::Core::CFontManager::GetInstance();

	SallyAPI::Core::CFont* font = NULL;

	if (m_strFontName.length() != 0)
		font = GetCurrentFont(m_strFontName);
	else
		font = GetCurrentFont("labelbox.font");

	int borderRight = 4 + CONTROL_HEIGHT;

	RECT r = GetTextRect(GUI_THEME_LABELBOX_LEFT, GUI_THEME_LABELBOX_RIGHT, 4, borderRight, GUI_THEME_LABELBOX_TOP, GUI_THEME_LABELBOX_BOTTOM, 4, 4);
	RECT rectSize = font->CalcualteSize(m_strText, m_iAlign, r);

	int scrollbarHeight = rectSize.bottom - (r.bottom - r.top) + 8;
	m_pScrollbar->SetMaxPosition(scrollbarHeight);
	m_pScrollbar->SetPosition(0);

	if (scrollbarHeight > 0)
		m_pScrollbar->Visible(true);
	else
		m_pScrollbar->Visible(false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CLabelBox::SendMessageToChilds(SallyAPI::GUI::CGUIBaseObject* reporter,
/// int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
///
/// \brief	Send message to childs. 
///
/// \author	Christian Knobloch
/// \date	10.08.2011
///
/// \param [in,out]	reporter			If non-null, the reporter. 
/// \param	reporterId					Identifier for the reporter. 
/// \param	messageId					Identifier for the message. 
/// \param [in,out]	messageParameter	If non-null, the message parameter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CLabelBox::SendMessageToChilds(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case MS_SALLY_SALLY_THEME_CHANGED:
		AutoResize();
		UpdateScrollbar();
		break;
	}
	SallyAPI::GUI::CForm::SendMessageToChilds(reporter, reporterId, messageId, messageParameter);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CLabelBox::SetAutoResize(bool value)
///
/// \brief	Sets an automatic resize. If it is set to true, the control will automatically resize
///         it's height to fit in the text.
///
/// \author	Christian Knobloch
/// \date	17.08.2011
///
/// \param	value	true to value. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CLabelBox::SetAutoResize(bool value)
{
	m_bAutoResize = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CLabelBox::AutoResize()
///
/// \brief	Automatic resize. 
///
/// \author	Christian Knobloch
/// \date	17.08.2011
////////////////////////////////////////////////////////////////////////////////////////////////////

void CLabelBox::AutoResize()
{
	// should we auto resize the control?
	if (!m_bAutoResize)
		return;
		
	SallyAPI::Core::CFontManager* fontManager = SallyAPI::Core::CFontManager::GetInstance();

	SallyAPI::Core::CFont* font = NULL;

	if (m_strFontName.length() != 0)
		font = GetCurrentFont(m_strFontName);
	else
		font = GetCurrentFont("labelbox.font");

	int borderRight = 4;

	if (m_pScrollbar != NULL)
		borderRight += CONTROL_HEIGHT;

	RECT r = GetTextRect(GUI_THEME_LABELBOX_LEFT, GUI_THEME_LABELBOX_RIGHT, 4, borderRight, GUI_THEME_LABELBOX_TOP, GUI_THEME_LABELBOX_BOTTOM, 4, 4);
	RECT rectSize = font->CalcualteSize(m_strText, m_iAlign, r);

	Resize(GetWidth(), rectSize.bottom + 8);
}