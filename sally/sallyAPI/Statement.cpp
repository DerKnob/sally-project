////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Statement.cpp
///
/// \brief	Implements the statement class. 
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

#include "Statement.h"

using namespace SallyAPI::Database;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CStatement::CStatement(sqlite3* pS)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	pS	If non-null, the s. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CStatement::CStatement(sqlite3* pS) 
: m_pSqlite(pS), m_pResultSet(0)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CStatement::~CStatement()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CStatement::~CStatement() 
{
	delete m_pResultSet;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CStatement::Execute(const char* sql)
///
/// \brief	Executes. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \exception	CSQLException	Thrown when csql. 
///
/// \param	sql	The sql. 
///
/// \return	. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CStatement::Execute(const char* sql)
{
	int ret = sqlite3_exec(m_pSqlite,sql,0,0,0);
	if (ret != SQLITE_OK)
		throw CSQLException(ret);  

	return sqlite3_changes(m_pSqlite);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CResultSet* CStatement::ExecuteQuery(const char* sql)
///
/// \brief	Executes the query operation. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \exception	CSQLException	Thrown when csql. 
///
/// \param	sql	The sql. 
///
/// \return	null if it fails, else. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CResultSet* CStatement::ExecuteQuery(const char* sql) 
{
	sqlite3_stmt *stmt;
	int ret = sqlite3_prepare(m_pSqlite,sql,strlen(sql),&stmt,NULL);
	if (ret != SQLITE_OK)
		throw CSQLException(ret);

	delete m_pResultSet;
	m_pResultSet = new CResultSet(stmt);
	return m_pResultSet;
}
