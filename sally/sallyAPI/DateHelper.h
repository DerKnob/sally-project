////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\DateHelper.h
///
/// \brief	Declares the date helper class. 
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

#pragma once
#include "Define.h"
#include <string>

namespace SallyAPI
{
	namespace Date
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	DateHelper
		///
		/// \brief	Date helper. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY DateHelper
		{
		private:
			DateHelper();
			~DateHelper();
		public:
			static std::string	GetCurrentDateString(bool withMilliseconds = true);
			static std::string	GetDateString(SYSTEMTIME currentTime, bool withMilliseconds = true);
			static bool			IsLeapYear(const UINT uJahr);
			static short		GetDaysInMonth(const UINT uMonat, const UINT uJahr);
			static short		GetDaysInYears(const UINT uJahr);
			static short		GetWeekday(const UINT uTag, const UINT uMonat, const UINT uJahr);
			static short		GetDayOfYear(const UINT uTag, const UINT uMonat, const UINT uJahr);
			static short		GetCalendarWeek(const UINT uTag, const UINT uMonat, const UINT uJahr);
			static void			GetEasterDate(const UINT uJahr, UINT & uTag, UINT & uMonat);
			static void			GetFourthAdvent(const UINT uJahr, UINT & uTag, UINT & uMonat);
			static long			TimeDifferneceInYears(const SYSTEMTIME & Startzeit, const SYSTEMTIME & Endezeit);
			static long			TimeDifferenceInDays(const SYSTEMTIME & Startzeit, const SYSTEMTIME & Endezeit);
			static SYSTEMTIME	TimeDifferenceInDays(const SYSTEMTIME & Startzeit, const long & Endezeit);
			static ULONGLONG	GetMilliseconds();
		};
	}
}
