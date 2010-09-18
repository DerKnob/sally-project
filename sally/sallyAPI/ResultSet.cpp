////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ResultSet.cpp
///
/// \brief	Implements the result set class. 
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

#include "ResultSet.h"

using namespace SallyAPI::Database;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CResultSet::CResultSet(sqlite3_stmt* ps)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \exception	CSQLException	Thrown when csql. 
///
/// \param [in,out]	ps	If non-null, the ps. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CResultSet::CResultSet(sqlite3_stmt* ps) 

{
	if (ps == NULL) throw CSQLException("NULL statement object");	
	m_pStmt = ps;
	m_nRes = sqlite3_step(m_pStmt);
	if (m_nRes != SQLITE_OK && m_nRes != SQLITE_ROW && m_nRes != SQLITE_DONE)
		throw CSQLException(m_nRes);
	m_bSecondStep = true;
	std::vector<CColumn> vct;
	int col_count = sqlite3_column_count(m_pStmt);
	for (int i=0; i<col_count; ++i) 
	{
		CColumn col;
		col.m_strName = sqlite3_column_name(m_pStmt,i);
		try 
		{
			const char* type = sqlite3_column_decltype(m_pStmt,i);
			if (type)
				col.m_strType = type;
			else
				col.m_strType = "VARIANT"; // is this selection good?
		}
		catch(...) 
		{
			col.m_strType = "VARIANT";
		}
		vct.push_back(col);
	}
	m_pResultMetaData = new CResultSetMetaData(vct);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CResultSet::~CResultSet()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CResultSet::~CResultSet() 
{
	if (m_pStmt != NULL)  
	{
		sqlite3_finalize(m_pStmt);		
		m_pStmt = NULL;
	}
	delete m_pResultMetaData; 
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CResultSet::Next()
///
/// \brief	Nexts this object. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \exception	CSQLException	Thrown when csql. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CResultSet::Next() 
{
	if (m_nRes != SQLITE_ROW) return false;
	if (m_bSecondStep) 
	{ 
		m_bSecondStep = false; 
		return true; 
	}
	m_nRes = sqlite3_step(m_pStmt);
	if (m_nRes != SQLITE_OK && m_nRes != SQLITE_ROW && m_nRes != SQLITE_DONE)
		throw CSQLException(m_nRes);	
	if (m_nRes == SQLITE_DONE) return false;
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CResultSet::IsNull(int colNum)
///
/// \brief	Query if 'colNum' is null. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	colNum	The col number. 
///
/// \return	true if null, false if not. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CResultSet::IsNull(int colNum) 
{
	// as of now we don't support null
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CResultSet::GetString(int colNum)
///
/// \brief	Gets a string. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \exception	CSQLException	Thrown when csql. 
///
/// \param	colNum	The col number. 
///
/// \return	The string. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CResultSet::GetString(int colNum) 
{
	int i = colNum-1;
	if (i < 0) throw CSQLException("Invalid column number");
	std::string ret = (const char*)sqlite3_column_text(m_pStmt,(i));
	return ret;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CResultSet::GetInt(int colNum)
///
/// \brief	Gets an int. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	colNum	The col number. 
///
/// \return	The int. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CResultSet::GetInt(int colNum) 
{
	return atoi(GetString(colNum).c_str());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	long CResultSet::GetLong(int colNum)
///
/// \brief	Gets a long. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	colNum	The col number. 
///
/// \return	The long. 
////////////////////////////////////////////////////////////////////////////////////////////////////

long CResultSet::GetLong(int colNum) 
{
	return atol(GetString(colNum).c_str());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	unsigned int CResultSet::GetUInt(int colNum)
///
/// \brief	Gets a u int. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	colNum	The col number. 
///
/// \return	The u int. 
////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned int CResultSet::GetUInt(int colNum) 
{
	return (unsigned int)atoi(GetString(colNum).c_str());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	unsigned long CResultSet::GetULong(int colNum)
///
/// \brief	Gets a u long. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	colNum	The col number. 
///
/// \return	The u long. 
////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned long CResultSet::GetULong(int colNum) 
{
	return (unsigned long)atol(GetString(colNum).c_str());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	float CResultSet::GetFloat(int colNum)
///
/// \brief	Gets a float. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	colNum	The col number. 
///
/// \return	The float. 
////////////////////////////////////////////////////////////////////////////////////////////////////

float CResultSet::GetFloat(int colNum) 
{
	return ((float) atof(GetString(colNum).c_str()));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	double CResultSet::GetDouble(int colNum)
///
/// \brief	Gets a double. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	colNum	The col number. 
///
/// \return	The double. 
////////////////////////////////////////////////////////////////////////////////////////////////////

double CResultSet::GetDouble(int colNum) 
{
	return (double)atof(GetString(colNum).c_str());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CResultSet::GetBoolean(int colNum)
///
/// \brief	Gets a boolean. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	colNum	The col number. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CResultSet::GetBoolean(int colNum) 
{
	std::string str = SallyAPI::String::StringHelper::StringToLower(GetString(colNum));
	return (str == "true" || str == "1");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CResultSetMetaData* CResultSet::GetMetaData()
///
/// \brief	Gets the meta data. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	null if it fails, else the meta data. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CResultSetMetaData* CResultSet::GetMetaData() 
{
	return m_pResultMetaData;
}