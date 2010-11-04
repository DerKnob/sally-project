////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\CoverLoader.cpp
///
/// \brief	Implements the cover loader class. 
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

#include "CoverLoader.h"

CCoverLoader::CCoverLoader()
{
}

CCoverLoader::~CCoverLoader()
{
}

void CCoverLoader::SetStaticValues(SallyAPI::GUI::CAppBase* mainWindow)
{
	m_pMainWindow = mainWindow;
}

void CCoverLoader::SetValues(const std::string& artist, const std::string& album,
							 const std::string& imageFile, const std::string& mp3File)
{
	m_strArtist = artist;
	m_strAlbum = album;
	m_strImageFile = imageFile;
	m_strMp3File = mp3File;
}

void CCoverLoader::RunEx()
{
	SallyAPI::GUI::CPicture* pictureTemp = NULL;

	pictureTemp = LoadImage();

	if ((pictureTemp == NULL) && (m_strAlbum.length() > 0) && (m_strArtist.length() > 0))
	{
		m_AmazonCover.GetCover(m_strArtist, m_strAlbum, m_strImageFile);
		pictureTemp = LoadImage();
	}

	// update database
	if ((pictureTemp != NULL) && (m_strAlbum.length() > 0) && (m_strArtist.length() > 0))
	{
		CMediaDatabase::SetAlbumInDatabase(m_pMainWindow, m_strAlbum, m_strArtist, true);
	}

	CParameterPicture messageParameter(pictureTemp, m_strMp3File);
	m_pMainWindow->SendMessageToParent(m_pMainWindow, m_pMainWindow->GetControlId(),
		GUI_APP_ALBUM_COVER_MP3HELPER_LOADED, &messageParameter);
}

SallyAPI::GUI::CPicture* CCoverLoader::LoadImage()
{
	SallyAPI::GUI::CPicture* pictureTemp;

	pictureTemp = new SallyAPI::GUI::CPicture();
	if (pictureTemp->LoadTexture(m_strImageFile))
		return pictureTemp;

	delete pictureTemp;
	return NULL;
}