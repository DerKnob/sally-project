////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Label.cpp
///
/// \brief	Implements the label class. 
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

#include "Label.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CLabel::CLabel(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width,
/// int controlId) :SallyAPI::GUI::CControl(parent, x, y, width, CONTROL_HEIGHT, controlId),
/// m_bBold(false), m_bDrawBackground(false), m_bBig(false)
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

CLabel::CLabel(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int controlId)
	:SallyAPI::GUI::CControl(parent, x, y, width, CONTROL_HEIGHT, controlId),
	m_bBold(false), m_bDrawBackground(false), m_bBig(false), m_bFitBackgroundToText(false)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CLabel::~CLabel()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CLabel::~CLabel()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CLabel::RenderControl()
///
/// \brief	Renders a control. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CLabel::RenderControl()
{
	int borderLeft = 4;
	int borderRight = 4;
	std::string fontName;

	if (m_strFontName.length() != 0)
	{
		fontName = m_strFontName;
	}
	else if (m_bBig)
	{
		if (!m_bBold)
			fontName = "labelbig.font";
		else
			fontName = "labelbig.font.bold";
	}
	else
	{
		if (!m_bBold)
			fontName = "label.font";
		else
			fontName = "label.font.bold";
	}

	if (m_bDrawBackground)
	{
		RECT rectSize;
		rectSize.left = 0;
		rectSize.right = 0;
		rectSize.top = 0;
		rectSize.bottom = 0;

		if (m_bFitBackgroundToText)
		{
			SallyAPI::Core::CFont* font = GetCurrentFont(fontName);
			RECT rect = font->CalcualteSize(m_strText, DT_SINGLELINE, rectSize);
			
			if (m_bBig)
				rectSize.right = rect.right + 10 + 10 + 8;
			else
				rectSize.right = rect.right + 5 + 5 + 8;
		}
		else
		{
			rectSize.right = m_iWidth;
		}

		if (rectSize.right > m_iWidth)
			rectSize.right = m_iWidth;

		if (m_bBig)
		{
			borderLeft += 10;
			borderRight += 10;

			int widthCorrection = rectSize.right - m_iWidth;
			int xCorrection = 0;
			if ((m_iAlign != -1) && ((m_iAlign & DT_CENTER) == DT_CENTER))
				xCorrection = (m_iWidth - rectSize.right) / 2;
			if ((m_iAlign != -1) && ((m_iAlign & DT_RIGHT) == DT_RIGHT))
				xCorrection = m_iWidth - rectSize.right;

			DrawButtonBackground(GUI_THEME_LABEL_BIG_LEFT, GUI_THEME_LABEL_BIG, GUI_THEME_LABEL_BIG_RIGHT, xCorrection, 0, widthCorrection, 0);
		}
		else
		{
			borderLeft += 5;
			borderRight += 5;

			int widthCorrection = rectSize.right - m_iWidth;
			int xCorrection = 0;
			if ((m_iAlign != -1) && ((m_iAlign & DT_CENTER) == DT_CENTER))
				xCorrection = (m_iWidth - rectSize.right) / 2;
			if ((m_iAlign != -1) && ((m_iAlign & DT_RIGHT) == DT_RIGHT))
				xCorrection = m_iWidth - rectSize.right;

			DrawButtonBackground(GUI_THEME_LABEL_LEFT, GUI_THEME_LABEL, GUI_THEME_LABEL_RIGHT, xCorrection, 0, widthCorrection, 0);
		}
	}

	DrawText(NULL, NULL, borderLeft, borderRight, fontName);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CLabel::ProcessMouseUp(int x, int y)
///
/// \brief	Process a mouse up. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	x	The x coordinate. 
/// \param	y	The y coordinate. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CLabel::ProcessMouseUp(int x, int y)
{
	if (!m_bVisible || !m_bEnabled)
		return false;

	if (x > m_iXAbsolut && x < m_iXAbsolut + m_iWidth)
	{
		if (y > m_iYAbsolut && y < m_iYAbsolut + m_iHeight)
		{
			SallyAPI::GUI::SendMessage::CParameterPoint message(x, y);
			m_pParent->SendMessageToParent(this, m_iControlId, GUI_LABEL_CLICKED, &message);
			return true;
		}
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CLabel::SetBold(bool bold)
///
/// \brief	Sets a bold. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	bold	true to bold. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CLabel::SetBold(bool bold)
{
	m_bBold = bold;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CLabel::SetFont(const std::string& fontName)
///
/// \brief	Sets a font. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	fontName	Name of the font. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CLabel::SetFont(const std::string& fontName)
{
	m_strFontName = fontName;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CLabel::SetDrawBackground(bool drawBackground)
///
/// \brief	Sets a draw background. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	drawBackground	true to draw background. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CLabel::SetDrawBackground(bool drawBackground)
{
	m_bDrawBackground = drawBackground;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CLabel::SetBig(bool big)
///
/// \brief	Sets a big. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	big	true to big. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CLabel::SetBig(bool big)
{
	m_bBig = big;
	if (m_bBig)
		Resize(m_iWidth, CONTROL_HEIGHT * 2);
	else
		Resize(m_iWidth, CONTROL_HEIGHT);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CLabel::SetFitBackgroundToText(bool value)
///
/// \brief	If true, the background is resized to be only visible behind the text.
///			Default is false.
///
/// \author	Christian Knobloch
/// \date	19.12.2011
///
/// \param	value	true to value. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CLabel::SetFitBackgroundToText(bool value)
{
	m_bFitBackgroundToText = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CLabel::Timer(float timeDelta)
///
/// \brief	Timers. 
///
/// \author	Christian Knobloch
/// \date	26.05.2011
///
/// \param	timeDelta	The time delta. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CLabel::Timer(float timeDelta)
{
	SallyAPI::GUI::CControl::Timer(timeDelta);

	if ((m_bPressed) && (m_fTimeMouseDown != -1) && (m_fTimeDelta > m_fTimeMouseDown + 0.5))
	{
		SallyAPI::GUI::SendMessage::CParameterHoldClick messageParameter;
		m_pParent->SendMessageToParent(this, GetControlId(), GUI_LABEL_HOLDCLICKED, &messageParameter);

		if (messageParameter.IsHandled())
		{
			m_fTimeMouseUp = m_fTimeDelta + SallyAPI::Core::CGame::GetCounter()->GetElapsedTimeStatic();

			m_bPressed = false;
			ResetMouse();
		}
		m_fTimeMouseDown = -1;
	}
}