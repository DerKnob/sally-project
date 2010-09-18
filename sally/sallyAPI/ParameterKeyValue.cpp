////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ParameterKeyValue.cpp
///
/// \brief	Implements the parameter key value class. 
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

#include "ParameterKeyValue.h"

using namespace SallyAPI::GUI::SendMessage;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CParameterKeyValue::CParameterKeyValue(const std::string& key,
/// const std::string& value)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	key		The key. 
/// \param	value	The value. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CParameterKeyValue::CParameterKeyValue(const std::string& key, const std::string& value)
	:m_strValue(value), m_strKey(key)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CParameterKeyValue::~CParameterKeyValue()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CParameterKeyValue::~CParameterKeyValue()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CParameterKeyValue::GetValue()
///
/// \brief	Gets the value. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The value. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CParameterKeyValue::GetValue()
{
	return m_strValue;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CParameterKeyValue::GetKey()
///
/// \brief	Gets the key. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The key. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CParameterKeyValue::GetKey()
{
	return m_strKey;
}