////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ParameterApplicationInfo.cpp
///
/// \brief	Implements the parameter application information class. 
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

#include "ParameterApplicationInfo.h"

using namespace SallyAPI::GUI::SendMessage;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CParameterApplicationInfo::CParameterApplicationInfo(const std::string& explicidAppName)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	explicidAppName	Name of the explicid application. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CParameterApplicationInfo::CParameterApplicationInfo(const std::string& explicidAppName)
	:m_pWindow(NULL), m_strExplicidAppName(explicidAppName)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CParameterApplicationInfo::~CParameterApplicationInfo()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CParameterApplicationInfo::~CParameterApplicationInfo()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::GUI::CApplicationWindow* CParameterApplicationInfo::GetWindow()
///
/// \brief	Gets the window. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	null if it fails, else the window. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::GUI::CApplicationWindow* CParameterApplicationInfo::GetWindow()
{
	return m_pWindow;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CParameterApplicationInfo::SetWindow(SallyAPI::GUI::CApplicationWindow* window)
///
/// \brief	Sets a window. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	window	If non-null, the window. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CParameterApplicationInfo::SetWindow(SallyAPI::GUI::CApplicationWindow* window)
{
	m_pWindow = window;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CParameterApplicationInfo::GetExplicidAppName()
///
/// \brief	Gets the explicid application name. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The explicid application name. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CParameterApplicationInfo::GetExplicidAppName()
{
	return m_strExplicidAppName;
}
