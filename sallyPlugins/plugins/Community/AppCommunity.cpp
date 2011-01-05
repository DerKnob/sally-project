////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	Community\AppCommunity.cpp
///
/// \brief	Implements the application community class. 
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

#include "AppCommunity.h"

CAppCommunity::CAppCommunity(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, const std::string& pluginPath)
	:SallyAPI::GUI::CApplicationWindow(parent, graphicId, pluginPath)
{
	m_pApplicationImage = new SallyAPI::GUI::CImageBox(this, 10, 20, 50, 50);
	m_pApplicationImage->SetImageId(GetGraphicId());
	this->AddChild(m_pApplicationImage);

	m_pWelcome = new SallyAPI::GUI::CLabel(this, 10 + 50 + 10, 10, 300);
	m_pWelcome->SetBold(true);
	m_pWelcome->SetLocalised(false);
	this->AddChild(m_pWelcome);

	m_pUpdateStatus = new SallyAPI::GUI::CButton(this, 10 + 50 + 10, 40, 500, CONTROL_HEIGHT, GUI_APP_UPDATE_FACEBOOK_STATUS);
	m_pUpdateStatus->SetText("post a message to your Facebook wall");
	m_pUpdateStatus->SetImageId(GUI_THEME_SALLY_ICON_FACEBOOK);
	this->AddChild(m_pUpdateStatus);

	m_pUpdateStatusEdit = new SallyAPI::GUI::CEdit(this, 0, 0, 0, 0);
	m_pUpdateStatusEdit->Visible(false);
	this->AddChild(m_pUpdateStatusEdit);

	m_iShowRows = (WINDOW_HEIGHT - 80) / (CONTROL_GROUP_HEIGHT + 10);
	m_iShowCols = (WINDOW_WIDTH - 10) / (CONTROL_GROUP_WIDTH + 10);

	int showRowsDelta = ((WINDOW_HEIGHT - 80) % (CONTROL_GROUP_HEIGHT + 10)) / 2;
	int showColsDelta = ((WINDOW_WIDTH - 10) % (CONTROL_GROUP_WIDTH + 10));

	showColsDelta = showColsDelta / m_iShowCols;

	m_iShowCount = m_iShowRows * m_iShowCols;

	int i = 0;
	for (int j = 0; j < m_iShowCols; ++j)
	{
		for (int k = 0; k < m_iShowRows; ++k)
		{
			CControlGroup* temp = new CControlGroup(this, 10 + ((10 + CONTROL_GROUP_WIDTH + showColsDelta) * j),
				showRowsDelta + 80 + (k * (CONTROL_GROUP_HEIGHT + 10)), showColsDelta + CONTROL_GROUP_WIDTH);
			this->AddChild(temp);

			m_vControlGroup.push_back(temp);			
			++i;
		}
	}

	// to the the community status updates
	SallyAPI::Facebook::CFacebookManager* facebookManager = SallyAPI::Facebook::CFacebookManager::GetInstance();
	facebookManager->RegisterStatusUpdateNotifier(this);
}

CAppCommunity::~CAppCommunity()
{
}

bool CAppCommunity::IsFacebookNeeded()
{
	return true;
}

void CAppCommunity::Visible(bool visible)
{
	OnCommandUpdateView();

	SallyAPI::GUI::CApplicationWindow::Visible(visible);
}

void CAppCommunity::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int iMessageID, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (iMessageID)
	{
	case GUI_EDIT_CHANGED:
		UpdateFacebookStatus();
		return;
	case GUI_BUTTON_CLICKED:
		switch (reporterId)
		{
		case GUI_APP_UPDATE_FACEBOOK_STATUS:
			OnCommandUpdateFacebookStatus();
			return;
		}
		break;
	case MS_SALLY_APP_LOAD_COMPLETE:
	case MS_SALLY_FACEBOOK_STATUS_UPDATE:
		OnCommandUpdateStatus();
		return;
	}
	CApplicationWindow::SendMessageToParent(reporter, reporterId, iMessageID, messageParameter);
}

void CAppCommunity::OnCommandUpdateFacebookStatus()
{
	m_pUpdateStatusEdit->SetText("");

	SendMessageToParent(m_pUpdateStatusEdit, NULL, MS_SALLY_SHOW_KEYBOARD);
}

void CAppCommunity::UpdateFacebookStatus()
{
	if (m_pUpdateStatusEdit->GetText() == "")
		return;

	SallyAPI::Facebook::CFacebookManager* facebookManager = SallyAPI::Facebook::CFacebookManager::GetInstance();
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();

	std::string message;
	std::string description;
	std::string link;
	std::string image;
	std::string errorMessage;

	message = m_pUpdateStatusEdit->GetText();

	facebookManager->PostMessageToWall(message, description, link, image, errorMessage);

	SallyAPI::GUI::SendMessage::CParameterOnScreenMenu messageOnScreenMenu(GUI_THEME_SALLY_FACEBOOK, "Added");
	this->SendMessageToParent(this, 0, MS_SALLY_ON_SCREEN_MENU, &messageOnScreenMenu);
}

void CAppCommunity::OnCommandUpdateView()
{
	SallyAPI::Facebook::CFacebookManager* facebookManager = SallyAPI::Facebook::CFacebookManager::GetInstance();
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::Config::CLanguageManager* lang = config->GetLanguageLocalization();

	// now get the latest news
	std::string message = lang->GetString("Welcome %s", facebookManager->GetFacebookUserName().c_str(), NULL);
	m_pWelcome->SetText(message);
}

void CAppCommunity::OnCommandUpdateStatus()
{
	OnCommandUpdateView();

	SallyAPI::Facebook::CFacebookManager* facebookManager = SallyAPI::Facebook::CFacebookManager::GetInstance();
	SallyAPI::Facebook::CFacebookDB* facebookDB = SallyAPI::Facebook::CFacebookDB::GetInstance();
	
	std::vector<SallyAPI::Facebook::CStatusMessage> status = facebookDB->GetLastMessages(m_iShowCount);
	std::vector<SallyAPI::Facebook::CStatusMessage>::iterator iter = status.begin();

	// load user image
	m_pApplicationImage->SetImageId(facebookManager->GetFacebookUserImageId(facebookManager->GetFacebookUserId()));

	int i = 0;
	while (iter != status.end())
	{
		SallyAPI::Facebook::CStatusMessage statusMessage = *iter;

		SallyAPI::GUI::SendMessage::CParameterApplicationInfo applicationInfo(statusMessage.GetExplicidAppName());
		SendMessageToParent(this, 0, MS_SALLY_GET_APPLICATION_INFO, &applicationInfo);

		// we have already the image loaded
		m_vControlGroup.at(i)->Visible(true);
		m_vControlGroup.at(i)->SetImageId(facebookManager->GetFacebookUserImageId(statusMessage.GetUserId()));
		m_vControlGroup.at(i)->SetValue(statusMessage.GetName(), statusMessage.GetMessageString(),
			statusMessage.GetCreateDate(), statusMessage.GetAction(), statusMessage.GetActionName(),
			applicationInfo.GetWindow());
		++iter;
		++i;
	}
	while (i < m_iShowCount)
	{
		m_vControlGroup.at(i)->Visible(false);
		++i;
	}
}