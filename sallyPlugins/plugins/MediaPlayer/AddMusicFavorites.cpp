////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\AddMusicFavorites.cpp
///
/// \brief	Implements the add music favorites class. 
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

#include "AddMusicFavorites.h"

CAddMusicFavorites::CAddMusicFavorites(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, CPlaylist* playlist)
	:SallyAPI::GUI::CForm(parent, 0, -WINDOW_HEIGHT, WINDOW_WIDTH - MENU_WIDTH, WINDOW_HEIGHT),
	m_pPlaylist(playlist)
{
	std::vector<int>	imageListMostPlayed;
	imageListMostPlayed.push_back(GUI_THEME_SALLY_ICON_MIMETYPE_MP3);
	imageListMostPlayed.push_back(GUI_THEME_SALLY_ICON_MIMETYPE_VIDEO);

	m_pListViewFavourites = new SallyAPI::GUI::CListView(this, WINDOW_BORDER_H, WINDOW_BORDER_V + CONTROL_HEIGHT + 10,
		WINDOW_WIDTH - MENU_WIDTH - (WINDOW_BORDER_H * 2), WINDOW_HEIGHT - WINDOW_BORDER_V - WINDOW_BORDER_V - CONTROL_HEIGHT - 10,
		1, GUI_THEME_SALLY_ICON_ADD, imageListMostPlayed);
	m_pListViewFavourites->SetLocalised(false);
	this->AddChild(m_pListViewFavourites);

	int widthAdvanced = 250;
	if (WINDOW_WIDTH < 1024)
		widthAdvanced = 30;

	m_pButtonAddCompleteFolderFavourites = new SallyAPI::GUI::CButton(this,
		WINDOW_WIDTH - MENU_WIDTH - WINDOW_BORDER_H - widthAdvanced, WINDOW_BORDER_V, widthAdvanced, CONTROL_HEIGHT,
		GUI_APP_ADD_ALL_RESULTS_MOST_PLAYED, SallyAPI::GUI::BUTTON_TYPE_NORMAL);
	m_pButtonAddCompleteFolderFavourites->SetImageId(GUI_THEME_SALLY_ICON_ADD);
	m_pButtonAddCompleteFolderFavourites->SetText("Add All");
	this->AddChild(m_pButtonAddCompleteFolderFavourites);

	m_pDropDownFavourites = new SallyAPI::GUI::CDropDown(this, WINDOW_BORDER_H, WINDOW_BORDER_V, 250);
	this->AddChild(m_pDropDownFavourites);

	SallyAPI::GUI::CDropDownItem itemMostPlayed1("0", "Most Played", GUI_THEME_SALLY_ICON_MIMETYPE_MP3);
	m_pDropDownFavourites->AddItem(itemMostPlayed1);

	SallyAPI::GUI::CDropDownItem itemMostPlayed2("1", "Never Played", GUI_THEME_SALLY_ICON_MIMETYPE_MP3);
	m_pDropDownFavourites->AddItem(itemMostPlayed2);

	SallyAPI::GUI::CDropDownItem itemBestRated1("2", "Rated 5", GUI_THEME_SALLY_ICON_RATING_ON);
	m_pDropDownFavourites->AddItem(itemBestRated1);

	SallyAPI::GUI::CDropDownItem itemBestRated2("3", "Rated 4", GUI_THEME_SALLY_ICON_RATING_ON);
	m_pDropDownFavourites->AddItem(itemBestRated2);

	SallyAPI::GUI::CDropDownItem itemBestRated3("4", "Rated 3", GUI_THEME_SALLY_ICON_RATING_ON);
	m_pDropDownFavourites->AddItem(itemBestRated3);

	SallyAPI::GUI::CDropDownItem itemBestRated4("5", "Rated 2", GUI_THEME_SALLY_ICON_RATING_ON);
	m_pDropDownFavourites->AddItem(itemBestRated4);

	SallyAPI::GUI::CDropDownItem itemBestRated5("6", "Rated 1", GUI_THEME_SALLY_ICON_RATING_ON);
	m_pDropDownFavourites->AddItem(itemBestRated5);

	SallyAPI::GUI::CDropDownItem itemBestRated6("7", "Not Rated", GUI_THEME_SALLY_ICON_RATING_OFF);
	m_pDropDownFavourites->AddItem(itemBestRated6);

	SallyAPI::GUI::CDropDownItem itemBestRated7("8", "New Added", GUI_THEME_SALLY_ICON_DATE);
	m_pDropDownFavourites->AddItem(itemBestRated7);

	SallyAPI::GUI::CDropDownItem itemBestRated8("9", "Good but long not played", GUI_THEME_SALLY_ICON_RATING_ON);
	m_pDropDownFavourites->AddItem(itemBestRated8);

	m_pDropDownFavourites->SelectItemByIdentifier("0");

	m_pDropDownAdvanced = new SallyAPI::GUI::CDropDown(this, WINDOW_BORDER_H + 260, WINDOW_BORDER_V, 250);
	m_pDropDownAdvanced->Visible(false);
	this->AddChild(m_pDropDownAdvanced);

	UpdateFavouritesPlayed();
}

CAddMusicFavorites::~CAddMusicFavorites()
{
}

void CAddMusicFavorites::AddAllToPlaylistFromListView(SallyAPI::GUI::CListView *listView)
{
	int i = listView->GetListSize();
	for (int k = 0; k < i; k++)
	{
		SallyAPI::GUI::SendMessage::CParameterInteger messageInteger(k);
		AddToPlaylistFromListView(&messageInteger, listView);
	}
	return;
}

void CAddMusicFavorites::AddToPlaylistFromListView(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter, SallyAPI::GUI::CListView	*listView)
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

void CAddMusicFavorites::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int iMessageID, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (iMessageID)
	{
	case GUI_LISTVIEW_ITEM_DOUBLECLICKED:
		if (reporter == m_pListViewFavourites)
		{
			m_pParent->SendMessageToParent(this, 0, GUI_APP_PLAY_LAST_ADDED);
		}
		return;
	case GUI_LISTVIEW_ITEM_ACTION_CLICKED:
	case GUI_LISTVIEW_ITEM_CLICKED:
		if (reporter == m_pListViewFavourites)
		{
			AddToPlaylistFromListView(messageParameter, m_pListViewFavourites);
			return;
		}
		break;
	case GUI_DROPDOWN_CHANGED:
		UpdateFavouritesPlayed();
		return;
	}

	switch(reporterId)
	{
	case GUI_APP_ADD_ALL_RESULTS_MOST_PLAYED:
		AddAllToPlaylistFromListView(m_pListViewFavourites);
		return;
	}
	SallyAPI::GUI::CForm::SendMessageToParent(reporter, reporterId, iMessageID, messageParameter);
}

void CAddMusicFavorites::UpdateFavouritesPlayed()
{
	std::string favoritesString = m_pDropDownFavourites->GetSelectedIdentifier();
	int favoritesType = SallyAPI::String::StringHelper::ConvertToInt(favoritesString);

	m_pDropDownAdvanced->Clear();

	// update controls
	if (favoritesType == 9)
	{
		m_pDropDownAdvanced->Visible(true);

		SallyAPI::GUI::CDropDownItem itemAdvacnedPlayed1("0", "at last 1 month ago", GUI_THEME_SALLY_ICON_DATE);
		m_pDropDownAdvanced->AddItem(itemAdvacnedPlayed1);

		SallyAPI::GUI::CDropDownItem itemAdvacnedPlayed2("1", "at last 6 months ago", GUI_THEME_SALLY_ICON_DATE);
		m_pDropDownAdvanced->AddItem(itemAdvacnedPlayed2);

		SallyAPI::GUI::CDropDownItem itemAdvacnedPlayed3("2", "at last 1 year ago", GUI_THEME_SALLY_ICON_DATE);
		m_pDropDownAdvanced->AddItem(itemAdvacnedPlayed3);

		SallyAPI::GUI::CDropDownItem itemAdvacnedPlayed4("3", "at last 2 years ago", GUI_THEME_SALLY_ICON_DATE);
		m_pDropDownAdvanced->AddItem(itemAdvacnedPlayed4);

		SallyAPI::GUI::CDropDownItem itemAdvacnedPlayed5("4", "at last 3 years ago", GUI_THEME_SALLY_ICON_DATE);
		m_pDropDownAdvanced->AddItem(itemAdvacnedPlayed5);

		SallyAPI::GUI::CDropDownItem itemAdvacnedPlayed6("5", "at last 4 years ago", GUI_THEME_SALLY_ICON_DATE);
		m_pDropDownAdvanced->AddItem(itemAdvacnedPlayed6);

		SallyAPI::GUI::CDropDownItem itemAdvacnedPlayed7("6", "at last 5 years ago", GUI_THEME_SALLY_ICON_DATE);
		m_pDropDownAdvanced->AddItem(itemAdvacnedPlayed7);

		m_pDropDownAdvanced->SelectItemByIdentifier("0");
	}
	else
	{
		m_pDropDownAdvanced->Visible(false);
	}

	std::string advancedString = m_pDropDownAdvanced->GetSelectedIdentifier();
	int advancedType = SallyAPI::String::StringHelper::ConvertToInt(advancedString);

	// get the stuff from the database
	CMediaDatabase::GetStatisticFromDatabase(dynamic_cast<SallyAPI::GUI::CAppBase*> (m_pParent), m_pListViewFavourites, favoritesType, advancedType);	
}