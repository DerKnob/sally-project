////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ParameterInteger.cpp
///
/// \brief	Implements the parameter integer class. 
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

#include "ParameterInteger.h"

using namespace SallyAPI::GUI::SendMessage;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CParameterInteger::CParameterInteger(int value)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	value	The value. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CParameterInteger::CParameterInteger(int value)
	:m_iValue(value)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CParameterInteger::~CParameterInteger()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CParameterInteger::~CParameterInteger()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CParameterInteger::GetInteger()
///
/// \brief	Gets the integer. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The integer. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CParameterInteger::GetInteger()
{
	return m_iValue;
}
