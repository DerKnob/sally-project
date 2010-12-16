////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\PlaylistManager.h
///
/// \brief	Declares the playlist manager class. 
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

#pragma once
#include <sallyAPI\sallyAPI.h>
#include "Playlist.h"

class CPlaylistManager :
	public SallyAPI::GUI::CForm
{
private:
	CPlaylist*		m_pPlaylist;

	// for the thread
	SallyAPI::GUI::CThreadStarter*	m_pTSAddToList;
	std::string						m_strListToLoad;
	bool							m_bSetAutoPlaylistName;


	SallyAPI::GUI::CButtonBar*			m_pMenu;
	SallyAPI::GUI::CButtonBarButton*	m_pMenuPlaylist;
	SallyAPI::GUI::CButtonBarButton*	m_pMenuAutoPlaylist;

	SallyAPI::GUI::CListViewExt*		m_pListViewToDeleteFrom;
	int									m_iItemToDelete;
	
	SallyAPI::GUI::CListViewExt*		m_pFileBrowserPlaylist;
	SallyAPI::GUI::CListViewExt*		m_pFileBrowserAutoPlaylist;

	void	OnCommandDeleteFile();
	void	OpenFolder(SallyAPI::GUI::CListViewExt* listView, std::string folder);
	void	AddListThread();
	void	OnCommandListViewClicked(SallyAPI::GUI::CGUIBaseObject* reporter, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);
	void	OnCommandAddList(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter, SallyAPI::GUI::CListViewExt* listView, bool setAutoPlaylistName);
	void	OnCommandDeletePlaylist(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter, SallyAPI::GUI::CListViewExt* listView);
public:
	CPlaylistManager(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, CPlaylist* playlist);
	virtual ~CPlaylistManager();

	void			Reload();
	virtual void	SendMessageToParent(SallyAPI::GUI::CGUIBaseObject *reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
};
