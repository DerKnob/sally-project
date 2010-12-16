////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ParameterOnScreenMenu.cpp
///
/// \brief	Implements the parameter on screen menu class. 
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

#include "ParameterOnScreenMenu.h"

using namespace SallyAPI::GUI::SendMessage;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CParameterOnScreenMenu::CParameterOnScreenMenu(int icon,
/// const std::string& text)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	icon	The icon. 
/// \param	text	The text. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CParameterOnScreenMenu::CParameterOnScreenMenu(int icon, const std::string& text)
	:m_iIcon(icon), m_strText(text)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CParameterOnScreenMenu::~CParameterOnScreenMenu()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CParameterOnScreenMenu::~CParameterOnScreenMenu()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CParameterOnScreenMenu::GetText()
///
/// \brief	Gets the text. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The text. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CParameterOnScreenMenu::GetText()
{
	return m_strText;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CParameterOnScreenMenu::GetIcon()
///
/// \brief	Gets the icon. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The icon. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CParameterOnScreenMenu::GetIcon()
{
	return m_iIcon;
}