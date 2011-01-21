////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\AlbumGetter.cpp
///
/// \brief	Implements the album getter class. 
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

#include "AlbumGetter.h"

CAlbumGetter::CAlbumGetter()
{
}

CAlbumGetter::~CAlbumGetter()
{
}

void CAlbumGetter::SetValues(SallyAPI::GUI::CAppBase* parentWindow,
							 SallyAPI::GUI::CListViewExt* listView, const std::string& albumName,
							 const std::string& artistName)
{
	m_pParentWindow = parentWindow;
	m_pListView = listView;
	m_pAlbumName = albumName;
	m_pArtistName = artistName;
}

void CAlbumGetter::RunEx()
{
	CMediaDatabase::GetAlbumTitelsFromDatabase(m_pParentWindow, m_pListView, m_pAlbumName, m_pArtistName);
}