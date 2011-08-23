////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\MediaPlayer.cpp
///
/// \brief	Implements the media player class. 
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

#include "MediaPlayer.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

static void* lockResource(void* data, void** p_pixels)
{
    struct ctx* videoCtx = (struct ctx*)data;

	*p_pixels = CMediaPlayer::LockTexture(videoCtx->currentPicture);
	return NULL;
}

static void unlockResource(void* data, void* id, void *const* p_pixels)
{
	struct ctx* videoCtx = (struct ctx*) data;

	CMediaPlayer::UnlockTexture(videoCtx->currentPicture);

	videoCtx->player->SwitchBuffer();
}

static void displayResource(void* data, void* id)
{
}

static void vlcEventManager(const libvlc_event_t* ev, void* data)
{
	struct ctx* videoCtx = (struct ctx*) data;

    switch (ev->type)
	{
	case libvlc_MediaPlayerEndReached:
		videoCtx->window->SendMessageToParent(videoCtx->window, GUI_APP_NEXT, GUI_BUTTON_CLICKED);
		break;
    }
	return;
}

int* CMediaPlayer::LockTexture(SallyAPI::GUI::CPicture* picture)
{
	SallyAPI::Core::CTexture* texture = picture->GetTexture();
	if (texture == NULL)
		return NULL;

	LPDIRECT3DTEXTURE9 d3dTexture = texture->GetTexture();
	if (d3dTexture == NULL)
		return NULL;

	IDirect3DSurface9* d3d_surf = NULL;
	d3dTexture->GetSurfaceLevel(0, &d3d_surf);

	if (d3d_surf == NULL)
		return NULL;

	// lock the surface
	D3DLOCKED_RECT locked_rect;
	if (d3d_surf->LockRect(&locked_rect, NULL, 0) != D3D_OK)
		return NULL;

	int* directXTextutureData = (int*)(locked_rect.pBits);

	return directXTextutureData;
}

int CMediaPlayer::GetTexturePitch(SallyAPI::GUI::CPicture* picture)
{
	SallyAPI::Core::CTexture* texture = picture->GetTexture();
	if (texture == NULL)
		return 0;

	LPDIRECT3DTEXTURE9 d3dTexture = texture->GetTexture();
	if (d3dTexture == NULL)
		return 0;

	IDirect3DSurface9* d3d_surf = NULL;
	d3dTexture->GetSurfaceLevel(0, &d3d_surf);

	if (d3d_surf == NULL)
		return 0;

	// lock the surface
	D3DLOCKED_RECT locked_rect;
	d3d_surf->LockRect(&locked_rect, NULL, 0);

	int pitchDirectX = locked_rect.Pitch;

	d3d_surf->UnlockRect();

	return pitchDirectX;
}

void CMediaPlayer::UnlockTexture(SallyAPI::GUI::CPicture* picture)
{
	SallyAPI::Core::CTexture* texture = picture->GetTexture();
	if (texture == NULL)
		return;

	LPDIRECT3DTEXTURE9 d3dTexture = texture->GetTexture();
	if (d3dTexture == NULL)
		return;

	IDirect3DSurface9* d3d_surf = NULL;
	d3dTexture->GetSurfaceLevel(0, &d3d_surf);

	if (d3d_surf == NULL)
		return;

	// we are done, unlock the surface
	d3d_surf->UnlockRect();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

CMediaPlayer::CMediaPlayer(SallyAPI::GUI::CImageBox* imageBox, SallyAPI::GUI::CApplicationWindow* parent)
	:m_pImageBox(imageBox), m_pParent(parent), m_ePlayState(PLAY_STATE_STOPPED), m_pVLCInstance(NULL),
	m_pMediaPlayer(NULL), m_pMedia(NULL), m_pMediaFile(NULL), m_bRestore(false), m_iCurrentNumber(-1)
{
	m_pVideoPicture1 = new SallyAPI::GUI::CPicture();
	m_pVideoPicture2 = new SallyAPI::GUI::CPicture();

	m_Context.currentPicture = m_pVideoPicture1;
	m_Context.window = m_pParent;
	m_Context.player = this;
}

CMediaPlayer::~CMediaPlayer()
{
	CleanUpMedia();

	m_pImageBox->SetImageId(GUI_NO_IMAGE);
	SafeDelete(m_pVideoPicture1);
	SafeDelete(m_pVideoPicture2);
}

void CMediaPlayer::CleanUpMedia()
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	if (m_pMediaPlayer != NULL)
	{
		libvlc_media_player_stop(m_pMediaPlayer);
		libvlc_media_player_release (m_pMediaPlayer);
		m_pMediaPlayer = NULL;
	}

	if (m_pMedia != NULL)
	{
		libvlc_media_release(m_pMedia);
		m_pMedia = NULL;
	}

	if (m_pVLCInstance != NULL)
	{
		libvlc_release(m_pVLCInstance);
		m_pVLCInstance = NULL;
	}

	m_ePlayState = PLAY_STATE_STOPPED;

	SafeDelete(m_pMediaFile);

	m_iWidth = -1;
	m_iHeight = -1;
	m_iCurrentNumber = -1;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

bool CMediaPlayer::RenderFile(const std::string& filename, int currentNumber)
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	CleanUpMedia();

	if (!SallyAPI::File::FileHelper::FileExists(filename))
		return false;

	if (CAudioFile::IsAudioFile(filename))
	{
		m_pMediaFile = new CAudioFile(filename);
	}
	else if (CVideoFile::IsVideoFile(filename))
	{
		m_pMediaFile = new CVideoFile(filename);
		if (InitOutputSize(filename) == false)
			return false;
	}
	else
	{
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	m_pVLCInstance = CreateVLCInstance();

	if (m_pVLCInstance == NULL)
		return false;

	/* Create a new item */
	std::string shortFilename = SallyAPI::String::PathHelper::GetShortFilename(filename);

	if (shortFilename.length() == 0)
		return false;

	m_pMedia = libvlc_media_new_path(m_pVLCInstance, shortFilename.c_str());

	/* Create a media player playing environement */
	m_pMediaPlayer = libvlc_media_player_new_from_media(m_pMedia);

	if (m_pMediaPlayer == NULL)
		return false;

	libvlc_event_manager_t* em = libvlc_media_player_event_manager(m_pMediaPlayer);

	libvlc_event_attach(em, libvlc_MediaPlayerEndReached, vlcEventManager, &m_Context);
	
	if (m_pMediaFile->GetType() == MEDIAFILE_VIDEO)
	{
		m_pVideoPicture1->CreateEmptyD3DFormat(m_iWidth, m_iHeight, D3DFMT_X8R8G8B8);
		m_pVideoPicture2->CreateEmptyD3DFormat(m_iWidth, m_iHeight, D3DFMT_X8R8G8B8);
		m_iPitch = CMediaPlayer::GetTexturePitch(m_pVideoPicture1);

		libvlc_video_set_callbacks(m_pMediaPlayer, lockResource, unlockResource, displayResource, &m_Context);
		libvlc_video_set_format(m_pMediaPlayer, "RV32", m_iWidth, m_iHeight, m_iPitch);
	}

	SetVolume(100);

	m_iCurrentNumber = currentNumber;
	return true;
}

bool CMediaPlayer::InitOutputSize(const std::string& filename)
{
	libvlc_instance_t* instance = CreateVLCInstance();

	if (instance == NULL)
		return false;

	/* Create a new item */
	std::string shortFilename = SallyAPI::String::PathHelper::GetShortFilename(filename);

	if (shortFilename.length() == 0)
		return false;

	libvlc_media_t* media = libvlc_media_new_path(instance, shortFilename.c_str());

	/* Create a media player playing environement */
	libvlc_media_player_t* mediaPlayer = libvlc_media_player_new_from_media(media);

	if (mediaPlayer != NULL)
	{
		m_pVideoPicture1->CreateEmptyD3DFormat(1, 1, D3DFMT_X8R8G8B8);
		m_pVideoPicture2->CreateEmptyD3DFormat(1, 1, D3DFMT_X8R8G8B8);
		m_iPitch = CMediaPlayer::GetTexturePitch(m_pVideoPicture1);

		libvlc_video_set_callbacks(mediaPlayer, lockResource, unlockResource, displayResource, &m_Context);
		libvlc_video_set_format(mediaPlayer, "RV32", 1, 1, 4);

		libvlc_media_player_play(mediaPlayer);

		libvlc_audio_set_volume(mediaPlayer, 0);

		int tries = 0;
		while ((m_iWidth == -1) && (tries < 10 * 10)) // 10 sec
		{
			libvlc_video_get_size(mediaPlayer, 0, (unsigned int*) &m_iWidth, (unsigned int*) &m_iHeight);
			Sleep(100);
			tries++;
		}
	}

	// cleanup
	if (mediaPlayer != NULL)
	{
		libvlc_media_player_stop(mediaPlayer);
		libvlc_media_player_release (mediaPlayer);
		mediaPlayer = NULL;
	}

	if (media != NULL)
	{
		libvlc_media_release(media);
		media = NULL;
	}

	if (instance != NULL)
	{
		libvlc_release(instance);
		instance = NULL;
	}

	return true;
}

libvlc_instance_t* CMediaPlayer::CreateVLCInstance()
{
	char pluginPathChar[MAX_PATH]; // create plugin path
	std::string pluginPath = "--plugin-path=";
	pluginPath.append(SallyAPI::Media::CMediaHelper::GetVLCPluginDirectory());

	strcpy_s(pluginPathChar, MAX_PATH, pluginPath.c_str());

	//////////////////////////////////////////////////////////////////////////
	const char * const vlc_args[] = {
		pluginPathChar, 
		"-I", "dummy",
		"--no-osd",
#ifdef _DEBUG
		"--verbose", "3",
		"--extraintf=logger", // Log anything
#endif
		"--ignore-config", // Don't use VLC's config
	};

	/*
     *  Initialise libVLC
     */
    libvlc_instance_t* instance = libvlc_new (sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
	
	return instance;
}

bool CMediaPlayer::Play()
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	if (m_pMediaPlayer == NULL)
		return false;

	/* play the media_player */
	libvlc_media_player_play (m_pMediaPlayer);

	PLAY_STATE playStateOld = m_ePlayState;
	m_ePlayState = PLAY_STATE_RUNNING;

	// return if the palyer was in the pause mode before
	if (playStateOld == PLAY_STATE_PAUSE)
		return true;

	RestoreState();
	return true;
}

long CMediaPlayer::GetDuration()
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	if (m_pMediaPlayer == NULL)
		return 0;

	long iLength = (long) libvlc_media_player_get_length(m_pMediaPlayer);
	return iLength;
}

long CMediaPlayer::GetCurrentPosition()
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	if (m_pMediaPlayer == NULL)
		return 0;

	m_iRestoreTitle = libvlc_media_player_get_title(m_pMediaPlayer);
	m_lRestorePosition = (long) libvlc_media_player_get_time(m_pMediaPlayer);

	//m_iRestoreAngel = GetCurrentAngel();
	m_iRestoreLanguage = libvlc_audio_get_track(m_pMediaPlayer);
	m_iRestoreSubtitel = libvlc_video_get_spu(m_pMediaPlayer);

	return m_lRestorePosition;
}

bool CMediaPlayer::SetPosition(long position)
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	if (m_pMediaPlayer == NULL)
		return false;

	libvlc_media_player_set_time(m_pMediaPlayer,(libvlc_time_t)position);
	return true;
}

bool CMediaPlayer::SetVolume(int newVolume)
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	if (m_pMediaPlayer == NULL)
		return false;

	int l = libvlc_audio_set_volume(m_pMediaPlayer, newVolume / 2);

	return true;
}

int CMediaPlayer::GetVolume()
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	if (m_pMediaPlayer == NULL)
		return 0;

	int iVolume = libvlc_audio_get_volume(m_pMediaPlayer);
	return iVolume;
}

bool CMediaPlayer::Pause()
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	if (m_pMediaPlayer == NULL)
		return false;

	libvlc_media_player_pause (m_pMediaPlayer);
	m_ePlayState = PLAY_STATE_PAUSE;
	return true;
}

bool CMediaPlayer::Stop()
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	if (m_ePlayState == PLAY_STATE_STOPPED)
		return false;

	if (m_pMediaPlayer == NULL)
		return false;

	/* Stop playing */
	libvlc_media_player_stop (m_pMediaPlayer);

	m_ePlayState = PLAY_STATE_STOPPED;
	return true;
}

bool CMediaPlayer::FastForward()
{
	long position = GetCurrentPosition();

	SallyAPI::System::CAutoLock lock(&m_Lock);

	if (m_pMediaPlayer == NULL)
		return false;

	libvlc_media_player_set_time(m_pMediaPlayer, position + 20000);
	return true;
}

bool CMediaPlayer::FastBackward()
{
	long position = GetCurrentPosition();

	SallyAPI::System::CAutoLock lock(&m_Lock);

	if (m_pMediaPlayer == NULL)
		return false;

	if (position - 2000 < 1)
		position = 1;

	libvlc_media_player_set_time(m_pMediaPlayer, position - 20000);
	return true;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

std::vector<std::string> CMediaPlayer::GetLanguages()
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	std::vector<std::string> result;

	if (!IsReady())
		return result;

	libvlc_track_description_t* list = libvlc_audio_get_track_description(m_pMediaPlayer);

	while (list != NULL)
	{
		result.push_back(list->psz_name);
		list = list->p_next;
	}

	return result;
}

std::vector<std::string> CMediaPlayer::GetChapters()
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	std::vector<std::string> result;

	if (!IsReady())
		return result;

	int title = libvlc_media_player_get_title(m_pMediaPlayer);
	libvlc_track_description_t* list = libvlc_video_get_chapter_description(m_pMediaPlayer, title);

	while (list != NULL)
	{
		result.push_back(list->psz_name);
		list = list->p_next;
	}

	return result;
}

std::vector<std::string> CMediaPlayer::GetAngels()
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	std::vector<std::string> result;

	if (!IsReady())
		return result;

	return result;
}

std::vector<std::string> CMediaPlayer::GetSubtitles()
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	std::vector<std::string> result;

	if (!IsReady())
		return result;

	libvlc_track_description_t* list = libvlc_video_get_spu_description(m_pMediaPlayer);

	while (list != NULL)
	{
		result.push_back(list->psz_name);
		list = list->p_next;
	}

	return result;
}

//////////////////////////////////////////////////////////////////////////

int CMediaPlayer::GetCurrentLanguage()
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	if (!IsReady())
		return -1;

	return libvlc_audio_get_track(m_pMediaPlayer);
}

int CMediaPlayer::GetCurrentChapter()
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	if (!IsReady())
		return -1;

	return libvlc_media_player_get_chapter(m_pMediaPlayer);
}

int CMediaPlayer::GetCurrentAngel()
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	if (!IsReady())
		return -1;

	return 0;
}

int CMediaPlayer::GetCurrentSubtitle()
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	if (!IsReady())
		return -1;

	return libvlc_video_get_spu(m_pMediaPlayer);
}

//////////////////////////////////////////////////////////////////////////

bool CMediaPlayer::SetLanguage(int i)
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	if (!IsReady())
		return false;

	libvlc_audio_set_track(m_pMediaPlayer, i);
	return true;
}

bool CMediaPlayer::SetChapter(int i)
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	if (!IsReady())
		return false;

	libvlc_media_player_set_chapter(m_pMediaPlayer, i);
	return true;
}

bool CMediaPlayer::SetAngel(int i)
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	if (!IsReady())
		return false;

	return true;
}

bool CMediaPlayer::SetSubtitle(int i)
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	if (!IsReady())
		return false;

	libvlc_video_set_spu(m_pMediaPlayer, i);
	return true;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

int CMediaPlayer::GetVideoWidth()
{
	return m_iWidth;
}

int CMediaPlayer::GetVideoHeight()
{
	return m_iHeight;
}

PLAY_STATE CMediaPlayer::GetState()
{
	return m_ePlayState;
}

bool CMediaPlayer::IsReady()
{
	if (m_pMediaPlayer == NULL)
		return false;

	libvlc_state_t state = libvlc_media_player_get_state(m_pMediaPlayer);

	if ((state == libvlc_Playing) || (state == libvlc_Paused) || (state == libvlc_Stopped))
		return true;

	return false;
}

void CMediaPlayer::SwitchBuffer()
{
	m_Context.mutex.Lock();
	m_pImageBox->SetPicture(m_Context.currentPicture);
	m_Context.mutex.Unlock();

	if (m_Context.currentPicture == m_pVideoPicture1)
		m_Context.currentPicture = m_pVideoPicture2;
	else
		m_Context.currentPicture = m_pVideoPicture1;
}

void CMediaPlayer::LockRender()
{
	m_Context.mutex.Lock();
}

void CMediaPlayer::UnlockRender()
{
	m_Context.mutex.Unlock();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void CMediaPlayer::OnSystemAPMSuspend()
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	if (m_pMediaPlayer == NULL)
		return;

	libvlc_state_t state = libvlc_media_player_get_state(m_pMediaPlayer);

	if (state != libvlc_Playing)
		return;

	m_bRestore = true;
}

bool CMediaPlayer::ShouldResume()
{
	return m_bRestore;
}

void CMediaPlayer::RestoreState()
{
	if (!m_bRestore)
		return;
		
	libvlc_media_player_set_title(m_pMediaPlayer, m_iRestoreTitle);
	libvlc_audio_set_track(m_pMediaPlayer, m_iRestoreLanguage);
	libvlc_media_player_set_time(m_pMediaPlayer,(libvlc_time_t)m_lRestorePosition);
	if (m_iRestoreSubtitel > 0)
		libvlc_video_set_spu(m_pMediaPlayer, m_iRestoreSubtitel);

	m_bRestore = false;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

MEDIAFILE CMediaPlayer::GetType()
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	if (m_pMediaFile == NULL)
		return MEDIAFILE_NOT_SET;
	
	return m_pMediaFile->GetType();
}

MP3FileInfo* CMediaPlayer::GetMp3Tag()
{
	LockMedia();

	if (m_pMediaFile == NULL)
		return NULL;

	if (m_pMediaFile->GetType() != MEDIAFILE_AUDIO)
		return NULL;
	
	CAudioFile* mp3File = (CAudioFile*) m_pMediaFile;

	return mp3File->GetMp3Tag();
}

void CMediaPlayer::LockMedia()
{
	m_Lock.Lock();
}

void CMediaPlayer::UnlockMedia()
{
	m_Lock.Unlock();
}


std::string CMediaPlayer::GetFilename()
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	if (m_pMediaFile == NULL)
		return "";

	return m_pMediaFile->GetFilename();
}

bool CMediaPlayer::ReloadMp3Tags()
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	if (m_pMediaFile == NULL)
		return false;

	if (m_pMediaFile->GetType() != MEDIAFILE_AUDIO)
		return false;

	CAudioFile* mp3File = (CAudioFile*) m_pMediaFile;

	mp3File->ReloadMp3Tags();
	return true;
}

std::string CMediaPlayer::GetFormatedText()
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	if (m_pMediaFile == NULL)
		return "";

	return m_pMediaFile->GetFormatedText();
}

std::string CMediaPlayer::GetCoverName()
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	if (m_pMediaFile == NULL)
		return "";

	if (m_pMediaFile->GetType() != MEDIAFILE_AUDIO)
		return "";

	CAudioFile* mp3File = (CAudioFile*) m_pMediaFile;

	return mp3File->GetCoverName();
}

int CMediaPlayer::GetCurrentNumber()
{
	return m_iCurrentNumber;
}