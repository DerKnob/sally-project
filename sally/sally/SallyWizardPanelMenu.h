////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\SallyWizardPanelMenu.h
///
/// \brief	Declares the sally wizard panel menu class. 
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
#include "SystemTime.h"

class CSallyWizardPanelMenu :
	public SallyAPI::GUI::CWizardPanel
{
private:
	SallyAPI::GUI::CLabelBox*	m_pLabelBox1;
	SallyAPI::GUI::CLabelBox*	m_pLabelBox2;
	SallyAPI::GUI::CLabelBox*	m_pLabelBox10;
	SallyAPI::GUI::CLabelBox*	m_pLabelBox11;
	SallyAPI::GUI::CLabelBox*	m_pLabelBox12;
	SallyAPI::GUI::CLabelBox*	m_pLabelBox13;
	SallyAPI::GUI::CLabelBox*	m_pLabelBox14;
	
	SallyAPI::GUI::CImageBox*	m_pScreensaver;
	SallyAPI::GUI::CImageBox*	m_pSettings;
	SallyAPI::GUI::CImageBox*	m_pShutdown;
	SallyAPI::GUI::CImageBox*	m_pMinimize;
	SallyAPI::GUI::CImageBox*	m_pLock;
	
	SallyAPI::GUI::CImageBox*	m_pBackgroundMenu;
	SallyAPI::GUI::CLabel*		m_pClock;
	SallyAPI::GUI::CLabel*		m_pDate;
	CSystemTime					m_SystemTime;
	SallyAPI::GUI::CImageBox*	m_pVolumeControl;
	SallyAPI::GUI::CImageBox*	m_pApplicationsButton;

	virtual void	RenderControl();
	void			UpdateClock();
public:
	CSallyWizardPanelMenu(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, const std::string& pluginPath);
	virtual ~CSallyWizardPanelMenu();

	virtual void SaveConfig();
};