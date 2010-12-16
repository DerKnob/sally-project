////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ButtonBar.cpp
///
/// \brief	Implements the button bar class. 
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

#include "ButtonBar.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CButtonBar::CButtonBar(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width,
/// int controlId) :SallyAPI::GUI::CForm(parent, x, y, width, CONTROL_HEIGHT, controlId),
/// m_iPositon(0)
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

CButtonBar::CButtonBar(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int controlId)
	:SallyAPI::GUI::CForm(parent, x, y, width, CONTROL_HEIGHT, controlId), m_iPositon(0)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CButtonBar::~CButtonBar()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CButtonBar::~CButtonBar()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CButtonBar::AddChild(SallyAPI::GUI::CControl* control)
///
/// \brief	Adds a child. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	control	If non-null, the control. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CButtonBar::AddChild(SallyAPI::GUI::CControl* control)
{
	SallyAPI::GUI::CForm::AddChild(control);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CButtonBar::AddChild(CButtonBarButton* button)
///
/// \brief	Adds a child. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	button	If non-null, the button. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CButtonBar::AddChild(CButtonBarButton* button)
{
	SallyAPI::GUI::CForm::AddChild(button);

	button->Move(m_iPositon, 0);
	m_iPositon += button->GetWidth();

	if (m_Buttons.size() == 0)
	{
		button->SetFirst(true);
	}
	else
	{
		m_Buttons[m_Buttons.size() - 1]->SetLast(false);
	}

	button->SetLast(true);
	m_Buttons.push_back(button);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::GUI::CButtonBarButton* CButtonBar::GetButton(int i)
///
/// \brief	Gets a button. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	i	The index. 
///
/// \return	null if it fails, else the button. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::GUI::CButtonBarButton* CButtonBar::GetButton(int i)
{
	if (i >= (int) m_Buttons.size())
		return NULL;
	return m_Buttons[i];
}