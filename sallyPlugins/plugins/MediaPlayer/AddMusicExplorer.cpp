////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\AddMusicExplorer.cpp
///
/// \brief	Implements the add music explorer class. 
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

#include "AddMusicExplorer.h"

CAddMusicExplorer::CAddMusicExplorer(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, CPlaylist* playlist)
	:SallyAPI::GUI::CForm(parent, 0, -WINDOW_HEIGHT, WINDOW_WIDTH - MENU_WIDTH, WINDOW_HEIGHT),
	m_pPlaylist(playlist)
{
	m_pFileBrowser = new SallyAPI::GUI::CFileBrowser(this, WINDOW_BORDER_H, WINDOW_BORDER_V,
		WINDOW_WIDTH - MENU_WIDTH - (WINDOW_BORDER_H * 2), WINDOW_HEIGHT - WINDOW_BORDER_V - WINDOW_BORDER_V);
	this->AddChild(m_pFileBrowser);

	std::map<std::string, int>	mimetypeList;

	// Video
	std::vector<std::string> fileExtensionsVideo = CVideoFile::GetFileExtensions();
	std::vector<std::string>::iterator iterVideo = fileExtensionsVideo.begin();

	while (iterVideo != fileExtensionsVideo.end())
	{
		std::string end = *iterVideo;
		mimetypeList[end] = GUI_THEME_SALLY_ICON_MIMETYPE_VIDEO;
		++iterVideo;
	}


	// Audio
	std::vector<std::string> fileExtensionsAudio = CAudioFile::GetFileExtensions();
	std::vector<std::string>::iterator iterAudio= fileExtensionsAudio.begin();

	while (iterAudio != fileExtensionsAudio.end())
	{
		std::string end = *iterAudio;
		mimetypeList[end] = GUI_THEME_SALLY_ICON_MIMETYPE_MP3;
		++iterAudio;
	}

	m_pFileBrowser->SetShowRemovableDisk(true);
	m_pFileBrowser->SetShowSubfolders(true);
	m_pFileBrowser->SetMimetypeList(mimetypeList);
	m_pFileBrowser->SetActionButtonCommand(GUI_APP_ADD_ALL_CURRENT_FOLDER);
	m_pFileBrowser->SetActionButtonImage(GUI_THEME_SALLY_ICON_ADD);
	m_pFileBrowser->SetActionButtonText("Add All");

	m_pMainApp = (dynamic_cast<SallyAPI::GUI::CAppBase*> (m_pParent));
}

CAddMusicExplorer::~CAddMusicExplorer()
{
}

void CAddMusicExplorer::LoadConfig()
{
	bool hd = m_pMainApp->GetPropertyBool("alwaysShowHds", true);
	m_pFileBrowser->SetShowHardDisks(hd);

	m_pFileBrowser->SetStartFolders(GenerateFolderList());

	std::string explorerPath = m_pMainApp->GetPropertyString("explorerPath", "");
	int explorerPathDepth = m_pMainApp->GetPropertyInt("explorerPathDepth", -1);
	if (explorerPath.length() > 0)
		m_pFileBrowser->SetFolder(explorerPath, explorerPathDepth);
	else
		m_pFileBrowser->Reset();
}

void CAddMusicExplorer::SaveConfig()
{
	m_pMainApp->SetPropertyString("explorerPath", m_pFileBrowser->GetCurrentFolder());
	m_pMainApp->SetPropertyInt("explorerPathDepth", m_pFileBrowser->GetCurrentFolderDepth());
}

std::vector<std::string> CAddMusicExplorer::GenerateFolderList()
{
	std::vector<std::string> list;

	for (int i = 0; i < 12; i++)
	{
		std::string dir;

		dir.append("dir");
		dir.append(SallyAPI::String::StringHelper::ConvertToString(i));
		dir.append(".path");

		std::string temp = m_pMainApp->GetPropertyString(dir);

		if (temp.length() > 0)
		{
			list.push_back(temp);
		}
	}
	return list;
}

void CAddMusicExplorer::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int iMessageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (iMessageId)
	{
	case GUI_LISTVIEW_ITEM_DOUBLECLICKED:
		if (reporter == m_pFileBrowser)
		{
			OnCommandDoubleClicked(messageParameter);
		}
		return;
	case GUI_LISTVIEW_ITEM_CLICKED:
		if (reporter == m_pFileBrowser)
		{
			AddToPlaylistFromFilebrowserItem(messageParameter);
		}
		return;
	case GUI_LISTVIEW_ITEM_ACTION_CLICKED:
		if (reporter == m_pFileBrowser)
		{
			AddToPlaylistFromFilebrowser(messageParameter);
		}
		return;
	}

	switch(reporterId)
	{
	case GUI_APP_ADD_ALL_CURRENT_FOLDER:
		OnCommandAddAllFromExplorer();
		return;
	}
	SallyAPI::GUI::CForm::SendMessageToParent(reporter, reporterId, iMessageId, messageParameter);
}

void CAddMusicExplorer::OnCommandDoubleClicked(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	SallyAPI::GUI::SendMessage::CParameterListItem* parameter = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterListItem*> (messageParameter);

	SallyAPI::GUI::CListViewExt* listView = m_pFileBrowser->GetListView();
	SallyAPI::GUI::CListViewItem* listItem = listView->GetItem(parameter->GetItem());

	// folder was clicked
	if (SallyAPI::File::FileHelper::IsDirectory(listItem->GetIdentifier()))
		return;

	SallyAPI::GUI::SendMessage::CParameterString playNow(listItem->GetIdentifier());
	m_pParent->SendMessageToParent(this, 0, GUI_APP_PLAY_LAST_ADDED, &playNow);
}

void CAddMusicExplorer::OnCommandAddAllFromExplorer()
{
	SallyAPI::GUI::CListViewExt* listView = m_pFileBrowser->GetListView();
	std::string folder = m_pFileBrowser->GetCurrentFolder();

	m_tAddToPlaylist.SetValues(listView, m_pPlaylist, this, folder);
	m_pParent->SendMessageToParent(this, 0, MS_SALLY_SHOW_WORKING);
	m_tAddToPlaylist.Start();
}

void CAddMusicExplorer::AddToPlaylistFromFilebrowserItem(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	SallyAPI::GUI::SendMessage::CParameterListItem* parameter = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterListItem*> (messageParameter);

	if (parameter == NULL)
		return;

	SallyAPI::GUI::CListViewExt* listView = m_pFileBrowser->GetListView();
	SallyAPI::GUI::CListViewItem* listItem = listView->GetItem(parameter->GetItem());

	// folder was clicked
	if (SallyAPI::File::FileHelper::IsDirectory(listItem->GetIdentifier()))
		return;

	int imageIndex = 1;
	if (CAudioFile::IsAudioFile(listItem->GetIdentifier()))
		imageIndex = 0;

	SallyAPI::GUI::CListViewItem listItemTemp(listItem->GetIdentifier(), listItem->GetText(1), imageIndex);

	if (m_pPlaylist->AddItem(listItemTemp) == false)
		return;

	SallyAPI::GUI::SendMessage::CParameterNotificationOSM messageOnScreenMenu(GUI_THEME_SALLY_OSM_ADD, "Added");
	m_pParent->SendMessageToParent(this, 0, MS_SALLY_NOTIFICATION_OSM, &messageOnScreenMenu);
}

void CAddMusicExplorer::AddToPlaylistFromFilebrowser(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	SallyAPI::GUI::SendMessage::CParameterListItem* parameter = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterListItem*> (messageParameter);

	if (parameter == NULL)
		return;

	SallyAPI::GUI::CListViewExt* listView = m_pFileBrowser->GetListView();
	SallyAPI::GUI::CListViewItem* listItem = listView->GetItem(parameter->GetItem());

	if (SallyAPI::File::FileHelper::IsDirectory(listItem->GetIdentifier()))
	{
		// folder clicked
		std::string folder = listItem->GetIdentifier();

		m_tAddToPlaylist.SetValues(listView, m_pPlaylist, this, folder);
		m_pParent->SendMessageToParent(this, 0, MS_SALLY_SHOW_WORKING);
		m_tAddToPlaylist.Start();		
	}
	else
	{
		AddToPlaylistFromFilebrowserItem(messageParameter);
	}
	return;
}