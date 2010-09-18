////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Processbar.cpp
///
/// \brief	Implements the processbar class. 
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

#include "Processbar.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CProcessbar::CProcessbar(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width,
/// int controlId) :SallyAPI::GUI::CControl(parent, x, y, width, CONTROL_HEIGHT, controlId)
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

CProcessbar::CProcessbar(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int controlId)
	:SallyAPI::GUI::CControl(parent, x, y, width, CONTROL_HEIGHT, controlId)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CProcessbar::~CProcessbar()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CProcessbar::~CProcessbar()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CProcessbar::RenderControl()
///
/// \brief	Renders a control. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CProcessbar::RenderControl()
{
	DrawButtonBackground(GUI_THEME_PROCESSBAR_LEFT, GUI_THEME_PROCESSBAR, GUI_THEME_PROCESSBAR_RIGHT);

	int x = 0, y = 0;
	int position = 0;

	if (m_iPosition > 0)
	{
		// Temp variables
		SallyAPI::GUI::CPicture* image = NULL;
		int imageWidthLeft = 0;
		int imageWidthRight = 0;
		int imageWidthMiddle = 1;

		image = GetPicture(GUI_THEME_PROCESSBAR_2LEFT);
		if (image != NULL) {
			imageWidthLeft = image->GetWidth();
		}
		image = GetPicture(GUI_THEME_PROCESSBAR_2RIGHT);
		if (image != NULL) {
			imageWidthRight = image->GetWidth();
		}
		image = GetPicture(GUI_THEME_PROCESSBAR_2);
		if (image != NULL) {
			imageWidthMiddle = image->GetWidth();
		}

		position = (int) ((float) ((float) m_iPosition / (float) m_iPositionMax)  * m_iWidth);

		DrawImagePart(GUI_THEME_PROCESSBAR_2LEFT, x, y, 0, 0, position, m_iHeight);
		if (position > imageWidthLeft)
		{
			if (position <= m_iWidth - imageWidthRight)
			{
				int i = (int)(position - imageWidthLeft);

				DrawImage(GUI_THEME_PROCESSBAR_2, x + imageWidthLeft, y, i, m_iHeight);
			}
			else
			{
				DrawImage(GUI_THEME_PROCESSBAR_2, x + imageWidthLeft, y, m_iWidth - (imageWidthLeft + imageWidthRight), m_iHeight);
			}

			if (position > m_iWidth - imageWidthRight)
			{
				DrawImagePart(GUI_THEME_PROCESSBAR_2RIGHT, x + m_iWidth - imageWidthRight, y, 0, 0, position - (m_iWidth - imageWidthRight), m_iHeight);
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CProcessbar::ProcessMouseUp(int x, int y)
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

bool CProcessbar::ProcessMouseUp(int x, int y)
{
	if (m_bMouseDown)
	{
		int xClicked = x - m_iXAbsolut;
		int pos;
		pos = (int) ((float) m_iPositionMax * (float) (xClicked) / m_iWidth);

		SallyAPI::GUI::SendMessage::CParameterInteger parameterInterger(pos);
		m_pParent->SendMessageToParent(this, m_iControlId, GUI_PROCESSBAR_CLICKED, &parameterInterger);
	}

	SallyAPI::GUI::CControl::ProcessMouseUp(x, y);
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CProcessbar::SetPosition(int position)
///
/// \brief	Sets a position. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	position	The position. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CProcessbar::SetPosition(int position)
{
	if ((position < 0) || (position > m_iPositionMax))
		return;

	m_iPosition = position;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CProcessbar::GetPosition()
///
/// \brief	Gets the position. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The position. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CProcessbar::GetPosition()
{
	return m_iPosition;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CProcessbar::SetMaxPosition(int position)
///
/// \brief	Sets a maximum position. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	position	The position. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CProcessbar::SetMaxPosition(int position)
{
	if (position < 0)
		return;

	m_iPositionMax = position;
	m_iPosition = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CProcessbar::ProcessMouseMove(int x, int y)
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

bool CProcessbar::ProcessMouseMove(int x, int y)
{
	if (m_bMouseDown == false)
		return false;

	int xClicked = x - m_iXAbsolut;
	int pos;
	pos = (int) ((float) m_iPositionMax * (float) (xClicked) / m_iWidth);

	SallyAPI::GUI::SendMessage::CParameterInteger parameterInterger(pos);
	m_pParent->SendMessageToParent(this, m_iControlId, GUI_PROCESSBAR_MOVED, &parameterInterger);
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CProcessbar::SendMessageToChilds(CGUIBaseObject* reporter, int reporterId,
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

void CProcessbar::SendMessageToChilds(CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_MESSAGE_MOUSE_UP_INFO:
		m_bMouseDown = false;
		return;
	}

	SallyAPI::GUI::CControl::SendMessageToChilds(reporter, reporterId, messageId, messageParameter);
}