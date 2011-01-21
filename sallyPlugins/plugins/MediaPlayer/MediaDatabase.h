////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\MediaDatabase.h
///
/// \brief	Declares the media database class. 
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
#include <sallyAPI\sallyAPI.h>
#include <string>
#include "MP3FileInfo.h"
#include "AudioFile.h"
#include "Album.h"

class CMediaDatabase
{
private:
	CMediaDatabase();
	~CMediaDatabase();
public:
	static void	GetAlbumsFromDatabase(SallyAPI::GUI::CAppBase* appBase, std::vector<CAlbum*>* albumList,
		const std::string& artist = "", const std::string& album = "", const std::string& genre = "");
	static void	GetAlbumsFromDatabaseNotLoaded(SallyAPI::GUI::CAppBase* appBase, std::vector<CAlbum*>* albumList);
	static void	SetAlbumInDatabase(SallyAPI::GUI::CAppBase* appBase, const std::string& album,
		const std::string& albumArtist, bool set);
	static void	SearchInDatabase(const std::string& searchFor, const std::string& searchType, SallyAPI::GUI::CListView* listView,
		SallyAPI::GUI::CAppBase* appBase);
	static void	GetAlbumTitelsFromDatabase(SallyAPI::GUI::CAppBase* appBase, SallyAPI::GUI::CListViewExt* listView,
		const std::string& album, const std::string& artist);
	static void	GetStatisticFromDatabase(SallyAPI::GUI::CAppBase* appBase, SallyAPI::GUI::CListView* listView, int type,
		int advancedType);

	static void	UpdatePlaytime(const std::string& filename, SallyAPI::GUI::CAppBase* appBase);
	static int	GetPlaytime(const std::string& filename, SallyAPI::GUI::CAppBase* appBase);
	static void	UpdateRating(const std::string& filename, SallyAPI::GUI::CAppBase* appBase, int rating);
	static int	GetRating(const std::string& filename, SallyAPI::GUI::CAppBase* appBase);
};