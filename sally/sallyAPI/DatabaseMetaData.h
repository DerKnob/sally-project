////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\DatabaseMetaData.h
///
/// \brief	Declares the database meta data class. 
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
#include <vector>
#include "DatabaseObject.h"
#include "SQLException.h"
#include "StringHelper.h"

namespace SallyAPI
{
	namespace Database
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CDatabaseMetaData
		///
		/// \brief	Database meta data. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CDatabaseMetaData
		{
		private:
			std::vector<CDatabaseObject> m_vObjects;
		public:
			CDatabaseMetaData(std::vector<CDatabaseObject> vct);
			~CDatabaseMetaData();

			int			GetNumObjects();
			std::string GetName(int colNum);
			std::string GetType(int colNum);
			std::string GetSql(int colNum);
			bool		DoesObjectExist(const char* name_,const char* type);
		};
	}
}