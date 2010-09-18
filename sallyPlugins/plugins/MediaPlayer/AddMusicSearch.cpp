////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\AddMusicSearch.cpp
///
/// \brief	Implements the add music search class. 
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

#include "AddMusicSearch.h"

CAddMusicSearch::CAddMusicSearch(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, CPlaylist* playlist)
	:SallyAPI::GUI::CForm(parent, 0, -WINDOW_HEIGHT, WINDOW_WIDTH - MENU_WIDTH, WINDOW_HEIGHT),
	m_pPlaylist(playlist)
{
	std::vector<int>	imageListSearch;
	imageListSearch.push_back(GUI_THEME_SALLY_ICON_MIMETYPE_MP3);
	imageListSearch.push_back(GUI_THEME_SALLY_ICON_MIMETYPE_VIDEO);

	m_pListViewSearchResult = new SallyAPI::GUI::CListView(this, WINDOW_BORDER_H, WINDOW_BORDER_V + CONTROL_HEIGHT + 10,
		WINDOW_WIDTH - MENU_WIDTH - (WINDOW_BORDER_H * 2), WINDOW_HEIGHT - WINDOW_BORDER_V - WINDOW_BORDER_V - CONTROL_HEIGHT - 10,
		1, GUI_THEME_SALLY_ICON_ADD, imageListSearch);
	m_pListViewSearchResult->SetLocalised(false);
	this->AddChild(m_pListViewSearchResult);

	m_pDropDownSearchType = new SallyAPI::GUI::CDropDown(this, WINDOW_BORDER_H, WINDOW_BORDER_V, 250, GUI_APP_SEARCH_TYPE);
	this->AddChild(m_pDropDownSearchType);

	SallyAPI::GUI::CDropDownItem itemAll("all", "Search for all Medias", GUI_THEME_SALLY_ICON_MIMETYPE_TEXT);
	m_pDropDownSearchType->AddItem(itemAll);

	SallyAPI::GUI::CDropDownItem itemMp3("music", "Search for Music", GUI_THEME_SALLY_ICON_MIMETYPE_MP3);
	m_pDropDownSearchType->AddItem(itemMp3);

	SallyAPI::GUI::CDropDownItem itemVideo("video", "Search for Video", GUI_THEME_SALLY_ICON_MIMETYPE_VIDEO);
	m_pDropDownSearchType->AddItem(itemVideo);

	m_pDropDownSearchType->SelectItemByIdentifier("all");

	m_pEditSearch = new SallyAPI::GUI::CEdit(this, WINDOW_BORDER_H + 250 + 10, WINDOW_BORDER_V,
		WINDOW_WIDTH - MENU_WIDTH - (WINDOW_BORDER_H * 2) - 200 - 10 - 250 - 10, GUI_APP_SEARCH_EDIT);
	m_pEditSearch->SetInfoText("click here to search");
	m_pEditSearch->SetImageId(GUI_THEME_SALLY_ICON_SEARCH);
	this->AddChild(m_pEditSearch);

	m_pButtonAddAllResults = new SallyAPI::GUI::CButton(this,
		WINDOW_WIDTH - MENU_WIDTH - WINDOW_BORDER_H - 200, WINDOW_BORDER_V, 200, CONTROL_HEIGHT,
		GUI_APP_ADD_ALL_RESULTS, SallyAPI::GUI::BUTTON_TYPE_NORMAL);
	m_pButtonAddAllResults->SetImageId(GUI_THEME_SALLY_ICON_ADD);
	m_pButtonAddAllResults->SetText("Add All");
	this->AddChild(m_pButtonAddAllResults);
}

CAddMusicSearch::~CAddMusicSearch()
{
}

void CAddMusicSearch::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int iMessageID, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (iMessageID)
	{
	case GUI_LISTVIEW_ITEM_DOUBLECLICKED:
		if (reporter == m_pListViewSearchResult)
		{
			m_pParent->SendMessageToParent(this, 0, GUI_APP_PLAY_LAST_ADDED);
		}
		return;
	case GUI_LISTVIEW_ITEM_ACTION_CLICKED:
	case GUI_LISTVIEW_ITEM_CLICKED:
		if (reporter == m_pListViewSearchResult)
		{
			AddToPlaylistFromListView(messageParameter, m_pListViewSearchResult);
			return;
		}
		break;
	case GUI_DROPDOWN_CHANGED:
		switch (reporterId)
		{
		case GUI_APP_SEARCH_TYPE:
			if (m_pEditSearch->GetText().length() > 0)
				CMediaDatabase::SearchInDatabase(m_pEditSearch->GetText(), m_pDropDownSearchType->GetSelectedIdentifier(), m_pListViewSearchResult, dynamic_cast<SallyAPI::GUI::CAppBase*> (m_pParent));
			else
				m_pListViewSearchResult->Clear();
			return;
		}
		break;
	case GUI_EDIT_CHANGED:
		switch (reporterId)
		{
		case GUI_APP_SEARCH_EDIT:
			if (m_pEditSearch->GetText().length() > 0)
				CMediaDatabase::SearchInDatabase(m_pEditSearch->GetText(), m_pDropDownSearchType->GetSelectedIdentifier(), m_pListViewSearchResult, dynamic_cast<SallyAPI::GUI::CAppBase*> (m_pParent));
			else
				m_pListViewSearchResult->Clear();
			return;
		}
		break;
	}

	switch(reporterId)
	{
	case GUI_APP_ADD_ALL_RESULTS:
		AddAllToPlaylistFromListView(m_pListViewSearchResult);
		return;
	}
	SallyAPI::GUI::CForm::SendMessageToParent(reporter, reporterId, iMessageID, messageParameter);
}

void CAddMusicSearch::AddAllToPlaylistFromListView(SallyAPI::GUI::CListView *listView)
{
	int i = listView->GetListSize();
	for (int k = 0; k < i; k++)
	{
		SallyAPI::GUI::SendMessage::CParameterInteger messageInteger(k);
		AddToPlaylistFromListView(&messageInteger, listView);
	}
	SallyAPI::GUI::SendMessage::CParameterOnScreenMenu messageOnScreenMenu(GUI_THEME_SALLY_OSM_ADD, "Added");
	m_pParent->SendMessageToParent(this, 0, MS_SALLY_ON_SCREEN_MENU, &messageOnScreenMenu);
	return;
}

void CAddMusicSearch::AddToPlaylistFromListView(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter, SallyAPI::GUI::CListView	*listView)
{
	SallyAPI::GUI::SendMessage::CParameterInteger* parameterInteger = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterInteger*> (messageParameter);

	SallyAPI::GUI::CListViewItem *listItem = listView->GetItem(parameterInteger->GetInteger());
	SallyAPI::GUI::CListViewItem listItemTemp(listItem->GetIdentifier(), listItem->GetText(),
		listItem->GetImageIndex());

	m_pPlaylist->AddItem(listItemTemp);

	SallyAPI::GUI::SendMessage::CParameterOnScreenMenu messageOnScreenMenu(GUI_THEME_SALLY_OSM_ADD, "Added");
	m_pParent->SendMessageToParent(this, 0, MS_SALLY_ON_SCREEN_MENU, &messageOnScreenMenu);
	return;
}

void CAddMusicSearch::OnCommandPopUpInfoNotify(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	SallyAPI::GUI::SendMessage::CParameterString* parameterString = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterString*> (messageParameter);

	if (parameterString == NULL)
		return;

	m_pEditSearch->SetText(parameterString->GetString());
}

void CAddMusicSearch::Activate()
{
	if (m_pEditSearch->GetText().length() == 0)
		m_pParent->SendMessageToParent(m_pEditSearch, GUI_APP_SEARCH_EDIT, MS_SALLY_SHOW_KEYBOARD);
}