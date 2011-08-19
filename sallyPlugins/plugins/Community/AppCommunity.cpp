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
	LoadApplicationImage("header.png", GUI_APP_HEADER);
	LoadApplicationImage("logo.png", GUI_APP_LOGO);
	LoadApplicationImage("home.png", GUI_APP_HOME);
	LoadApplicationImage("wall.png", GUI_APP_WALL);
	LoadApplicationImage("notifications.png", GUI_APP_NOTIFICATIONS);

	m_pImageHeader = new SallyAPI::GUI::CImageBox(this, 0, 0, WINDOW_WIDTH, 34);
	m_pImageHeader->SetImageId(GUI_APP_HEADER + GetGraphicId());
	m_pImageHeader->SetDiyplayType(SallyAPI::GUI::IMAGEBOX_DISPLAY_TYPE_STRETCH);
	this->AddChild(m_pImageHeader);

	m_pImageLogo = new SallyAPI::GUI::CImageBox(this, 6, 6, 90, 20);
	m_pImageLogo->SetImageId(GUI_APP_LOGO + GetGraphicId());
	this->AddChild(m_pImageLogo);

	m_pApplicationImage = new SallyAPI::GUI::CImageBox(this, 6 + 90 + 6, 6, 50, 50);
	m_pApplicationImage->SetImageId(GetGraphicId());
	this->AddChild(m_pApplicationImage);

	m_pWelcome = new SallyAPI::GUI::CLabel(this, 6 + 90 + 6 + 50 + 6, 1, 500);
	m_pWelcome->SetBold(true);
	m_pWelcome->SetLocalised(false);
	m_pWelcome->SetColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	this->AddChild(m_pWelcome);


	// tab control
	m_pTabControl = new SallyAPI::GUI::CTabcontrol(this, WINDOW_BORDER_H, MENU_HEIGHT + 10,
		this->GetWidth() - (2 * WINDOW_BORDER_H), this->GetHeight() - WINDOW_BORDER_V - MENU_HEIGHT - 10);
	this->AddChild(m_pTabControl);

	int height = m_pTabControl->GetFormHeight();
	int width = m_pTabControl->GetFormWidth();

	// tabs
	m_pTabHome = new SallyAPI::GUI::CTabcontrolItem(m_pTabControl, "Home", GUI_APP_HOME + GetGraphicId());
	m_pTabControl->AddTabItem(m_pTabHome);

	m_pTabHomeForm = new SallyAPI::GUI::CScrollForm(m_pTabHome->GetForm(), 10, 10, width - 20, height - 20);
	m_pTabHome->GetForm()->AddChild(m_pTabHomeForm);

	m_pTabNews = new SallyAPI::GUI::CTabcontrolItem(m_pTabControl, "News", GUI_APP_NOTIFICATIONS + GetGraphicId());
	m_pTabControl->AddTabItem(m_pTabNews);

	m_pTabNewsForm = new SallyAPI::GUI::CScrollForm(m_pTabNews->GetForm(), 10 + CONTROL_HEIGHT + 10, 10, width - 20, height - 20 - CONTROL_HEIGHT - 10);
	m_pTabNews->GetForm()->AddChild(m_pTabNewsForm);

	m_pTabWall = new SallyAPI::GUI::CTabcontrolItem(m_pTabControl, "Wall", GUI_APP_WALL + GetGraphicId());
	m_pTabControl->AddTabItem(m_pTabWall);

	m_pTabWallForm = new SallyAPI::GUI::CScrollForm(m_pTabWall->GetForm(), 10 + CONTROL_HEIGHT + 10, 10, width - 20, height - 20 - CONTROL_HEIGHT - 10);
	m_pTabWall->GetForm()->AddChild(m_pTabWallForm);

	m_pUpdateStatusEdit = new SallyAPI::GUI::CEdit(m_pTabWall->GetForm(), 10, 10, width - 150 - 20 - 10);
	m_pUpdateStatusEdit->SetInfoText("What are you doing?");
	m_pTabWall->GetForm()->AddChild(m_pUpdateStatusEdit);

	m_pUpdateStatus = new SallyAPI::GUI::CButton(m_pTabWall->GetForm(), width - 150 - 10, 10, 150, CONTROL_HEIGHT, GUI_APP_UPDATE_FACEBOOK_STATUS);
	m_pUpdateStatus->SetText("Send");
	m_pUpdateStatus->SetImageId(GUI_APP_WALL + GetGraphicId());
	m_pTabWall->GetForm()->AddChild(m_pUpdateStatus);

	m_iShowCount = 40;
	for (int i = 0; i < m_iShowCount; i++)
	{
		CControlGroup* temp = new CControlGroup(m_pTabHomeForm,
			10, 20 + (i * CONTROL_GROUP_HEIGHT), m_pTabHomeForm->GetWidth() - 20 - CONTROL_HEIGHT);
		m_pTabHomeForm->AddChild(temp);

		m_vControlGroupHome.push_back(temp);	
	}

	for (int i = 0; i < m_iShowCount; i++)
	{
		CControlGroup* temp = new CControlGroup(m_pTabWallForm,
			10, 20 + (i * CONTROL_GROUP_HEIGHT), m_pTabWallForm->GetWidth() - 20 - CONTROL_HEIGHT);
		m_pTabWallForm->AddChild(temp);

		m_vControlGroupWall.push_back(temp);	
	}

	for (int i = 0; i < m_iShowCount; i++)
	{
		CControlGroup* temp = new CControlGroup(m_pTabNewsForm,
			10, 20 + (i * CONTROL_GROUP_HEIGHT), m_pTabNewsForm->GetWidth() - 20 - CONTROL_HEIGHT);
		m_pTabNewsForm->AddChild(temp);

		m_vControlGroupNews.push_back(temp);	
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
	case GUI_BUTTON_CLICKED:
		switch (reporterId)
		{
		case GUI_APP_UPDATE_FACEBOOK_STATUS:
			UpdateFacebookStatus();
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
	m_pUpdateStatusEdit->SetText("");

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
	UpdateFacebookSally();
	UpdateFacebookNews();
	UpdateFacebookWall();
}

bool CAppCommunity::UpdateFacebookSally()
{
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
		m_vControlGroupHome.at(i)->Visible(true);
		m_vControlGroupHome.at(i)->SetImageId(facebookManager->GetFacebookUserImageId(statusMessage.GetUserId()));
		m_vControlGroupHome.at(i)->SetValue(statusMessage.GetName(), statusMessage.GetMessageString(),
			statusMessage.GetCreateDate(), statusMessage.GetAction(), statusMessage.GetActionName(),
			applicationInfo.GetWindow());
		++iter;
		++i;
	}

	m_pTabHomeForm->ResizeScrollArea(m_pTabHomeForm->GetWidth(), (i - 1)* (CONTROL_GROUP_HEIGHT + 20) + 20 - m_pTabHomeForm->GetHeight());

	while (i < m_iShowCount)
	{
		m_vControlGroupHome.at(i)->Visible(false);
		++i;
	}

	return true;
}

bool CAppCommunity::UpdateFacebookNews()
{
	return true;
}

bool CAppCommunity::UpdateFacebookWall()
{
	SallyAPI::Facebook::CFacebookManager* facebookManager = SallyAPI::Facebook::CFacebookManager::GetInstance();

	std::string dataResponse;
	std::string errorMessage;
	SallyAPI::Network::NETWORK_RETURN errorCode;
	bool result = facebookManager->GetWall(dataResponse, errorMessage, errorCode);

	if (result)
	{
		if (dataResponse.length() == 0)
			return false;

		if (dataResponse.find("<error>") != std::string::npos)
			return false;

		std::string tempFile = SallyAPI::Core::CGame::GetMediaFolder();
		tempFile.append("facebookActivate.xml");

		DeleteFile(tempFile.c_str());

		SallyAPI::File::FileHelper::AddLineToFile(tempFile, dataResponse);

		XMLNode xMainNode = XMLNode::parseFile(tempFile.c_str());
		if (xMainNode.isEmpty())
			return false;

		XMLNode root = xMainNode.getChildNode("sallycommunity");
		if (root.isEmpty())
			return false;

		// create
		XMLNode meXML;
		int i = 0;
		do
		{
			meXML = root.getChildNode("me", i);

			if (!meXML.isEmpty())
			{
				std::string userId = CheckForNull(meXML.getAttribute("userID"));
				std::string name = CheckForNull(meXML.getAttribute("name"));
				std::string access = CheckForNull(meXML.getAttribute("access"));

			}
			++i;
		}
		while (!meXML.isEmpty());

		// cleanup
		DeleteFile(tempFile.c_str());

	}

	return true;
}