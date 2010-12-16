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

#include "ListViewItem.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CListViewItem::CListViewItem(const std::string& identifier, const std::string& text,
/// int imageIndex)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	29.11.2010
///
/// \param	identifier	The identifier. 
/// \param	text		The text. 
/// \param	imageIndex	Zero-based index of the image. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CListViewItem::CListViewItem(const std::string& identifier, const std::string& text, int imageIndex)
	:m_strIdentifier(identifier)
{
	m_mText[0] = text;
	m_mImageId[0] = imageIndex;
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
/// \fn	std::string CListViewItem::GetText(int number)
///
/// \brief	Gets a text. 
///
/// \author	Christian Knobloch
/// \date	29.11.2010
///
/// \param	number	Number of. 
///
/// \return	The text. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CListViewItem::GetText(int number)
{
	return m_mText[number];
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
/// \fn	int CListViewItem::GetImageId(int number)
///
/// \brief	Gets an image identifier. 
///
/// \author	Christian Knobloch
/// \date	01.12.2010
///
/// \param	number	Number of. 
///
/// \return	The image identifier. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CListViewItem::GetImageId(int number)
{
	return m_mImageId[number];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CListViewItem::SetText(const std::string& value, int number)
///
/// \brief	Sets a text. 
///
/// \author	Christian Knobloch
/// \date	30.11.2010
///
/// \param	value	The value. 
/// \param	number	Number of. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListViewItem::SetText(const std::string& value, int number)
{
	m_mText[number] = value;
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
/// \fn	void CListViewItem::SetImageId(int value, int number)
///
/// \brief	Sets an image identifier. 
///
/// \author	Christian Knobloch
/// \date	01.12.2010
///
/// \param	value	The value. 
/// \param	number	Number of. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListViewItem::SetImageId(int value, int number)
{
	m_mImageId[number] = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CListViewItem::SetLocalised(LISTVIEW_LOCALISATION value, int number)
///
/// \brief	Sets a localised. 
///
/// \author	Christian Knobloch
/// \date	29.11.2010
///
/// \param	value	The value. 
/// \param	number	Number of. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListViewItem::SetLocalised(LISTVIEW_LOCALISATION value, int number)
{
	m_mLocalisation[number] = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	LISTVIEW_LOCALISATION CListViewItem::IsLocalised(int number)
///
/// \brief	Is localised. 
///
/// \author	Christian Knobloch
/// \date	29.11.2010
///
/// \param	number	Number of. 
///
/// \return	. 
////////////////////////////////////////////////////////////////////////////////////////////////////

LISTVIEW_LOCALISATION CListViewItem::IsLocalised(int number)
{
	if (m_mLocalisation[number] == NULL)
		return LISTVIEW_LOCALISATION_FROM_PARENT;
	return m_mLocalisation[number];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CListViewItem::SetSmallFont(bool value, int number)
///
/// \brief	Sets a small font. 
///
/// \author	Christian Knobloch
/// \date	30.11.2010
///
/// \param	value	true to value. 
/// \param	number	Number of. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListViewItem::SetSmallFont(bool value, int number)
{
	m_mFont[number] = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CListViewItem::GetSmallFont(int number)
///
/// \brief	Gets a small font. 
///
/// \author	Christian Knobloch
/// \date	30.11.2010
///
/// \param	number	Number of. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CListViewItem::GetSmallFont(int number)
{
	if (m_mFont[number] == NULL)
		return false;
	return m_mFont[number];
}