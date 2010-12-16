////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	_shared\SystemHelper.cpp
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
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
/// 
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
/// GNU General Public License for more details.
/// 
/// You should have received a copy of the GNU General Public License
/// along with this program. If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "SystemHelper.h"

using namespace SallyAPI::System;

SystemHelper::SystemHelper()
{
}

SystemHelper::~SystemHelper()
{
}

unsigned SystemHelper::TimeSeed()
{
	time_t now = time ( 0 );
	unsigned char* p = (unsigned char *)&now;
	unsigned seed = 0;
	size_t i;

	for ( i = 0; i < sizeof now; i++ )
		seed = seed * ( UCHAR_MAX + 2U ) + p[i];

	return seed;
}

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

int SystemHelper::ConvertSystemTimeToInt(SYSTEMTIME systemTime)
{
	int result = 0;

	result += systemTime.wDay;
	result += (systemTime.wMonth * 100);
	result += (systemTime.wYear * 10000);

	return result;
}