////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\AddMusicExplorer.h
///
/// \brief	Declares the add music explorer class. 
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

#pragma once
#include <sallyAPI\sallyAPI.h>
#include "Define.h"
#include "ThreadAddToPlaylist.h"
#include "AudioFile.h"
#include "VideoFile.h"
#include "Playlist.h"

class CAddMusicExplorer :
	public SallyAPI::GUI::CForm
{
private:
	CThreadAddToPlaylist				m_tAddToPlaylist;
	SallyAPI::GUI::CFileBrowser*		m_pFileBrowser;
	SallyAPI::GUI::CAppBase*			m_pMainApp;

	CPlaylist*	m_pPlaylist;

	void	OnCommandAddAllFromExplorer();
	void	OnCommandDoubleClicked(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);

	std::vector<std::string> GenerateFolderList();
	void	AddToPlaylistFromFilebrowserItem(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);
	void	AddToPlaylistFromFilebrowser(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);
public:
	CAddMusicExplorer(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, CPlaylist* playlist);
	virtual ~CAddMusicExplorer();

	virtual void	SendMessageToParent(SallyAPI::GUI::CGUIBaseObject *reporter, int reporterId, int iMessageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);

	void			LoadConfig();
	void			SaveConfig();
};
