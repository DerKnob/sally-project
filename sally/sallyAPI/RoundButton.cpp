////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\RoundButton.cpp
///
/// \brief	Implements the round button class. 
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

#include "RoundButton.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CRoundButton::CRoundButton(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y,
/// int controlId, SallyAPI::GUI::RoundButtonType type) :SallyAPI::GUI::CButton(parent, x, y, 0,
/// 0, controlId, BUTTON_TYPE_ONLY_IMAGE)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	parent	If non-null, the parent. 
/// \param	x				The x coordinate. 
/// \param	y				The y coordinate. 
/// \param	controlId		Identifier for the control. 
/// \param	type			The type. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CRoundButton::CRoundButton(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int controlId, SallyAPI::GUI::RoundButtonType type)
	:SallyAPI::GUI::CButton(parent, x, y, 0, 0, controlId, BUTTON_TYPE_ONLY_IMAGE)
{
	m_eRoundButtonType = type;

	if (type == ROUNDBUTTON_TYPE_SMALL)
	{
		Resize(55, 55);
	}
	else
	{
		Resize(70, 70);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CRoundButton::~CRoundButton()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CRoundButton::~CRoundButton()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CRoundButton::RenderControl()
///
/// \brief	Renders a control. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CRoundButton::RenderControl()
{
	if (m_eRoundButtonType == ROUNDBUTTON_TYPE_SMALL)
	{
		if (!m_bEnabled)
		{
			DrawImage(GUI_THEME_ROUNDBUTTON_SMALL_DISABLED, 0, 0);
		}
		else if ((m_bChecked) || (m_bPressed))
		{
			DrawImage(GUI_THEME_ROUNDBUTTON_SMALL_SELECTED, 0, 0);
		}
		else
		{
			DrawImage(GUI_THEME_ROUNDBUTTON_SMALL_NORMAL, 0, 0);
		}
	}
	else
	{
		if (!m_bEnabled)
		{
			DrawImage(GUI_THEME_ROUNDBUTTON_BIG_DISABLED, 0, 0);
		}
		else if ((m_bChecked) || (m_bPressed))
		{
			DrawImage(GUI_THEME_ROUNDBUTTON_BIG_SELECTED, 0, 0);
		}
		else
		{
			DrawImage(GUI_THEME_ROUNDBUTTON_BIG_NORMAL, 0, 0);
		}
	}
	DrawImage(m_iImage, (m_iWidth - m_iImageSize) / 2, (m_iHeight - m_iImageSize) / 2, m_iImageSize, m_iImageSize);
}
