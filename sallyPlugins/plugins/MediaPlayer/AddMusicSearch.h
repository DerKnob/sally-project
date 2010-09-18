////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\AddMusicSearch.h
///
/// \brief	Declares the add music search class. 
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

#pragma once
#include <sallyAPI\sallyAPI.h>
#include "Define.h"
#include "MediaDatabase.h"
#include "Playlist.h"

class CAddMusicSearch :
	public SallyAPI::GUI::CForm
{
private:
	SallyAPI::GUI::CDropDown*			m_pDropDownSearchType;
	SallyAPI::GUI::CButton*				m_pButtonAddAllResults;
	SallyAPI::GUI::CEdit*				m_pEditSearch;
	SallyAPI::GUI::CListView*			m_pListViewSearchResult;

	CPlaylist*	m_pPlaylist;

	void	OnCommandSearchChanger(int reporterId);
	void	AddAllToPlaylistFromListView(SallyAPI::GUI::CListView* listView);
	void	AddToPlaylistFromListView(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter, SallyAPI::GUI::CListView *listView);
public:
	CAddMusicSearch(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, CPlaylist* playlist);
	virtual ~CAddMusicSearch();

	virtual void	SendMessageToParent(SallyAPI::GUI::CGUIBaseObject *reporter, int reporterId, int iMessageID, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);

	void			OnCommandPopUpInfoNotify(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);
	void			Activate();
};
