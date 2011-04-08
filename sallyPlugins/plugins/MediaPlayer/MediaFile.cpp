////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\MediaFile.cpp
///
/// \brief	Implements the media file class. 
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

#include "MediaFile.h"

CMediaFile::CMediaFile(const std::string filename)
	:m_strFilename(filename)
{
	m_iID = 0;
}

CMediaFile::~CMediaFile()
{
}

MEDIAFILE CMediaFile::GetType()
{
	return m_eType;
}

std::string CMediaFile::GetFilename()
{
	return m_strFilename;
}

void CMediaFile::SetID(int ID)
{
	m_iID = ID;
}

int CMediaFile::GetID()
{
	return m_iID;
}