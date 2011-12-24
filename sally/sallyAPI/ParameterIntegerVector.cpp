////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ParameterIntegerVector.cpp
///
/// \brief	Implements the parameter integer vector class. 
///
/// \author	Christian Knobloch
/// \date	24.12.2011
///
/// This file is part of the Sally Project
/// 
/// Copyright(c) 2008-2011 Sally Project
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

#include "ParameterIntegerVector.h"

using namespace SallyAPI::GUI::SendMessage;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CParameterIntegerVector::CParameterIntegerVector(std::vector<int> changeList)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	24.12.2011
///
/// \param	changeList	List of changes. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CParameterIntegerVector::CParameterIntegerVector(std::vector<int> changeList)
{
	m_vChangeList = changeList;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CParameterIntegerVector::~CParameterIntegerVector()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	24.12.2011
////////////////////////////////////////////////////////////////////////////////////////////////////

CParameterIntegerVector::~CParameterIntegerVector()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::vector<int> CParameterIntegerVector::GetChangeList()
///
/// \brief	Gets the vector with all changes which where made. 
///
/// \author	Christian Knobloch
/// \date	24.12.2011
///
/// \return	The change list. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<int> CParameterIntegerVector::GetChangeList()
{
	return m_vChangeList;
}