////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\ConfigWindow.cpp
///
/// \brief	Implements the configuration Windows Form. 
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

#include "ConfigWindow.h"

#define SALLY_CONFIG		8000
#define SAVE_BUTTON			8001
#define RELOAD_BUTTON		8002
#define SAVE_DONE			8003
#define RELOAD_DONE			8004

CConfigWindow::CConfigWindow(SallyAPI::GUI::CGUIBaseObject* parent)
	:SallyAPI::GUI::CApplicationWindow(parent, 0, "")
{
	m_pConfigForm = new SallyAPI::GUI::CForm(this, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	this->AddChild(m_pConfigForm);

	m_pTopBorder = new SallyAPI::GUI::CImageBox(this, 0, 0, WINDOW_WIDTH, MENU_HEIGHT);
	m_pTopBorder->SetImageId(GUI_THEME_SALLY_SETTINGS_TOP_BORDER);
	this->AddChild(m_pTopBorder);

	m_pBottomBorder = new SallyAPI::GUI::CImageBox(this, 0, WINDOW_HEIGHT - MENU_HEIGHT, WINDOW_WIDTH, MENU_HEIGHT);
	m_pBottomBorder->SetImageId(GUI_THEME_SALLY_SETTINGS_BOTTOM_BORDER);
	this->AddChild(m_pBottomBorder);

	m_pLogo = new SallyAPI::GUI::CWorking(this, WINDOW_BORDER_H, 19, SallyAPI::GUI::WORKING_SMALL);
	this->AddChild(m_pLogo);

	m_pMenuDropDown = new SallyAPI::GUI::CDropDown(this, WINDOW_BORDER_H + 40, 15, 300);
	this->AddChild(m_pMenuDropDown);

	SallyAPI::GUI::CDropDownItem m_pSallyConfigMenu(SallyAPI::String::StringHelper::ConvertToString(SALLY_CONFIG),
		"Sally Config", GUI_THEME_SALLY_SETTINGS);
	m_pMenuDropDown->SetLocalised(false);
	m_pMenuDropDown->AddItem(m_pSallyConfigMenu);

	m_pMenuDropDown->SelectItemByIdentifier(SallyAPI::String::StringHelper::ConvertToString(SALLY_CONFIG));

	m_pSallyConfigPanel = new CSallyConfigPanel(m_pConfigForm, 0, "");
	m_pConfigForm->AddChild(m_pSallyConfigPanel);

	m_mConfigPanels[SALLY_CONFIG] = m_pSallyConfigPanel;
	m_pCurrentConfigPanel = m_pSallyConfigPanel;

	m_pSave = new SallyAPI::GUI::CButton(this, WINDOW_WIDTH - (WINDOW_BORDER_H + 150),
		WINDOW_HEIGHT - (CONTROL_HEIGHT + 15),
		150, CONTROL_HEIGHT, SAVE_BUTTON, SallyAPI::GUI::BUTTON_TYPE_NORMAL);
	m_pSave->SetText("Save");
	m_pSave->SetDefaultButton(true);
	m_pSave->SetImageId(GUI_THEME_SALLY_ICON_SAVE);
	this->AddChild(m_pSave);

	m_pReload = new SallyAPI::GUI::CButton(this, WINDOW_BORDER_H,
		WINDOW_HEIGHT - (CONTROL_HEIGHT + 15),
		150, CONTROL_HEIGHT, RELOAD_BUTTON, SallyAPI::GUI::BUTTON_TYPE_NORMAL);
	m_pReload->SetText("Reload");
	m_pReload->SetImageId(GUI_THEME_SALLY_ICON_RELOAD);
	this->AddChild(m_pReload);
}

CConfigWindow::~CConfigWindow()
{
}

CSallyConfigPanel* CConfigWindow::GetSallyConfigPanel()
{
	return m_pSallyConfigPanel;
}

void CConfigWindow::AddConfigPanel(SallyAPI::GUI::CApplicationWindow* applicationWindow,
								   SallyAPI::GUI::CConfigPanel* configPanel)
{
	SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();

	std::string info;
	info.append("LoadConfig started from ConfigPanel for '");
	info.append(applicationWindow->GetAppName());
	info.append("' (explicit AppName: '");
	info.append(applicationWindow->GetExplicitAppName());
	info.append("')");
	logger->Info(info);
	configPanel->LoadConfig();

	SallyAPI::GUI::CDropDownItem temp(SallyAPI::String::StringHelper::ConvertToString(applicationWindow->GetGraphicId()),
		applicationWindow->GetAppName(), applicationWindow->GetGraphicId());
	m_pMenuDropDown->AddItem(temp);

	m_mConfigPanels[applicationWindow->GetGraphicId()] = configPanel;
	configPanel->Visible(false);
	m_pConfigForm->AddChild(configPanel);
}

void CConfigWindow::SetLoadedPlugins(std::map<int, SallyAPI::GUI::CApplicationWindow*>* applicationWindowList)
{
	m_pSallyConfigPanel->SetLoadedPlugins(applicationWindowList);
}

void CConfigWindow::OnCommandConfigPanelClicked()
{
	int button = SallyAPI::String::StringHelper::ConvertToInt(m_pMenuDropDown->GetSelectedIdentifier());

	if (button == NULL)
		return;

	m_pCurrentConfigPanel->Visible(false);
	m_pCurrentConfigPanel->LoadConfig();

	SallyAPI::GUI::CConfigPanel* temp = m_mConfigPanels[button];
	m_pCurrentConfigPanel = temp;
	m_pCurrentConfigPanel->Visible(true);
}

void CConfigWindow::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUIM_UPDATE:
		m_pParent->SendMessageToParent(m_pParent, 0, GUIM_UPDATE);
		return;
	case GUI_DROPDOWN_CHANGED:
		if (reporter == m_pMenuDropDown)
		{
			OnCommandConfigPanelClicked();
		}
		return;
	case GUI_BUTTON_CLICKED:
		switch (reporterId)
		{
		case RELOAD_BUTTON:
			m_pParent->SendMessageToParent(this, GetGraphicId(), MS_SALLY_SHOW_WORKING);
			m_tSaveLoadThread.SetValues(m_pCurrentConfigPanel, this, RELOAD_DONE, false);
			m_tSaveLoadThread.Start();
			return;
		case SAVE_BUTTON:
			m_pParent->SendMessageToParent(this, GetGraphicId(), MS_SALLY_SHOW_WORKING);
			m_tSaveLoadThread.SetValues(m_pCurrentConfigPanel, this, SAVE_DONE, true);
			m_tSaveLoadThread.Start();
			return;
		}
		break;
	case MS_SALLY_SALLY_CONFIG_CHANGED:
		m_pParent->SendMessageToParent(reporter, reporterId, messageId);
		return;
	case RELOAD_DONE:
		m_pParent->SendMessageToParent(this, GetGraphicId(), MS_SALLY_HIDE_WORKING);
		return;
	case SAVE_DONE:
		m_pParent->SendMessageToParent(this, GetGraphicId(), MS_SALLY_HIDE_WORKING);
		SallyAPI::GUI::SendMessage::CParameterOnScreenMenu messageOnScreenMenu(GUI_THEME_SALLY_OSM_SAVE, "Saved");
		m_pParent->SendMessageToParent(this, 0, MS_SALLY_ON_SCREEN_MENU, &messageOnScreenMenu);
		return;
	}
	SallyAPI::GUI::CApplicationWindow::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}