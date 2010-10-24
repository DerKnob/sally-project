////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\AudioHelper.cpp
///
/// \brief	Implements the audio helper class. 
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

#include "AudioHelper.h"

CAudioHelper::CAudioHelper()
{
}

CAudioHelper::~CAudioHelper()
{
}

void CAudioHelper::SetStaticValues(SallyAPI::GUI::CLabel* track, SallyAPI::GUI::CLabel* album,
								   CPlaylist* playlist, SallyAPI::GUI::CAppBase* control,
								   std::map<std::string, CCoverLoader*>* coverLoaders)
{
	m_pTrack = track;
	m_pAlbum = album;
	m_pMainWindow = control;
	m_pPlaylist = playlist;
	m_pmCoverLoaders = coverLoaders;
}

void CAudioHelper::SetValues(CAudioFile* mp3, SallyAPI::GUI::CListViewItem* listItem)
{
	m_pMp3 = mp3;
	m_pListItem = listItem;
}

void CAudioHelper::RunEx()
{
	MP3FileInfo* id3Tag = m_pMp3->GetMp3Tag();

	// ID3 Tag Infos
	std::string tempTrack;
	std::string tempAblum;

	if (id3Tag != NULL)
	{
		if (id3Tag->GetSzArtist().length() != 0)
		{
			tempTrack = id3Tag->GetSzArtist();
			tempTrack.append(" - ");
			if (id3Tag->GetSzTitle().length() != 0)
			{
				tempTrack.append(id3Tag->GetSzTitle());
			}
			else
			{
				tempTrack.append(SallyAPI::String::PathHelper::GetFileFromPath(m_pMp3->GetFilename()));
			}
		}
		else
		{
			tempTrack = SallyAPI::String::PathHelper::GetFileFromPath(m_pMp3->GetFilename());
		}
		if (id3Tag->GetSzAlbum().length() != 0)
		{
			tempAblum = id3Tag->GetSzAlbum();
			if (id3Tag->GetSzTrack().length() != 0)
			{
				tempAblum.append(" - Track ");
				tempAblum.append(id3Tag->GetSzTrack());
			}
		}
	}
	else
	{
		tempTrack = SallyAPI::String::PathHelper::GetFileFromPath(m_pMp3->GetFilename());
	}
	m_pListItem->SetText(tempTrack);
	m_pPlaylist->UpdateView();

	m_pTrack->SetText(tempTrack);
	m_pAlbum->SetText(tempAblum);

	m_pMainWindow->SendMessageToParent(0, 0, GUI_APP_UPDATE_MP3_INFO);

	if (m_bPleaseStop)
	{
		return;
	}


	/************************************************************************/
	/* Load the cover                                                       */
	/************************************************************************/
	std::string	pictureFile = SallyAPI::String::PathHelper::GetDirectoryFromPath(m_pMp3->GetFilename());
	pictureFile.append(m_pMp3->GetCoverName());

	// thread
	CCoverLoader* downloader = new CCoverLoader();
	downloader->SetStaticValues(m_pMainWindow);

	if (id3Tag != NULL)
		downloader->SetValues(id3Tag->GetSzArtist(), id3Tag->GetSzAlbum(), id3Tag->GetSzBand(), pictureFile, m_pMp3->GetFilename());
	else
		downloader->SetValues("", "", pictureFile, m_pMp3->GetFilename());

	SallyAPI::System::CSmartThreadPool* smtp = new SallyAPI::System::CSmartThreadPool();
	smtp->SetAutoDelete(true);
	smtp->AddThread(downloader);
	smtp->Start();

	(*m_pmCoverLoaders)[m_pMp3->GetFilename()] = downloader;

	if (m_bPleaseStop)
	{
		return;
	}

	/************************************************************************/
	/* Update the Playtime in the database                                  */
	/************************************************************************/
	CMediaDatabase::UpdatePlaytime(m_pMp3->GetFilename(), m_pMainWindow);	
}