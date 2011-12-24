////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ParameterInfoPopup.cpp
///
/// \brief	Implements the parameter information popup class. 
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

#include "ParameterInfoPopup.h"

using namespace SallyAPI::GUI::SendMessage;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CParameterInfoPopup::CParameterInfoPopup(int icon,
/// const std::string& text, const std::string& description)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	icon		The icon. 
/// \param	text		The text. 
/// \param	description	The description. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CParameterInfoPopup::CParameterInfoPopup(int icon, const std::string& text,
										 const std::string& description)
	:m_iIcon(icon), m_strText(text), m_strDescription(description), m_pPicture(NULL), m_pID(0)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CParameterInfoPopup::CParameterInfoPopup(SallyAPI::GUI::CPicture* picture,
/// const std::string& text, const std::string& description)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	picture	If non-null, the picture. 
/// \param	text			The text. 
/// \param	description		The description. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CParameterInfoPopup::CParameterInfoPopup(SallyAPI::GUI::CPicture* picture, const std::string& text, const std::string& description)
	:m_iIcon(0), m_strText(text), m_strDescription(description), m_pPicture(picture), m_pID(0)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CParameterInfoPopup::~CParameterInfoPopup()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CParameterInfoPopup::~CParameterInfoPopup()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CParameterInfoPopup::GetDescription()
///
/// \brief	Gets the description. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The description. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CParameterInfoPopup::GetDescription()
{
	return m_strDescription;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CParameterInfoPopup::GetText()
///
/// \brief	Gets the text. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The text. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CParameterInfoPopup::GetText()
{
	return m_strText;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CParameterInfoPopup::GetIcon()
///
/// \brief	Gets the icon. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The icon. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CParameterInfoPopup::GetIcon()
{
	return m_iIcon;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::GUI::CPicture* CParameterInfoPopup::GetPicture()
///
/// \brief	Gets the picture. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	null if it fails, else the picture. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::GUI::CPicture* CParameterInfoPopup::GetPicture()
{
	return m_pPicture;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CParameterInfoPopup::GetId()
///
/// \brief	Gets the identifier. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The identifier. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CParameterInfoPopup::GetId()
{
	return m_pID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CParameterInfoPopup::SetId(int id)
///
/// \brief	Sets an identifier. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	id	The identifier. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CParameterInfoPopup::SetId(int id)
{
	m_pID = id;
}