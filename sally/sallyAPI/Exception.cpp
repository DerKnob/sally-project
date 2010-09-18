////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Exception.cpp
///
/// \brief	Implements the exception class. 
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

#include "Exception.h"

using namespace SallyAPI::Exception;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CException::CException(const std::string& message)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	message	The message. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CException::CException(const std::string& message)
	:m_strMessage(message)
{
	m_ESW.ShowCallstack();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CException::~CException()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CException::~CException()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CException::GetCause()
///
/// \brief	Gets the cause. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The cause. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CException::GetCause()
{
	return m_strMessage;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CException::GetStacktrace()
///
/// \brief	Gets the stacktrace. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The stacktrace. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CException::GetStacktrace()
{
	return m_ESW.GetStack();
}
