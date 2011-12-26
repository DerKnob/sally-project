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

#include "ParameterNotificationInfo.h"

using namespace SallyAPI::GUI::SendMessage;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CParameterNotificationInfo::CParameterNotificationInfo(int icon,
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

CParameterNotificationInfo::CParameterNotificationInfo(int icon, const std::string& text,
										 const std::string& description)
	:m_iIcon(icon), m_strText(text), m_strDescription(description), m_pPicture(NULL), m_pID(0)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CParameterNotificationInfo::CParameterNotificationInfo(SallyAPI::GUI::CPicture* picture,
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

CParameterNotificationInfo::CParameterNotificationInfo(SallyAPI::GUI::CPicture* picture, const std::string& text, const std::string& description)
	:m_iIcon(0), m_strText(text), m_strDescription(description), m_pPicture(picture), m_pID(0)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CParameterNotificationInfo::~CParameterNotificationInfo()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CParameterNotificationInfo::~CParameterNotificationInfo()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CParameterNotificationInfo::GetDescription()
///
/// \brief	Gets the description. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The description. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CParameterNotificationInfo::GetDescription()
{
	return m_strDescription;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CParameterNotificationInfo::GetText()
///
/// \brief	Gets the text. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The text. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CParameterNotificationInfo::GetText()
{
	return m_strText;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CParameterNotificationInfo::GetIcon()
///
/// \brief	Gets the icon. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The icon. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CParameterNotificationInfo::GetIcon()
{
	return m_iIcon;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::GUI::CPicture* CParameterNotificationInfo::GetPicture()
///
/// \brief	Gets the picture. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	null if it fails, else the picture. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::GUI::CPicture* CParameterNotificationInfo::GetPicture()
{
	return m_pPicture;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CParameterNotificationInfo::GetId()
///
/// \brief	Gets the identifier. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The identifier. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CParameterNotificationInfo::GetId()
{
	return m_pID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CParameterNotificationInfo::SetId(int id)
///
/// \brief	Sets an identifier. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	id	The identifier. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CParameterNotificationInfo::SetId(int id)
{
	m_pID = id;
}