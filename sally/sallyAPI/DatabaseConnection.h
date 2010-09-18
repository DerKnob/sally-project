////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\DatabaseConnection.h
///
/// \brief	Declares the database connection class. 
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
#include <sqlite3.h>
#include <map>
#include <string>
#include "DatabaseMetaData.h"
#include "Statement.h"
#include "DatabaseObject.h"
#include "StringHelper.h"

namespace SallyAPI
{
	namespace Database
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CDatabaseConnection
		///
		/// \brief	Database connection. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CDatabaseConnection
		{
		private:
			static std::map<std::string, SallyAPI::Database::CDatabaseConnection*>	m_pOpenFiles;
			static CRITICAL_SECTION													m_critSectOpenCloseDatabase;

			int					m_iConnectedThreads;
			sqlite3*			m_pSqlite;
			CStatement*			m_pStatement;
			CDatabaseMetaData*	m_pDatabaseMetaData;
			CRITICAL_SECTION	m_critSectLockDatabase;
			

			CDatabaseConnection();
			~CDatabaseConnection();

			void				ConnectThread();
			void				DisconnectThread();

			void				OpenDB(const std::string& db_name);
			void				CloseDB();
		public:
			int					GetConnectedThreads();

			static SallyAPI::Database::CDatabaseConnection*	Open(const std::string& file);
			static void			Close(const std::string& file);

			static void			Init();
			static void			Destroy();
			
			CStatement*			CreateStatement();
			CDatabaseMetaData*	GetDatabaseMetaData();
			void				RefreshMetaData();			
			void				LockDatabase();
			void				ReleaseDatabase();
		};
	}
}
