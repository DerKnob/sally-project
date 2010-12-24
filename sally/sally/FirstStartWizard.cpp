////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\FirstStartWizard.cpp
///
/// \brief	Implements the first start wizard class. 
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

#include "FirstStartWizard.h"

#define GUI_NEXT		GUI_APP_PLUGIN_ID + 1
#define GUI_PREVIOUS	GUI_APP_PLUGIN_ID + 2
#define SAVE_LOAD_DONE	8001

CFirstStartWizard::CFirstStartWizard(SallyAPI::GUI::CGUIBaseObject* parent)
	:SallyAPI::GUI::CPopUpWindow(parent, 0, ""), m_iCurrentPanel(0)
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance(); 
	SallyAPI::System::COption* option = config->GetOption();

	m_pButtonClose->Visible(false);

	m_pPanelWelcome = new CSallyWizardPanelWelcome(this, 0, "");
	this->AddChild(m_pPanelWelcome);

	m_pPanelMenu = new CSallyWizardPanelMenu(this, 0, "");
	this->AddChild(m_pPanelMenu);

	m_pPanelFacebook = new CSallyWizardPanelFacebook(this, 0, "");
	this->AddChild(m_pPanelFacebook);

	if (option->GetPropertyBool("sally", "firstStart", true))
	{
		m_vWizardPanelsNew.push_back(m_pPanelWelcome);
		m_vWizardPanelsNew.push_back(m_pPanelMenu);
		m_vWizardPanelsNew.push_back(m_pPanelFacebook);
	}
	m_vWizardPanelsAll.push_back(m_pPanelWelcome);
	m_vWizardPanelsAll.push_back(m_pPanelMenu);
	m_vWizardPanelsAll.push_back(m_pPanelFacebook);

	m_pButtonPrevious = new SallyAPI::GUI::CButton(this,
		(WINDOW_WIDTH - WIZARD_PANEL_WIDTH) / 2,
		(WINDOW_HEIGHT - WIZARD_PANEL_HEIGHT) / 2 + WIZARD_PANEL_HEIGHT + 10 - 20,
		150, CONTROL_HEIGHT, GUI_PREVIOUS);
	m_pButtonPrevious->SetText("Previous");
	m_pButtonPrevious->SetImageId(GUI_THEME_SALLY_ICON_PREVIOUS);
	m_pButtonPrevious->Enable(false);
	this->AddChild(m_pButtonPrevious);

	m_pButtonNext = new SallyAPI::GUI::CButton(this,
		(WINDOW_WIDTH - WIZARD_PANEL_WIDTH) / 2 + WIZARD_PANEL_WIDTH - 150,
		(WINDOW_HEIGHT - WIZARD_PANEL_HEIGHT) / 2 + WIZARD_PANEL_HEIGHT + 10 - 20,
		150, CONTROL_HEIGHT, GUI_NEXT);
	m_pButtonNext->SetText("Next");
	m_pButtonNext->SetImageId(GUI_THEME_SALLY_ICON_NEXT);
	m_pButtonNext->SetImageRight();
	m_pButtonNext->SetAlign(DT_RIGHT | DT_VCENTER);
	this->AddChild(m_pButtonNext);
}

CFirstStartWizard::~CFirstStartWizard()
{
}

void CFirstStartWizard::AddWizardPanel(SallyAPI::GUI::CApplicationWindow* applicationWindow, SallyAPI::GUI::CWizardPanel* wizardPanel)
{
	SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();

	std::string info;
	info.append("Wizard for '");
	info.append(applicationWindow->GetAppName());
	info.append("' (explicit AppName: '");
	info.append(applicationWindow->GetExplicitAppName());
	info.append("')");
	logger->Info(info);
	
	wizardPanel->SetParent(this);

	if (wizardPanel->IsFirstStart())
		m_vWizardPanelsNew.push_back(wizardPanel);
	m_vWizardPanelsAll.push_back(wizardPanel);

	wizardPanel->Visible(false);
	this->AddChild(wizardPanel);
}

void CFirstStartWizard::SetNewPanelsStart()
{
	m_bAll = false;
	m_iCurrentPanel = 0;

	// hide all
	for (int i = 0; i < (int) m_vWizardPanelsAll.size(); ++i)
	{
		m_vWizardPanelsAll[i]->Visible(false);
	}

	if (m_vWizardPanelsNew.size() > 0)
	{
		m_vWizardPanelsNew[0]->Visible(true);
		m_vWizardPanelsNew[0]->LoadConfig();
	}

	m_vWizardPanelsCurrent = &m_vWizardPanelsNew;
}

void CFirstStartWizard::SetAllPanelsStart()
{
	m_bAll = true;
	m_iCurrentPanel = 0;

	// hide all
	for (int i = 0; i < (int) m_vWizardPanelsAll.size(); ++i)
	{
		m_vWizardPanelsAll[i]->Visible(false);
	}

	if (m_vWizardPanelsAll.size() > 0)
	{
		m_vWizardPanelsAll[0]->Visible(true);
		m_vWizardPanelsAll[0]->LoadConfig();
	}

	m_vWizardPanelsCurrent = &m_vWizardPanelsAll;
}

bool CFirstStartWizard::HasNewApps()
{
	if (m_vWizardPanelsNew.size() > 0)
		return true;
	return false;
}

void CFirstStartWizard::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_CONTROL_BLENDED:
		OnCommandBlended(reporter);
		break;
	case GUI_BUTTON_CLICKED:
		switch (reporterId)
		{
		case GUI_NEXT:
			OnCommandNextPanel();
			return;
		case GUI_PREVIOUS:
			OnCommandPreviousPanel();
			return;
		}
		break;
	case SAVE_LOAD_DONE:
		OnCommandSaveDone();
		return;
	}
	SallyAPI::GUI::CPopUpWindow::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

void CFirstStartWizard::OnCommandBlended(SallyAPI::GUI::CGUIBaseObject* reporter)
{
	SallyAPI::GUI::CWizardPanel* wizard = dynamic_cast<SallyAPI::GUI::CWizardPanel*> (reporter);

	if (wizard == NULL)
		return;

	if (wizard->GetAlphaBlending() == 0)
	{
		wizard->Visible(false);

		(*m_vWizardPanelsCurrent)[m_iCurrentPanel]->SetAlphaBlending(0);
		(*m_vWizardPanelsCurrent)[m_iCurrentPanel]->BlendAnimated(255, 800);
		(*m_vWizardPanelsCurrent)[m_iCurrentPanel]->Visible(true);
	}
}

void CFirstStartWizard::OnCommandNextPanel()
{
	m_pParent->SendMessageToParent(this, GetGraphicId(), MS_SALLY_SHOW_WORKING);

	m_tSaveThread.WaitForStop();

	m_tSaveThread.SetValues((*m_vWizardPanelsCurrent)[m_iCurrentPanel], this, SAVE_LOAD_DONE);
	m_tSaveThread.Start();
}

void CFirstStartWizard::OnCommandSaveDone()
{
	m_pParent->SendMessageToParent(this, GetGraphicId(), MS_SALLY_HIDE_WORKING);

	// last panel
	if (m_iCurrentPanel + 1 == (*m_vWizardPanelsCurrent).size())
	{
		SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance(); 
		SallyAPI::System::COption* option = config->GetOption();
		option->SetPropertyBool("sally", "firstStart", false);

		m_pParent->SendMessageToParent(this, 0, MS_SALLY_HIDE_POPUP_VIEW);
		if (!m_bAll)
			m_pParent->SendMessageToParent(this, 0, MS_SALLY_SHOW_APPLICATION_SELECTOR);
		return;
	}

	// hide the old one
	(*m_vWizardPanelsCurrent)[m_iCurrentPanel]->BlendAnimated(0, 800);

	// go to the next panel
	++m_iCurrentPanel;

	(*m_vWizardPanelsCurrent)[m_iCurrentPanel]->LoadConfig();

	m_pButtonPrevious->Enable(true);
}

void CFirstStartWizard::OnCommandPreviousPanel()
{
	// last panel
	if (m_iCurrentPanel - 1 == -1)
		return;

	// hide the old one
	(*m_vWizardPanelsCurrent)[m_iCurrentPanel]->BlendAnimated(0, 800);

	// go to the next panel
	--m_iCurrentPanel;

	if (m_iCurrentPanel == 0)
		m_pButtonPrevious->Enable(false);
}