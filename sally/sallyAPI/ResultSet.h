////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ResultSet.h
///
/// \brief	Declares the result set class. 
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
#include <sqlite3.h>
#include "SQLException.h"
#include "ResultSetMetaData.h"
#include "StringHelper.h"

namespace SallyAPI
{
	namespace Database
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CResultSet
		///
		/// \brief	Result set. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CResultSet
		{
		private:
			sqlite3_stmt*		m_pStmt;
			CResultSetMetaData*	m_pResultMetaData;
			int					m_nRes;
			bool				m_bSecondStep;
		public:
			CResultSet(sqlite3_stmt *ps);
			~CResultSet();

			bool			Next();
			bool			IsNull(int colNum);
			std::string		GetString(int colNum);
			int				GetInt(int colNum);
			long			GetLong(int colNum);
			unsigned int	GetUInt(int colNum);
			unsigned long	GetULong(int colNum);
			float			GetFloat(int colNum);
			double			GetDouble(int colNum);
			bool			GetBoolean(int colNum);

			CResultSetMetaData* GetMetaData();
		};
	}
}