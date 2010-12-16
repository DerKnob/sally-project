////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ParameterScrollList.cpp
///
/// \brief	Implements the parameter scroll list class. 
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

#include "ParameterScrollList.h"

using namespace SallyAPI::GUI::SendMessage;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CParameterScrollList::CParameterScrollList(const SallyAPI::GUI::CScrollListItem& item)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	item	The item. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CParameterScrollList::CParameterScrollList(const SallyAPI::GUI::CScrollListItem& item)
	:m_Item(item)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CParameterScrollList::~CParameterScrollList()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CParameterScrollList::~CParameterScrollList()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::GUI::CScrollListItem CParameterScrollList::GetItem()
///
/// \brief	Gets the item. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The item. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::GUI::CScrollListItem CParameterScrollList::GetItem()
{
	return m_Item;
}