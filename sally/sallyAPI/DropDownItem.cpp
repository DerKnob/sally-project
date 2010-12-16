////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\DropDownItem.cpp
///
/// \brief	Implements the drop down item class. 
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

#include "DropDownItem.h"
#include <string>

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CDropDownItem::CDropDownItem(const std::string& identifier, const std::string& text,
/// int image)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	identifier	The identifier. 
/// \param	text		The text. 
/// \param	image		The image. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CDropDownItem::CDropDownItem(const std::string& identifier, const std::string& text, int image)
	:m_strIdentifier(identifier), m_iImage(image), m_strText(text), m_eLocalisation(DROPDOWN_LOCALISATION_FROM_PARENT)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CDropDownItem::~CDropDownItem()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CDropDownItem::~CDropDownItem()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CDropDownItem::GetText()
///
/// \brief	Gets the text. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The text. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CDropDownItem::GetText()
{
	return m_strText;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CDropDownItem::GetIdentifier()
///
/// \brief	Gets the identifier. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The identifier. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CDropDownItem::GetIdentifier()
{
	return m_strIdentifier;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CDropDownItem::GetImageId()
///
/// \brief	Gets the image identifier. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The image identifier. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CDropDownItem::GetImageId()
{
	return m_iImage;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CDropDownItem::SetText(const std::string& text)
///
/// \brief	Sets a text. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	text	The text. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CDropDownItem::SetText(const std::string& text)
{
	m_strText = text;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CDropDownItem::SetIdentifier(const std::string& identifier)
///
/// \brief	Sets an identifier. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	identifier	The identifier. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CDropDownItem::SetIdentifier(const std::string& identifier)
{
	m_strIdentifier = identifier;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CDropDownItem::SetImageId(int image)
///
/// \brief	Sets an image identifier. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	image	The image. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CDropDownItem::SetImageId(int image)
{
	m_iImage = image;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CDropDownItem::SetLocalised(DROPDOWN_LOCALISATION value)
///
/// \brief	Sets a localised. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	value	The value. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CDropDownItem::SetLocalised(DROPDOWN_LOCALISATION value)
{
	m_eLocalisation = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	DROPDOWN_LOCALISATION CDropDownItem::IsLocalised()
///
/// \brief	Is localised. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	. 
////////////////////////////////////////////////////////////////////////////////////////////////////

DROPDOWN_LOCALISATION CDropDownItem::IsLocalised()
{
	return m_eLocalisation;
}