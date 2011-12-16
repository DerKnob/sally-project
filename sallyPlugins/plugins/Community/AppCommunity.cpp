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
	LoadApplicationImage("properties.png", GUI_APP_PROPERTIES);
	LoadApplicationImage("home.png", GUI_APP_HOME);
	LoadApplicationImage("wall.png", GUI_APP_WALL);
	LoadApplicationImage("notifications.png", GUI_APP_NOTIFICATIONS);

	m_pImageHeader = new SallyAPI::GUI::CImageBox(this, 0, 0, WINDOW_WIDTH, 34);
	m_pImageHeader->SetImageId(GUI_APP_HEADER + GetGraphicId());
	m_pImageHeader->SetDisplayType(SallyAPI::GUI::IMAGEBOX_DISPLAY_TYPE_STRETCH);
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

	m_pButtonProperties = new SallyAPI::GUI::CButton(this,
		WINDOW_WIDTH - MENU_WIDTH - 10 - 32, 1, 32, 32, GUI_APP_PROPERTIES, SallyAPI::GUI::BUTTON_TYPE_ONLY_IMAGE);
	m_pButtonProperties->SetImageId(GUI_APP_PROPERTIES + GetGraphicId());
	this->AddChild(m_pButtonProperties);

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

	m_pTabWall = new SallyAPI::GUI::CTabcontrolItem(m_pTabControl, "Wall", GUI_APP_WALL + GetGraphicId());
	m_pTabControl->AddTabItem(m_pTabWall);

	m_pTabWallForm = new SallyAPI::GUI::CScrollForm(m_pTabWall->GetForm(), 10, 10 + CONTROL_HEIGHT + 10, width - 20, height - 20 - CONTROL_HEIGHT - 10);
	m_pTabWall->GetForm()->AddChild(m_pTabWallForm);

	m_pTabNews = new SallyAPI::GUI::CTabcontrolItem(m_pTabControl, "News", GUI_APP_NOTIFICATIONS + GetGraphicId());
	m_pTabControl->AddTabItem(m_pTabNews);

	m_pTabNewsForm = new SallyAPI::GUI::CScrollForm(m_pTabNews->GetForm(), 10, 10, width - 20, height - 20);
	m_pTabNews->GetForm()->AddChild(m_pTabNewsForm);

	m_pUpdateStatusEdit = new SallyAPI::GUI::CEdit(m_pTabWall->GetForm(), 10, 10, width - 150 - 20 - 10);
	m_pUpdateStatusEdit->SetInfoText("What are you doing?");
	m_pTabWall->GetForm()->AddChild(m_pUpdateStatusEdit);

	m_pUpdateStatus = new SallyAPI::GUI::CButton(m_pTabWall->GetForm(), width - 150 - 10, 10, 150, CONTROL_HEIGHT, GUI_APP_UPDATE_FACEBOOK_STATUS);
	m_pUpdateStatus->SetText("Send");
	m_pUpdateStatus->SetImageId(GUI_APP_WALL + GetGraphicId());
	m_pTabWall->GetForm()->AddChild(m_pUpdateStatus);

	for (int i = 0; i < SHOW_COUNT; i++)
	{
		CControlGroup* temp = new CControlGroup(m_pTabHomeForm,
			10, 20 + (i * CONTROL_GROUP_HEIGHT), m_pTabHomeForm->GetWidth() - 20 - CONTROL_HEIGHT);
		temp->Visible(false);
		m_pTabHomeForm->AddChild(temp);

		m_vControlGroupHome.push_back(temp);	
	}

	for (int i = 0; i < SHOW_COUNT; i++)
	{
		CControlGroup* temp = new CControlGroup(m_pTabWallForm,
			10, 20 + (i * CONTROL_GROUP_HEIGHT), m_pTabWallForm->GetWidth() - 20 - CONTROL_HEIGHT);
		temp->Visible(false);
		m_pTabWallForm->AddChild(temp);

		m_vControlGroupWall.push_back(temp);	
	}

	for (int i = 0; i < SHOW_COUNT; i++)
	{
		CControlGroup* temp = new CControlGroup(m_pTabNewsForm,
			10, 20 + (i * CONTROL_GROUP_HEIGHT), m_pTabNewsForm->GetWidth() - 20 - CONTROL_HEIGHT);
		temp->Visible(false);
		m_pTabNewsForm->AddChild(temp);

		m_vControlGroupNews.push_back(temp);	
	}

	// to the the community status updates
	SallyAPI::Facebook::CFacebookManager* facebookManager = SallyAPI::Facebook::CFacebookManager::GetInstance();
	facebookManager->RegisterStatusUpdateNotifier(this);

	m_pThreadUpdateStatus = new SallyAPI::GUI::CThreadStarter(this, 0, GUI_APP_UPDATE_FACEBOOK_STATUS_THREAD);
}

CAppCommunity::~CAppCommunity()
{
	m_pThreadUpdateStatus->WaitForStop(true);
	SafeDelete(m_pThreadUpdateStatus);
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
		case GUI_APP_PROPERTIES:
			m_pParent->SendMessageToParent(this, GetGraphicId(), MS_SALLY_SHOW_FACEBOOK_CONFIG);
			return;
		case GUI_APP_UPDATE_FACEBOOK_STATUS:
			UpdateFacebookStatus();
			return;
		}
		break;
	case MS_SALLY_APP_LOAD_COMPLETE:
	case MS_SALLY_FACEBOOK_STATUS_UPDATE:
		m_pThreadUpdateStatus->Start();
		return;
	case GUI_APP_UPDATE_FACEBOOK_STATUS_THREAD:
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
	if (!UpdateFacebookWall())
	{
		m_pTabWallForm->ResizeScrollArea(-1, -1);
		for (int i = 0; i < SHOW_COUNT; i++)
		{
			m_vControlGroupWall.at(i)->Visible(false);
		}
	}
	if (!UpdateFacebookNews())
	{
		m_pTabNewsForm->ResizeScrollArea(-1, -1);
		for (int i = 0; i < SHOW_COUNT; i++)
		{
			m_vControlGroupNews.at(i)->Visible(false);
		}
	}
}

bool CAppCommunity::UpdateFacebookSally()
{
	SallyAPI::Facebook::CFacebookManager* facebookManager = SallyAPI::Facebook::CFacebookManager::GetInstance();
	SallyAPI::Facebook::CFacebookDB* facebookDB = SallyAPI::Facebook::CFacebookDB::GetInstance();

	std::vector<SallyAPI::Facebook::CStatusMessage> status = facebookDB->GetLastMessages(SHOW_COUNT);
	std::vector<SallyAPI::Facebook::CStatusMessage>::iterator iter = status.begin();

	// load user image
	m_pApplicationImage->SetImageId(facebookManager->GetFacebookUserImageId(facebookManager->GetFacebookUserId()));

	int i = 0;
	int heightUsed = 20;
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

		m_vControlGroupHome.at(i)->Move(10, heightUsed);
		heightUsed = heightUsed + m_vControlGroupHome.at(i)->GetHeight() + 20;

		++iter;
		++i;
	}
	m_pTabHomeForm->ResizeScrollArea(m_pTabHomeForm->GetWidth(), heightUsed);

	while (i < SHOW_COUNT)
	{
		m_vControlGroupHome.at(i)->Visible(false);
		++i;
	}

	return true;
}

bool CAppCommunity::UpdateFacebookNews()
{
	SallyAPI::Facebook::CFacebookManager* facebookManager = SallyAPI::Facebook::CFacebookManager::GetInstance();

	std::string dataResponse;
	std::string errorMessage;
	SallyAPI::Network::NETWORK_RETURN errorCode;
	bool result = facebookManager->GetNews(dataResponse, errorMessage, errorCode);

	if (!result)
		return false;

	return GetFeeds(dataResponse, &m_vControlGroupNews, m_pTabNewsForm);
}

bool CAppCommunity::UpdateFacebookWall()
{
	SallyAPI::Facebook::CFacebookManager* facebookManager = SallyAPI::Facebook::CFacebookManager::GetInstance();

	std::string dataResponse;
	std::string errorMessage;
	SallyAPI::Network::NETWORK_RETURN errorCode;
	bool result = facebookManager->GetWall(dataResponse, errorMessage, errorCode);

	if (!result)
		return false;

	return GetFeeds(dataResponse, &m_vControlGroupWall, m_pTabWallForm);
}

bool CAppCommunity::GetFeeds(std::string& dataResponse, std::vector<CControlGroup*>* controlGroup,
							 SallyAPI::GUI::CScrollForm* scrollForm)
{
	SallyAPI::Facebook::CFacebookManager* facebookManager = SallyAPI::Facebook::CFacebookManager::GetInstance();

	if (dataResponse.length() == 0)
		return false;

	if (dataResponse.find("<error>") != std::string::npos)
		return false;

	std::string uid = SallyAPI::System::SystemHelper::GenerateUniqueID();
	std::string tempFile = SallyAPI::Core::CGame::GetMediaFolder();
	tempFile.append(uid);
	tempFile.append(".xml");

	DeleteFile(tempFile.c_str());

	SallyAPI::File::FileHelper::AddLineToFile(tempFile, dataResponse);

	XMLNode xMainNode = XMLNode::parseFile(tempFile.c_str());
	if (xMainNode.isEmpty())
		return false;

	XMLNode root = xMainNode.getChildNode("sallycommunity");
	if (root.isEmpty())
		return false;

	XMLNode feeds = root.getChildNode("feeds");
	if (feeds.isEmpty())
		return false;

	// create
	XMLNode feed;
	int feedCounter = 0;
	int feedReadCounter = 0;
	int heightUsed = 20;
	do
	{
		feed = feeds.getChildNode("feed", feedCounter);

		if (!feed.isEmpty())
		{
			std::string fromName = CheckForNull(feed.getAttribute("fromName"));
			std::string createdTime = CheckForNull(feed.getAttribute("createdTime"));
			std::string fromId = CheckForNull(feed.getAttribute("fromId"));
			const char* messageChar = feed.getText();

			if (messageChar != NULL)
			{
				fromName = SallyAPI::Network::NetworkHelper::URLDecode(fromName);
				std::string message = SallyAPI::Network::NetworkHelper::URLDecode(messageChar);

				if (facebookManager->LoadFacebookUserImage(fromId) == false)
				{
					// download image
					std::string imageFolder = SallyAPI::Core::CGame::GetMediaFolder();;
					imageFolder.append("Facebook\\");

					facebookManager->DownloadFacebookUserImage(imageFolder, fromId);
				}

				controlGroup->at(feedReadCounter)->Visible(true);
				controlGroup->at(feedReadCounter)->SetImageId(facebookManager->GetFacebookUserImageId(fromId));
				controlGroup->at(feedReadCounter)->SetValue(fromName, message, createdTime, "", "", NULL);

				controlGroup->at(feedReadCounter)->Move(10, heightUsed);
				heightUsed = heightUsed + controlGroup->at(feedReadCounter)->GetHeight() + 20;

				++feedReadCounter;
			}
		}
		++feedCounter;
	}
	while (!feed.isEmpty());

	// cleanup
	DeleteFile(tempFile.c_str());

	scrollForm->ResizeScrollArea(scrollForm->GetWidth(), heightUsed);

	while (feedReadCounter < SHOW_COUNT)
	{
		controlGroup->at(feedReadCounter)->Visible(false);
		++feedReadCounter;
	}

	return true;
}