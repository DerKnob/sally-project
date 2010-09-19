////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Scrollbar.cpp
///
/// \brief	Implements the scrollbar class. 
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

#include "Scrollbar.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CScrollbar::CScrollbar(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width,
/// int height, SCROLLBAR_ALIGNMENT scrollbarAlignment,
/// int controlId) :SallyAPI::GUI::CControl(parent, x, y, width, height, controlId),
/// m_eScrollbarAlignment(scrollbarAlignment), m_iPosition(0), m_iPositionMax(0),
/// m_bShowScrollbarIfNotScrollable(true), m_iKnobHeight(CONTROL_HEIGHT)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	26.04.2010
///
/// \param [in,out]	parent		If non-null, the parent. 
/// \param	x					The x coordinate. 
/// \param	y					The y coordinate. 
/// \param	width				The width. 
/// \param	height				The height. 
/// \param	scrollbarAlignment	The scrollbar alignment. 
/// \param	controlId			Identifier for the control. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CScrollbar::CScrollbar(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int height, SCROLLBAR_ALIGNMENT scrollbarAlignment, int controlId)
	:SallyAPI::GUI::CControl(parent, x, y, width, height, controlId), m_eScrollbarAlignment(scrollbarAlignment),
	m_iPosition(0), m_iPositionMax(0), m_bShowScrollbarIfNotScrollable(true), m_iKnobHeight(CONTROL_HEIGHT)
{
	if (m_eScrollbarAlignment == SCROLLBAR_ALIGNMENT_VERTICAL)
		m_iWidth = CONTROL_HEIGHT;
	else
		m_iHeight = CONTROL_HEIGHT;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CScrollbar::~CScrollbar()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	26.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CScrollbar::~CScrollbar()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CScrollbar::RenderControl()
///
/// \brief	Renders a control. 
///
/// \author	Christian Knobloch
/// \date	26.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CScrollbar::RenderControl()
{
	if (m_eScrollbarAlignment == SCROLLBAR_ALIGNMENT_VERTICAL)
	{
		DrawVertical();
	}
	else
	{
		DrawHorizontal();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CScrollbar::DrawVertical()
///
/// \brief	Draws a vertical scrollbar. 
///
/// \author	Christian Knobloch
/// \date	26.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CScrollbar::DrawVertical()
{
	if (!m_bShowScrollbarIfNotScrollable && (m_iPositionMax <= 0))
		return;

	SallyAPI::GUI::CPicture* image = NULL;
	int imageHeightTop = 0;
	int imageHeightBottom = 0;
	int imageHeightMiddle = 1;

	image = GetPicture(GUI_THEME_SCROLLBAR_V_BAR_NORMAL_TOP);
	if (image != NULL) {
		imageHeightTop = image->GetHeight();
	}
	image = GetPicture(GUI_THEME_SCROLLBAR_V_BAR_NORMAL_BOTTOM);
	if (image != NULL) {
		imageHeightBottom = image->GetHeight();
	}
	image = GetPicture(GUI_THEME_SCROLLBAR_V_BAR_NORMAL_CENTER);
	if (image != NULL) {
		imageHeightMiddle = image->GetHeight();
	}

	DrawImage(GUI_THEME_SCROLLBAR_V_BAR_NORMAL_TOP, 0, 0,
		CONTROL_HEIGHT, imageHeightTop);
	DrawImage(GUI_THEME_SCROLLBAR_V_BAR_NORMAL_CENTER, 0, imageHeightTop,
		CONTROL_HEIGHT, m_iHeight - (imageHeightTop + imageHeightBottom));
	DrawImage(GUI_THEME_SCROLLBAR_V_BAR_NORMAL_BOTTOM, 0, m_iHeight - imageHeightBottom,
		CONTROL_HEIGHT, imageHeightBottom);


	if (m_iPositionMax > 0)
	{
		float temp = ((float) (m_iPositionMax + m_iHeight) / (float)m_iHeight);
		m_iKnobHeight = (int) (m_iHeight / temp);

		if (m_iKnobHeight < CONTROL_HEIGHT)
			m_iKnobHeight = CONTROL_HEIGHT;

		m_iBarHeight = m_iHeight - m_iKnobHeight;

		float percent = (float) m_iPositionMax / (float) m_iPosition;
		m_iKnobPosition = (int) (m_iBarHeight / percent);

		SallyAPI::GUI::CPicture* image = NULL;
		int imageHeightTop = 0;
		int imageHeightBottom = 0;
		int imageHeightMiddle = 1;

		image = GetPicture(GUI_THEME_SCROLLBAR_V_KNOB_NORMAL_TOP);
		if (image != NULL) {
			imageHeightTop = image->GetHeight();
		}
		image = GetPicture(GUI_THEME_SCROLLBAR_V_KNOB_NORMAL_BOTTOM);
		if (image != NULL) {
			imageHeightBottom = image->GetHeight();
		}
		image = GetPicture(GUI_THEME_SCROLLBAR_V_KNOB_NORMAL_CENTER);
		if (image != NULL) {
			imageHeightMiddle = image->GetHeight();
		}

		DrawImage(GUI_THEME_SCROLLBAR_V_KNOB_NORMAL_TOP, 0, m_iKnobPosition,
			CONTROL_HEIGHT, imageHeightTop);
		DrawImage(GUI_THEME_SCROLLBAR_V_KNOB_NORMAL_CENTER, 0, m_iKnobPosition + imageHeightTop,
			CONTROL_HEIGHT, m_iKnobHeight - (imageHeightTop + imageHeightBottom));
		DrawImage(GUI_THEME_SCROLLBAR_V_KNOB_NORMAL_BOTTOM, 0, m_iKnobPosition + m_iKnobHeight - imageHeightBottom,
			CONTROL_HEIGHT, imageHeightBottom);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CScrollbar::DrawHorizontal()
///
/// \brief	Draw a horizontal scrollbar. 
///
/// \author	Christian Knobloch
/// \date	26.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CScrollbar::DrawHorizontal()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CScrollbar::SetMaxPosition(int position)
///
/// \brief	Sets a maximum position. 
///
/// \author	Christian Knobloch
/// \date	26.04.2010
///
/// \param	position	The position. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CScrollbar::SetMaxPosition(int position)
{
	if (position < 0)
		position = 0;;

	m_iPositionMax = position;
	m_iPosition = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CScrollbar::SetPosition(int position)
///
/// \brief	Sets a position. 
///
/// \author	Christian Knobloch
/// \date	26.04.2010
///
/// \param	position	The position. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CScrollbar::SetPosition(int position)
{
	if (position < 0)
		position = 0;

	if (m_eScrollbarAlignment == SCROLLBAR_ALIGNMENT_VERTICAL)
	{
		if (position > m_iPositionMax)
			position = m_iPositionMax;
	}
	else
	{
		if (position > m_iPositionMax)
			position = m_iPositionMax;
	}

	m_iPosition = position;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CScrollbar::GetPosition()
///
/// \brief	Gets the position. 
///
/// \author	Christian Knobloch
/// \date	26.04.2010
///
/// \return	The position. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CScrollbar::GetPosition()
{
	return m_iPosition;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CScrollbar::ProcessMouseUp(int x, int y)
///
/// \brief	Process a mouse up. 
///
/// \author	Christian Knobloch
/// \date	26.04.2010
///
/// \param	x	The x coordinate. 
/// \param	y	The y coordinate. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CScrollbar::ProcessMouseUp(int x, int y)
{
	if (m_bMouseDown)
	{
		int yClicked = y - m_iYAbsolut;

		if (yClicked < m_iKnobPosition)
		{
			m_iPosition -= m_iHeight;
			if (m_iPosition < 0)
				m_iPosition = 0;

			SallyAPI::GUI::SendMessage::CParameterInteger parameterInterger(m_iPosition);
			m_pParent->SendMessageToParent(this, m_iControlId, GUI_SCROLLBAR_CLICKED, &parameterInterger);
		}
		else if (yClicked > m_iKnobPosition + m_iKnobHeight)
		{
			m_iPosition += m_iHeight;
			if (m_iPosition > m_iPositionMax)
				m_iPosition = m_iPositionMax;
			
			SallyAPI::GUI::SendMessage::CParameterInteger parameterInterger(m_iPosition);
			m_pParent->SendMessageToParent(this, m_iControlId, GUI_SCROLLBAR_CLICKED, &parameterInterger);
		}
	}

	SallyAPI::GUI::CControl::ProcessMouseUp(x, y);
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CScrollbar::ProcessMouseMove(int x, int y)
///
/// \brief	Process a mouse move. 
///
/// \author	Christian Knobloch
/// \date	26.04.2010
///
/// \param	x	The x coordinate. 
/// \param	y	The y coordinate. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CScrollbar::ProcessMouseMove(int x, int y)
{
	if (!m_bMouseDown)
		return false;

	m_iKnobPosition = y - m_iYAbsolut;
	m_iKnobPosition = m_iKnobPosition - m_iMouseDownKnobOffSet;

	if (m_iKnobPosition < 0)
		m_iKnobPosition = 0;
	else if (m_iKnobPosition > m_iHeight- m_iKnobHeight)
		m_iKnobPosition = m_iHeight - m_iKnobHeight;

	float percent = (float) m_iBarHeight / (float) m_iKnobPosition;
	m_iPosition = (int) ((float) m_iPositionMax / percent);
	
	SallyAPI::GUI::SendMessage::CParameterInteger parameterInterger(m_iPosition);
	m_pParent->SendMessageToParent(this, m_iControlId, GUI_SCROLLBAR_MOVED, &parameterInterger);
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CScrollbar::CheckProcessMouseDown(int x, int y)
///
/// \brief	Check process mouse down. 
///
/// \author	Christian Knobloch
/// \date	26.04.2010
///
/// \param	x	The x coordinate. 
/// \param	y	The y coordinate. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CScrollbar::CheckProcessMouseDown(int x, int y)
{
	if (!m_bVisible || !m_bEnabled)
		return false;

	if (!m_bVisible || !m_bEnabled)
		return false;

	if (x > m_iXAbsolut - 20 && x < m_iXAbsolut + m_iWidth + 20)
	{
		if (y > m_iYAbsolut && y < m_iYAbsolut + m_iHeight)
		{
			return ProcessMouseDown(x, y);
		}
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CScrollbar::ProcessMouseDown(int x, int y)
///
/// \brief	Process a mouse down. 
///
/// \author	Christian Knobloch
/// \date	30.04.2010
///
/// \param	x	The x coordinate. 
/// \param	y	The y coordinate. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CScrollbar::ProcessMouseDown(int x, int y)
{
	m_iMouseDownKnobOffSet = y - m_iYAbsolut - m_iKnobPosition;
	return SallyAPI::GUI::CControl::ProcessMouseDown(x, y);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CScrollbar::SendMessageToChilds(CGUIBaseObject* reporter, int reporterId,
/// int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
///
/// \brief	Send message to childs. 
///
/// \author	Christian Knobloch
/// \date	26.04.2010
///
/// \param [in,out]	reporter			If non-null, the reporter. 
/// \param	reporterId					Identifier for the reporter. 
/// \param	messageId					Identifier for the message. 
/// \param [in,out]	messageParameter	If non-null, the message parameter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CScrollbar::SendMessageToChilds(CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_MESSAGE_MOUSE_UP_INFO:
		m_bMouseDown = false;
		return;
	}

	SallyAPI::GUI::CControl::SendMessageToChilds(reporter, reporterId, messageId, messageParameter);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int SallyAPI::GUI::CScrollbar::GetMaxPosition(void)
///
/// \brief	Gets the maximum position. 
///
/// \author	Christian Knobloch
/// \date	27.04.2010
///
/// \return	The maximum position. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int SallyAPI::GUI::CScrollbar::GetMaxPosition(void)
{
	return m_iPositionMax;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void SallyAPI::GUI::CScrollbar::ShowScrollbarIfNotScrollable(bool value)
///
/// \brief	Shows the scrollbar if not scrollable. 
///
/// \author	Christian Knobloch
/// \date	27.04.2010
///
/// \param	value	true to value. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CScrollbar::ShowScrollbarIfNotScrollable(bool value)
{
	m_bShowScrollbarIfNotScrollable = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool SallyAPI::GUI::CScrollbar::IsScrollbarVisibleIfNotScrollbable(void)
///
/// \brief	Query if this object is scrollbar visible if not scrollbable. 
///
/// \author	Christian Knobloch
/// \date	27.04.2010
///
/// \return	true if scrollbar visible if not scrollbable, false if not. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CScrollbar::IsScrollbarVisibleIfNotScrollbable()
{
	return m_bShowScrollbarIfNotScrollable;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CScrollbar::SendMessageToParent(CGUIBaseObject*reporter,
/// int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase*messageParameter)
///
/// \brief	Send message to parent. 
///
/// \author	Christian Knobloch
/// \date	30.04.2010
///
/// \param [in,out]	reporter			If non-null, the reporter. 
/// \param	reporterId					Identifier for the reporter. 
/// \param	messageId					Identifier for the message. 
/// \param [in,out]	messageParameter	If non-null, the message parameter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CScrollbar::SendMessageToParent(CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase*messageParameter)
{
	SallyAPI::GUI::CControl::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}