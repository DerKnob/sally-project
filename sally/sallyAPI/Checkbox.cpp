////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Checkbox.cpp
///
/// \brief	Implements the checkbox class. 
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

#include "Checkbox.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CCheckbox::CCheckbox(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width,
/// 	int controlId) :SallyAPI::GUI::CControl(parent, x, y, width, CONTROL_HEIGHT, controlId),
/// 	m_bChecked(false)
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

CCheckbox::CCheckbox(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int controlId)
	:SallyAPI::GUI::CControl(parent, x, y, width, CONTROL_HEIGHT, controlId), m_bChecked(false)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CCheckbox::~CCheckbox()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CCheckbox::~CCheckbox()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CCheckbox::RenderControl()
///
/// \brief	Renders a control. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CCheckbox::RenderControl()
{
	DrawButtonBackground(GUI_THEME_CHECKBOX_LEFT, GUI_THEME_CHECKBOX, GUI_THEME_CHECKBOX_RIGHT);
	if (m_bChecked)
	{
		DrawImage(GUI_THEME_CHECKBOX_CHECKED, 0, 0);
	}
	SallyAPI::GUI::CControl::DrawText(GUI_THEME_CHECKBOX_LEFT, GUI_THEME_CHECKBOX_RIGHT, 4, 4, "checkbox.font");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CCheckbox::ProcessMouseUp(int x, int y)
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

bool CCheckbox::ProcessMouseUp(int x, int y)
{
	if (m_bMouseDown)
	{
		if (GetCheckStatus())
			SetCheckStatus(false);
		else
			SetCheckStatus(true);
		m_pParent->SendMessageToParent(this, m_iControlId, GUI_CHECKBOX_CLICKED);
	}

	SallyAPI::GUI::CControl::ProcessMouseUp(x, y);

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CCheckbox::GetCheckStatus()
///
/// \brief	Gets the check status. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CCheckbox::GetCheckStatus()
{
	return m_bChecked;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CCheckbox::SetCheckStatus(bool checked)
///
/// \brief	Sets a check status. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	checked	true to checked. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CCheckbox::SetCheckStatus(bool checked)
{
	m_bChecked = checked;
}
