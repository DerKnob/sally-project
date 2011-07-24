////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\SallyWizardPanelMenu.cpp
///
/// \brief	Implements the sally wizard panel menu class. 
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

#include "SallyWizardPanelMenu.h"

CSallyWizardPanelMenu::CSallyWizardPanelMenu(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, const std::string& pluginPath)
	:SallyAPI::GUI::CWizardPanel(parent, graphicId, pluginPath)
{
	m_pImageLogo->SetImageId(GUI_THEME_SALLY_LOGO_BIG);

	m_pLabelHeader->SetText("The Menu");

	m_pLabelBox1 = new SallyAPI::GUI::CLabelBox(this, 20, 50 + CONTROL_HEIGHT, WIZARD_PANEL_WIDTH - 50 - MENU_WIDTH, 100);
	m_pLabelBox1->SetText("Top right you will see the menu.\n" \
		"It shows you the current time, date and you can change the system volume or access the application selector menu by clicking on the current application icon. " \
		"If you click on the clock you can configure alarm clocks, start a timer or use a stop watch.");
	m_pLabelBox1->SetLocalised(true);
	this->AddChild(m_pLabelBox1);

	m_pLabelBox2 = new SallyAPI::GUI::CLabelBox(this, 20, 50 + CONTROL_HEIGHT + 100 + 20, WIZARD_PANEL_WIDTH - 40, 150);
	m_pLabelBox2->SetText("To open the application selector click on the image of the current selected application. In this example you see the image of the Sally settings dialog.\n\n" \
		"In the application selector menu you will see all loaded application direct in the middle. At the bottom you will have three more options:");
	m_pLabelBox2->SetLocalised(true);
	this->AddChild(m_pLabelBox2);

	m_pScreensaver = new SallyAPI::GUI::CImageBox(this, 20, 320, 48, 48);
	m_pScreensaver->SetImageId(GUI_THEME_SALLY_SETTINGS);
	this->AddChild(m_pScreensaver);

	m_pLabelBox10 = new SallyAPI::GUI::CLabelBox(this, 70, 320, (WIZARD_PANEL_WIDTH / 2) - 70, 48);
	m_pLabelBox10->SetText("to the settings dialog");
	m_pLabelBox10->SetAlign(DT_LEFT | DT_VCENTER | DT_WORDBREAK);
	m_pLabelBox10->SetLocalised(true);
	this->AddChild(m_pLabelBox10);

	m_pScreensaver = new SallyAPI::GUI::CImageBox(this, 20, 380, 48, 48);
	m_pScreensaver->SetImageId(GUI_THEME_SALLY_SCREENSAVER);
	this->AddChild(m_pScreensaver);

	m_pLabelBox11 = new SallyAPI::GUI::CLabelBox(this, 70, 380, (WIZARD_PANEL_WIDTH / 2) - 70, 48);
	m_pLabelBox11->SetText("start screensaver");
	m_pLabelBox11->SetAlign(DT_LEFT | DT_VCENTER | DT_WORDBREAK);
	m_pLabelBox11->SetLocalised(true);
	this->AddChild(m_pLabelBox11);

	m_pShutdown = new SallyAPI::GUI::CImageBox(this, 20, 440, 48, 48);
	m_pShutdown->SetImageId(GUI_THEME_SALLY_LOGOUT);
	this->AddChild(m_pShutdown);

	m_pLabelBox12 = new SallyAPI::GUI::CLabelBox(this, 70, 440, (WIZARD_PANEL_WIDTH / 2) - 70, 48);
	m_pLabelBox12->SetText("shutdown Sally");
	m_pLabelBox12->SetAlign(DT_LEFT | DT_VCENTER | DT_WORDBREAK);
	m_pLabelBox12->SetLocalised(true);
	this->AddChild(m_pLabelBox12);

	m_pShutdown = new SallyAPI::GUI::CImageBox(this, WIZARD_PANEL_WIDTH / 2 + 20, 320, 48, 48);
	m_pShutdown->SetImageId(GUI_THEME_SALLY_MINIMIZE);
	this->AddChild(m_pShutdown);

	m_pLabelBox13 = new SallyAPI::GUI::CLabelBox(this, WIZARD_PANEL_WIDTH / 2 + 70, 320, (WIZARD_PANEL_WIDTH / 2) - 70, 48);
	m_pLabelBox13->SetText("show the Windows Desktop");
	m_pLabelBox13->SetAlign(DT_LEFT | DT_VCENTER | DT_WORDBREAK);
	m_pLabelBox13->SetLocalised(true);
	this->AddChild(m_pLabelBox13);

	m_pLock = new SallyAPI::GUI::CImageBox(this, WIZARD_PANEL_WIDTH / 2 + 20, 380, 48, 48);
	m_pLock->SetImageId(GUI_THEME_SALLY_LOCK);
	this->AddChild(m_pLock);

	m_pLabelBox14 = new SallyAPI::GUI::CLabelBox(this, WIZARD_PANEL_WIDTH / 2 + 70, 380, (WIZARD_PANEL_WIDTH / 2) - 70, 48);
	m_pLabelBox14->SetText("lock the screen - you can set a password in the settings");
	m_pLabelBox14->SetAlign(DT_LEFT | DT_VCENTER | DT_WORDBREAK);
	m_pLabelBox14->SetLocalised(true);
	this->AddChild(m_pLabelBox14);

	/************************************************************************/
	/* create the menu                                                      */
	/************************************************************************/
	m_pBackgroundMenu = new SallyAPI::GUI::CImageBox(this, WIZARD_PANEL_WIDTH - 220, 50 + CONTROL_HEIGHT, MENU_WIDTH, MENU_HEIGHT);
	m_pBackgroundMenu->SetImageId(GUI_THEME_SALLY_MENU);
	this->AddChild(m_pBackgroundMenu);

	m_pClock = new SallyAPI::GUI::CLabel(m_pBackgroundMenu, 16, 24, 174);
	m_pClock->SetLocalised(false);
	m_pClock->SetAlign(DT_RIGHT | DT_VCENTER);
	m_pClock->SetFont("menu.font.big");
	this->AddChild(m_pClock);

	m_pDate = new SallyAPI::GUI::CLabel(m_pBackgroundMenu, 16, 5, 174);
	m_pDate->SetLocalised(false);
	m_pDate->SetAlign(DT_RIGHT | DT_VCENTER);
	m_pDate->SetFont("menu.font.small");
	this->AddChild(m_pDate);

	m_pApplicationsButton = new SallyAPI::GUI::CImageBox(m_pBackgroundMenu, 15, 6, 48, 48);
	m_pApplicationsButton->SetImageId(GUI_THEME_SALLY_SETTINGS);
	this->AddChild(m_pApplicationsButton);

	m_pVolumeControl = new SallyAPI::GUI::CImageBox(m_pBackgroundMenu, 95, 27, 22, 22);
	m_pVolumeControl->SetImageId(GUI_THEME_SALLY_AUDIO_HIGH);
	this->AddChild(m_pVolumeControl);
}

CSallyWizardPanelMenu::~CSallyWizardPanelMenu()
{
}

void CSallyWizardPanelMenu::RenderControl()
{
	UpdateClock();
	SallyAPI::GUI::CWizardPanel::RenderControl();
}

void CSallyWizardPanelMenu::UpdateClock()
{
	m_SystemTime.Update();

	m_pClock->SetText(m_SystemTime.GetTime());
	m_pDate->SetText(m_SystemTime.GetDate());
}

void CSallyWizardPanelMenu::SaveConfig()
{
}