////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ParameterKeyboardRequestWords.cpp
///
/// \brief	Implements the parameter keyboard request words class. 
///
/// \author	Christian Knobloch
/// \date	08.07.2010
///
/// This file is part of the Sally Project
/// 
/// Copyright(c) 2008-2011 Sally Project
/// http://www.sally-project.org/
///
/// This program is free software: you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
/// 
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
/// GNU General Public License for more details.
/// 
/// You should have received a copy of the GNU General Public License
/// along with this program. If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "ParameterKeyboardRequestWords.h"

using namespace SallyAPI::GUI::SendMessage;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CParameterKeyboardRequestWords::CParameterKeyboardRequestWords(const std::string& searchWord)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	08.07.2011
///
/// \param	searchWord	The search word. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CParameterKeyboardRequestWords::CParameterKeyboardRequestWords(const std::string& searchWord)
{
	m_strSearchWord = searchWord;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CParameterKeyboardRequestWords::~CParameterKeyboardRequestWords()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	08.07.2011
////////////////////////////////////////////////////////////////////////////////////////////////////

CParameterKeyboardRequestWords::~CParameterKeyboardRequestWords()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CParameterKeyboardRequestWords::GetSearchWord()
///
/// \brief	Gets the search word. 
///
/// \author	Christian Knobloch
/// \date	08.07.2011
///
/// \return	The search word. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CParameterKeyboardRequestWords::GetSearchWord()
{
	return m_strSearchWord;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CParameterKeyboardRequestWords::SetResult(const std::vector<std::string>& result)
///
/// \brief	Sets a result. 
///
/// \author	Christian Knobloch
/// \date	08.07.2011
///
/// \param	result	The result. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CParameterKeyboardRequestWords::SetResult(const std::vector<std::string>& result)
{
	m_vResult = result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::vector<std::string> CParameterKeyboardRequestWords::GetResult()
///
/// \brief	Gets the result. 
///
/// \author	Christian Knobloch
/// \date	08.07.2011
///
/// \return	The result. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<std::string> CParameterKeyboardRequestWords::GetResult()
{
	return m_vResult;
}