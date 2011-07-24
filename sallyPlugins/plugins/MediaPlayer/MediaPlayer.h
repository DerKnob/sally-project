////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\MediaPlayer.h
///
/// \brief	Declares the media player class. 
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
#include "Define.h"
#include <sallyAPI/sallyAPI.h>
#include <DShow.h>
#include <D3d9.h>
#include <Vmr9.h>
#define __IDxtCompositor_INTERFACE_DEFINED__
#define __IDxtAlphaSetter_INTERFACE_DEFINED__
#define __IDxtJpeg_INTERFACE_DEFINED__
#define __IDxtKey_INTERFACE_DEFINED__
#include "AudioFile.h"
#include "VideoFile.h"

class CMediaPlayer
{
private:
	IGraphBuilder*						m_pGraphBuilder;
	IMediaControl*						m_pMediaControl;
	IMediaPosition*						m_pMediaPosition;
	IBasicAudio*						m_pBasicAudio;
	IBaseFilter*						m_pBaseFilter;
	IVMRFilterConfig9*					m_pFilterConfig;
	IVMRSurfaceAllocatorNotify9*		m_lpIVMRSurfAllocNotify;
	REFTIME								m_rPlayPostion;
	OAFilterState						m_oafPlayState;
	IBaseFilter*						m_pWMAsfReader;
	IFileSourceFilter*					m_pSourceFilterReader;

	SallyAPI::System::CCritSection		m_Lock;
	
	SallyAPI::Core::CTextureAllocator*	m_pAllocator;
	DWORD_PTR							m_DWUserId;

	CMediaFile*							m_pMediaFile;
	SallyAPI::GUI::CPicture*			m_pVideoPicture;
	SallyAPI::GUI::CControl*			m_pParent;
	HANDLE								m_hLogfile;

	bool			RenderFile(WCHAR wstrSoundPath[MAX_PATH]);

	void			CleanUpMedia();
	void			ShowErrorMessage(const std::string& showMessage);
public:
	CMediaPlayer(SallyAPI::GUI::CPicture* videoPicture, SallyAPI::GUI::CControl* parent);
	~CMediaPlayer();

	OAFilterState	GetState();
	void			OnDeviceLost();
	bool			ShouldResume();

	bool			SetPosition(int position);
	int				GetCurrentPosition();
	int				GetDuration();

	long			GetVolume();
	bool			SetVolume(long newVolume);

	bool			Play();
	bool			Pause();
	bool			Stop();

	bool			RenderFile(const std::string& filename);

	int				GetVideoHeight();
	int				GetVideoWidth();

	MEDIAFILE		GetType();
	MP3FileInfo*	GetMp3Tag(); // this methode will lock the media - call UnlockMedia when finished

	void			UnlockMedia();
	void			LockMedia();

	std::string		GetFilename();
	bool			ReloadMp3Tags();
	std::string		GetFormatedText();
	std::string		GetCoverName();
};
