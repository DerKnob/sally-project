////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\Album.cpp
///
/// \brief	Implements the album class. 
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

#include "Album.h"

CAlbum::CAlbum(const std::string& album, const std::string& artist, const std::string& band, const std::string& filename)
	:m_strAlbum(album), m_strArtist(artist), m_strBand(band), m_strFilename(filename)
{
}

CAlbum::~CAlbum(void)
{
}


std::string	CAlbum::GetAlbum()
{
	return m_strAlbum;
}

std::string	CAlbum::GetArtist()
{
	return m_strArtist;
}

std::string	CAlbum::GetBand()
{
	return m_strBand;
}

std::string	CAlbum::GetFilename()
{
	return m_strFilename;
}

std::string	CAlbum::GetCoverName()
{
	if (m_strCoverName.length() == 0)
	{
		if (m_strBand.length() > 0)
			m_strCoverName = CAudioFile::GetCoverName(m_strBand, m_strAlbum);
		else
			m_strCoverName = CAudioFile::GetCoverName(m_strArtist, m_strAlbum);
	}
	return m_strCoverName;
}