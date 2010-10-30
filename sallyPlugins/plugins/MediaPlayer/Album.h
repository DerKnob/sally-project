////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\Album.h
///
/// \brief	Declares the album class. 
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

#pragma once
#include <sallyAPI\sallyAPI.h>
#include "AudioFile.h"

class CAlbum
{
private:
	std::string	m_strAlbum;
	std::string	m_strArtist;
	std::string	m_strBand;
	std::string	m_strFilename;
	std::string	m_strCoverName;
public:
	CAlbum(const std::string& album, const std::string& artist, const std::string& band, const std::string& filename);
	~CAlbum();

	std::string GetCoverName();
	std::string	GetAlbum();
	std::string	GetArtist();
	std::string	GetBand();
	std::string	GetFilename();
};
