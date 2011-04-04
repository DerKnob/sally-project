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

#include "AudioHelper.h"

CAudioHelper::CAudioHelper()
{
}

CAudioHelper::~CAudioHelper()
{
}

void CAudioHelper::SetStaticValues(CPlaylist* playlist, SallyAPI::GUI::CAppBase* control,
								   std::map<std::string, CCoverLoader*>* coverLoaders)
{
	m_pMainWindow = control;
	m_pPlaylist = playlist;
	m_pmCoverLoaders = coverLoaders;
}

void CAudioHelper::SetValues(CAudioFile* mp3)
{
	m_pMp3 = mp3;
}

void CAudioHelper::RunEx()
{
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

	MP3FileInfo* id3Tag = m_pMp3->GetMp3Tag();

	if (id3Tag != NULL)
	{
		if (id3Tag->GetSzBand().length() > 0)
			downloader->SetValues(id3Tag->GetSzBand(), id3Tag->GetSzAlbum(), pictureFile, m_pMp3->GetFilename());
		else
			downloader->SetValues(id3Tag->GetSzArtist(), id3Tag->GetSzAlbum(), pictureFile, m_pMp3->GetFilename());
	}
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