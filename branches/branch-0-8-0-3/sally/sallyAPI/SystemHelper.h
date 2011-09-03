////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\SystemHelper.h
///
/// \brief	Declares the system helper class. 
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
#include "PathHelper.h"
#include "StringHelper.h"

namespace SallyAPI
{
	namespace System
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	SystemHelper
		///
		/// \brief	System helper. 
		///
		/// \author	Christian Knobloch
		/// \date	07.07.2011
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY SystemHelper
		{
		private:
			SystemHelper();
			~SystemHelper();
		public:
			static std::string	GetModulePath();
			static int			ConvertSystemTimeToInt(SYSTEMTIME systemTime);
			static std::string	GenerateUniqueID();
		};
	}
}