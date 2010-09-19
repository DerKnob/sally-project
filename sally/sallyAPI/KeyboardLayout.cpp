////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\KeyboardLayout.cpp
///
/// \brief	Implements the keyboard layout class. 
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

#include "KeyboardLayout.h"

using namespace SallyAPI::Config;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CKeyboardLayout::CKeyboardLayout(const std::string& keyboardLayout)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	keyboardLayout	The keyboard layout. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CKeyboardLayout::CKeyboardLayout(const std::string& keyboardLayout)
	:m_strKeyboardLayout(keyboardLayout)
{
	LoadLayout();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CKeyboardLayout::~CKeyboardLayout()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CKeyboardLayout::~CKeyboardLayout()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CKeyboardLayout::LoadLayout()
///
/// \brief	Loads the layout. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CKeyboardLayout::LoadLayout()
{
	std::string filename = SallyAPI::System::SystemHelper::GetModulePath();

	filename.append("keyboards\\");
	filename.append(m_strKeyboardLayout);
	filename.append("\\sally-project.layout");

	m_vLayout = SallyAPI::File::FileHelper::ReadFileToVector(filename);
	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::vector<std::string> CKeyboardLayout::GetKeyboardLayout()
///
/// \brief	Gets the keyboard layout. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The keyboard layout. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<std::string> CKeyboardLayout::GetKeyboardLayout()
{
	return m_vLayout;
}