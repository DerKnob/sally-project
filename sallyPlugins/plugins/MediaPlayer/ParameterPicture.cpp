////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\ParameterPicture.cpp
///
/// \brief	Implements the parameter picture class. 
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

#include "ParameterPicture.h"

CParameterPicture::CParameterPicture(SallyAPI::GUI::CPicture* picture, std::string filename)
	:m_pPicture(picture), m_strFilename(filename)
{
}

CParameterPicture::~CParameterPicture()
{
}

SallyAPI::GUI::CPicture* CParameterPicture::GetPicture()
{
	return m_pPicture;
}

std::string CParameterPicture::GetFilename()
{
	return m_strFilename;
}