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
	m_pApplicationImage = new SallyAPI::GUI::CImageBox(this, WINDOW_BORDER_H, 10, 128, 128);
	m_pApplicationImage->SetImageId(GetGraphicId());
	this->AddChild(m_pApplicationImage);

	m_pWelcome = new SallyAPI::GUI::CLabel(this, WINDOW_BORDER_H + 128 + 10, 10, 300);
	m_pWelcome->SetBold(true);
	m_pWelcome->SetLocalised(false);
	this->AddChild(m_pWelcome);

	m_pNewsInfo = new SallyAPI::GUI::CLabel(this, WINDOW_BORDER_H + 128 + 10, 10 + CONTROL_HEIGHT + 10, 300);
	m_pNewsInfo->SetText("Latest news:");
	this->AddChild(m_pNewsInfo);

	m_pNewsMessage = new SallyAPI::GUI::CLabelBox(this, WINDOW_BORDER_H + 128 + 10, 
		10 + CONTROL_HEIGHT + 10 + CONTROL_HEIGHT, WINDOW_WIDTH - (MENU_WIDTH + 180), 58);
	this->AddChild(m_pNewsMessage);

	m_iShowRows = (WINDOW_HEIGHT - 186) / (CONTROL_GROUP_HEIGHT + 10);
	m_iShowCols = (WINDOW_WIDTH - 10) / (CONTROL_GROUP_WIDTH + 10);

	int showRowsDelta = ((WINDOW_HEIGHT - 186) % (CONTROL_GROUP_HEIGHT + 10)) / 2;
	int showColsDelta = ((WINDOW_WIDTH - 10) % (CONTROL_GROUP_WIDTH + 10));

	showColsDelta = showColsDelta / m_iShowCols;

	m_iShowCount = m_iShowRows * m_iShowCols;

	int i = 0;
	for (int j = 0; j < m_iShowCols; ++j)
	{
		for (int k = 0; k < m_iShowRows; ++k)
		{
			CControlGroup* temp = new CControlGroup(this, 10 + ((10 + CONTROL_GROUP_WIDTH + showColsDelta) * j),
				showRowsDelta + 186 + (k * (CONTROL_GROUP_HEIGHT + 10)), showColsDelta + CONTROL_GROUP_WIDTH);
			this->AddChild(temp);

			m_vControlGroup.push_back(temp);			
			++i;
		}
	}

	// to the the community status updates
	SallyAPI::Community::CCommunityManager* communityManager = SallyAPI::Community::CCommunityManager::GetInstance();
	communityManager->RegisterStatusUpdateNotifier(this);
}

CAppCommunity::~CAppCommunity()
{
	DeleteOldImages();
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
	case MS_SALLY_APP_LOAD_COMPLETE:
	case MS_SALLY_COMMUNITY_STATUS_UPDATE:
		OnCommandUpdateStatus();
		return;
	}
	CApplicationWindow::SendMessageToParent(reporter, reporterId, iMessageID, messageParameter);
}

void CAppCommunity::DeleteOldImages()
{
	EnterRenderLock();

	std::map<std::string, SallyAPI::GUI::CPicture*>::iterator iter = m_pPictures.begin();

	while (iter != m_pPictures.end())
	{
		SallyAPI::GUI::CPicture* picture = iter->second;

		SafeDelete(picture);
		++iter;
	}
	m_pPictures.clear();

	LeaveRenderLock();
}

void CAppCommunity::OnCommandUpdateView()
{
	SallyAPI::Community::CCommunityManager* communityManager = SallyAPI::Community::CCommunityManager::GetInstance();
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::Config::CLanguageManager* lang = config->GetLanguageLocalization();
	SallyAPI::System::COption* option = config->GetOption();

	if (!communityManager->IsEnabled())
	{
		m_pNewsInfo->Visible(false);
		m_pNewsMessage->SetFont("attention.font");
		m_pNewsMessage->SetText("Sally community is not enabled.");
		return;
	}

	// now get the latest news
	m_pNewsInfo->Visible(true);
	std::string message = lang->GetString("Welcome %s", communityManager->GetUsername().c_str(), NULL);
	m_pWelcome->SetText(message);

	if (m_pNewsMessage->GetText().compare("Sally community is not enabled.") == 0)
	{
		m_pNewsMessage->SetText("");
	}
}

void CAppCommunity::OnCommandGetNews()
{
	SallyAPI::Community::CCommunityManager* communityManager = SallyAPI::Community::CCommunityManager::GetInstance();
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::Config::CLanguageManager* lang = config->GetLanguageLocalization();
	SallyAPI::System::COption* option = config->GetOption();

	// download News
	std::string errorMessage;
	std::map<std::string, std::string> requestMap;

	requestMap["isoCode"] = option->GetPropertyString("sally", "language", "EN");

	std::string requestResult = communityManager->RequestData("getNews", requestMap, errorMessage);

	if ((errorMessage.length() != 0) || (requestResult.length() == 0))
	{
		m_pNewsInfo->Visible(false);
		m_pNewsMessage->SetFont("attention.font");
		m_pNewsMessage->SetText(errorMessage);
		return;
	}
	m_pNewsInfo->Visible(true);

	std::string tempFile = SallyAPI::System::SallyHelper::GetMediaDirectory(this);
	tempFile.append("communityNews.xml");

	DeleteFile(tempFile.c_str());

	SallyAPI::File::FileHelper::AddLineToFile(tempFile, requestResult);

	XMLNode xMainNode = XMLNode::openFileHelper(tempFile.c_str());
	if (xMainNode.isEmpty())
		return;

	XMLNode sallycommunity = xMainNode.getChildNode("sallycommunity");
	if (sallycommunity.isEmpty())
		return;

	// create
	XMLNode messageXML;
	int i = 0;
	do
	{
		messageXML = sallycommunity.getChildNode("message", i);

		if (!messageXML.isEmpty())
		{
			const char* messageChar = messageXML.getText();

			if (messageChar != NULL)
			{
				m_pNewsInfo->Visible(true);
				m_pNewsMessage->SetFont("");
				m_pNewsMessage->SetText(messageChar);
			}
		}
		++i;
	}
	while (!messageXML.isEmpty());

	// cleanup
	DeleteFile(tempFile.c_str());
}

void CAppCommunity::OnCommandUpdateStatus()
{
	OnCommandUpdateView();
	OnCommandGetNews();

	SallyAPI::Community::CCommunityDB* communityDB = SallyAPI::Community::CCommunityDB::GetInstance();
	
	std::vector<SallyAPI::Community::CStatusMessage> status = communityDB->GetLastMessages(m_iShowCount);
	std::vector<SallyAPI::Community::CStatusMessage>::iterator iter = status.begin();

	std::map<std::string, SallyAPI::GUI::CPicture*>	m_pPicturesNew;

	int i = 0;
	while (iter != status.end())
	{
		SallyAPI::Community::CStatusMessage statu = *iter;

		SallyAPI::GUI::SendMessage::CParameterApplicationInfo applicationInfo(statu.GetExplicidAppName());
		SendMessageToParent(this, 0, MS_SALLY_GET_APPLICATION_INFO, &applicationInfo);

		if (statu.GetAvatar().length() > 0)
		{
			// we have already the image loaded
			if (m_pPicturesNew[statu.GetAvatar()] != NULL)
			{
				SallyAPI::GUI::CPicture* picture = m_pPicturesNew[statu.GetAvatar()];
				m_vControlGroup.at(i)->SetPicture(picture);
			}
			else
			{
				std::string imageFile = SallyAPI::Core::CGame::GetMediaFolder();
				imageFile.append("Community\\");
				imageFile.append(statu.GetAvatar());

				if (SallyAPI::File::FileHelper::FileExists(imageFile))
				{
					SallyAPI::GUI::CPicture* picture = new SallyAPI::GUI::CPicture();
					if (picture->LoadTexture(imageFile))
					{
						m_pPicturesNew[statu.GetAvatar()] = picture;
						m_vControlGroup.at(i)->SetPicture(picture);
					}
					else
					{
						// error while loading file
						SafeDelete(picture);
						m_vControlGroup.at(i)->SetImageId(GUI_THEME_SALLY_COMMUNITY);
					}
				}
			}
		}
		else
		{
			m_vControlGroup.at(i)->SetImageId(GUI_THEME_SALLY_COMMUNITY);
		}

		m_vControlGroup.at(i)->Visible(true);
		m_vControlGroup.at(i)->SetValue(statu.GetNickName(), statu.GetMessageString(), statu.GetAction(),
			statu.GetActionName(), applicationInfo.GetWindow());
		++iter;
		++i;
	}
	while (i < m_iShowCount)
	{
		m_vControlGroup.at(i)->Visible(false);
		++i;
	}

	// delete old images
	DeleteOldImages();

	m_pPictures = m_pPicturesNew;
}