////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\PathHelper.h
///
/// \brief	Declares the path helper class. 
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
#include "StringHelper.h"

namespace SallyAPI
{
	namespace String
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	PathHelper
		///
		/// \brief	Path helper. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY PathHelper
		{
		private:
			PathHelper();
			~PathHelper();
		public:

			static std::string		CorrectFileName(const std::string& str);
			static std::string		CorrectPath(const std::string& sPath);
			static std::string		GetDirectoryFromPath(const std::string& sFile);
			static std::string		GetUpperDirectory(const std::string& sFile);
			static std::string		GetFileFromPath(const std::string& str, const std::string sep = "\\");
			static void				CreateRecursiveDirectory(const std::string& str);
		};
	}
}