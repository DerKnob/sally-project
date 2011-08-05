////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\MediaPlayer.h
///
/// \brief	Declares the media player class. 
///
/// \author	Christian Knobloch
/// \date	05.08.2011
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
#include <sallyAPI/sallyAPI.h>
#include <vlc/vlc.h>
#include "AudioFile.h"
#include "VideoFile.h"

#pragma comment(lib, "libvlc.lib")

enum PLAY_STATE {PLAY_STATE_STOPPED, PLAY_STATE_RUNNING, PLAY_STATE_PAUSE};

class CMediaPlayer;

struct ctx
{
	SallyAPI::GUI::CPicture*			currentPicture;
	SallyAPI::System::CCritSection		mutex;
	SallyAPI::GUI::CApplicationWindow*	window;
	CMediaPlayer*						player;
};

class CMediaPlayer
{
private:
	// vlc variables
	libvlc_instance_t*			m_pVLCInstance;
	libvlc_media_t*				m_pMedia;
	libvlc_media_player_t*		m_pMediaPlayer;
	ctx							m_Context;
	CMediaFile*					m_pMediaFile;

	SallyAPI::GUI::CImageBox*			m_pImageBox;
	SallyAPI::GUI::CPicture*			m_pVideoPicture1;
	SallyAPI::GUI::CPicture*			m_pVideoPicture2;
	SallyAPI::GUI::CApplicationWindow*	m_pParent;
	PLAY_STATE							m_ePlayState;
	SallyAPI::System::CCritSection		m_Lock;
	int									m_iWidth;
	int									m_iHeight;
	int									m_iPitch;

	long			m_lRestorePosition;
	int				m_iRestoreTitle;
	int				m_iRestoreAngel;
	int				m_iRestoreLanguage;
	int				m_iRestoreSubtitel;

	bool			IsReady();
	void			CleanUpMedia();
	void			ShowErrorMessage(const std::string& showMessage);
public:
	CMediaPlayer(SallyAPI::GUI::CImageBox* imageBox, SallyAPI::GUI::CApplicationWindow* parent);
	~CMediaPlayer();

	PLAY_STATE		GetState();

	bool			SetPosition(long position);
	long			GetCurrentPosition();
	long			GetDuration();

	int				GetVolume();
	bool			SetVolume(int newVolume);

	bool			RenderFile(const std::string& filename);

	bool			Play();
	bool			Pause();
	bool			Stop();
	bool			FastForward();
	bool			FastBackward();

	int				GetVideoHeight();
	int				GetVideoWidth();

	/** Rendering **/
	static int		GetTexturePitch(SallyAPI::GUI::CPicture* picture);
	static int*		LockTexture(SallyAPI::GUI::CPicture* picture);
	static void		UnlockTexture(SallyAPI::GUI::CPicture* picture);

	void			LockRender();
	void			UnlockRender();

	void			SwitchBuffer();

	void			OnDeviceLost();
	bool			ShouldResume();
	void			RestoreState();


	/** extra **/
	MEDIAFILE		GetType();
	MP3FileInfo*	GetMp3Tag(); // this methode will lock the media - call UnlockMedia when finished

	void			UnlockMedia();
	void			LockMedia();

	std::string		GetFilename();
	bool			ReloadMp3Tags();
	std::string		GetFormatedText();
	std::string		GetCoverName();
};
