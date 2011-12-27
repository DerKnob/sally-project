////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\FirstStartWizard.h
///
/// \brief	Declares the first start wizard class. 
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
#include "SallyWizardPanelWelcome.h"
#include "SallyWizardPanelMenu.h"
#include "SallyWizardPanelFacebook.h"
#include "SaveThread.h"

class CFirstStartWizard
	: public SallyAPI::GUI::CPopUpWindow
{
private:
	std::vector<SallyAPI::GUI::CWizardPanel*>	m_vWizardPanelsAll;
	std::vector<SallyAPI::GUI::CWizardPanel*>	m_vWizardPanelsNew;
	std::vector<SallyAPI::GUI::CWizardPanel*>*	m_vWizardPanelsCurrent;
	SallyAPI::GUI::CButton*						m_pButtonNext;
	SallyAPI::GUI::CButton*						m_pButtonPrevious;
	int											m_iCurrentPanel;
	CSaveThread									m_tSaveThread;
	bool										m_bAll;
	
	CSallyWizardPanelWelcome*	m_pPanelWelcome;
	CSallyWizardPanelMenu*		m_pPanelMenu;
	CSallyWizardPanelFacebook*	m_pPanelFacebook;

	void OnCommandBlended(SallyAPI::GUI::CGUIBaseObject* reporter);
	void OnCommandNextPanel();
	void OnCommandPreviousPanel();
	void OnCommandSaveDone();
public:
	CFirstStartWizard(SallyAPI::GUI::CGUIBaseObject* parent);
	virtual ~CFirstStartWizard();

	void AddWizardPanel(SallyAPI::GUI::CApplicationWindow* applicationWindow, SallyAPI::GUI::CWizardPanel* wizardPanel);

	virtual void SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);

	bool HasNewApps();
	void SetNewPanelsStart();
	void SetAllPanelsStart();
};