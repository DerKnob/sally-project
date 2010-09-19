////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\TabcontrolItem.cpp
///
/// \brief	Implements the tabcontrol item class. 
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

#include "TabcontrolItem.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CTabcontrolItem::CTabcontrolItem(SallyAPI::GUI::CGUIBaseObject* parent,
/// const std::string& text, int icon)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	parent	If non-null, the parent. 
/// \param	text			The text. 
/// \param	icon			The icon. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CTabcontrolItem::CTabcontrolItem(SallyAPI::GUI::CGUIBaseObject* parent, const std::string& text, int icon)
{
	m_pForm = new SallyAPI::GUI::CForm(parent, 0, 0, 0, 0);
	m_pForm->Visible(false);

	m_pButton = new SallyAPI::GUI::CTabcontrolButton(parent);
	m_pButton->SetText(text);
	m_pButton->SetImageId(icon);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CTabcontrolItem::~CTabcontrolItem()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CTabcontrolItem::~CTabcontrolItem()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::GUI::CForm* CTabcontrolItem::GetForm()
///
/// \brief	Gets the form. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	null if it fails, else the form. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::GUI::CForm* CTabcontrolItem::GetForm()
{
	return m_pForm;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::GUI::CTabcontrolButton* CTabcontrolItem::GetButton()
///
/// \brief	Gets the button. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	null if it fails, else the button. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::GUI::CTabcontrolButton* CTabcontrolItem::GetButton()
{
	return m_pButton;
}