////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\SystemTime.cpp
///
/// \brief	Implements the system time class. 
///
/// \author	Christian Knobloch
/// \date	13.09.2010
///
/// This file is part of the Sally Project
/// 
/// Copyright(c) 2008-2010 Sally Project
/// http://www.sally-project.de/
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

#include "SystemTime.h"

CSystemTime::CSystemTime()
{
}

CSystemTime::~CSystemTime()
{
}

void CSystemTime::Update()
{
	GetLocalTime(&m_Current);
}

std::string CSystemTime::GetTime()
{
	std::string sTime;
	TCHAR	temp[3];

	if (m_Current.wHour < 10)
		sTime.append("0");
	
	_itoa_s(m_Current.wHour, temp, 3, 10);
	sTime.append(temp);

	sTime.append(":");

	if (m_Current.wMinute < 10)
		sTime.append("0");

	_itoa_s(m_Current.wMinute, temp, 3, 10);
	sTime.append(temp);

	sTime.append(":");

	if (m_Current.wSecond < 10)
		sTime.append("0");

	_itoa_s(m_Current.wSecond, temp, 3, 10);
	sTime.append(temp);

	return sTime;
}

std::string CSystemTime::GetDate()
{
	std::string sDate;
	TCHAR	temp[5];

	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::Config::CLanguageManager* lang = config->GetLanguageLocalization();

	switch (m_Current.wDayOfWeek)
	{
	case 1:
		sDate.append(lang->GetString("Monday"));
		break;
	case 2:
		sDate.append(lang->GetString("Tuesday"));
		break;
	case 3:
		sDate.append(lang->GetString("Wednesday"));
		break;
	case 4:
		sDate.append(lang->GetString("Thursday"));
		break;
	case 5:
		sDate.append(lang->GetString("Friday"));
		break;
	case 6:
		sDate.append(lang->GetString("Saturday"));
		break;
	case 0:
		sDate.append(lang->GetString("Sunday"));
		break;
	}
	sDate.append(", ");
	
	if (m_Current.wDay < 10)
		sDate.append("0");
	_itoa_s(m_Current.wDay, temp, 5, 10);
	sDate.append(temp);

	sDate.append(".");

	if (m_Current.wMonth < 10)
		sDate.append("0");

	_itoa_s(m_Current.wMonth, temp, 5, 10);
	sDate.append(temp);

	sDate.append(".");

	_itoa_s(m_Current.wYear, temp, 5, 10);
	sDate.append(temp);

	return sDate;
}
