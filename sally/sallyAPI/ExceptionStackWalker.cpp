////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ExceptionStackWalker.cpp
///
/// \brief	Implements the exception stack walker class. 
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

#include "ExceptionStackWalker.h"

using namespace SallyAPI::Exception;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CExceptionStackWalker::CExceptionStackWalker()
///
/// \brief	Default constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CExceptionStackWalker::CExceptionStackWalker() : StackWalker()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CExceptionStackWalker::CExceptionStackWalker(DWORD dwProcessId, HANDLE hProcess)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	dwProcessId	Identifier for the dw process. 
/// \param	hProcess	Handle of the process. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CExceptionStackWalker::CExceptionStackWalker(DWORD dwProcessId, HANDLE hProcess) 
: StackWalker(dwProcessId, hProcess)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CExceptionStackWalker::OnOutput(LPCSTR szText)
///
/// \brief	Executes the output action. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	szText	The size text. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CExceptionStackWalker::OnOutput(LPCSTR szText)
{
	m_strOutput.empty();
	m_strOutput.append(szText);
	StackWalker::OnOutput(szText);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CExceptionStackWalker::GetStack()
///
/// \brief	Gets the stack. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The stack. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CExceptionStackWalker::GetStack()
{
	return m_strOutput;
}