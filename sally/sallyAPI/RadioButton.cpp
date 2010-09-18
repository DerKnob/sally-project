////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\RadioButton.cpp
///
/// \brief	Implements the radio button class. 
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

#include "RadioButton.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CRadioButton::CRadioButton(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width,
/// int controlId) :SallyAPI::GUI::CControl(parent, x, y, width, CONTROL_HEIGHT, controlId),
/// m_bChecked(false)
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

CRadioButton::CRadioButton(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int controlId)
	:SallyAPI::GUI::CControl(parent, x, y, width, CONTROL_HEIGHT, controlId), m_bChecked(false)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CRadioButton::~CRadioButton()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CRadioButton::~CRadioButton()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CRadioButton::RenderControl()
///
/// \brief	Renders a control. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CRadioButton::RenderControl()
{
	DrawButtonBackground(GUI_THEME_RADIO_LEFT, GUI_THEME_RADIO, GUI_THEME_RADIO_RIGHT);
	if (m_bChecked)
	{
		DrawImage(GUI_THEME_RADIO_CHECKED, 0, 0);
	}
	DrawText(GUI_THEME_RADIO_LEFT, GUI_THEME_RADIO_RIGHT, 4, 4, "radiobutton.font");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CRadioButton::ProcessMouseUp(int x, int y)
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

bool CRadioButton::ProcessMouseUp(int x, int y)
{
	if (m_bMouseDown)
	{
		if (GetCheckStatus())
			SetCheckStatus(false);
		else
			SetCheckStatus(true);
		m_pParent->SendMessageToParent(this, m_iControlId, GUI_RADIOBUTTON_CLICKED);
	}

	SallyAPI::GUI::CControl::ProcessMouseUp(x, y);

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CRadioButton::GetCheckStatus()
///
/// \brief	Gets the check status. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CRadioButton::GetCheckStatus()
{
	return m_bChecked;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CRadioButton::SetCheckStatus(bool checked)
///
/// \brief	Sets a check status. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	checked	true to checked. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CRadioButton::SetCheckStatus(bool checked)
{
	m_bChecked = checked;
}
