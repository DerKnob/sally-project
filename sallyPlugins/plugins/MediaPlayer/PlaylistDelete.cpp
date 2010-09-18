////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\PlaylistDelete.cpp
///
/// \brief	Implements the playlist delete class. 
///
/// \author	Christian Knobloch
/// \date	13.09.2010
///
/// This file is part of the Sally Project
/// 
/// Copyright(c) 2008-2010 Sally Project
/// http://www.sally-project.de/
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

#include "PlaylistDelete.h"

CPlaylistDelete::CPlaylistDelete(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, CPlaylist* playlist)
	:SallyAPI::GUI::CForm(parent, 0, -WINDOW_HEIGHT, WINDOW_WIDTH - MENU_WIDTH, WINDOW_HEIGHT),
	m_pPlaylist(playlist)
{
	m_pMenu = new SallyAPI::GUI::CButtonBar(this, (WINDOW_WIDTH - MENU_WIDTH - 400) / 2, WINDOW_BORDER_V, 400);
	this->AddChild(m_pMenu);

	m_pMenuPlaylist = new SallyAPI::GUI::CButtonBarButton(m_pMenu, 200, GUI_APP_MY_PLAYLISTS);
	m_pMenuPlaylist->SetText("My Playlists");
	m_pMenuPlaylist->SetImageId(GUI_THEME_SALLY_ICON_FOLDER);
	m_pMenuPlaylist->SetCheckStatus(true);
	m_pMenu->AddChild(m_pMenuPlaylist);

	m_pMenuAutoPlaylist = new SallyAPI::GUI::CButtonBarButton(m_pMenu, 200, GUI_APP_AUTO_PLAYLISTS);
	m_pMenuAutoPlaylist->SetText("Auto Playlists");
	m_pMenuAutoPlaylist->SetImageId(GUI_THEME_SALLY_ICON_MEDIA_PLAY);
	m_pMenu->AddChild(m_pMenuAutoPlaylist);

	m_pLableDialogText = new SallyAPI::GUI::CLabelBox(this, WINDOW_BORDER_H, WINDOW_BORDER_V + 40,
		WINDOW_WIDTH - MENU_WIDTH - (WINDOW_BORDER_H * 2), 70);
	m_pLableDialogText->SetLocalised(true);
	m_pLableDialogText->SetText("Please select the playlist you want to delete:");
	m_pLableDialogText->SetLocalised(true);
	this->AddChild(m_pLableDialogText);

	/************************************************************************/
	/* Playlist Browser                                                     */
	/************************************************************************/
	m_pFileBrowserPlaylist = new SallyAPI::GUI::CFileBrowser(this, WINDOW_BORDER_H,
		WINDOW_BORDER_V + 40 + 60,
		WINDOW_WIDTH - MENU_WIDTH - (WINDOW_BORDER_H * 2),
		WINDOW_HEIGHT - WINDOW_BORDER_V - WINDOW_BORDER_V - 40 - 60);
	this->AddChild(m_pFileBrowserPlaylist);

	// Genereate Mimetypes
	std::map<std::string, int> playlistMimetypeList;
	playlistMimetypeList[".m3u"] = GUI_THEME_SALLY_ICON_MIMETYPE_MP3;

	// Generate Folders
	std::string playlistName = SallyAPI::System::SallyHelper::GetMediaDirectory(dynamic_cast<SallyAPI::GUI::CAppBase*> (m_pParent));
	playlistName.append("Playlist\\");

	std::vector<std::string> foldersPlaylist;
	foldersPlaylist.push_back(playlistName);

	m_pFileBrowserPlaylist->SetActionImageId(GUI_THEME_SALLY_ICON_REMOVE);
	m_pFileBrowserPlaylist->SetMimetypeList(playlistMimetypeList);
	m_pFileBrowserPlaylist->SetShowRemovableDisk(false);
	m_pFileBrowserPlaylist->SetStartFolders(foldersPlaylist);
	m_pFileBrowserPlaylist->SetShowSubfolders(false);
	m_pFileBrowserPlaylist->Reset();

	/************************************************************************/
	/* Auto Playlist Browser                                                */
	/************************************************************************/
	m_pFileBrowserAutoPlaylist = new SallyAPI::GUI::CFileBrowser(this, WINDOW_BORDER_H,
		WINDOW_BORDER_V + 40 + 60,
		WINDOW_WIDTH - MENU_WIDTH - (WINDOW_BORDER_H * 2),
		WINDOW_HEIGHT - WINDOW_BORDER_V - WINDOW_BORDER_V - 40 - 60);
	m_pFileBrowserAutoPlaylist->Visible(false);
	this->AddChild(m_pFileBrowserAutoPlaylist);

	// Genereate Mimetypes
	std::map<std::string, int> autoPlaylistMimetypeList;
	autoPlaylistMimetypeList[".m3u"] = GUI_THEME_SALLY_ICON_MIMETYPE_MP3;

	// Generate Folders
	std::string autoPlaylistName = SallyAPI::System::SallyHelper::GetMediaDirectory(dynamic_cast<SallyAPI::GUI::CAppBase*> (m_pParent));
	autoPlaylistName.append("AutoPlaylist\\");

	std::vector<std::string> foldersAutoPlaylist;
	foldersAutoPlaylist.push_back(autoPlaylistName);

	m_pFileBrowserAutoPlaylist->SetActionImageId(GUI_THEME_SALLY_ICON_REMOVE);
	m_pFileBrowserAutoPlaylist->SetMimetypeList(autoPlaylistMimetypeList);
	m_pFileBrowserAutoPlaylist->SetShowRemovableDisk(false);
	m_pFileBrowserAutoPlaylist->SetStartFolders(foldersAutoPlaylist);
	m_pFileBrowserAutoPlaylist->SetShowSubfolders(false);
	m_pFileBrowserAutoPlaylist->Reset();
}

CPlaylistDelete::~CPlaylistDelete()
{
}

void CPlaylistDelete::Reload()
{
	m_pFileBrowserPlaylist->Reset();
	m_pFileBrowserAutoPlaylist->Reset();
}

void CPlaylistDelete::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_BUTTON_CLICKED:
		switch (reporterId)
		{
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
		}
		break;
	case GUI_LISTVIEW_ITEM_CLICKED:
	case GUI_LISTVIEW_ITEM_ACTION_CLICKED:
		if (reporter == m_pFileBrowserPlaylist)
			OnCommandDeletePlaylist(messageParameter, m_pFileBrowserPlaylist);
		else if (reporter == m_pFileBrowserAutoPlaylist)
			OnCommandDeletePlaylist(messageParameter, m_pFileBrowserAutoPlaylist);
		return;
	case MS_DIALOG_YES:
		if (reporterId == GUI_APP_DELETE_PLAYLIST)
		{
			DeleteFile(m_strPlaylistName.c_str());
			m_pFileBrowserPlaylist->Reset();
			m_pFileBrowserAutoPlaylist->Reset();
		}
		return;
	}
	SallyAPI::GUI::CForm::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

void CPlaylistDelete::OnCommandDeletePlaylist(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter, SallyAPI::GUI::CFileBrowser* fileBrowser)
{
	SallyAPI::GUI::SendMessage::CParameterInteger* parameterInteger = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterInteger*> (messageParameter);

	SallyAPI::GUI::CListView* listView = fileBrowser->GetListView();
	SallyAPI::GUI::CListViewItem* listItem = listView->GetItem(parameterInteger->GetInteger());
	if (listItem == NULL)
		return;

	m_strPlaylistName = listItem->GetIdentifier();

	// Overwrite?
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::Config::CLanguageManager* languageManager = config->GetLanguageLocalization();

	std::string infoMessage = languageManager->GetString("Do you really want to delete the '%s' playlist?", listItem->GetText().c_str(), NULL);

	SallyAPI::GUI::SendMessage::CParameterQuestionBox questionBox(GUI_APP_DELETE_PLAYLIST, this, infoMessage);
	m_pParent->SendMessageToParent(this, m_iControlId, MS_SALLY_SHOW_QUESTIONBOX, &questionBox);

}