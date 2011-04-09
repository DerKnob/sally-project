////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\GetCovers.cpp
///
/// \brief	Implements the get covers class. 
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

#include "GetCovers.h"

CGetCovers::CGetCovers()
{
	m_iMessageId = 0;
	m_pWindow = 0;
}

CGetCovers::~CGetCovers()
{
}

void CGetCovers::RunEx()
{
	std::vector<CAlbum*> albumList;

	CMediaDatabase::GetAlbumsFromDatabaseNotLoaded(m_pWindow, &albumList);

	// set processbar max
	m_pProcessbar->SetMaxPosition((int) albumList.size());

	std::vector<CAlbum*>::iterator iter = albumList.begin();

	while ((iter != albumList.end()) && (!m_bPleaseStop))
	{
		CAlbum* album = *iter;
		
		std::string outfile = SallyAPI::String::PathHelper::GetDirectoryFromPath(album->GetFilename());
		outfile.append(album->GetCoverName());

		bool exists = SallyAPI::File::FileHelper::FileExists(outfile);
		
		if (exists)
		{
			// Datei exists already
			CMediaDatabase::SetAlbumInDatabase(m_pWindow, album->GetAlbum(), album->GetArtist(), true);
		}
		else
		{
			// Get it from Amazon
			bool result = false;

			result = m_AmazonCover.GetMusicCover(album->GetArtist(), album->GetAlbum(), outfile);

			if (result)
			{
				CMediaDatabase::SetAlbumInDatabase(m_pWindow, album->GetAlbum(), album->GetArtist(), true);
			}
			else
			{
				CMediaDatabase::SetAlbumInDatabase(m_pWindow, album->GetAlbum(), album->GetArtist(), false);
			}
		}
		++iter;

		// update processbar
		m_pProcessbar->SetPosition(m_pProcessbar->GetPosition() + 1);
	}

	// Delete vector
	iter = albumList.begin();
	while (iter != albumList.end())
	{
		SafeDelete(*iter);
		++iter;
	}

	if (!m_bPleaseStop)
		m_pWindow->SendMessageToParent(0, 0, m_iMessageId);
}

void CGetCovers::SetValues(int messageId, SallyAPI::GUI::CAppBase* window, SallyAPI::GUI::CProcessbar* processbar)
{
	m_iMessageId = messageId;
	m_pWindow = window;
	m_pProcessbar = processbar;
}