////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ParameterListItem.h
///
/// \brief	Implements the parameter list item class. 
///
/// \author	Christian Knobloch
/// \date	29.11.2010
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

#include "ParameterListItem.h"

using namespace SallyAPI::GUI::SendMessage;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CParameterListItem::CParameterListItem(int item, int button)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	29.11.2010
///
/// \param	item	The item. 
/// \param	button	The button. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CParameterListItem::CParameterListItem(int item, int button)
	:m_iItem(item), m_iButton(button)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CParameterListItem::~CParameterListItem()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	29.11.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CParameterListItem::~CParameterListItem()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CParameterListItem::GetItem()
///
/// \brief	Gets the item. 
///
/// \author	Christian Knobloch
/// \date	29.11.2010
///
/// \return	The item. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CParameterListItem::GetItem()
{
	return m_iItem;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CParameterListItem::GetButton()
///
/// \brief	Gets the button. 
///
/// \author	Christian Knobloch
/// \date	29.11.2010
///
/// \return	The button. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CParameterListItem::GetButton()
{
	return m_iButton;
}