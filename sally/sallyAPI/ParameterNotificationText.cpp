////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ParameterNotificationText.cpp
///
/// \brief	Implements the parameter notification text class. 
///
/// \author	Christian Knobloch
/// \date	26.12.2011
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

#include "ParameterNotificationText.h"

using namespace SallyAPI::GUI::SendMessage;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CParameterNotificationText::CParameterNotificationText(const std::string& text)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	26.12.2011
///
/// \param	text	The text. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CParameterNotificationText::CParameterNotificationText(const std::string& text)
	:m_strText(text)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CParameterNotificationText::~CParameterNotificationText()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	26.12.2011
////////////////////////////////////////////////////////////////////////////////////////////////////

CParameterNotificationText::~CParameterNotificationText()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CParameterNotificationText::GetText()
///
/// \brief	Gets the text. 
///
/// \author	Christian Knobloch
/// \date	26.12.2011
///
/// \return	The text. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CParameterNotificationText::GetText()
{
	return m_strText;
}
