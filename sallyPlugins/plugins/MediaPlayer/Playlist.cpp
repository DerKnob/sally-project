////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\Playlist.cpp
///
/// \brief	Implements the playlist class. 
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

#include "Playlist.h"

CPlaylist::CPlaylist(SallyAPI::GUI::CAppBase* appBase, SallyAPI::GUI::CListViewExt* listView,
					 const std::string& explicitAppName)
	:m_pListViewPlaylist(listView), m_strExplicitAppName(explicitAppName), m_iResolverStartItem(0),
	m_bPlaylistDirty(false), m_pAppBase(appBase)
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	// restore autoPlaylistName
	m_strAutoPlaylistName = option->GetPropertyString("config", "playlistAutoPlaylistName");
}

CPlaylist::~CPlaylist()
{
}

void CPlaylist::RunEx()
{
	while ((!m_bPleaseStop)&& (m_iResolverStartItem < m_pListViewPlaylist->GetListSize()))
	{
		Sleep(10);

		SallyAPI::System::CAutoLock lock(&m_Lock);

		SallyAPI::GUI::CListViewItem* item = m_pListViewPlaylist->GetItem(m_iResolverStartItem);
		std::string filename = item->GetIdentifier();

		std::string artist;
		
		if (SallyAPI::String::StringHelper::StringEndsWith(filename, ".mp3"))
		{
			MP3FileInfo id3Tag;
			bool init = id3Tag.Init(filename);

			if (init)
			{
				std::string tempTrack;

				if (id3Tag.GetSzArtist().length() != 0)
				{
					artist = tempTrack = id3Tag.GetSzArtist();
					tempTrack.append(" - ");
					if (id3Tag.GetSzTitle().length() != 0)
					{
						tempTrack.append(id3Tag.GetSzTitle());
					}
					else
					{
						tempTrack.append(SallyAPI::String::PathHelper::GetFileFromPath(filename));
					}
				}
				else
				{
					tempTrack = SallyAPI::String::PathHelper::GetFileFromPath(filename);
				}
				item->SetText(tempTrack, 1);

				m_pListViewPlaylist->UpdateView();
			}
		}

		if (artist.length() == 0)
		{
			// ToDo - tweak
			artist = SallyAPI::String::PathHelper::GetFileFromPath(filename);
		}

		// now update the band list
		if (artist.length() > 0)
		{
			m_mBandList[artist] = m_mBandList[artist] + 1;	
		}

		m_iResolverStartItem++;
	}
}

void CPlaylist::Stop()
{
	WaitForStop();
}

void CPlaylist::StartResolver()
{
	Start();
}

void CPlaylist::DisableResolver(bool value)
{
	m_bResolverOn = value;
}

void CPlaylist::LoadPlaylist(const std::string& filename)
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	CPlaylistHelper::LoadPlaylist(m_pListViewPlaylist, filename);

	StartResolver();

	m_bPlaylistDirty = true;
}

void CPlaylist::SavePlaylist(const std::string& filename, bool isDefaultPlaylist)
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	if ((isDefaultPlaylist) && (!m_bPlaylistDirty))
		return;

	std::vector<SallyAPI::GUI::CListViewItem> itemList;

	std::vector<SallyAPI::GUI::CListViewItem*>* temp = m_pListViewPlaylist->GetListItems();
	std::vector<SallyAPI::GUI::CListViewItem*>::iterator iter;

	for(iter = temp->begin(); iter != temp->end(); iter++)
	{
		SallyAPI::GUI::CListViewItem* listItem = *iter;

		itemList.push_back(*listItem);
	}

	CPlaylistHelper::SavePlaylist(itemList, filename);

	if (isDefaultPlaylist)
		m_bPlaylistDirty = false;
}

bool CPlaylist::AddItem(SallyAPI::GUI::CListViewItem& listItemTemp)
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	// should we prevent duplicates
	if (m_pAppBase->GetPropertyBool("preventduplicatesinplaylist"))
	{
		std::vector<SallyAPI::GUI::CListViewItem*>* temp = m_pListViewPlaylist->GetListItems();
		std::vector<SallyAPI::GUI::CListViewItem*>::iterator iter;

		for(iter = temp->begin(); iter != temp->end(); iter++)
		{
			SallyAPI::GUI::CListViewItem* listItem = *iter;

			if (listItem->GetIdentifier().compare(listItemTemp.GetIdentifier()) == 0)
				return false; // already in the playlist
		}
	}

	m_bPlaylistDirty = true;

	if (listItemTemp.GetImageId() == 0)
		listItemTemp.SetImageId(GUI_THEME_SALLY_ICON_MIMETYPE_MP3, 1);
	else
		listItemTemp.SetImageId(GUI_THEME_SALLY_ICON_MIMETYPE_VIDEO, 1);

	listItemTemp.SetText(listItemTemp.GetText(), 1);

	// reset the first item
	listItemTemp.SetText("", 0);
	listItemTemp.SetImageId(GUI_THEME_SALLY_ICON_REMOVE, 0);
	
	listItemTemp.SetType(SallyAPI::GUI::LISTVIEWITEM_TYPE_SORTER, 3);

	m_pListViewPlaylist->AddItem(listItemTemp);

	if (m_bResolverOn)
		StartResolver();

	return true;
}

void CPlaylist::RemoveItem(int number)
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	m_bPlaylistDirty = true;

	m_pListViewPlaylist->RemoveItem(number + 1);

	if (number < m_iResolverStartItem)
		--m_iResolverStartItem;
}

SallyAPI::GUI::CListViewItem CPlaylist::GetItem(int number)
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	SallyAPI::GUI::CListViewItem* listItem = m_pListViewPlaylist->GetItem(number);
	if (listItem == NULL)
	{
		SallyAPI::GUI::CListViewItem listItemCopy("", "", 0);
		return listItemCopy;
	}

	SallyAPI::GUI::CListViewItem listItemCopy(*listItem);

	return listItemCopy;
}

SallyAPI::GUI::CListViewItem* CPlaylist::GetOrginalItem(int number)
{
	SallyAPI::GUI::CListViewItem* listItem = m_pListViewPlaylist->GetItem(number);

	return listItem;
}

int CPlaylist::GetListSize()
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	return m_pListViewPlaylist->GetListSize();
}

void CPlaylist::SetActive(int number)
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	m_pListViewPlaylist->SetActive(number);
}

void CPlaylist::SetStartItem(int number)
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	m_pListViewPlaylist->SetStartItem(number - (m_pListViewPlaylist->GetMaxDisplayedElements() / 2));
}

std::vector<SallyAPI::GUI::CListViewItem> CPlaylist::GetListItems()
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	std::vector<SallyAPI::GUI::CListViewItem> itemList;

	std::vector<SallyAPI::GUI::CListViewItem*>* temp = m_pListViewPlaylist->GetListItems();
	std::vector<SallyAPI::GUI::CListViewItem*>::iterator iter;

	for(iter = temp->begin(); iter != temp->end(); iter++)
	{
		SallyAPI::GUI::CListViewItem* listItem = *iter;

		itemList.push_back(*listItem);
	}

	return itemList;
}

void CPlaylist::Clear()
{
	this->WaitForStop();

	SallyAPI::System::CAutoLock lock(&m_Lock);

	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	bool result = option->GetPropertyBool("config", "fullautomaticplaylisthistory", true);
	
	if ((result) && (GetListSize() > 0))
	{
		// have we loaded a playlist?
		if (m_strAutoPlaylistName.length() != 0)
		{
			DeleteFile(m_strAutoPlaylistName.c_str());
		}

		std::vector<SallyAPI::GUI::CListViewItem> itemList = GetListItems();

		m_strAutoPlaylistName = SallyAPI::System::SallyHelper::GetMediaDirectory(m_strExplicitAppName);
		m_strAutoPlaylistName.append("AutoPlaylist\\");

		SallyAPI::System::CSmartThreadPool* smtp = new SallyAPI::System::CSmartThreadPool();
		smtp->SetAutoDelete(true);

		smtp->AddThread(new CPlaylistSaver(itemList, m_mBandList, m_strAutoPlaylistName));
		smtp->Start();
	}

	// empty last playlistName
	m_strAutoPlaylistName = "";
	m_mBandList.clear();
	m_pListViewPlaylist->Clear();
	m_iResolverStartItem = 0;
	m_bPlaylistDirty = true;
}

void CPlaylist::UpdateView()
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	m_pListViewPlaylist->UpdateView();
}

void CPlaylist::SetItemText(int number, const std::string& text)
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	SallyAPI::GUI::CListViewItem* listItem = m_pListViewPlaylist->GetItem(number);

	if (listItem == NULL)
		return;

	listItem->SetText(text, 1);
}

void CPlaylist::SetAutoPlaylistName(const std::string& autoPlaylistName)
{
	m_strAutoPlaylistName = autoPlaylistName;

	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	option->SetPropertyString("config", "playlistAutoPlaylistName", m_strAutoPlaylistName);
}

int CPlaylist::FindNumberByIdentifier(const std::string& identifier)
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	std::vector<SallyAPI::GUI::CListViewItem*>* temp = m_pListViewPlaylist->GetListItems();
	std::vector<SallyAPI::GUI::CListViewItem*>::iterator iter;

	int i = 0;

	for(iter = temp->begin(); iter != temp->end(); iter++)
	{
		SallyAPI::GUI::CListViewItem* listItem = *iter;

		if (identifier.compare(listItem->GetIdentifier()) == 0)
		{
			return i;
		}
		++i;
	}
	return -1;
}