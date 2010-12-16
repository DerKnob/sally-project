////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ResultSetMetaData.cpp
///
/// \brief	Implements the result set meta data class. 
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

#include "ResultSetMetaData.h"

using namespace SallyAPI::Database;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CResultSetMetaData::CResultSetMetaData(std::vector<CColumn> vct)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	vct	The vct. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CResultSetMetaData::CResultSetMetaData(std::vector<CColumn> vct)
: m_vColumns(vct)
{  
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CResultSetMetaData::~CResultSetMetaData()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CResultSetMetaData::~CResultSetMetaData()
{  
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CResultSetMetaData::GetColumnCount()
///
/// \brief	Gets the column count. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The column count. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CResultSetMetaData::GetColumnCount() 
{
	return m_vColumns.size();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CResultSetMetaData::GetColumnName(int colNum)
///
/// \brief	Gets a column name. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \exception	CSQLException	Thrown when csql. 
///
/// \param	colNum	The col number. 
///
/// \return	The column name. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CResultSetMetaData::GetColumnName(int colNum) 
{
	int i = colNum-1;
	if (i < 0 || i >= ((int) m_vColumns.size()))
		throw CSQLException("Invalid column number");
	return m_vColumns[i].m_strName;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CResultSetMetaData::GetColumnType(int colNum)
///
/// \brief	Gets a column type. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \exception	CSQLException	Thrown when csql. 
///
/// \param	colNum	The col number. 
///
/// \return	The column type. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CResultSetMetaData::GetColumnType(int colNum) 
{
	int i = colNum-1;
	if (i < 0 || i >= ((int) m_vColumns.size()))
		throw CSQLException("Invalid column number");
	return m_vColumns[i].m_strType;
}