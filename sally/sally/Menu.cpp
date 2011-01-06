////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\Menu.cpp
///
/// \brief	Implements the menu class. 
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

#include "Menu.h"

#define GUI_ALARM_CLOCK_NOTIFER			50000

CMenu::CMenu(SallyAPI::GUI::CGUIBaseObject* parent, std::map<int, SallyAPI::GUI::CApplicationWindow*>* appWindows)
	:SallyAPI::GUI::CForm(parent, WINDOW_WIDTH - MENU_WIDTH, 0, MENU_WIDTH, MENU_HEIGHT)
{
	// background menu
	m_pBackgroundMenu = new SallyAPI::GUI::CImageBox(this, 0, 0, MENU_WIDTH, MENU_HEIGHT);
	m_pBackgroundMenu->SetImageId(GUI_THEME_SALLY_MENU);
	this->AddChild(m_pBackgroundMenu);

	// Volume Control
	m_pVolumeControl = new SallyAPI::GUI::CButton(this, 95, 27, 22, 22, 
		0, BUTTON_TYPE_ONLY_IMAGE);
	m_pVolumeControl->SetImageId(GUI_THEME_SALLY_AUDIO_HIGH);
	this->AddChild(m_pVolumeControl);

	m_pVolumeControlNotifer = new SallyAPI::GUI::CButton(this, 90, 22, 32, 32, 
		GUI_SHOW_VOLUME_CONTROL, BUTTON_TYPE_ONLY_IMAGE);
	this->AddChild(m_pVolumeControlNotifer);

	m_pAlarmClockNotifer = new SallyAPI::GUI::CButton(this, 122, 0, 70, MENU_HEIGHT, 
		GUI_ALARM_CLOCK_NOTIFER, BUTTON_TYPE_ONLY_IMAGE);
	this->AddChild(m_pAlarmClockNotifer);

	// Application Selector PopUp
	m_pApplicationSelection = new CMenuApplicationSelector(m_pParent, appWindows);
	m_pParent->SendMessageToParent(m_pApplicationSelection, 0, MS_SALLY_ADD_CHILD);

	m_pAlarmClock = new CAlarmClock(m_pParent);
	m_pParent->SendMessageToParent(m_pAlarmClock, 0, MS_SALLY_ADD_CHILD);

	m_pApplicationsButton = new SallyAPI::GUI::CButton(this, 18, 6, 48, 48,
		MS_SALLY_SHOW_APPLICATION_SELECTOR, BUTTON_TYPE_ONLY_IMAGE);
	m_pApplicationsButton->SetImageId(GUI_THEME_SALLY_SETTINGS);
	this->AddChild(m_pApplicationsButton);

	m_pClock = new SallyAPI::GUI::CLabel(this, 16, 24, 178);
	m_pClock->SetAlign(DT_RIGHT | DT_VCENTER);
	m_pClock->SetLocalised(false);
	m_pClock->SetFont("menu.font.big");
	this->AddChild(m_pClock);

	m_pDate = new SallyAPI::GUI::CLabel(this, 16, 5, 178);
	m_pDate->SetAlign(DT_RIGHT | DT_VCENTER);
	m_pDate->SetLocalised(false);
	m_pDate->SetFont("menu.font.small");
	this->AddChild(m_pDate);
	
	if (appWindows->size() > 0)
	{
		std::map<int, CApplicationWindow*>::iterator it = appWindows->begin();
		CApplicationWindow* appWindow = it->second;

		m_pApplicationsButton->SetImageId(appWindow->GetGraphicId());
	}

	SallyAPI::Sound::Volume::CVolumeManager* volumeManger = SallyAPI::Sound::Volume::CVolumeManager::GetInstance();
	volumeManger->RegisterListener(this);

	UpdateVolume();
}

CMenu::~CMenu()
{
	SallyAPI::Sound::Volume::CVolumeManager* volumeManger = SallyAPI::Sound::Volume::CVolumeManager::GetInstance();
	volumeManger->UnregisterListener(this);
}

void CMenu::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_BUTTON_CLICKED:
		switch (reporterId)
		{
		case GUI_SHOW_VOLUME_CONTROL:
			m_pParent->SendMessageToParent(this, m_iControlId, MS_SALLY_SHOW_VOLUME);
			return;
		case GUI_ALARM_CLOCK_NOTIFER:
			m_pParent->SendMessageToParent(m_pAlarmClock, 0, MS_SALLY_SHOW_POPUP_VIEW);
			return;
		case MS_SALLY_SHOW_APPLICATION_SELECTOR:
			m_pParent->SendMessageToParent(m_pApplicationSelection, 0, MS_SALLY_SHOW_POPUP_VIEW);
			return;
		}
		break;
	case MS_SALLY_HIDE_ALARM_WINDOW:
		m_pAlarmClock->SendMessageToParent(reporter, reporterId, messageId, messageParameter);
		return;
	case MS_SALLY_SALLY_NEW_UPDATE_AVAIL:
		m_pApplicationSelection->SendMessageToParent(reporter, reporterId, messageId);
		return;
	case GUI_THEME_SALLY_SETTINGS:
	case MS_SALLY_CHANGE_APP:
		m_pApplicationsButton->SetImageId(reporterId);
		return;
	case MS_SALLY_VOLUME_CHANGED:
		UpdateVolume();
		return;
	}
	SallyAPI::GUI::CForm::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

void CMenu::RenderControl()
{
	UpdateClock();
	CForm::RenderControl();
}

void CMenu::UpdateVolume()
{
	SallyAPI::Sound::Volume::CVolumeManager* volumeManger = SallyAPI::Sound::Volume::CVolumeManager::GetInstance();

	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	if ((volumeManger->IsWindowsVistaVolumeManagerAvailable() == false) ||
		(option->GetPropertyBool("sally", "volumeWindows", true) == false))
	{
		// reset to high
		m_pVolumeControl->SetImageId(GUI_THEME_SALLY_AUDIO_HIGH);
		return;
	}

	if (volumeManger->GetMuted())
		m_pVolumeControl->SetImageId(GUI_THEME_SALLY_AUDIO_MUTED);
	else
	{
		if (volumeManger->GetVolume() < 1000 / 3)
			m_pVolumeControl->SetImageId(GUI_THEME_SALLY_AUDIO_LOW);
		else if (volumeManger->GetVolume() < 1000 / 2)
			m_pVolumeControl->SetImageId(GUI_THEME_SALLY_AUDIO_MEDIUM);
		else
			m_pVolumeControl->SetImageId(GUI_THEME_SALLY_AUDIO_HIGH);
	}
}

void CMenu::UpdateClock()
{
	m_SystemTime.Update();

	m_pClock->SetText(m_SystemTime.GetTime());
	m_pDate->SetText(m_SystemTime.GetDate());
}

void CMenu::ApplicationClicked(int applicationId, const std::string& name)
{
	m_pApplicationSelection->ApplicationClicked(applicationId, name);
}