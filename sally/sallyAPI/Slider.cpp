////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Slider.cpp
///
/// \brief	Implements the slider class. 
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

#include "Slider.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CSlider::CSlider(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width,
/// int controlId) :SallyAPI::GUI::CControl(parent, x, y, width, CONTROL_HEIGHT, controlId),
/// m_iPosition(0), m_iPositionMax(0)
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

CSlider::CSlider(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int controlId)
	:SallyAPI::GUI::CControl(parent, x, y, width, CONTROL_HEIGHT, controlId), m_iPosition(0), m_iPositionMax(0),
	m_bDrawPgrogressbar(true)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CSlider::~CSlider()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CSlider::~CSlider()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CSlider::RenderControl()
///
/// \brief	Renders a control. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CSlider::RenderControl()
{
	DrawButtonBackground(GUI_THEME_SLIDER_LEFT, GUI_THEME_SLIDER, GUI_THEME_SLIDER_RIGHT);

	int x = 0, y = 0;
	int position = (int) ((float) ((float) m_iPosition / (float) m_iPositionMax)  * m_iWidth);

	SallyAPI::GUI::CPicture* image = NULL;
	int imageKnobWidth = 0;

	image = GetPicture(GUI_THEME_SLIDER_KNOB);
	if (image != NULL) {
		imageKnobWidth = image->GetWidth();
	}
	int positionKnobCenter = (position) - (imageKnobWidth / 2);
	if (positionKnobCenter < 0)
		positionKnobCenter = 0;
	if (positionKnobCenter > m_iWidth - imageKnobWidth)
		positionKnobCenter = m_iWidth  - imageKnobWidth;

	if ((m_iPosition > 0) && (m_bDrawPgrogressbar))
	{
		// Temp variables
		SallyAPI::GUI::CPicture* image = NULL;
		int imageWidthLeft = 0;
		int imageWidthRight = 0;
		int imageWidthMiddle = 1;

		image = GetPicture(GUI_THEME_SLIDER_2LEFT);
		if (image != NULL) {
			imageWidthLeft = image->GetWidth();
		}
		image = GetPicture(GUI_THEME_SLIDER_2RIGHT);
		if (image != NULL) {
			imageWidthRight = image->GetWidth();
		}
		image = GetPicture(GUI_THEME_SLIDER_2);
		if (image != NULL) {
			imageWidthMiddle = image->GetWidth();
		}

		DrawImagePart(GUI_THEME_SLIDER_2LEFT, x, y, 0, 0, position, m_iHeight);

		if (position > imageWidthLeft)
		{
			int i = positionKnobCenter - imageWidthLeft + (imageKnobWidth / 2);

			if (i > m_iWidth - (imageWidthLeft))
				i = m_iWidth - (imageWidthLeft);

			DrawImage(GUI_THEME_SLIDER_2, x + imageWidthLeft, y, i, m_iHeight);

			DrawImage(GUI_THEME_SLIDER_2RIGHT, x + imageWidthLeft + i, y);
		}
	}
	DrawImage(GUI_THEME_SLIDER_KNOB, positionKnobCenter, y);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CSlider::ProcessMouseUp(int x, int y)
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

bool CSlider::ProcessMouseUp(int x, int y)
{
	if (m_bMouseDown)
	{
		int xClicked = x - m_iXAbsolut;
		int pos;
		pos = (int) ((float) m_iPositionMax * (float) (xClicked) / m_iWidth);

		m_iPosition = pos;

		SallyAPI::GUI::SendMessage::CParameterInteger parameterInterger(pos);
		m_pParent->SendMessageToParent(this, m_iControlId, GUI_SLIDER_CLICKED, &parameterInterger);
	}

	SallyAPI::GUI::CControl::ProcessMouseUp(x, y);
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CSlider::GetPosition()
///
/// \brief	Gets the position of the knob. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The position. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CSlider::GetPosition()
{
	return m_iPosition;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CSlider::SetPosition(int position)
///
/// \brief	Sets a position. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	position	The position. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CSlider::SetPosition(int position)
{
	if (position < 0)
		position = 0;
	if (position > m_iPositionMax)
		position = m_iPositionMax;

	m_iPosition = position;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CSlider::SetMaxPosition(int position)
///
/// \brief	Sets a maximum position. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	position	The position. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CSlider::SetMaxPosition(int position)
{
	if (position < 0)
		return;

	m_iPositionMax = position;
	m_iPosition = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CSlider::SetDrawProgressbar(bool value)
///
/// \brief	Sets if the progressbar should be drawn.
///			If set to false, you will get only the background and the knob drawn.
///
/// \author	Christian Knobloch
/// \date	26.12.2011
///
/// \param	value	true to value. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CSlider::SetDrawProgressbar(bool value)
{
	m_bDrawPgrogressbar = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CSlider::ProcessMouseMove(int x, int y)
///
/// \brief	Process a mouse move. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	x	The x coordinate. 
/// \param	y	The y coordinate. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CSlider::ProcessMouseMove(int x, int y)
{
	if (!m_bMouseDown)
		return false;

	int xClicked = x - m_iXAbsolut;

	int pos;

	if (xClicked < 0)
		pos = 0;
	else if (xClicked > m_iWidth)
		pos = m_iPositionMax;
	else 
		pos = (int) ((float) m_iPositionMax * (float) (xClicked) / m_iWidth);

	m_iPosition = pos;

	SallyAPI::GUI::SendMessage::CParameterInteger parameterInterger(pos);
	m_pParent->SendMessageToParent(this, m_iControlId, GUI_SLIDER_MOVED, &parameterInterger);
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CSlider::CheckProcessMouseDown(int x, int y)
///
/// \brief	Check process mouse down. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	x	The x coordinate. 
/// \param	y	The y coordinate. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CSlider::CheckProcessMouseDown(int x, int y)
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
/// \fn	void CSlider::SendMessageToChilds(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId,
/// int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
///
/// \brief	Send message to childs. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	reporter			If non-null, the reporter. 
/// \param	reporterId					Identifier for the reporter. 
/// \param	messageId					Identifier for the message. 
/// \param [in,out]	messageParameter	If non-null, the message parameter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CSlider::SendMessageToChilds(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_MESSAGE_MOUSE_UP_INFO:
		m_bMouseDown = false;
		return;
	}

	SallyAPI::GUI::CControl::SendMessageToChilds(reporter, reporterId, messageId, messageParameter);
}