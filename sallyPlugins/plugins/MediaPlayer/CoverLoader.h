////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\CoverLoader.h
///
/// \brief	Declares the cover loader class. 
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
#include "Define.h"
#include <SallyAPI/sallyApi.h>
#include "ParameterPicture.h"
#include "MediaDatabase.h"

class CCoverLoader
	: public SallyAPI::System::CThread
{
private:
	SallyAPI::Media::CAmazonInfo	m_AmazonCover;
	
	SallyAPI::GUI::CAppBase*		m_pMainWindow;

	std::string						m_strArtist;
	std::string						m_strAlbum;
	std::string						m_strBand;
	std::string						m_strImageFile;
	std::string						m_strMp3File;

	SallyAPI::GUI::CPicture*	LoadImage();

	virtual void	RunEx();
public:
	CCoverLoader();
	virtual ~CCoverLoader();

	void	SetStaticValues(SallyAPI::GUI::CAppBase* mainWindow);

	void	SetValues(const std::string& artist, const std::string& album, const std::string& band,
		const std::string& imageFile, const std::string& mp3File);
};
