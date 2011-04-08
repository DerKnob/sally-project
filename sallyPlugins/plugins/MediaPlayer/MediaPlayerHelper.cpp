////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\MediaPlayerHelper.cpp
///
/// \brief	Implements the media player helper class. 
///
/// \author	Christian Knobloch
/// \date	08.04.2010
///
/// This file is part of the Sally Project
/// 
/// Copyright(c) 2008-2011 Sally Project
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

#include "MediaPlayerHelper.h"

CMediaPlayerHelper::CMediaPlayerHelper()
{
}

CMediaPlayerHelper::~CMediaPlayerHelper()
{
}

void CMediaPlayerHelper::SetStaticValues(CPlaylist* playlist, SallyAPI::GUI::CAppBase* control,
								   std::map<std::string, CCoverLoader*>* coverLoaders)
{
	m_pMainWindow = control;
	m_pPlaylist = playlist;
	m_pmCoverLoaders = coverLoaders;
}

void CMediaPlayerHelper::SetValues(CMediaPlayer* mediaPlayer)
{
	m_pMediaPlayer = mediaPlayer;
}

void CMediaPlayerHelper::RunEx()
{
	if (m_pMediaPlayer->GetType() == MEDIAFILE_AUDIO)
		ProcessAudioFile();
	else if (m_pMediaPlayer->GetType() == MEDIAFILE_VIDEO)
		ProcessVideoFile();

	if (m_bPleaseStop)
		return;

	/************************************************************************/
	/* Update the Playtime in the database                                  */
	/************************************************************************/
	CMediaDatabase::UpdatePlaytime(m_pMediaPlayer->GetFilename(), m_pMainWindow);
}


void CMediaPlayerHelper::ProcessVideoFile()
{
	m_pMainWindow->SendMessageToParent(0, 0, GUI_APP_UPDATE_VIDEO_INFO);
}

void CMediaPlayerHelper::ProcessAudioFile()
{
	m_pMainWindow->SendMessageToParent(0, 0, GUI_APP_UPDATE_MP3_INFO);

	if (m_bPleaseStop)
		return;

	std::string filename = m_pMediaPlayer->GetFilename();

	/************************************************************************/
	/* Load the cover                                                       */
	/************************************************************************/
	std::string	pictureFile = SallyAPI::String::PathHelper::GetDirectoryFromPath(filename);
	pictureFile.append(m_pMediaPlayer->GetCoverName());

	// thread
	CCoverLoader* downloader = new CCoverLoader();
	downloader->SetStaticValues(m_pMainWindow);

	MP3FileInfo* id3Tag = m_pMediaPlayer->GetMp3Tag();

	if (id3Tag != NULL)
	{
		if (id3Tag->GetSzBand().length() > 0)
			downloader->SetValues(id3Tag->GetSzBand(), id3Tag->GetSzAlbum(), pictureFile, filename);
		else
			downloader->SetValues(id3Tag->GetSzArtist(), id3Tag->GetSzAlbum(), pictureFile, filename);
	}
	else
		downloader->SetValues("", "", pictureFile, filename);

	m_pMediaPlayer->UnlockMedia();

	SallyAPI::System::CSmartThreadPool* smtp = new SallyAPI::System::CSmartThreadPool();
	smtp->SetAutoDelete(true);
	smtp->AddThread(downloader);
	smtp->Start();

	(*m_pmCoverLoaders)[filename] = downloader;
}