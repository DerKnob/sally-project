////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\DatabaseConnection.cpp
///
/// \brief	Implements the database connection class. 
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

#include "DatabaseConnection.h"

using namespace SallyAPI::Database;

std::map<std::string, SallyAPI::Database::CDatabaseConnection*> SallyAPI::Database::CDatabaseConnection::m_pOpenFiles;
CRITICAL_SECTION SallyAPI::Database::CDatabaseConnection::m_critSectOpenCloseDatabase;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CDatabaseConnection::CDatabaseConnection()
///
/// \brief	Default constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CDatabaseConnection::CDatabaseConnection()
: m_pSqlite(0), m_pStatement(0), m_pDatabaseMetaData(0), m_iConnectedThreads(0)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CDatabaseConnection::~CDatabaseConnection()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CDatabaseConnection::~CDatabaseConnection() 
{
	CloseDB();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::Database::CDatabaseConnection* CDatabaseConnection::Open(const std::string& file)
///
/// \brief	Opens the given file. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	file	The file. 
///
/// \return	null if it fails, else. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::Database::CDatabaseConnection* CDatabaseConnection::Open(const std::string& file)
{
	EnterCriticalSection(&m_critSectOpenCloseDatabase);

	std::string fileIdent = SallyAPI::String::StringHelper::StringToLower(file);
	if (m_pOpenFiles[fileIdent] != 0)
	{
		m_pOpenFiles[fileIdent]->ConnectThread();

		LeaveCriticalSection(&m_critSectOpenCloseDatabase);
		return m_pOpenFiles[fileIdent];
	}

	SallyAPI::Database::CDatabaseConnection* newConnection = new SallyAPI::Database::CDatabaseConnection();

	newConnection->OpenDB(fileIdent);

	// Save Connection
	m_pOpenFiles[fileIdent] = newConnection;

	LeaveCriticalSection(&m_critSectOpenCloseDatabase);

	return newConnection;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CDatabaseConnection::Close(const std::string& file)
///
/// \brief	Closes the given file. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	file	The file. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CDatabaseConnection::Close(const std::string& file)
{
	EnterCriticalSection(&m_critSectOpenCloseDatabase);

	std::string fileIdent = SallyAPI::String::StringHelper::StringToLower(file);
	m_pOpenFiles[fileIdent]->DisconnectThread();

	if (m_pOpenFiles[fileIdent]->GetConnectedThreads() != 0)
	{
		LeaveCriticalSection(&m_critSectOpenCloseDatabase);
		return;
	}

	// no thread connected - kill it
	m_pOpenFiles[fileIdent]->CloseDB();
	delete m_pOpenFiles[fileIdent];
	m_pOpenFiles[fileIdent] = 0;

	LeaveCriticalSection(&m_critSectOpenCloseDatabase);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CDatabaseConnection::ConnectThread()
///
/// \brief	Connects the thread. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CDatabaseConnection::ConnectThread()
{
	++m_iConnectedThreads;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CDatabaseConnection::DisconnectThread()
///
/// \brief	Disconnects a thread. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CDatabaseConnection::DisconnectThread()
{
	--m_iConnectedThreads;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CDatabaseConnection::GetConnectedThreads()
///
/// \brief	Gets the connected threads. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The connected threads. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CDatabaseConnection::GetConnectedThreads()
{
	return m_iConnectedThreads;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CDatabaseConnection::OpenDB(const std::string& db_name)
///
/// \brief	Opens a database. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \exception	CSQLException	Thrown when csql. 
///
/// \param	db_name	Name of the database. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CDatabaseConnection::OpenDB(const std::string& db_name)
{
	int ret = sqlite3_open(db_name.c_str(), &m_pSqlite);
	if (ret != SQLITE_OK) 
	{		
		CloseDB();
		throw CSQLException(ret);
	}
	m_pDatabaseMetaData = NULL;

	// fills in the CMetaData
	RefreshMetaData();

	++m_iConnectedThreads;

	InitializeCriticalSection(&m_critSectLockDatabase);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CStatement* CDatabaseConnection::CreateStatement()
///
/// \brief	Creates the statement. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	null if it fails, else. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CStatement* CDatabaseConnection::CreateStatement() 
{
	delete m_pStatement;
	m_pStatement = new CStatement(m_pSqlite);
	return m_pStatement;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CDatabaseMetaData* CDatabaseConnection::GetDatabaseMetaData()
///
/// \brief	Gets the database meta data. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	null if it fails, else the database meta data. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CDatabaseMetaData* CDatabaseConnection::GetDatabaseMetaData() 
{	
	return m_pDatabaseMetaData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CDatabaseConnection::RefreshMetaData()
///
/// \brief	Refresh meta data. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \exception	CSQLException	Thrown when csql. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CDatabaseConnection::RefreshMetaData()
{
	sqlite3_stmt *stmt;
	std::string sql = "select type,name,sql from sqlite_master";
	int ret = sqlite3_prepare(m_pSqlite,sql.c_str(),sql.length(),&stmt,NULL);	
	if (ret != SQLITE_OK)
		throw CSQLException(ret);
	int res = sqlite3_step(stmt);
	std::vector<CDatabaseObject> vct;
	while (true) 
	{
		if (res != SQLITE_ROW && res != SQLITE_OK && res != SQLITE_DONE)
			throw CSQLException(res);
		if (res != SQLITE_ROW) break;		
		CDatabaseObject dobj;
		if (sqlite3_column_text(stmt,0) != NULL)
			dobj.m_strType = (const char*)sqlite3_column_text(stmt,0);
		if (sqlite3_column_text(stmt,1) != NULL)
			dobj.m_strName = (const char*)sqlite3_column_text(stmt,1);
		if (sqlite3_column_text(stmt,2) != NULL)
			dobj.m_strSql = (const char*)sqlite3_column_text(stmt,2);
		vct.push_back(dobj);
		res = sqlite3_step(stmt);
	}	
	sqlite3_finalize(stmt);
	delete m_pDatabaseMetaData;
	m_pDatabaseMetaData = new CDatabaseMetaData(vct);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CDatabaseConnection::CloseDB()
///
/// \brief	Closes the database. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CDatabaseConnection::CloseDB() 
{
	delete m_pStatement;
	m_pStatement = NULL;
	delete m_pDatabaseMetaData;
	m_pDatabaseMetaData = NULL;
	if (m_pSqlite != NULL) 
	{
		sqlite3_close(m_pSqlite);
		m_pSqlite = NULL;
	}
	DeleteCriticalSection(&m_critSectLockDatabase);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CDatabaseConnection::LockDatabase()
///
/// \brief	Locks the database. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CDatabaseConnection::LockDatabase()
{
	EnterCriticalSection(&m_critSectLockDatabase);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CDatabaseConnection::ReleaseDatabase()
///
/// \brief	Releases a database. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CDatabaseConnection::ReleaseDatabase()
{
	LeaveCriticalSection(&m_critSectLockDatabase);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CDatabaseConnection::Init()
///
/// \brief	Initialises this object. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CDatabaseConnection::Init()
{
	InitializeCriticalSection(&m_critSectOpenCloseDatabase);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CDatabaseConnection::Destroy()
///
/// \brief	Destroys this object. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CDatabaseConnection::Destroy()
{
	DeleteCriticalSection(&m_critSectOpenCloseDatabase);
}