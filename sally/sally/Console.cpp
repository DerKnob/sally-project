////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\Console.cpp
///
/// \brief	Implements the console class. 
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

#include "Console.h"

CConsole::CConsole(void)
{
	m_pFrames = new SallyAPI::GUI::CLabel(this, 10, 10, 60);
	m_pFrames->SetLocalised(false);
	this->AddChild(m_pFrames);

	m_pDrawCount = new SallyAPI::GUI::CLabel(this, 10, 50, 60);
	m_pDrawCount->SetLocalised(false);
	this->AddChild(m_pDrawCount);
}

CConsole::~CConsole(void)
{
}

void CConsole::SetFrames(DWORD framesPerSecond)
{
	std::string info = SallyAPI::String::StringHelper::ConvertToString((int) framesPerSecond);
	info.append("fps");
	m_pFrames->SetText(info);
}

void CConsole::SetDrawCount(int drawCount)
{
	std::string info = SallyAPI::String::StringHelper::ConvertToString((int) drawCount);
	m_pDrawCount->SetText(info);
}