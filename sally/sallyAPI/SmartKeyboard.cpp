////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\SmartKeyboard.cpp
///
/// \brief	Implements the smart keyboard class. 
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

#include "SmartKeyboard.h"

using namespace SallyAPI::Config;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CSmartKeyboard::CSmartKeyboard(const std::string& keyboardLayout)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	keyboardLayout	The keyboard layout. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CSmartKeyboard::CSmartKeyboard(const std::string& keyboardLayout)
	:m_strKeyboardLayout(keyboardLayout)
{
	LoadSmartKeyboard();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CSmartKeyboard::~CSmartKeyboard()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CSmartKeyboard::~CSmartKeyboard()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CSmartKeyboard::LoadSmartKeyboard()
///
/// \brief	Loads the smart keyboard. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CSmartKeyboard::LoadSmartKeyboard()
{
	std::string defaultFilename = SallyAPI::System::SystemHelper::GetModulePath();

	defaultFilename.append("keyboards\\");
	defaultFilename.append("default.smart");

	std::vector<std::string> defaultFilelist = SallyAPI::File::FileHelper::ReadFileToVector(defaultFilename);

	std::string filename = SallyAPI::System::SystemHelper::GetModulePath();

	filename.append("keyboards\\");
	filename.append(m_strKeyboardLayout);
	filename.append("\\sally-project.smart");

	m_vWordList = SallyAPI::File::FileHelper::ReadFileToVector(filename, &defaultFilelist);
	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::vector<std::string>& CSmartKeyboard::GetSmartKeyboard()
///
/// \brief	Gets the smart keyboard. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The smart keyboard. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<std::string>& CSmartKeyboard::GetSmartKeyboard()
{
	return m_vWordList;
}