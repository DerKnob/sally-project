////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ParameterListItemHoldClick.cpp
///
/// \brief	Implements the parameter list item hold click class. 
///
/// \author	Christian Knobloch
/// \date	24.06.2011
///
/// This file is part of the Sally Project
/// 
/// Copyright(c) 2008-2011 Sally Project
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

#include "ParameterListItemHoldClick.h"

using namespace SallyAPI::GUI::SendMessage;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CParameterListItemHoldClick::CParameterListItemHoldClick(int item, int button)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	24.06.2011
///
/// \param	item	The item. 
/// \param	button	The button. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CParameterListItemHoldClick::CParameterListItemHoldClick(int item, int button)
	:m_iItem(item), m_iButton(button), m_bHandled(false)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CParameterListItemHoldClick::~CParameterListItemHoldClick()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	24.06.2011
////////////////////////////////////////////////////////////////////////////////////////////////////

CParameterListItemHoldClick::~CParameterListItemHoldClick()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CParameterListItemHoldClick::GetItem()
///
/// \brief	Gets the item. 
///
/// \author	Christian Knobloch
/// \date	24.06.2011
///
/// \return	The item. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CParameterListItemHoldClick::GetItem()
{
	return m_iItem;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CParameterListItemHoldClick::GetButton()
///
/// \brief	Gets the button. 
///
/// \author	Christian Knobloch
/// \date	24.06.2011
///
/// \return	The button. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CParameterListItemHoldClick::GetButton()
{
	return m_iButton;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CParameterListItemHoldClick::SetHandled(bool handled)
///
/// \brief	Sets a handled. 
///
/// \author	Christian Knobloch
/// \date	24.06.2011
///
/// \param	handled	true to handled. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CParameterListItemHoldClick::SetHandled(bool handled)
{
	m_bHandled = handled;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CParameterListItemHoldClick::IsHandled()
///
/// \brief	Query if this object is handled. 
///
/// \author	Christian Knobloch
/// \date	24.06.2011
///
/// \return	true if handled, false if not. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CParameterListItemHoldClick::IsHandled()
{
	return m_bHandled;
}