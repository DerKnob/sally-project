////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\Playlist.h
///
/// \brief	Declares the playlist class. 
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
#include <sallyAPI/sallyAPI.h>
#include "PlaylistHelper.h"
#include "PlaylistSaver.h"

class CPlaylist
	: private SallyAPI::System::CThread
{
private:
	SallyAPI::GUI::CListViewExt*	m_pListViewPlaylist;
	SallyAPI::System::CCritSection	m_Lock;	
	std::string						m_strExplicitAppName;
	std::string						m_strAutoPlaylistName;
	int								m_iResolverStartItem;
	bool							m_bResolverOn;
	std::map<std::string, int>		m_mBandList;
	bool							m_bPlaylistDirty;
	SallyAPI::GUI::CAppBase*		m_pAppBase;

	void			UpdateShortPlaylistImageText();

	virtual void	RunEx();
public:
	CPlaylist(SallyAPI::GUI::CAppBase* appBase, SallyAPI::GUI::CListViewExt* listView,
		const std::string& explicitAppName);
	~CPlaylist();

	void		SetActive(int number);
	void		SetStartItem(int number);
	void		RemoveItem(int number);
	bool		AddItem(SallyAPI::GUI::CListViewItem& listItemTemp);
	void		Clear();
	void		UpdateView();
	
	int			GetListSize();
	void		LoadPlaylist(const std::string& filename);
	void		SavePlaylist(const std::string& filename, bool isDefaultPlaylist);

	SallyAPI::GUI::CListViewItem*				GetOrginalItem(int number);
	SallyAPI::GUI::CListViewItem				GetItem(int number);
	std::vector<SallyAPI::GUI::CListViewItem>	GetListItems();
	
	void		SetItemText(int number, const std::string& text);
	void		SetItemImage(int number, int image, const std::string& imageText);
	void		ResetImage(int number);

	void		Stop();
	void		StartResolver();
	void		DisableResolver(bool value);

	void		SetAutoPlaylistName(const std::string& autoPlaylistName);

	int			FindNumberByIdentifier(const std::string& identifier);
};
