////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	_shared\StringHelper.h
///
/// \brief	Declares the string helper class. 
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

#pragma once
#include "Define.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

namespace SallyAPI
{
	namespace String
	{
		class DLL_API_SALLY StringHelper
		{
		private:
			StringHelper();
			~StringHelper();
		public:
			static std::string		GetStringBetween(const std::string& searchInString, const std::string& first, const std::string& last);
			static bool 			StringEndsWith(const std::string& str, const std::string& end);
			static std::string		StringToUpper(const std::string& strToConvert);
			static std::string		StringToLower(const std::string& strToConvert);
			static std::vector<std::string> TokenizeString(const std::string& sString, const std::string& token);
			static int				ConvertToInt(const std::string& sString);
			static std::string		ConvertToString(int i);
			static double			ConvertToDouble(const std::string& sString);
			static float			ConvertToFloat(const std::string& sString);
			static std::string		ConvertFloatToString(float i);
			static std::string		ConvertDoubleToString(double i);
			static std::string		ReplaceString(const std::string& in, const std::string& replaceThis, const std::string& replaceWith);

			static std::string		base64_encode(unsigned char const* , unsigned int len);
			static std::string		base64_decode(std::string const& s);
		};
	}
}