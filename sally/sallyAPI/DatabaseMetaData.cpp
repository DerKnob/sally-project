////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\DatabaseMetaData.cpp
///
/// \brief	Implements the database meta data class. 
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

#include "DatabaseMetaData.h"

using namespace SallyAPI::Database;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CDatabaseMetaData::CDatabaseMetaData(std::vector<CDatabaseObject> vct)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	vct	The vct. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CDatabaseMetaData::CDatabaseMetaData(std::vector<CDatabaseObject> vct) 
: m_vObjects(vct)
{  
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CDatabaseMetaData::~CDatabaseMetaData()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CDatabaseMetaData::~CDatabaseMetaData() 
{  
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CDatabaseMetaData::GetNumObjects()
///
/// \brief	Gets the number objects. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The number objects. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CDatabaseMetaData::GetNumObjects() 
{
	return m_vObjects.size();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CDatabaseMetaData::GetName(int colNum)
///
/// \brief	Gets a name. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \exception	CSQLException	Thrown when csql. 
///
/// \param	colNum	The col number. 
///
/// \return	The name. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CDatabaseMetaData::GetName(int colNum)
{
	int i = colNum - 1;
	if (i < 0 || i >= ((int) m_vObjects.size())) 
		throw CSQLException("Invalid column number");
	return m_vObjects[i].m_strName;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CDatabaseMetaData::GetType(int colNum)
///
/// \brief	Gets a type. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \exception	CSQLException	Thrown when csql. 
///
/// \param	colNum	The col number. 
///
/// \return	The type. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CDatabaseMetaData::GetType(int colNum)
{
	int i = colNum - 1;
	if (i < 0 || i >= ((int) m_vObjects.size())) 
		throw CSQLException("Invalid column number");
	return m_vObjects[i].m_strType;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CDatabaseMetaData::GetSql(int colNum)
///
/// \brief	Gets a sql. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \exception	CSQLException	Thrown when csql. 
///
/// \param	colNum	The col number. 
///
/// \return	The sql. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CDatabaseMetaData::GetSql(int colNum)
{
	int i = colNum - 1;
	if (i < 0 || i >= ((int) m_vObjects.size())) 
		throw CSQLException("Invalid column number");
	return m_vObjects[i].m_strSql;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CDatabaseMetaData::DoesObjectExist(const char* name_, const char* type_)
///
/// \brief	Query if 'name_' does object exist. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	name_	The name. 
/// \param	type_	The type. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CDatabaseMetaData::DoesObjectExist(const char* name_, 
											 const char* type_)
{
	int sz = m_vObjects.size();
	for (int i=0; i<sz; ++i) 
	{
		if ((SallyAPI::String::StringHelper::StringToLower(m_vObjects[i].m_strName) == SallyAPI::String::StringHelper::StringToLower(name_))
			&& (SallyAPI::String::StringHelper::StringToLower(m_vObjects[i].m_strType) == SallyAPI::String::StringHelper::StringToLower(type_)))
			return true;
	}
	return false;
}
