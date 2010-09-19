////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\KeyboardBrain.h
///
/// \brief	Declares the keyboard brain class. 
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

#pragma once
#include "Define.h"
#include <sallyAPI\sallyAPI.h>

class CKeyboardBrain :
	public SallyAPI::System::CThread
{
private:
	std::string m_strSentence;
	std::string m_strMediaDirectory;

	void AddSentenceToDatabase(std::string& sentence);
	void AddWordToDatabase(std::string& word);

	virtual void	RunEx();
public:
	CKeyboardBrain();
	virtual ~CKeyboardBrain();

	void SetValues(const std::string& sentence);
};
