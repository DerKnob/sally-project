////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ParameterNotificationOSM.cpp
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

#include "ParameterNotificationOSM.h"

using namespace SallyAPI::GUI::SendMessage;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CParameterNotificationOSM::CParameterNotificationOSM(int icon,
/// const std::string& text)
///
/// \brief	Constructor. Will create a text on screen popup.
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	icon	The icon. 
/// \param	text	The text. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CParameterNotificationOSM::CParameterNotificationOSM(int icon, const std::string& text)
	:m_iIcon(icon), m_strText(text), m_iProcessbar(-1)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CParameterNotificationOSM::CParameterNotificationOSM(int icon, int processbar)
///
/// \brief	Constructor. Will create a processbar on screen popup.
///
/// \author	Christian Knobloch
/// \date	21.12.2011
///
/// \param	icon		The icon. 
/// \param	processbar	The processbar. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CParameterNotificationOSM::CParameterNotificationOSM(int icon, int processbar)
	:m_iIcon(icon), m_iProcessbar(processbar)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CParameterNotificationOSM::~CParameterNotificationOSM()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CParameterNotificationOSM::~CParameterNotificationOSM()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CParameterNotificationOSM::GetText()
///
/// \brief	Gets the text. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The text. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CParameterNotificationOSM::GetText()
{
	return m_strText;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CParameterNotificationOSM::GetIcon()
///
/// \brief	Gets the icon. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The icon. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CParameterNotificationOSM::GetIcon()
{
	return m_iIcon;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CParameterNotificationOSM::GetProcessbar()
///
/// \brief	Gets the processbar. 
///
/// \author	Christian Knobloch
/// \date	21.12.2011
///
/// \return	The processbar. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CParameterNotificationOSM::GetProcessbar()
{
	return m_iProcessbar;
}