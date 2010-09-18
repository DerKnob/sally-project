////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ListViewItem.cpp
///
/// \brief	Implements the list view item class. 
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

#include "ListViewItem.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CListViewItem::CListViewItem(const std::string& identifier, const std::string& firstLine,
/// int imageIndex)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	identifier	The identifier. 
/// \param	firstLine	The first line. 
/// \param	imageIndex	Zero-based index of the image. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CListViewItem::CListViewItem(const std::string& identifier, const std::string& firstLine, int imageIndex)
	:m_strIdentifier(identifier), m_iImageIndex(imageIndex), m_strFirstLine(firstLine),
	m_eLocalisation(LISTVIEW_LOCALISATION_FROM_PARENT)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CListViewItem::~CListViewItem()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CListViewItem::~CListViewItem()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CListViewItem::GetText()
///
/// \brief	Gets the text. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The text. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CListViewItem::GetText()
{
	return m_strFirstLine;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CListViewItem::GetIdentifier()
///
/// \brief	Gets the identifier. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The identifier. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CListViewItem::GetIdentifier()
{
	return m_strIdentifier;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CListViewItem::GetImageIndex()
///
/// \brief	Gets the image index. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The image index. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CListViewItem::GetImageIndex()
{
	return m_iImageIndex;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CListViewItem::SetText(const std::string& firstLine)
///
/// \brief	Sets a text. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	firstLine	The first line. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListViewItem::SetText(const std::string& firstLine)
{
	m_strFirstLine = firstLine;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CListViewItem::SetIdentifier(const std::string& identifier)
///
/// \brief	Sets an identifier. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	identifier	The identifier. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListViewItem::SetIdentifier(const std::string& identifier)
{
	m_strIdentifier = identifier;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CListViewItem::SetImageIndex(int imageIndex)
///
/// \brief	Sets an image index. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	imageIndex	Zero-based index of the image. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListViewItem::SetImageIndex(int imageIndex)
{
	m_iImageIndex = imageIndex;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CListViewItem::SetLocalised(LISTVIEW_LOCALISATION value)
///
/// \brief	Sets a localised. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	value	The value. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListViewItem::SetLocalised(LISTVIEW_LOCALISATION value)
{
	m_eLocalisation = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	LISTVIEW_LOCALISATION CListViewItem::IsLocalised()
///
/// \brief	Is localised. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	. 
////////////////////////////////////////////////////////////////////////////////////////////////////

LISTVIEW_LOCALISATION CListViewItem::IsLocalised()
{
	return m_eLocalisation;
}