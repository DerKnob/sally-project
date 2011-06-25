////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\StringCompareWithArray.cpp
///
/// \brief	Implements the string compare with array class. 
///
/// \author	Christian Knobloch
/// \date	25.06.2011
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

#include "StringCompareWithArray.h"

using namespace SallyAPI::String;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CStringCompareWithArray::CStringCompareWithArray(std::map<std::string,
/// std::string>* foldersFilesDate)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	25.06.2011
///
/// \param [in,out]	foldersFilesDate	If non-null, date of the folders files. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CStringCompareWithArray::CStringCompareWithArray(std::map<std::string, std::string>* foldersFilesDate)
	: m_pmFoldersFilesDate(foldersFilesDate)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CStringCompareWithArray::~CStringCompareWithArray()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	25.06.2011
////////////////////////////////////////////////////////////////////////////////////////////////////

CStringCompareWithArray::~CStringCompareWithArray()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CStringCompareWithArray::operator()(const std::string &left,
/// const std::string &right) const
///
/// \brief	operator casting operator. 
///
/// \author	Christian Knobloch
/// \date	25.06.2011
///
/// \return	The result of the operation. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CStringCompareWithArray::operator()(const std::string &left, const std::string &right)
{
	std::string dataLeft = (*m_pmFoldersFilesDate)[left];
	std::string dataRight = (*m_pmFoldersFilesDate)[right];

	for(std::string::const_iterator lit = dataLeft.begin(), rit = dataRight.begin(); lit != dataLeft.end() && rit != dataRight.end(); ++lit, ++rit )
	{
		if(tolower(*lit) > tolower(*rit) )
			return true;
		else if(tolower(*lit) < tolower(*rit) )
			return false;
	}
	if(dataLeft.size() > dataRight.size())
		return true;
	return false;
}