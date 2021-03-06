////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\SaveLoadThread.h
///
/// \brief	Declares the save load thread class. 
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

class CSaveLoadThread
	: public SallyAPI::System::CThread
{
private:
	SallyAPI::GUI::CConfigPanel*		m_pConfigPanel;
	SallyAPI::GUI::CApplicationWindow*	m_pAppWindow;
	int									m_iMessage;
	bool								m_bSave;

	virtual void RunEx();
public:
	CSaveLoadThread();
	virtual ~CSaveLoadThread();

	void SetValues(SallyAPI::GUI::CConfigPanel*	configPanel, SallyAPI::GUI::CApplicationWindow* appWindow, int message, bool save);
};
