////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\PlaylistManager.cpp
///
/// \brief	Implements the playlist manager class. 
///
/// \author	Christian Knobloch
/// \date	29.11.2010
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

#include "PlaylistManager.h"

bool DateCompare(const std::string &left, const std::string &right)
{
	std::string dateLeft = SallyAPI::File::FileHelper::GetFormatedFileCreateDate(left);
	std::string dateRight = SallyAPI::File::FileHelper::GetFormatedFileCreateDate(right);

	for(std::string::const_iterator lit = dateLeft.begin(), rit = dateRight.begin(); lit != dateLeft.end() && rit != dateRight.end(); ++lit, ++rit )
	{
		if(tolower(*lit) > tolower(*rit) )
			return true;
		else if(tolower(*lit) < tolower(*rit) )
			return false;
	}
	if(dateLeft.size() > dateRight.size())
		return true;
	return false;
}

CPlaylistManager::CPlaylistManager(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, CPlaylist* playlist)
	:SallyAPI::GUI::CForm(parent, 0, -WINDOW_HEIGHT, WINDOW_WIDTH - MENU_WIDTH, WINDOW_HEIGHT),
	m_pPlaylist(playlist), m_pListViewToDeleteFrom(NULL)
{
	m_pMenu = new SallyAPI::GUI::CButtonBar(this, WINDOW_BORDER_H, WINDOW_BORDER_V, 500);
	this->AddChild(m_pMenu);

	m_pMenuPlaylist = new SallyAPI::GUI::CButtonBarButton(m_pMenu, 250, GUI_APP_MY_PLAYLISTS);
	m_pMenuPlaylist->SetText("My Playlists");
	m_pMenuPlaylist->SetImageId(GUI_THEME_SALLY_ICON_FOLDER);
	m_pMenuPlaylist->SetCheckStatus(true);
	m_pMenu->AddChild(m_pMenuPlaylist);

	m_pMenuAutoPlaylist = new SallyAPI::GUI::CButtonBarButton(m_pMenu, 250, GUI_APP_AUTO_PLAYLISTS);
	m_pMenuAutoPlaylist->SetText("Auto Playlists");
	m_pMenuAutoPlaylist->SetImageId(GUI_THEME_SALLY_ICON_MEDIA_PLAY);
	m_pMenu->AddChild(m_pMenuAutoPlaylist);

	m_pButtonClear = new SallyAPI::GUI::CButton(this, WINDOW_BORDER_H, WINDOW_BORDER_V + CONTROL_HEIGHT + 10, CONTROL_HEIGHT, CONTROL_HEIGHT, GUI_APP_CLEAR_TEXT_SEARCH);
	m_pButtonClear->SetImageId(GUI_THEME_SALLY_KEYBOARD_CLEAR);
	this->AddChild(m_pButtonClear);

	m_pEditSearch = new SallyAPI::GUI::CEdit(this, WINDOW_BORDER_H + CONTROL_HEIGHT + 10, WINDOW_BORDER_V + CONTROL_HEIGHT + 10, 330, GUI_APP_SEARCH);
	m_pEditSearch->SetInfoText("click here to search");
	this->AddChild(m_pEditSearch);

	m_pMenuSort = new SallyAPI::GUI::CButtonBar(this, WINDOW_WIDTH - MENU_WIDTH - WINDOW_BORDER_V - 60, WINDOW_BORDER_V + CONTROL_HEIGHT + 10, 60);
	this->AddChild(m_pMenuSort);

	m_pMenuSortName = new SallyAPI::GUI::CButtonBarButton(m_pMenuSort, 30, GUI_APP_SORT_BY_NAME);
	m_pMenuSortName->SetImageId(GUI_THEME_SALLY_ICON_MIMETYPE_TEXT);
	m_pMenuSortName->SetCheckStatus(true);
	m_pMenuSort->AddChild(m_pMenuSortName);

	m_pMenuSortDate = new SallyAPI::GUI::CButtonBarButton(m_pMenuSort, 30, GUI_APP_SORT_BY_DATE);
	m_pMenuSortDate->SetImageId(GUI_THEME_SALLY_ICON_DATE);
	m_pMenuSort->AddChild(m_pMenuSortDate);

	/************************************************************************/
	/* Playlist Browser                                                     */
	/************************************************************************/
	std::map<int, int> columns;

	columns[0] = 100;
	columns[1] = 100;
	columns[2] = 0;
	columns[3] = 120;
	
	m_pFileBrowserPlaylist = new SallyAPI::GUI::CListViewExt(this,
		WINDOW_BORDER_H,
		WINDOW_BORDER_V + ((CONTROL_HEIGHT + 10) * 2),
		WINDOW_WIDTH - MENU_WIDTH - (WINDOW_BORDER_H * 2),
		WINDOW_HEIGHT - WINDOW_BORDER_V - WINDOW_BORDER_V - ((CONTROL_HEIGHT + 10) * 2),
		4, columns);
	this->AddChild(m_pFileBrowserPlaylist);

	m_pFileBrowserAutoPlaylist = new SallyAPI::GUI::CListViewExt(this,
		WINDOW_BORDER_H,
		WINDOW_BORDER_V + ((CONTROL_HEIGHT + 10) * 2),
		WINDOW_WIDTH - MENU_WIDTH - (WINDOW_BORDER_H * 2),
		WINDOW_HEIGHT - WINDOW_BORDER_V - WINDOW_BORDER_V - ((CONTROL_HEIGHT + 10) * 2),
		4, columns);
	m_pFileBrowserAutoPlaylist->Visible(false);
	this->AddChild(m_pFileBrowserAutoPlaylist);

	/************************************************************************/
	m_pTSAddToList = new SallyAPI::GUI::CThreadStarter(this, GUI_LISTVIEW_ITEM_CLICKED, GUI_BUTTON_CLICKED);

	ReloadFileList();
}

CPlaylistManager::~CPlaylistManager()
{
	m_pTSAddToList->WaitForStop();
	
	SafeDelete(m_pTSAddToList);
}

void CPlaylistManager::ReloadFileList()
{
	// Generate Folders
	std::string playlistName = SallyAPI::System::SallyHelper::GetMediaDirectory(dynamic_cast<SallyAPI::GUI::CAppBase*> (m_pParent));
	playlistName.append("Playlist\\");

	OpenFolder(m_pFileBrowserPlaylist, playlistName);

	// Generate Folders
	playlistName = SallyAPI::System::SallyHelper::GetMediaDirectory(dynamic_cast<SallyAPI::GUI::CAppBase*> (m_pParent));
	playlistName.append("AutoPlaylist\\");

	OpenFolder(m_pFileBrowserAutoPlaylist, playlistName);
}

void CPlaylistManager::OpenFolder(SallyAPI::GUI::CListViewExt* listView, std::string folder)
{
	std::vector<std::string> files;

	HANDLE				hFile;
	WIN32_FIND_DATA		FileInformation;

	listView->Clear();

	std::string firstFile;

	folder = SallyAPI::String::PathHelper::CorrectPath(folder);

	firstFile.append(folder);
	if (m_pEditSearch->GetText().length() > 0)
	{
		firstFile.append("*");
		firstFile.append(m_pEditSearch->GetText());
	}
	firstFile.append("*.m3u");

	hFile = FindFirstFile(firstFile.c_str(), &FileInformation);

	if(hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			if ((strcmp(FileInformation.cFileName, ".") != NULL) &&
				(strcmp(FileInformation.cFileName, "..") != NULL))
			{
				std::string filename;
				filename.append(folder);
				filename.append(FileInformation.cFileName);

				if (!(FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					files.push_back(filename);
				}
			}
		} while(FindNextFile(hFile, &FileInformation) == TRUE);
	}
	FindClose(hFile);

	if (m_pMenuSortName->GetCheckStatus())
		std::sort(files.begin(), files.end(), SallyAPI::String::StringHelper::StringCompareCaseInsensitivity);
	else
		std::sort(files.begin(), files.end(), DateCompare);

	std::vector<std::string>::iterator	filesIterator = files.begin();
	while (filesIterator != files.end())
	{
		std::string filename = (*filesIterator);

		SallyAPI::GUI::CListViewItem listItem(filename);

		listItem.SetText("Delete", 0);
		listItem.SetText("Add", 1);
		listItem.SetText(SallyAPI::String::PathHelper::GetFileFromPath(filename), 2);
		listItem.SetText(SallyAPI::File::FileHelper::GetFormatedFileCreateDate(filename), 3);

		listItem.SetImageId(GUI_THEME_SALLY_ICON_DELETE, 0);
		listItem.SetImageId(GUI_THEME_SALLY_ICON_ADD, 1);
		listItem.SetImageId(GUI_THEME_SALLY_ICON_MIMETYPE_MP3, 2);
		listItem.SetImageId(GUI_NO_IMAGE, 3);

		listItem.SetLocalised(SallyAPI::GUI::LISTVIEW_LOCALISATION_TRUE, 0);
		listItem.SetLocalised(SallyAPI::GUI::LISTVIEW_LOCALISATION_TRUE, 1);
		listItem.SetLocalised(SallyAPI::GUI::LISTVIEW_LOCALISATION_FALSE, 2);
		listItem.SetLocalised(SallyAPI::GUI::LISTVIEW_LOCALISATION_FALSE, 3);

		listItem.SetSmallFont(true, 0);
		listItem.SetSmallFont(true, 1);
		listItem.SetSmallFont(false, 2);
		listItem.SetSmallFont(true, 3);

		listView->AddItem(listItem);

		++filesIterator;
	}
	return;
}

void CPlaylistManager::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_BUTTON_CLICKED:
		switch (reporterId)
		{
		case GUI_LISTVIEW_ITEM_CLICKED:
			AddListThread();
			return;
		case GUI_APP_MY_PLAYLISTS:
			m_pMenuPlaylist->SetCheckStatus(true);
			m_pMenuAutoPlaylist->SetCheckStatus(false);
			m_pFileBrowserPlaylist->Visible(true);
			m_pFileBrowserAutoPlaylist->Visible(false);
			return;
		case GUI_APP_AUTO_PLAYLISTS:
			m_pMenuPlaylist->SetCheckStatus(false);
			m_pMenuAutoPlaylist->SetCheckStatus(true);
			m_pFileBrowserPlaylist->Visible(false);
			m_pFileBrowserAutoPlaylist->Visible(true);
			return;
		case GUI_APP_CLEAR_TEXT_SEARCH:
			m_pEditSearch->SetText(""); // will trigger the search
			return;
		case GUI_APP_SORT_BY_NAME:
			m_pMenuSortName->SetCheckStatus(true);
			m_pMenuSortDate->SetCheckStatus(false);
			ReloadFileList();
			return;
		case GUI_APP_SORT_BY_DATE:
			m_pMenuSortName->SetCheckStatus(false);
			m_pMenuSortDate->SetCheckStatus(true);
			ReloadFileList();
			return;
		}
		break;
	case GUI_LISTVIEW_ITEM_CLICKED:
		OnCommandListViewClicked(reporter, messageParameter);
		return;
	case GUI_EDIT_CHANGED:
		switch (reporterId)
		{
		case GUI_APP_SEARCH:
			ReloadFileList();
			return;
		}
		break;
	case MS_DIALOG_YES:
		if (reporterId == GUI_APP_DELETE_PLAYLIST)
		{
			OnCommandDeleteFile();
		}
		return;
	}
	SallyAPI::GUI::CForm::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

void CPlaylistManager::OnCommandDeleteFile()
{
	SallyAPI::GUI::CListViewItem* listItem = m_pListViewToDeleteFrom->GetItem(m_iItemToDelete);
	if (listItem == NULL)
		return;

	DeleteFile(listItem->GetIdentifier().c_str());

	m_pListViewToDeleteFrom->RemoveItem(m_iItemToDelete + 1);
}

void CPlaylistManager::OnCommandListViewClicked(SallyAPI::GUI::CGUIBaseObject* reporter, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	SallyAPI::GUI::SendMessage::CParameterListItem* parameterListItem = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterListItem*> (messageParameter);

	if (parameterListItem == NULL)
		return;

	// delete?
	if (parameterListItem->GetButton() == 0)
	{
		if (reporter == m_pFileBrowserPlaylist)
			OnCommandDeletePlaylist(messageParameter, m_pFileBrowserPlaylist);
		else if (reporter == m_pFileBrowserAutoPlaylist)
			OnCommandDeletePlaylist(messageParameter, m_pFileBrowserAutoPlaylist);
	}
	else
	{
		if (reporter == m_pFileBrowserPlaylist)
			OnCommandAddList(messageParameter, m_pFileBrowserPlaylist, false);
		else if (reporter == m_pFileBrowserAutoPlaylist)
			OnCommandAddList(messageParameter, m_pFileBrowserAutoPlaylist, true);
	}
}

void CPlaylistManager::OnCommandAddList(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter, SallyAPI::GUI::CListViewExt* listView, bool setAutoPlaylistName)
{
	SallyAPI::GUI::SendMessage::CParameterListItem* parameterListItem = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterListItem*> (messageParameter);

	if (parameterListItem == NULL)
		return;

	SallyAPI::GUI::CListViewItem* listItem = listView->GetItem(parameterListItem->GetItem());
	if (listItem == NULL)
		return;

	m_strListToLoad = listItem->GetIdentifier();
	m_bSetAutoPlaylistName = setAutoPlaylistName;

	m_pParent->SendMessageToParent(this, 0, MS_SALLY_SHOW_WORKING);

	m_pTSAddToList->Start();
}

void CPlaylistManager::AddListThread()
{
	m_pPlaylist->LoadPlaylist(m_strListToLoad);

	SallyAPI::GUI::SendMessage::CParameterOnScreenMenu messageOnScreenMenu(GUI_THEME_SALLY_OSM_ADD, "Added");
	m_pParent->SendMessageToParent(this, 0, MS_SALLY_ON_SCREEN_MENU, &messageOnScreenMenu);

	if (m_bSetAutoPlaylistName)
	{
		// set the auto playlist name
		m_pPlaylist->SetAutoPlaylistName(m_strListToLoad);
	}

	m_pParent->SendMessageToParent(this, 0, MS_SALLY_HIDE_WORKING);
}

void CPlaylistManager::OnCommandDeletePlaylist(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter, SallyAPI::GUI::CListViewExt* listView)
{
	SallyAPI::GUI::SendMessage::CParameterListItem* parameterListItem = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterListItem*> (messageParameter);

	if (parameterListItem == NULL)
		return;

	SallyAPI::GUI::CListViewItem* listItem = listView->GetItem(parameterListItem->GetItem());
	if (listItem == NULL)
		return;

	m_pListViewToDeleteFrom = listView;
	m_iItemToDelete = parameterListItem->GetItem();

	// Overwrite?
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::Config::CLanguageManager* languageManager = config->GetLanguageLocalization();

	std::string infoMessage = languageManager->GetString("Do you really want to delete the '%s' playlist?", listItem->GetText(2).c_str(), NULL);

	SallyAPI::GUI::SendMessage::CParameterQuestionBox questionBox(GUI_APP_DELETE_PLAYLIST, this, infoMessage);
	m_pParent->SendMessageToParent(this, m_iControlId, MS_SALLY_SHOW_QUESTIONBOX, &questionBox);

}