////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\SallyConfigPanel.cpp
///
/// \brief	Implements the sally configuration panel class. 
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

#include "SallyConfigPanel.h"

#define GUI_FIRSTSTART_WIZARD				50000
#define GUI_SHOW_FACEBOOK_CONFIG			50001
#define GUI_USE_INTERNET_EXPLORER_SETTINGS	50002
#define GUI_VOLUME_WINDOWS					50003
#define GUI_VOLUME_APP						50004
#define GUI_UPDATE_CHECKED					50005
#define GUI_LEFT_RIGH_1						50006
#define GUI_LEFT_RIGH_2						50007
#define GUI_CHECK_FOR_UPDATE				50008
#define GUI_GET_UPDATE						50009

CSallyConfigPanel::CSallyConfigPanel(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, const std::string& pluginPath)
	:SallyAPI::GUI::CConfigPanel(parent, graphicId, pluginPath)
{
	m_pTabControl = new SallyAPI::GUI::CTabcontrol(this, WINDOW_BORDER_H, WINDOW_BORDER_V,
		this->GetWidth() - (2 * WINDOW_BORDER_H), this->GetHeight() - (2 * WINDOW_BORDER_V));
	this->AddChild(m_pTabControl);

	int height = m_pTabControl->GetFormHeight();
	int width = m_pTabControl->GetFormWidth();

	m_pTabInfo = new SallyAPI::GUI::CTabcontrolItem(m_pTabControl, "Info", GUI_THEME_SALLY_ICON_INFO);
	m_pTabControl->AddTabItem(m_pTabInfo);

	m_pTabProperties_1 = new SallyAPI::GUI::CTabcontrolItem(m_pTabControl, "Properties", GUI_THEME_SALLY_ICON_PROPERTIES);
	m_pTabControl->AddTabItem(m_pTabProperties_1);

	m_pTabScreensaver = new SallyAPI::GUI::CTabcontrolItem(m_pTabControl, "Screensaver", GUI_THEME_SALLY_SCREENSAVER_SMALL);
	m_pTabControl->AddTabItem(m_pTabScreensaver);

	m_pTabFacebook = new SallyAPI::GUI::CTabcontrolItem(m_pTabControl, "Facebook", GUI_THEME_SALLY_ICON_FACEBOOK);
	m_pTabControl->AddTabItem(m_pTabFacebook);

	m_pTabNetwork = new SallyAPI::GUI::CTabcontrolItem(m_pTabControl, "Network", GUI_THEME_SALLY_ICON_NETWORK);
	m_pTabControl->AddTabItem(m_pTabNetwork);

	m_pTabLock = new SallyAPI::GUI::CTabcontrolItem(m_pTabControl, "Screen Lock", GUI_THEME_SALLY_ICON_SYSTEM_LOCK);
	m_pTabControl->AddTabItem(m_pTabLock);

	m_pTabUpdate = new SallyAPI::GUI::CTabcontrolItem(m_pTabControl, "Update", GUI_THEME_SALLY_ICON_TIMER);
	m_pTabControl->AddTabItem(m_pTabUpdate);

	m_pTabAbout = new SallyAPI::GUI::CTabcontrolItem(m_pTabControl, "About", GUI_THEME_SALLY_ICON_ABOUT);
	m_pTabControl->AddTabItem(m_pTabAbout);

	//////////////////////////////////////////////////////////////////////////

	// Info Tab
	for (int i = 0; i < 6; ++i)
	{
		m_pTabInfoTextDescription[i] = new SallyAPI::GUI::CLabel(m_pTabInfo->GetForm(), WINDOW_BORDER_H, (CONTROL_HEIGHT + 10) * i + WINDOW_BORDER_V, 180);
		m_pTabInfo->GetForm()->AddChild(m_pTabInfoTextDescription[i]);
	}
	int positonY = 0;
	for (; positonY < 6; ++positonY)
	{
		m_pTabInfoText[positonY] = new SallyAPI::GUI::CLabel(m_pTabInfo->GetForm(), WINDOW_BORDER_H + 190, (CONTROL_HEIGHT + 10) * positonY  + WINDOW_BORDER_V, 500);
		m_pTabInfoText[positonY]->SetLocalised(false);
		m_pTabInfoText[positonY]->SetBold(true);
		m_pTabInfo->GetForm()->AddChild(m_pTabInfoText[positonY]);
	}
	m_pTabInfoTextDescription[0]->SetText("Sally Version:");
	m_pTabInfoTextDescription[1]->SetText("Release:");
	m_pTabInfoTextDescription[2]->SetText("Sally API Version:");
	m_pTabInfoTextDescription[3]->SetText("PC Name:");
	m_pTabInfoTextDescription[4]->SetText("IP Address:");
	m_pTabInfoTextDescription[5]->SetText("Plugins Loaded:");

	m_pLoadedPlugins = new SallyAPI::GUI::CEditBox(m_pTabInfo->GetForm(),
		WINDOW_BORDER_H, 
		(CONTROL_HEIGHT + 10) * positonY + WINDOW_BORDER_V,
		m_pTabInfo->GetForm()->GetWidth() - (WINDOW_BORDER_H * 2),
		m_pTabInfo->GetForm()->GetHeight() - WINDOW_BORDER_V - ((CONTROL_HEIGHT + 10) * positonY + WINDOW_BORDER_V));
	m_pTabInfo->GetForm()->AddChild(m_pLoadedPlugins);

	// About Tab
	m_pAbout = new SallyAPI::GUI::CEditBox(m_pTabAbout->GetForm(),
		WINDOW_BORDER_H, WINDOW_BORDER_V,
		m_pTabAbout->GetForm()->GetWidth() - (WINDOW_BORDER_H * 2),
		m_pTabAbout->GetForm()->GetHeight() - (WINDOW_BORDER_V * 2));
	m_pTabAbout->GetForm()->AddChild(m_pAbout);

	m_pAbout->SetText("Sally Project is under GPL.\n\n'Clean'-Theme icons are from the Tango Desktop Project Team (http://tango.freedesktop.org/).\nThe icons are licensed under the 'Creative Commons Attribution Share-Alike license' (http://creativecommons.org/licenses/by-sa/2.5/).\n\nThis Application uses libVLC, the SQLite Database, ID3Lib library, Box2D physics engine and XMLParser (from Frank Vanden Berghen).\n\nBig thanks to the VLC team which has done a great work in the past years!\n\nThe Team: Christian Knobloch, Dominik Haselmeier, Felix Eichinger, Hendrik Koch, Rainer Wolf, Yvonne Ködel\n\nThanks to: Benjamin Schwarz, Christian Burger, Eva Höhne, Hannes Knauer, Herbert Wirkner, Markus Weinkauf, Michael Widlok, Oliver Neuner, Ruben Schui, Sarah Lang, Tobias Hornauer, Tobias Knauer");

	// Properties Tab
	m_pTabPropertiesTextDescription[0] = new SallyAPI::GUI::CLabel(m_pTabProperties_1->GetForm(),
		WINDOW_BORDER_H + 190 + 150 + 10, (CONTROL_HEIGHT + 10) *  0 + WINDOW_BORDER_V, 180);
	m_pTabProperties_1->GetForm()->AddChild(m_pTabPropertiesTextDescription[0]);

	m_pTabPropertiesTextDescription[1] = new SallyAPI::GUI::CLabel(m_pTabProperties_1->GetForm(),
		WINDOW_BORDER_H + 190 + 150 + 10, (CONTROL_HEIGHT + 10) *  1 + WINDOW_BORDER_V, 180);
	m_pTabProperties_1->GetForm()->AddChild(m_pTabPropertiesTextDescription[1]);

	for (int i = 2; i < PROPERTY_FIELDS; ++i)
	{
		m_pTabPropertiesTextDescription[i] = new SallyAPI::GUI::CLabel(m_pTabProperties_1->GetForm(),
			WINDOW_BORDER_H, (CONTROL_HEIGHT + 10) *  (i - 2) + WINDOW_BORDER_V, 180);
		m_pTabProperties_1->GetForm()->AddChild(m_pTabPropertiesTextDescription[i]);
	}
	m_pTabPropertiesTextDescription[0]->SetText("Speech Output:");
	m_pTabPropertiesTextDescription[1]->SetText("Speech Input:");
	m_pTabPropertiesTextDescription[2]->SetText("GUI Language:");
	m_pTabPropertiesTextDescription[3]->SetText("Keyboard Layout:");
	m_pTabPropertiesTextDescription[4]->SetText("Theme:");
	m_pTabPropertiesTextDescription[5]->SetText("StartUp Screen:");
	m_pTabPropertiesTextDescription[6]->SetText("Show PopUp Infos:");
	m_pTabPropertiesTextDescription[7]->SetText("Font Antialiasing:");
	m_pTabPropertiesTextDescription[8]->SetText("enable Global Keyhooks:");
	m_pTabPropertiesTextDescription[9]->SetText("Volume Control affects:");

	m_pSpeechInput = new SallyAPI::GUI::CDropDown(m_pTabProperties_1->GetForm(),
		WINDOW_BORDER_H + 190 + 150 + 10 + 190, WINDOW_BORDER_V, 150);
	m_pSpeechInput->Enable(false);
	m_pSpeechInput->SetLocalised(false);
	m_pTabProperties_1->GetForm()->AddChild(m_pSpeechInput);

	m_pSpeechOutput = new SallyAPI::GUI::CDropDown(m_pTabProperties_1->GetForm(),
		WINDOW_BORDER_H + 190 + 150 + 10 + 190, 40 + WINDOW_BORDER_V, 150);
	m_pSpeechOutput->Enable(false);
	m_pSpeechOutput->SetLocalised(false);
	m_pTabProperties_1->GetForm()->AddChild(m_pSpeechOutput);

	m_pGUILanguage = new SallyAPI::GUI::CDropDown(m_pTabProperties_1->GetForm(),
		WINDOW_BORDER_H + 190, WINDOW_BORDER_V, 150);
	m_pGUILanguage->SetLocalised(false);
	m_pTabProperties_1->GetForm()->AddChild(m_pGUILanguage);
	
	m_pKeyboardLayout = new SallyAPI::GUI::CDropDown(m_pTabProperties_1->GetForm(),
		WINDOW_BORDER_H + 190, 40 + WINDOW_BORDER_V, 150);
	m_pKeyboardLayout->SetLocalised(false);
	m_pTabProperties_1->GetForm()->AddChild(m_pKeyboardLayout);

	m_pTheme = new SallyAPI::GUI::CDropDown(m_pTabProperties_1->GetForm(),
		WINDOW_BORDER_H + 190, 80 + WINDOW_BORDER_V, 250);
	m_pTheme->SetLocalised(false);
	m_pTabProperties_1->GetForm()->AddChild(m_pTheme);

	m_pStartUp = new SallyAPI::GUI::CDropDown(m_pTabProperties_1->GetForm(), WINDOW_BORDER_H + 190, 120 + WINDOW_BORDER_V, 250);
	m_pTabProperties_1->GetForm()->AddChild(m_pStartUp);

	m_pShowPopUpInfos = new SallyAPI::GUI::CCheckbox(m_pTabProperties_1->GetForm(),
		WINDOW_BORDER_H + 190, 160 + WINDOW_BORDER_V, 250);
	m_pTabProperties_1->GetForm()->AddChild(m_pShowPopUpInfos);

	m_pFontAntialasing = new SallyAPI::GUI::CCheckbox(m_pTabProperties_1->GetForm(),
		WINDOW_BORDER_H + 190, 200 + WINDOW_BORDER_V, 250);
	m_pTabProperties_1->GetForm()->AddChild(m_pFontAntialasing);

	m_pGlobalKeyHook = new SallyAPI::GUI::CCheckbox(m_pTabProperties_1->GetForm(),
		WINDOW_BORDER_H + 190, 240 + WINDOW_BORDER_V, 250);
	m_pTabProperties_1->GetForm()->AddChild(m_pGlobalKeyHook);

	m_pVolumeWindows = new SallyAPI::GUI::CRadioButton(m_pTabProperties_1->GetForm(),
		WINDOW_BORDER_H + 190, 280 + WINDOW_BORDER_V, 180, GUI_VOLUME_WINDOWS);
	m_pVolumeWindows->SetText("Windows Volume");
	m_pTabProperties_1->GetForm()->AddChild(m_pVolumeWindows);

	m_pVolumeApp = new SallyAPI::GUI::CRadioButton(m_pTabProperties_1->GetForm(),
		WINDOW_BORDER_H + 190 + 180, 280 + WINDOW_BORDER_V, 180, GUI_VOLUME_APP);
	m_pVolumeApp->SetText("Application Volume");
	m_pTabProperties_1->GetForm()->AddChild(m_pVolumeApp);

	m_pFirstStartUpWizard = new SallyAPI::GUI::CButton(m_pTabProperties_1->GetForm(),
		WINDOW_BORDER_H, 320 + WINDOW_BORDER_V, 400, CONTROL_HEIGHT, GUI_FIRSTSTART_WIZARD);
	m_pFirstStartUpWizard->SetText("Start First Startup Wizard again");
	m_pFirstStartUpWizard->SetImageId(GUI_THEME_SALLY_LOGO_SMALL);
	m_pTabProperties_1->GetForm()->AddChild(m_pFirstStartUpWizard);

	// Screensaver Tab
	for (int i = 0; i < 2; ++i)
	{
		m_pTabScreensaverTextDescription[i] = new SallyAPI::GUI::CLabel(m_pTabScreensaver->GetForm(), WINDOW_BORDER_H, (CONTROL_HEIGHT + 10) *  i + WINDOW_BORDER_V, 180);
		m_pTabScreensaver->GetForm()->AddChild(m_pTabScreensaverTextDescription[i]);
	}

	m_pScreensaver = new SallyAPI::GUI::CDropDown(m_pTabScreensaver->GetForm(), WINDOW_BORDER_H + 190, WINDOW_BORDER_V, 250);
	m_pTabScreensaver->GetForm()->AddChild(m_pScreensaver);

	SallyAPI::GUI::CDropDownItem itemScreensaver("", "Disabled", GUI_THEME_SALLY_ICON_CANCEL);
	m_pScreensaver->SetLocalised(false);
	m_pScreensaver->AddItem(itemScreensaver);

	m_pScreensaver->SelectItemByIdentifier("");
	
	m_pTabScreensaverTextDescription[0]->SetText("Screensaver:");
	m_pTabScreensaverTextDescription[1]->SetText("Wait Time:");

	m_pWaitTime = new SallyAPI::GUI::CNumberSelector(m_pTabScreensaver->GetForm(), WINDOW_BORDER_H + 190, 40 + WINDOW_BORDER_V, 100);
	m_pWaitTime->SetMinValue(1);
	m_pWaitTime->SetMaxValue(240);
	m_pTabScreensaver->GetForm()->AddChild(m_pWaitTime);

	m_pWaitTimeLabel = new SallyAPI::GUI::CLabel(m_pTabScreensaver->GetForm(), WINDOW_BORDER_H + 190 + 110, 40 + WINDOW_BORDER_V, 80);
	m_pWaitTimeLabel->SetText("min.");
	m_pTabScreensaver->GetForm()->AddChild(m_pWaitTimeLabel);

	m_pFullscreenLeftRight = new SallyAPI::GUI::CLabelBox(m_pTabScreensaver->GetForm(), 20, WINDOW_BORDER_V + 90, 500, 50);
	m_pFullscreenLeftRight->SetLocalised(true);
	m_pFullscreenLeftRight->SetText("How should the move to left and right behave in screensaver mode?");
	m_pTabScreensaver->GetForm()->AddChild(m_pFullscreenLeftRight);

	m_pFullscreenLeftRight_1 = new SallyAPI::GUI::CRadioButton(m_pTabScreensaver->GetForm(), WINDOW_BORDER_H, WINDOW_BORDER_V + 150, 220, GUI_LEFT_RIGH_1);
	m_pFullscreenLeftRight_1->SetText("move to left is previous");
	m_pTabScreensaver->GetForm()->AddChild(m_pFullscreenLeftRight_1);

	m_pFullscreenLeftRight_2 = new SallyAPI::GUI::CRadioButton(m_pTabScreensaver->GetForm(), WINDOW_BORDER_H + 220, WINDOW_BORDER_V + 150, 220, GUI_LEFT_RIGH_2);
	m_pFullscreenLeftRight_2->SetText("move to left is next");
	m_pTabScreensaver->GetForm()->AddChild(m_pFullscreenLeftRight_2);

	// Facebook
	m_pFacebookLabel = new SallyAPI::GUI::CLabelBox(m_pTabFacebook->GetForm(), 20, WINDOW_BORDER_V, m_pTabFacebook->GetForm()->GetWidth() - 80, 200);
	m_pFacebookLabel->SetText("You can connect Sally to your Facebook profile.\n\nIf 'Facebook Connection' is enabled you can for example use the 'I Like' buttons in the applications.\nAlso the Facebook app can be used and you can for example see what our friends are listening to while using Sally or you can post a status message to your Facebook account.");
	m_pFacebookLabel->SetLocalised(true);
	m_pTabFacebook->GetForm()->AddChild(m_pFacebookLabel);

	m_pShowFacebookConfig = new SallyAPI::GUI::CButton(m_pTabFacebook->GetForm(), 20, 220, 250, CONTROL_HEIGHT, GUI_SHOW_FACEBOOK_CONFIG);
	m_pShowFacebookConfig->SetText("Configure Facebook");
	m_pShowFacebookConfig->SetImageId(GUI_THEME_SALLY_ICON_FACEBOOK);
	m_pTabFacebook->GetForm()->AddChild(m_pShowFacebookConfig);

	m_pShowFacebookPopUpInfos = new SallyAPI::GUI::CCheckbox(m_pTabFacebook->GetForm(), 20, 260, 400);
	m_pShowFacebookPopUpInfos->SetText("Show Facebook PopUp Infos");
	m_pTabFacebook->GetForm()->AddChild(m_pShowFacebookPopUpInfos);

	// Network
	m_pUseInternetExplorerProxy = new SallyAPI::GUI::CCheckbox(m_pTabNetwork->GetForm(), 20, WINDOW_BORDER_V, m_pTabFacebook->GetForm()->GetWidth() - 40, GUI_USE_INTERNET_EXPLORER_SETTINGS);
	m_pUseInternetExplorerProxy->SetText("Use Internet Explorer Settings");
	m_pTabNetwork->GetForm()->AddChild(m_pUseInternetExplorerProxy);

	m_pLabelServer = new SallyAPI::GUI::CLabel(m_pTabNetwork->GetForm(), 20, WINDOW_BORDER_V + 40, 200);
	m_pLabelServer->SetText("Server and Port:");
	m_pTabNetwork->GetForm()->AddChild(m_pLabelServer);

	m_pLabelBypass = new SallyAPI::GUI::CLabel(m_pTabNetwork->GetForm(), 20, WINDOW_BORDER_V + 80, 200);
	m_pLabelBypass->SetText("Bypass Addresses:");
	m_pTabNetwork->GetForm()->AddChild(m_pLabelBypass);

	m_pEditServer = new SallyAPI::GUI::CEdit(m_pTabNetwork->GetForm(), 20 + 220, WINDOW_BORDER_V + 40, m_pTabFacebook->GetForm()->GetWidth() - 260);
	m_pEditServer->SetInfoText("example: server:port");
	m_pTabNetwork->GetForm()->AddChild(m_pEditServer);

	m_pEditBypass = new SallyAPI::GUI::CEdit(m_pTabNetwork->GetForm(), 20 + 220, WINDOW_BORDER_V + 80, m_pTabFacebook->GetForm()->GetWidth() - 260);
	m_pEditBypass->SetInfoText("example: server1;server2;server3");
	m_pTabNetwork->GetForm()->AddChild(m_pEditBypass);

	// lock
	m_pLabelLock = new SallyAPI::GUI::CLabelBox(m_pTabLock->GetForm(), 20, WINDOW_BORDER_V, 500, 50);
	m_pLabelLock->SetLocalised(true);
	m_pLabelLock->SetText("You can configure a PIN to lock Sally.\nYou can lock Sally from the application selector menu.");
	m_pTabLock->GetForm()->AddChild(m_pLabelLock);

	m_pLabelLock1 = new SallyAPI::GUI::CLabel(m_pTabLock->GetForm(), 20, WINDOW_BORDER_V + 60, 150);
	m_pLabelLock1->SetText("PIN:");
	m_pTabLock->GetForm()->AddChild(m_pLabelLock1);

	m_pLabelLock2 = new SallyAPI::GUI::CLabel(m_pTabLock->GetForm(), 20, WINDOW_BORDER_V + 100, 150);
	m_pLabelLock2->SetText("Retype PIN:");
	m_pTabLock->GetForm()->AddChild(m_pLabelLock2);

	m_pEditLock1 = new SallyAPI::GUI::CEdit(m_pTabLock->GetForm(), 20 + 170, WINDOW_BORDER_V + 60, 100);
	m_pEditLock1->SetPassword(true);
	m_pEditLock1->SetMaxCharacterInputLength(6);
	m_pEditLock1->SetNumberOnly(true);
	m_pTabLock->GetForm()->AddChild(m_pEditLock1);

	m_pEditLock2 = new SallyAPI::GUI::CEdit(m_pTabLock->GetForm(), 20 + 170, WINDOW_BORDER_V + 100, 100);
	m_pEditLock2->SetPassword(true);
	m_pEditLock2->SetMaxCharacterInputLength(6);
	m_pEditLock2->SetNumberOnly(true);
	m_pTabLock->GetForm()->AddChild(m_pEditLock2);

	// Update
	m_pCheckboxAutoUpdate = new SallyAPI::GUI::CCheckbox(m_pTabUpdate->GetForm(), 20, WINDOW_BORDER_V, 400);
	m_pCheckboxAutoUpdate->SetText("enable Sally auto update");
	m_pTabUpdate->GetForm()->AddChild(m_pCheckboxAutoUpdate);

	m_pButtonCheckForUpdate = new SallyAPI::GUI::CButton(m_pTabUpdate->GetForm(), 20, WINDOW_BORDER_V + 40, 250, CONTROL_HEIGHT, GUI_CHECK_FOR_UPDATE);
	m_pButtonCheckForUpdate->SetText("Check for Update");
	m_pButtonCheckForUpdate->SetImageId(GUI_THEME_SALLY_ICON_TIMER);
	m_pTabUpdate->GetForm()->AddChild(m_pButtonCheckForUpdate);

	m_pUpdateAvailable = new SallyAPI::GUI::CButton(m_pTabUpdate->GetForm(), 20, WINDOW_BORDER_V + 100, 350, 0, GUI_GET_UPDATE);
	m_pUpdateAvailable->SetText("Install Update (quit Sally)");
	m_pUpdateAvailable->SetImageId(GUI_THEME_SALLY_LOGO_SMALL);
	m_pUpdateAvailable->Visible(false);
	m_pTabUpdate->GetForm()->AddChild(m_pUpdateAvailable);

	m_pUpdateInfo = new SallyAPI::GUI::CLabelBox(m_pTabUpdate->GetForm(), 20, WINDOW_BORDER_V + 100, 350, 60);
	m_pUpdateInfo->Visible(false);
	m_pUpdateInfo->SetLocalised(true);
	m_pTabUpdate->GetForm()->AddChild(m_pUpdateInfo);

	// add to startup
	SallyAPI::GUI::CDropDownItem itemApplication("", "Application Selector Menu", GUI_THEME_SALLY_LOGO_SMALL);
	m_pStartUp->SetLocalised(false);
	m_pStartUp->AddItem(itemApplication);

	m_pUpdateProcessWatcher = new CUpdateProcessWatcher(this, GUI_UPDATE_CHECKED);

	SallyAPI::Scheduler::CSchedulerManager* schedulerManager = SallyAPI::Scheduler::CSchedulerManager::GetInstance();

	SallyAPI::Scheduler::CScheduler scheduler(this, 0, MS_SALLY_SALLY_UPDATE_CHECKER, "sallyUpdater", 5);
	schedulerManager->AddScheduler(scheduler);

	LoadConfig();
}

CSallyConfigPanel::~CSallyConfigPanel()
{
	m_pUpdateProcessWatcher->WaitForStop();
}

void CSallyConfigPanel::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_UPDATE_CHECKED:
		OnCommandShowUpdateMessage();
		return;
	case MS_SALLY_SALLY_UPDATE_CHECKER:
		OnCommandCheckUpdate(false);
		return;
	case GUI_RADIOBUTTON_CLICKED:
		switch (reporterId)
		{
		case GUI_LEFT_RIGH_1:
			m_pFullscreenLeftRight_2->SetCheckStatus(false);
			return;
		case GUI_LEFT_RIGH_2:
			m_pFullscreenLeftRight_1->SetCheckStatus(false);
			return;
		case GUI_VOLUME_WINDOWS:
			m_pVolumeApp->SetCheckStatus(false);
			return;
		case GUI_VOLUME_APP:
			m_pVolumeWindows->SetCheckStatus(false);
			return;
		}
		break;
	case GUI_CHECKBOX_CLICKED:
		switch (reporterId)
		{
		case GUI_USE_INTERNET_EXPLORER_SETTINGS:
			if (m_pUseInternetExplorerProxy->GetCheckStatus())
			{
				m_pEditServer->Enable(false);
				m_pEditBypass->Enable(false);
			}
			else
			{
				m_pEditServer->Enable(true);
				m_pEditBypass->Enable(true);
			}
			return;
		}
		break;
	case GUI_BUTTON_CLICKED:
		switch (reporterId)
		{
		case GUI_GET_UPDATE:
			OnCommandGetUpdate();
			return;
		case GUI_CHECK_FOR_UPDATE:
			OnCommandCheckUpdate(true);
			return;
		case GUI_SHOW_FACEBOOK_CONFIG:
			m_pParent->SendMessageToParent(this, 0, MS_SALLY_SHOW_FACEBOOK_CONFIG);
			return;
		case GUI_FIRSTSTART_WIZARD:
			m_pParent->SendMessageToParent(this, 0, MS_SALLY_SHOW_FIRST_START_WIZARD);
			return;
		}
		return;
	}
	SallyAPI::GUI::CConfigPanel::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

void CSallyConfigPanel::OnCommandCheckUpdate(bool userAction)
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	if ((!option->GetPropertyBool("sally", "autoUpdate", true)) && (!userAction))
		return;

	m_pButtonCheckForUpdate->Enable(false);
	m_pUpdateInfo->SetText("checking for update");

	m_pUpdateProcessWatcher->Start();
}

void CSallyConfigPanel::OnCommandGetUpdate()
{
	m_pParent->SendMessageToParent(this, 0, GUIM_UPDATE);
}

void CSallyConfigPanel::OnCommandShowUpdateMessage()
{
	if (CUpdateHelper::UpdatePending())
	{
		m_pParent->SendMessageToParent(this, 0, MS_SALLY_SALLY_NEW_UPDATE_AVAIL);
		m_pUpdateAvailable->Visible(true);
		m_pUpdateInfo->Visible(false);
	}
	else
	{
		m_pUpdateAvailable->Visible(false);
		m_pUpdateInfo->Visible(true);
		m_pUpdateInfo->SetText("no update available");
	}

	m_pButtonCheckForUpdate->Enable(true);
}

void CSallyConfigPanel::LoadConfig()
{
	m_pTabInfoText[0]->SetText(SALLY_VERSION);
	m_pTabInfoText[1]->SetText(SALLY_DESC);
	m_pTabInfoText[2]->SetText(SALLY_API_VERSION);

	DWORD		dwCharRead;
	char		computerName[MAX_PATH];
	char		caHostName[MAX_PATH];
	hostent*	pstHostEntry = NULL;
	in_addr		stInetAddress;
	int			ilRetVal = 0;

	dwCharRead = MAX_PATH;
	GetComputerName(computerName,&dwCharRead);
	m_pTabInfoText[3]->SetText(computerName);

	gethostname(caHostName, MAX_PATH);
	pstHostEntry = gethostbyname(caHostName); 
	if(pstHostEntry != NULL)
	{
		std::string ipAddress;
		// IP-Adressen bestimmen: 
		for(int ilAddrIndex = 0; pstHostEntry->h_addr_list[ilAddrIndex]; ++ilAddrIndex) 
		{ 
			if (ilRetVal > 0) 
			{
				ipAddress.append(";");
			}
			stInetAddress.S_un.S_addr = *((DWORD*)pstHostEntry->h_addr_list[ilAddrIndex]); 
			ipAddress.append(inet_ntoa(stInetAddress)); 
			++ilRetVal; 
		}
		m_pTabInfoText[4]->SetText(ipAddress);
	}

	FillThemes();
	FillGUILanguages();
	FillInputLanguages();
	FillOutputLanguages();
	FillKeyboardLayouts();

	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	m_pGUILanguage->SelectItemByIdentifier(option->GetPropertyString("sally", "language", "sally-project.language.en"));
	m_pSpeechInput->SelectItemByIdentifier(option->GetPropertyString("sally", "speechInput", "sally-project.language.en"));
	m_pSpeechOutput->SelectItemByIdentifier(option->GetPropertyString("sally", "speechOutput", "sally-project.language.en"));
	m_pKeyboardLayout->SelectItemByIdentifier(option->GetPropertyString("sally", "keyboardLayout", "sally-project.keyboard.en"));
	m_pTheme->SelectItemByIdentifier(option->GetPropertyString("sally", "theme", "de.der-knob.sally.theme.clean"));
	m_pScreensaver->SelectItemByIdentifier(option->GetPropertyString("sally", "screensaver", "de.der-knob.sally.app.imagebrowser"));
	m_pStartUp->SelectItemByIdentifier(option->GetPropertyString("sally", "startupApp", ""));
	m_pShowPopUpInfos->SetCheckStatus(option->GetPropertyBool("sally", "showPopupInfos", true));
	m_pFontAntialasing->SetCheckStatus(option->GetPropertyBool("sally", "fontAntialasing", true));
	m_pGlobalKeyHook->SetCheckStatus(option->GetPropertyBool("sally", "globalKeyHook", false));
	m_pShowFacebookPopUpInfos->SetCheckStatus(option->GetPropertyBool("sally", "showFacebookPopupInfos", true));

	m_pWaitTime->SetValue(option->GetPropertyInt("sally", "waitTime", 10));

	// network settings
	m_pUseInternetExplorerProxy->SetCheckStatus(option->GetPropertyBool("network", "useInternetExplorer", true));
	m_pEditServer->SetText(option->GetPropertyString("network", "proxyServer", ""));
	m_pEditBypass->SetText(option->GetPropertyString("network", "proxyBypass", ""));

	if (m_pUseInternetExplorerProxy->GetCheckStatus())
	{
		m_pEditServer->Enable(false);
		m_pEditBypass->Enable(false);
	}
	else
	{
		m_pEditServer->Enable(true);
		m_pEditBypass->Enable(true);
	}

	// lock settings
	m_pEditLock1->SetText(option->GetPropertyString("lock", "password", ""));
	m_pEditLock2->SetText(option->GetPropertyString("lock", "password", ""));

	// update
	m_pCheckboxAutoUpdate->SetCheckStatus(option->GetPropertyBool("sally", "autoUpdate", true));	

	// volume control
	if (option->GetPropertyBool("sally", "volumeWindows", true))
	{
		m_pVolumeWindows->SetCheckStatus(true);
		m_pVolumeApp->SetCheckStatus(false);
	}
	else
	{
		m_pVolumeWindows->SetCheckStatus(false);
		m_pVolumeApp->SetCheckStatus(true);
	}

	// left right settings
	if (option->GetPropertyBool("sally", "leftEQprevious", true))
	{
		m_pFullscreenLeftRight_1->SetCheckStatus(true);
		m_pFullscreenLeftRight_2->SetCheckStatus(false);
	}
	else
	{
		m_pFullscreenLeftRight_1->SetCheckStatus(false);
		m_pFullscreenLeftRight_2->SetCheckStatus(true);
	}
}

void CSallyConfigPanel::SetLoadedPlugins(std::map<int, SallyAPI::GUI::CApplicationWindow*>* applicationWindowList)
{
	std::string plugins;

	std::map<int, SallyAPI::GUI::CApplicationWindow*>::iterator iter = applicationWindowList->begin();

	while (iter != applicationWindowList->end())
	{
		SallyAPI::GUI::CApplicationWindow* appWindow = iter->second;
		plugins.append(appWindow->GetAppName());
		plugins.append(" (v. ");
		plugins.append(appWindow->GetVersion());
		plugins.append(") by ");
		plugins.append(appWindow->GetAuthor());
		plugins.append("\n");

		if (appWindow->HasScreensaver())
		{
			SallyAPI::GUI::CDropDownItem item(appWindow->GetExplicitAppName(), appWindow->GetAppName(), appWindow->GetGraphicId());
			m_pScreensaver->AddItem(item);
		}

		// add to startup
		SallyAPI::GUI::CDropDownItem item(appWindow->GetExplicitAppName(), appWindow->GetAppName(), appWindow->GetGraphicId());
		m_pStartUp->AddItem(item);

		++iter;
	}

	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	m_pLoadedPlugins->SetText(plugins);
	m_pScreensaver->SelectItemByIdentifier(option->GetPropertyString("sally", "screensaver", "de.der-knob.sally.app.imagebrowser"));
	m_pStartUp->SelectItemByIdentifier(option->GetPropertyString("sally", "startupApp", ""));
}

std::vector<int> CSallyConfigPanel::SaveConfig()
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::Core::CFontManager* fontManager = SallyAPI::Core::CFontManager::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	option->SetPropertyString("sally", "screensaver", m_pScreensaver->GetSelectedIdentifier());
	option->SetPropertyString("sally", "startupApp", m_pStartUp->GetSelectedIdentifier());
	option->SetPropertyInt("sally", "waitTime", m_pWaitTime->GetValue());
	option->SetPropertyBool("sally", "showPopupInfos", m_pShowPopUpInfos->GetCheckStatus());
	option->SetPropertyBool("sally", "showFacebookPopupInfos", m_pShowFacebookPopUpInfos->GetCheckStatus());

	// network settings
	option->SetPropertyBool("network", "useInternetExplorer", m_pUseInternetExplorerProxy->GetCheckStatus());
	option->SetPropertyString("network", "proxyServer", m_pEditServer->GetText());
	option->SetPropertyString("network", "proxyBypass", m_pEditBypass->GetText());

	if (m_pEditLock1->GetText().compare(m_pEditLock2->GetText()) == 0)
	{
		option->SetPropertyString("lock", "password", m_pEditLock1->GetText());
	}
	else
	{
		SallyAPI::GUI::SendMessage::CParameterMessageBox sendMessageParameterMessageBox(0, this,
			"The PINs entered are different.\nPIN was not saved.", SallyAPI::GUI::MESSAGEBOX_ICON_WARNING);
		m_pParent->SendMessageToParent(this, m_iControlId, MS_SALLY_SHOW_MESSAGEBOX, &sendMessageParameterMessageBox);
	}

	// auto update
	option->SetPropertyBool("sally", "autoUpdate", m_pCheckboxAutoUpdate->GetCheckStatus());

	// volume control
	if (m_pVolumeWindows->GetCheckStatus())
	{
		bool oldValue = option->GetPropertyBool("sally", "volumeWindows", true);
		option->SetPropertyBool("sally", "volumeWindows", true);

		if (oldValue == false)
		{
			// if the value was changed
			SallyAPI::Sound::Volume::CVolumeManager* volumeManger = SallyAPI::Sound::Volume::CVolumeManager::GetInstance();
			volumeManger->OnVolumeChange();
		}
	}
	else
	{
		bool oldValue = option->GetPropertyBool("sally", "volumeWindows", true);
		option->SetPropertyBool("sally", "volumeWindows", false);

		if (oldValue == true)
		{
			// if the value was changed
			SallyAPI::Sound::Volume::CVolumeManager* volumeManger = SallyAPI::Sound::Volume::CVolumeManager::GetInstance();
			volumeManger->OnVolumeChange();
		}
	}

	// left right fullscreen
	if (m_pFullscreenLeftRight_1->GetCheckStatus())
	{
		option->SetPropertyBool("sally", "leftEQprevious", true);
	}
	else
	{
		option->SetPropertyBool("sally", "leftEQprevious", false);
	}

	if (option->GetPropertyString("sally", "speechInput").compare(m_pSpeechInput->GetSelectedIdentifier()) != 0)
	{
		option->SetPropertyString("sally", "speechInput", m_pSpeechInput->GetSelectedIdentifier());
		config->ReloadInputLanguage();
	}

	if (option->GetPropertyString("sally", "speechOutput").compare(m_pSpeechInput->GetSelectedIdentifier()) != 0)
	{
		option->SetPropertyString("sally", "speechOutput", m_pSpeechOutput->GetSelectedIdentifier());
		config->ReloadOutputLanguage();
	}

	if (option->GetPropertyString("sally", "keyboardLayout").compare(m_pKeyboardLayout->GetSelectedIdentifier()) != 0)
	{
		option->SetPropertyString("sally", "keyboardLayout", m_pKeyboardLayout->GetSelectedIdentifier());
		config->ReloadKeyboard();
	}

	if (option->GetPropertyString("sally", "language").compare(m_pGUILanguage->GetSelectedIdentifier()) != 0)
	{
		option->SetPropertyString("sally", "language", m_pGUILanguage->GetSelectedIdentifier());
		config->ReloadLanguage();
	}

	bool keyHook = m_pGlobalKeyHook->GetCheckStatus();
	if (keyHook)
		::PostMessage(SallyAPI::Core::CGame::GetHWnd(), WM_ENABLE_KEY_HOOK, 0, 0);
	else
		::PostMessage(SallyAPI::Core::CGame::GetHWnd(), WM_DISABLE_KEY_HOOK, 0, 0);

	option->SetPropertyBool("sally", "globalKeyHook", keyHook);

	bool reloadFont = false;
	if (option->GetPropertyString("sally", "theme").compare(m_pTheme->GetSelectedIdentifier()) != 0)
	{
		option->SetPropertyString("sally", "theme", m_pTheme->GetSelectedIdentifier());
		config->ReloadTheme();
		reloadFont = true;
	}

	bool bFontAntialasing = m_pFontAntialasing->GetCheckStatus();
	if (bFontAntialasing != option->GetPropertyBool("sally", "fontAntialasing", true))
	{
		option->SetPropertyBool("sally", "fontAntialasing", bFontAntialasing);
		reloadFont = true;
	}

	// should we reload the fonts?
	if (reloadFont)
	{
		fontManager->Reload();
		m_pParent->SendMessageToParent(this, 0, MS_SALLY_SALLY_THEME_CHANGED);
	}

	m_pParent->SendMessageToParent(this, 0, MS_SALLY_SALLY_CONFIG_CHANGED);

	// return a empty change list
	std::vector<int> result;
	return result;
}

void CSallyConfigPanel::FillGUILanguages()
{
	m_pGUILanguage->Clear();

	std::string folder = SallyAPI::System::SystemHelper::GetModulePath();
	folder.append("languages\\");

	std::string searchQuery = folder;
	searchQuery.append("*");

	HANDLE				hFolder;
	WIN32_FIND_DATA		folderInformation;

	hFolder = FindFirstFile(searchQuery.c_str(), &folderInformation);

	if(hFolder != INVALID_HANDLE_VALUE)
	{
		do
		{
			if ((folderInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				&& (strcmp(folderInformation.cFileName, "..") != 0)
				&& (strcmp(folderInformation.cFileName, ".") != 0))
			{
				std::string iniFile = folder;
				iniFile.append(folderInformation.cFileName);
				iniFile.append("\\");
				iniFile.append("manifest.ini");

				std::string name = SallyAPI::System::COption::GetPropertyStringStatic("info", "name", "", iniFile);
				if (name.length() > 0)
				{
					std::string uid = SallyAPI::System::COption::GetPropertyStringStatic("info", "uid", "", iniFile);
					std::string isocode = SallyAPI::System::COption::GetPropertyStringStatic("info", "isocode", "", iniFile);
					int flagId = SallyAPI::System::SallyHelper::GetFlagImageId(isocode);
					
					SallyAPI::GUI::CDropDownItem item(folderInformation.cFileName, name, flagId);
					m_pGUILanguage->AddItem(item);
				}
			}
		} while(FindNextFile(hFolder, &folderInformation) == TRUE);
	}
	FindClose(hFolder);

	m_pGUILanguage->SortByName();
}

void CSallyConfigPanel::FillInputLanguages()
{
	m_pSpeechInput->Clear();

	std::string folder = SallyAPI::System::SystemHelper::GetModulePath();
	folder.append("languages\\");

	std::string searchQuery = folder;
	searchQuery.append("*");

	HANDLE				hFolder;
	WIN32_FIND_DATA		folderInformation;

	hFolder = FindFirstFile(searchQuery.c_str(), &folderInformation);

	if(hFolder != INVALID_HANDLE_VALUE)
	{
		do
		{
			if ((folderInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				&& (strcmp(folderInformation.cFileName, "..") != 0)
				&& (strcmp(folderInformation.cFileName, ".") != 0))
			{
				std::string iniFile = folder;
				iniFile.append(folderInformation.cFileName);
				iniFile.append("\\");
				iniFile.append("manifest.ini");

				std::string name = SallyAPI::System::COption::GetPropertyStringStatic("info", "name", "", iniFile);
				if (name.length() > 0)
				{
					std::string uid = SallyAPI::System::COption::GetPropertyStringStatic("info", "uid", "", iniFile);
					std::string isocode = SallyAPI::System::COption::GetPropertyStringStatic("info", "isocode", "", iniFile);
					int flagId = SallyAPI::System::SallyHelper::GetFlagImageId(isocode);
					
					SallyAPI::GUI::CDropDownItem item(folderInformation.cFileName, name, flagId);
					m_pSpeechInput->AddItem(item);
				}
			}
		} while(FindNextFile(hFolder, &folderInformation) == TRUE);
	}
	FindClose(hFolder);

	m_pSpeechInput->SortByName();
}

void CSallyConfigPanel::FillOutputLanguages()
{
	m_pSpeechOutput->Clear();

	std::string folder = SallyAPI::System::SystemHelper::GetModulePath();
	folder.append("languages\\");

	std::string searchQuery = folder;
	searchQuery.append("*");

	HANDLE				hFolder;
	WIN32_FIND_DATA		folderInformation;

	hFolder = FindFirstFile(searchQuery.c_str(), &folderInformation);

	if(hFolder != INVALID_HANDLE_VALUE)
	{
		do
		{
			if ((folderInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				&& (strcmp(folderInformation.cFileName, "..") != 0)
				&& (strcmp(folderInformation.cFileName, ".") != 0))
			{
				std::string iniFile = folder;
				iniFile.append(folderInformation.cFileName);
				iniFile.append("\\");
				iniFile.append("manifest.ini");

				std::string name = SallyAPI::System::COption::GetPropertyStringStatic("info", "name", "", iniFile);
				if (name.length() > 0)
				{
					std::string uid = SallyAPI::System::COption::GetPropertyStringStatic("info", "uid", "", iniFile);
					std::string isocode = SallyAPI::System::COption::GetPropertyStringStatic("info", "isocode", "", iniFile);
					int flagId = SallyAPI::System::SallyHelper::GetFlagImageId(isocode);
					
					SallyAPI::GUI::CDropDownItem item(folderInformation.cFileName, name, flagId);
					m_pSpeechOutput->AddItem(item);
				}
			}
		} while(FindNextFile(hFolder, &folderInformation) == TRUE);
	}
	FindClose(hFolder);

	m_pSpeechOutput->SortByName();
}

void CSallyConfigPanel::FillKeyboardLayouts()
{
	m_pKeyboardLayout->Clear();

	std::string folder = SallyAPI::System::SystemHelper::GetModulePath();
	folder.append("keyboards\\");

	std::string searchQuery = folder;
	searchQuery.append("*");

	HANDLE				hFolder;
	WIN32_FIND_DATA		folderInformation;

	hFolder = FindFirstFile(searchQuery.c_str(), &folderInformation);

	if(hFolder != INVALID_HANDLE_VALUE)
	{
		do
		{
			if ((folderInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				&& (strcmp(folderInformation.cFileName, "..") != 0)
				&& (strcmp(folderInformation.cFileName, ".") != 0))
			{
				std::string iniFile = folder;
				iniFile.append(folderInformation.cFileName);
				iniFile.append("\\");
				iniFile.append("manifest.ini");

				std::string name = SallyAPI::System::COption::GetPropertyStringStatic("info", "name", "", iniFile);
				if (name.length() > 0)
				{
					std::string uid = SallyAPI::System::COption::GetPropertyStringStatic("info", "uid", "", iniFile);
					std::string isocode = SallyAPI::System::COption::GetPropertyStringStatic("info", "isocode", "", iniFile);
					int flagId = SallyAPI::System::SallyHelper::GetFlagImageId(isocode);
					
					SallyAPI::GUI::CDropDownItem item(folderInformation.cFileName, name, flagId);
					m_pKeyboardLayout->AddItem(item);
				}
			}
		} while(FindNextFile(hFolder, &folderInformation) == TRUE);
	}
	FindClose(hFolder);

	m_pKeyboardLayout->SortByName();
}

void CSallyConfigPanel::FillThemes()
{
	m_pTheme->Clear();

	std::string folder = SallyAPI::System::SystemHelper::GetModulePath();
	folder.append("themes\\");

	std::string searchQuery = folder;
	searchQuery.append("*");

	HANDLE				hFolder;
	WIN32_FIND_DATA		folderInformation;

	hFolder = FindFirstFile(searchQuery.c_str(), &folderInformation);

	if(hFolder != INVALID_HANDLE_VALUE)
	{
		do
		{
			if ((folderInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				&& (strcmp(folderInformation.cFileName, "..") != 0)
				&& (strcmp(folderInformation.cFileName, ".") != 0))
			{
				std::string iniFile = folder;
				iniFile.append(folderInformation.cFileName);
				iniFile.append("\\");
				iniFile.append("manifest.ini");

				std::string name = SallyAPI::System::COption::GetPropertyStringStatic("info", "name", "", iniFile);
				if (name.length() > 0)
				{
					SallyAPI::GUI::CDropDownItem item(folderInformation.cFileName, name, NULL);
					m_pTheme->AddItem(item);
				}
			}
		} while(FindNextFile(hFolder, &folderInformation) == TRUE);
	}
	FindClose(hFolder);

	m_pTheme->SortByName();
}