////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\MenuApplicationSelector.cpp
///
/// \brief	Implements the menu application selector class. 
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

#include "MenuApplicationSelector.h"

using namespace SallyAPI::GUI;

#define GUI_GET_UPDATE					50001
#define GUI_CURRENT_APPLICATION			50002

CMenuApplicationSelector::CMenuApplicationSelector(SallyAPI::GUI::CGUIBaseObject* parent,
	std::map<int, SallyAPI::GUI::CApplicationWindow*>* appWindows)
	:SallyAPI::GUI::CPopUpWindow(parent, 0, "")
{
	m_pListInfo = new SallyAPI::GUI::CLabel(this, WINDOW_BORDER_H, 80, 400);
	m_pListInfo->SetText("Select your application:");
	m_pListInfo->SetFont("menu.font");
	this->AddChild(m_pListInfo);

	m_pList = new SallyAPI::GUI::CScrollList(this, WINDOW_BORDER_H, 150, WINDOW_WIDTH / 2, WINDOW_HEIGHT - 300, 80, 30);
	m_pList->SetFont("menu.scrolllist.font");
	this->AddChild(m_pList);

	int x = (WINDOW_WIDTH / 2) + ((WINDOW_WIDTH / 2) - 128) / 2;
	int y = (WINDOW_HEIGHT - (128 + 80)) / 2;

	m_pSelectedAppIcon = new SallyAPI::GUI::CButton(this, x, y, 128, 128,
		GUI_CURRENT_APPLICATION, SallyAPI::GUI::BUTTON_TYPE_ONLY_IMAGE);
	this->AddChild(m_pSelectedAppIcon);

	m_pSelectedAppName = new SallyAPI::GUI::CLabel(this, (WINDOW_WIDTH / 2) + ((WINDOW_WIDTH / 2) - 200) / 2, (WINDOW_HEIGHT - (128 + 80)) / 2 + 128 + 10, 200);
	m_pSelectedAppName->SetLocalised(false);
	m_pSelectedAppName->SetFont("menu.font");
	m_pSelectedAppName->SetAlign(DT_CENTER | DT_VCENTER);
	this->AddChild(m_pSelectedAppName);

	// add the windows to the list - but sort it first
	std::map<int, SallyAPI::GUI::CApplicationWindow*>::iterator	itApp;
	std::map<std::string, SallyAPI::GUI::CApplicationWindow*>	sortedAppList;

	for (itApp = appWindows->begin(); itApp != appWindows->end(); ++itApp)
	{
		SallyAPI::GUI::CApplicationWindow* appWindow = itApp->second;

		if (!appWindow->HasApplication())
			continue;

		sortedAppList[SallyAPI::String::StringHelper::StringToLower(appWindow->GetAppName())] = appWindow;
	}

	// now add the sorted list
	std::map<std::string, SallyAPI::GUI::CApplicationWindow*>::iterator	itSortedList;

	for (itSortedList = sortedAppList.begin(); itSortedList != sortedAppList.end(); ++itSortedList)
	{
		SallyAPI::GUI::CApplicationWindow* appWindow = itSortedList->second;

		if (!appWindow->HasApplication())
			continue;

		SallyAPI::GUI::CScrollListItem item(appWindow->GetGraphicId(), appWindow->GetGraphicId(), appWindow->GetAppName());
		m_pList->AddItem(item);
	}

	// create the rest of the buttons
	m_pLogout = new SallyAPI::GUI::CButton(this, 20 , WINDOW_HEIGHT - (20 + 64), 64, 64,
		MS_SALLY_APP_SHOW_SHUTDOWN, SallyAPI::GUI::BUTTON_TYPE_ONLY_IMAGE);
	m_pLogout->SetImageId(GUI_THEME_SALLY_LOGOUT);
	this->AddChild(m_pLogout);

	m_pWindows = new SallyAPI::GUI::CButton(this, 20 + 20 + 64, WINDOW_HEIGHT - (20 + 64), 64, 64,
		GUI_THEME_SALLY_MINIMIZE, SallyAPI::GUI::BUTTON_TYPE_ONLY_IMAGE);
	m_pWindows->SetImageId(GUI_THEME_SALLY_MINIMIZE);
	this->AddChild(m_pWindows);

	m_pLockScreen = new SallyAPI::GUI::CButton(this, WINDOW_WIDTH - (60 + 64 + 64 + 64) , WINDOW_HEIGHT - (20 + 64) , 64, 64,
		GUI_THEME_SALLY_LOCK, SallyAPI::GUI::BUTTON_TYPE_ONLY_IMAGE);
	m_pLockScreen->SetImageId(GUI_THEME_SALLY_LOCK);
	this->AddChild(m_pLockScreen);

	m_pSettings = new SallyAPI::GUI::CButton(this, WINDOW_WIDTH - (40 + 64 + 64) , WINDOW_HEIGHT - (20 + 64) , 64, 64,
		GUI_THEME_SALLY_SETTINGS, SallyAPI::GUI::BUTTON_TYPE_ONLY_IMAGE);
	m_pSettings->SetImageId(GUI_THEME_SALLY_SETTINGS);
	this->AddChild(m_pSettings);

	m_pScreensaver = new SallyAPI::GUI::CButton(this, WINDOW_WIDTH - (20 + 64) , WINDOW_HEIGHT - (20 + 64) , 64, 64,
		GUI_THEME_SALLY_SCREENSAVER, SallyAPI::GUI::BUTTON_TYPE_ONLY_IMAGE);
	m_pScreensaver->SetImageId(GUI_THEME_SALLY_SCREENSAVER);
	this->AddChild(m_pScreensaver);

	m_pUpdateAvailable = new SallyAPI::GUI::CButton(this, 20, 20, 350, 0, GUI_GET_UPDATE);
	m_pUpdateAvailable->SetText("Install Update (quit Sally)");
	m_pUpdateAvailable->SetImageId(GUI_THEME_SALLY_LOGO_SMALL);
	m_pUpdateAvailable->Visible(false);
	this->AddChild(m_pUpdateAvailable);

	m_pBackground->SetImageId(GUI_THEME_SALLY_POPUP_APPSELECTOR);
}

CMenuApplicationSelector::~CMenuApplicationSelector()
{
}

void CMenuApplicationSelector::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_SCROLLLIST_CLICKED:
		OnCommandApplicationChanged(messageParameter);
		return;
	case GUI_BUTTON_CLICKED:
		switch (reporterId)
		{
		case GUI_CURRENT_APPLICATION:
			this->SendMessageToParent(m_pButtonClose, GUI_THEME_SALLY_CLOSE, GUI_BUTTON_CLICKED);
			return;
		case GUI_GET_UPDATE:
			OnCommandGetUpdate();
			return;
		case GUI_THEME_SALLY_SCREENSAVER:
			SallyAPI::GUI::CPopUpWindow::SendMessageToParent(NULL, reporterId, MS_SALLY_SCREENSAVER_START);
			SallyAPI::GUI::CPopUpWindow::SendMessageToParent(this, 0, MS_SALLY_HIDE_POPUP_VIEW);
			return;
		case GUI_THEME_SALLY_MINIMIZE:
			ShowWindow(SallyAPI::Core::CGame::GetHWnd(), SW_MINIMIZE);
			return;
		case GUI_THEME_SALLY_SETTINGS:
			SallyAPI::GUI::CPopUpWindow::SendMessageToParent(NULL, 0, MS_SALLY_SHOW_SETTINGS);
			SallyAPI::GUI::CPopUpWindow::SendMessageToParent(this, 0, MS_SALLY_HIDE_POPUP_VIEW);
			return;
		case GUI_THEME_SALLY_LOCK:
			SallyAPI::GUI::CPopUpWindow::SendMessageToParent(this, 0, MS_SALLY_HIDE_POPUP_VIEW);
			SallyAPI::GUI::CPopUpWindow::SendMessageToParent(this, 0, MS_SALLY_LOCK_SCREEN);
			return;
		case MS_SALLY_APP_SHOW_SHUTDOWN:
			SallyAPI::GUI::CPopUpWindow::SendMessageToParent(this, 0, MS_SALLY_APP_SHOW_SHUTDOWN);
			return;
		}
		break;
	case MS_SALLY_SALLY_NEW_UPDATE_AVAIL:
		m_pUpdateAvailable->Visible(true);
		return;
	}
	SallyAPI::GUI::CPopUpWindow::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

void CMenuApplicationSelector::OnCommandGetUpdate()
{
	m_pParent->SendMessageToParent(m_pParent, 0, GUIM_UPDATE);
}

void CMenuApplicationSelector::ApplicationClicked(int applicationId, const std::string& name)
{
	m_pSelectedAppIcon->SetImageId(applicationId);
	m_pSelectedAppName->SetText(name);
}

void CMenuApplicationSelector::OnCommandApplicationChanged(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	SallyAPI::GUI::SendMessage::CParameterScrollList* sendMessageParameterScrollList = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterScrollList*> (messageParameter);

	if (sendMessageParameterScrollList == NULL)
		return;

	int reporterId = sendMessageParameterScrollList->GetItem().GetCommand();

	SallyAPI::GUI::SendMessage::CParameterInteger messageParameterInteger(reporterId);
	m_pParent->SendMessageToParent(this, reporterId, MS_SALLY_CHANGE_APP, &messageParameterInteger);
	m_pParent->SendMessageToParent(this, 0, MS_SALLY_HIDE_POPUP_VIEW);
}