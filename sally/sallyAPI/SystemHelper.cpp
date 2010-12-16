////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\SystemHelper.cpp
///
/// \brief	Implements the system helper class. 
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

#include "SystemHelper.h"

using namespace SallyAPI::System;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SystemHelper::SystemHelper()
///
/// \brief	Default constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

SystemHelper::SystemHelper()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SystemHelper::~SystemHelper()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

SystemHelper::~SystemHelper()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string SystemHelper::GetModulePath()
///
/// \brief	Gets the module path. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The module path. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string SystemHelper::GetModulePath()
{
	std::string sModulePath;
	std::string result;

	char* pdest;
	char modulepath[MAX_PATH];

	GetModuleFileName (GetModuleHandle (NULL),modulepath,254);
	pdest = strrchr (modulepath,'\\');
	modulepath [pdest - modulepath+1] = '\0';

	sModulePath.append(modulepath);

	result = SallyAPI::String::PathHelper::CorrectPath(sModulePath);
	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int SystemHelper::ConvertSystemTimeToInt(SYSTEMTIME systemTime)
///
/// \brief	Convert system time to int. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	systemTime	Time of the system. 
///
/// \return	. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int SystemHelper::ConvertSystemTimeToInt(SYSTEMTIME systemTime)
{
	int result = 0;

	result += systemTime.wDay;
	result += (systemTime.wMonth * 100);
	result += (systemTime.wYear * 10000);

	return result;
}