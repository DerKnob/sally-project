////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ScrollListItem.cpp
///
/// \brief	Implements the scroll list item class. 
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

#include "ScrollListItem.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CScrollListItem::CScrollListItem(int graphicId, int command, const std::string& name)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	graphicId	Identifier for the graphic. 
/// \param	command		The command. 
/// \param	name		The name. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CScrollListItem::CScrollListItem(int graphicId, int command, const std::string& name)
	: m_iGraphicId(graphicId), m_iCommand(command), m_strName(name)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CScrollListItem::~CScrollListItem()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CScrollListItem::~CScrollListItem()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CScrollListItem::GetGraphicId()
///
/// \brief	Gets the graphic identifier. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The graphic identifier. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CScrollListItem::GetGraphicId()
{
	return m_iGraphicId;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CScrollListItem::GetCommand()
///
/// \brief	Gets the command. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The command. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CScrollListItem::GetCommand()
{
	return m_iCommand;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CScrollListItem::GetName()
///
/// \brief	Gets the name. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The name. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CScrollListItem::GetName()
{
	return m_strName;
}
