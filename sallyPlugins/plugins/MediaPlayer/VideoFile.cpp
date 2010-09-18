////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\VideoFile.cpp
///
/// \brief	Implements the video file class. 
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

#include "VideoFile.h"

CVideoFile::CVideoFile(const std::string fileName) : CMediaFile(fileName)
{
	m_eType = MEDIAFILE_VIDEO;
}

CVideoFile::~CVideoFile(void)
{
}

SallyAPI::GUI::CPicture* CVideoFile::GetPicture()
{
	return NULL;
}

std::string CVideoFile::GetFormatedText()
{
	return SallyAPI::String::PathHelper::GetFileFromPath(m_strFileName);
}

bool CVideoFile::IsVideoFile(const std::string& filename)
{
	std::vector<std::string> fileExtensions = GetFileExtensions();
	std::vector<std::string>::iterator iter = fileExtensions.begin();

	while (iter != fileExtensions.end())
	{
		std::string end = *iter;
		if (SallyAPI::String::StringHelper::StringEndsWith(filename, end))
			return true;
		++iter;
	}
	return false;
}

std::vector<std::string> CVideoFile::GetFileExtensions()
{
	static std::vector<std::string> m_FileExtensions;

	// init on first call
	if (m_FileExtensions.size() == 0)
	{
		m_FileExtensions.push_back(".asf");
		m_FileExtensions.push_back(".avi");
		m_FileExtensions.push_back(".divx");
		m_FileExtensions.push_back(".dv");
		m_FileExtensions.push_back(".flv");
		m_FileExtensions.push_back(".gxf");
		m_FileExtensions.push_back(".m1v");
		m_FileExtensions.push_back(".m2v");
		m_FileExtensions.push_back(".m2ts");
		m_FileExtensions.push_back(".m4v");
		m_FileExtensions.push_back(".mkv");
		m_FileExtensions.push_back(".mov");
		m_FileExtensions.push_back(".mp4");
		m_FileExtensions.push_back(".mpeg");
		m_FileExtensions.push_back(".mpeg1");
		m_FileExtensions.push_back(".mpeg2");
		m_FileExtensions.push_back(".mpeg4");
		m_FileExtensions.push_back(".mpg");
		m_FileExtensions.push_back(".mts");
		m_FileExtensions.push_back(".mxf");
		m_FileExtensions.push_back(".nuv");
		m_FileExtensions.push_back(".ogm");
		m_FileExtensions.push_back(".ogx");
		m_FileExtensions.push_back(".ogv");
		m_FileExtensions.push_back(".rmvb");
		m_FileExtensions.push_back(".ts");
		m_FileExtensions.push_back(".vob");
		m_FileExtensions.push_back(".vp8");
		m_FileExtensions.push_back(".wmv");
	}

	return m_FileExtensions;
}