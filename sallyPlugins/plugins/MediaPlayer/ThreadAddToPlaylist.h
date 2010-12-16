////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\ThreadAddToPlaylist.h
///
/// \brief	Declares the thread add to playlist class. 
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
#include "AudioFile.h"
#include "VideoFile.h"
#include "Playlist.h"

class CThreadAddToPlaylist :
	public SallyAPI::System::CThread
{
private:
	SallyAPI::GUI::CListView*		m_pListViewSource;
	CPlaylist*						m_pPlaylist;
	SallyAPI::GUI::CForm*			m_pSearchWindow;
	std::string						m_strFolder;

	void			AddToPlaylistFromFilebrowser(std::string& folder);
	virtual void	RunEx();
public:
	CThreadAddToPlaylist();
	virtual ~CThreadAddToPlaylist();

	void SetValues(SallyAPI::GUI::CListView* listViewSource, CPlaylist* playlist, SallyAPI::GUI::CForm* searchWindow, const std::string& folder);
};
