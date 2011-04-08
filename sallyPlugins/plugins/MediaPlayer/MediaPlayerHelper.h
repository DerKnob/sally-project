////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\MediaPlayerHelper.h
///
/// \brief	Declares the media player helper class. 
///
/// \author	Christian Knobloch
/// \date	08.04.2010
///
/// This file is part of the Sally Project
/// 
/// Copyright(c) 2008-2011 Sally Project
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
#include "Define.h"
#include <sallyAPI\sallyAPI.h>
#include "MediaDatabase.h"
#include "Playlist.h"
#include "CoverLoader.h"
#include "MediaPlayer.h"

class CMediaPlayerHelper :
	public SallyAPI::System::CThread
{
private:
	CMediaPlayer*							m_pMediaPlayer;
	SallyAPI::GUI::CAppBase*				m_pMainWindow;
	CPlaylist*								m_pPlaylist;
	std::map<std::string, CCoverLoader*>*	m_pmCoverLoaders;

	void			ProcessAudioFile();
	void			ProcessVideoFile();

	virtual void	RunEx();
public:
	CMediaPlayerHelper();
	virtual ~CMediaPlayerHelper();

	void SetStaticValues(CPlaylist* playlist, SallyAPI::GUI::CAppBase* control,
		std::map<std::string, CCoverLoader*>* coverLoaders);

	void SetValues(CMediaPlayer* mediaPlayer);
};
