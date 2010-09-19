////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\SaveThread.h
///
/// \brief	Declares the save thread class. 
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

class CSaveThread
	: public SallyAPI::System::CThread
{
private:
	SallyAPI::GUI::CWizardPanel*	m_pWizardPanel;
	SallyAPI::GUI::CControl*		m_pParentControl;
	int								m_iMessage;

	virtual void RunEx();
public:
	CSaveThread();
	virtual ~CSaveThread();

	void SetValues(SallyAPI::GUI::CWizardPanel*	wizardPanel, SallyAPI::GUI::CControl* appWindow, int message);
};
