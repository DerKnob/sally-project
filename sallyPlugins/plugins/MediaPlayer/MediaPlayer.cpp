////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\MediaPlayer.cpp
///
/// \brief	Implements the media player class. 
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

#include "MediaPlayer.h"

CMediaPlayer::CMediaPlayer(SallyAPI::GUI::CPicture* videoPicture, SallyAPI::GUI::CControl* parent)
	:m_pVideoPicture(videoPicture), m_pParent(parent), m_pMediaFile(NULL)
{
	m_DWUserId = 0xACDCACDC;

	m_pGraphBuilder = NULL;
	m_pMediaControl = NULL;
	m_pMediaPosition = NULL;
	m_pBasicAudio = NULL;
	m_pBaseFilter = NULL;
	m_pFilterConfig = NULL;
	m_lpIVMRSurfAllocNotify = NULL;
	m_pAllocator = NULL;
	m_pSourceFilterReader = NULL;
	m_pWMAsfReader = NULL;
	m_hLogfile = NULL;

	m_rPlayPostion = -1;
	m_oafPlayState = State_Stopped;
}

CMediaPlayer::~CMediaPlayer()
{
	CleanUpMedia();
}

void CMediaPlayer::CleanUpMedia()
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	if (m_pMediaControl != NULL)
	{
		OAFilterState state;
		do
		{
			m_pMediaControl->Stop();
			m_pMediaControl->GetState(0, &state);
		} while (state != State_Stopped);

		m_pMediaControl->Release();
		m_pMediaControl = NULL;
	}
	if (m_lpIVMRSurfAllocNotify != NULL)
	{
		m_lpIVMRSurfAllocNotify->Release();
		m_lpIVMRSurfAllocNotify = NULL;
	}
	if (m_pWMAsfReader != NULL)
	{
		m_pWMAsfReader->Release();
		m_pWMAsfReader = NULL;
	}
	if (m_pSourceFilterReader != NULL)
	{
		m_pSourceFilterReader->Release();
		m_pSourceFilterReader = NULL;
	}
	if (m_pAllocator != NULL)
	{
		m_pAllocator->TerminateDevice(m_DWUserId);
		m_pAllocator = NULL;
	}
	if (m_pFilterConfig != NULL)
	{
		m_pFilterConfig->Release();
		m_pFilterConfig = NULL;
	}
	if (m_pBaseFilter != NULL)
	{
		m_pBaseFilter->Stop();
		m_pBaseFilter->Release();
		m_pBaseFilter = NULL;
	}
	if (m_pBasicAudio != NULL)
	{
		m_pBasicAudio->Release();
		m_pBasicAudio = NULL;
	}
	if (m_pMediaPosition != NULL)
	{
		m_pMediaPosition->Release();
		m_pMediaPosition = NULL;
	}
	if (m_pGraphBuilder != NULL)
	{
		m_pGraphBuilder->Release();
		m_pGraphBuilder = NULL;
	}
	if (m_hLogfile != NULL)
	{
		CloseHandle(m_hLogfile);
		m_hLogfile = NULL;
	}

	SafeDelete(m_pMediaFile);

	::CoUninitialize();
}

OAFilterState CMediaPlayer::GetState()
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	if (m_pMediaControl == NULL)
		return State_Stopped;

	OAFilterState state;
	m_pMediaControl->GetState(INFINITE, &state);
	return state;
}

int CMediaPlayer::GetDuration()
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	if (m_pMediaPosition == NULL)
		return 0;

	REFTIME refDuration;
	m_pMediaPosition->get_Duration(&refDuration);
	return (int) refDuration;
}

int CMediaPlayer::GetCurrentPosition()
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	if (m_pMediaPosition == NULL)
		return 0;

	REFTIME refDuration;
	m_pMediaPosition->get_CurrentPosition(&refDuration);
	return (int) refDuration;
}

bool CMediaPlayer::SetCurrentPosition(int position)
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	if (m_pMediaPosition == NULL)
		return false;

	m_pMediaPosition->put_CurrentPosition(position);
	return true;
}

bool CMediaPlayer::SetVolume(long newVolume)
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	if (m_pBasicAudio == NULL)
		return false;

	m_pBasicAudio->put_Volume(newVolume);
	return true;
}

long CMediaPlayer::GetVolume()
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	if (m_pBasicAudio == NULL)
		return 0;

	long currentVolume;
	m_pBasicAudio->get_Volume(&currentVolume);
	return currentVolume;
}

bool CMediaPlayer::Play()
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	if (m_pMediaPosition == NULL)
		return false;

	SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();
	HRESULT error;

	if (GetState() == State_Paused)
	{
		error = m_pMediaControl->Run();
		if ((error != S_OK) && (error != S_FALSE)) {
			ShowErrorMessage("The file '%s' can not be played.");
			logger->Error("MediaPlayer: Play() m_pMediaControl->Run");
			logger->Error(error);
			return false;
		}
		return true;
	}

	if (m_rPlayPostion == -1)
	{
		// normal play
		m_pMediaPosition->put_CurrentPosition(0.1);
		error = m_pMediaControl->Run();
		if ((error != S_OK) && (error != S_FALSE)) {
			ShowErrorMessage("The file '%s' can not be played.");
			logger->Error("MediaPlayer: Play() m_pMediaControl->Run");
			logger->Error(error);
			return false;
		}
	}
	else
	{
		// on device lost play restart
		m_pMediaPosition->put_CurrentPosition(m_rPlayPostion);
		error = m_pMediaControl->Run();
		if ((error != S_OK) && (error != S_FALSE)) {
			ShowErrorMessage("The file '%s' can not be played.");
			logger->Error("MediaPlayer: Play() m_pMediaControl->Run");
			logger->Error(error);
			return false;
		}
		if (m_oafPlayState != State_Running)
			m_pParent->SendMessageToParent(NULL, GUI_APP_PLAY, GUI_BUTTON_CLICKED); //m_pMediaControl->Pause();

		m_rPlayPostion = -1;
	}
	return true;
}

bool CMediaPlayer::Pause()
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	if (m_pMediaPosition == NULL)
		return false;

	m_pMediaControl->Pause();

	return true;
}

bool CMediaPlayer::Stop()
{
	if (m_pGraphBuilder == NULL)
		return false;

	CleanUpMedia();

	return true;
}

bool CMediaPlayer::RenderFile(const std::string& filename)
{
	HRESULT error;
	SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();

	// clean up the old media pointers
	CleanUpMedia();

	SallyAPI::System::CAutoLock lock(&m_Lock);

	if (CAudioFile::IsAudioFile(filename))
		m_pMediaFile = new CAudioFile(filename);
	else if (CVideoFile::IsVideoFile(filename))
		m_pMediaFile = new CVideoFile(filename);
	else
		return false;

	::CoInitializeEx(NULL, COINIT_MULTITHREADED);

	/************************************************************************/
	/* Now create the media interfaces                                      */
	/************************************************************************/
	error = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**) &m_pGraphBuilder);
	if ((m_pGraphBuilder == NULL) || (error != S_OK)) {
		ShowErrorMessage("The file '%s' can not be played.");
		logger->Error("MediaPlayer: RenderFile() CoCreateInstance CLSID_FilterGraph");
		logger->Error(error);
		return false;
	}

	SallyAPI::GUI::CApplicationWindow* applicationWindow = 	dynamic_cast<SallyAPI::GUI::CApplicationWindow*> (m_pParent);
	if (applicationWindow == NULL)
		return false;

	if (applicationWindow->GetPropertyString("debugLevel", "info").compare("debug") == 0)
	{
		std::string filename = SallyAPI::System::SallyHelper::GetMediaDirectory(applicationWindow);
		filename.append("mediaPlayerDebug.log");

		m_hLogfile = CreateFile(filename.c_str(), GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_ALWAYS, 0, 0);

		long fileSizeHigh = GetFileSize(m_hLogfile, NULL);
		SetFilePointer(m_hLogfile, fileSizeHigh, 0, FILE_BEGIN);

		m_pGraphBuilder->SetLogFile((DWORD_PTR) m_hLogfile);
	}

	error = m_pGraphBuilder->QueryInterface(IID_IMediaControl, (void**)&m_pMediaControl);
	if ((m_pMediaControl == NULL) || (error != S_OK)) {
		ShowErrorMessage("The file '%s' can not be played.");
		logger->Error("MediaPlayer: RenderFile() QueryInterface->IID_IMediaControl");
		logger->Error(error);
		return false;
	}

	error = m_pGraphBuilder->QueryInterface(IID_IMediaPosition, (void**)&m_pMediaPosition);
	if ((m_pMediaPosition == NULL) || (error != S_OK)) {
		ShowErrorMessage("The file '%s' can not be played.");
		logger->Error("MediaPlayer: RenderFile() QueryInterface->IID_IMediaPosition");
		logger->Error(error);
		return false;
	}

	error = m_pGraphBuilder->QueryInterface(IID_IBasicAudio, (void**)&m_pBasicAudio);
	if ((m_pBasicAudio == NULL) || (error != S_OK)) {
		ShowErrorMessage("The file '%s' can not be played.");
		logger->Error("MediaPlayer: RenderFile() QueryInterface->IID_IBasicAudio");
		logger->Error(error);
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	// DirectX Renderer
	// If Video File init the video specific stuff
	if (m_pMediaFile->GetType() == MEDIAFILE_VIDEO)
	{
		error = CoCreateInstance(CLSID_VideoMixingRenderer9, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**) &m_pBaseFilter);
		if ((m_pBaseFilter == NULL) || (error != S_OK)) {
			ShowErrorMessage("The file '%s' can not be played.");
			logger->Error("MediaPlayer: RenderFile() QueryInterface->CLSID_VideoMixingRenderer9");
			logger->Error(error);
			return false;
		}

		error = m_pBaseFilter->QueryInterface(IID_IVMRFilterConfig9, reinterpret_cast<void**>(&m_pFilterConfig)) ;
		if ((m_pFilterConfig == NULL) || (error != S_OK)) {
			ShowErrorMessage("The file '%s' can not be played.");
			logger->Error("MediaPlayer: RenderFile() QueryInterface->IID_IVMRFilterConfig9");
			logger->Error(error);
			return false;
		}

		m_pFilterConfig->SetRenderingMode(VMR9Mode_Renderless);

		error = m_pBaseFilter->QueryInterface(IID_IVMRSurfaceAllocatorNotify9, (void**)(&m_lpIVMRSurfAllocNotify));
		if ((m_lpIVMRSurfAllocNotify == NULL) || (error != S_OK)) {
			ShowErrorMessage("The file '%s' can not be played.");
			logger->Error("MediaPlayer: RenderFile() QueryInterface->IID_IVMRSurfaceAllocatorNotify9");
			logger->Error(error);
			return false;
		}

		m_pAllocator = new SallyAPI::Core::CTextureAllocator(m_pVideoPicture);

		m_lpIVMRSurfAllocNotify->AdviseSurfaceAllocator(m_DWUserId, m_pAllocator);
		m_pAllocator->AdviseNotify(m_lpIVMRSurfAllocNotify);

		error = m_pGraphBuilder->AddFilter(m_pBaseFilter, L"Video Mixing Renderer 9");
		if (error != S_OK) {
			ShowErrorMessage("The file '%s' can not be played.");
			logger->Error("MediaPlayer: RenderFile() Graph->AddFilter");
			logger->Error(error);
			return false;
		}
	}

	//Convert the path to unicode and than render the file
	WCHAR wstrSoundPath[MAX_PATH];
	MultiByteToWideChar(CP_ACP, 0, filename.c_str(), -1, wstrSoundPath, MAX_PATH);

	if (m_pMediaFile->GetType() == MEDIAFILE_AUDIO)
	{
		// mp4 file filter
		if (!RenderFile(wstrSoundPath))
		{
			if (m_pWMAsfReader == NULL)
			{
				m_pGraphBuilder->RemoveFilter(m_pWMAsfReader);
			}

			error = m_pGraphBuilder->RenderFile(wstrSoundPath, NULL);
			if (error != S_OK)
			{
				ShowErrorMessage("The file '%s' can not be played.");
				logger->Error("MediaPlayer: RenderFile() Graph->RenderFile");
				logger->Error(error);
				return false;
			}
		}
	}
	else
	{
		// video file renderer
		error = m_pGraphBuilder->RenderFile(wstrSoundPath, NULL);
		if (error != S_OK)
		{
			ShowErrorMessage("The file '%s' can not be played.");
			logger->Error("MediaPlayer: RenderFile() Graph->RenderFile");
			logger->Error(error);
			return false;
		}
	}
	return true;
}

bool CMediaPlayer::RenderFile(WCHAR wstrSoundPath[MAX_PATH])
{
	HRESULT error;
	SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();

	error = CoCreateInstance(CLSID_WMAsfReader, NULL, CLSCTX_INPROC,  IID_IBaseFilter, (void**)&m_pWMAsfReader);
	if ((m_pWMAsfReader == NULL) || (error != S_OK)) {
		logger->Error("MediaPlayer: RenderFile() CoCreateInstance CLSID_WMAsfReader");
		logger->Error(error);
		return false;
	}

	error = m_pWMAsfReader->QueryInterface(IID_IFileSourceFilter, (void**) &m_pSourceFilterReader);
	if ((m_pSourceFilterReader == NULL) || (error != S_OK)) {
		logger->Error("MediaPlayer: RenderFile() m_pWMAsfReader->QueryInterface");
		logger->Error(error);
		return false;
	}

	error = m_pSourceFilterReader->Load(wstrSoundPath, NULL);
	if (error != S_OK) {
		logger->Error("MediaPlayer: RenderFile() m_pSourceFilterReader->Load");
		logger->Error(error);
		return false;
	}

	error = m_pGraphBuilder->AddFilter(m_pWMAsfReader, wstrSoundPath);
	if (error != S_OK) {
		logger->Error("MediaPlayer: RenderFile() m_pGraphBuilder->AddFilter");
		logger->Error(error);
		return false;
	}

	IEnumPins* EnumPins;
	error = m_pWMAsfReader->EnumPins(&EnumPins);
	if (error != S_OK) {
		logger->Error("MediaPlayer: RenderFile() m_pWMAsfReader->EnumPins");
		logger->Error(error);

		EnumPins->Release();
		return false;
	}

	IPin*	piPin;
	while (EnumPins->Next(1, &piPin, 0) == NOERROR)
	{
		error = m_pGraphBuilder->Render(piPin);
		if (error != S_OK) {
			logger->Error("MediaPlayer: RenderFile() m_pGraphBuilder->Render");
			logger->Error(error);

			piPin->Release();
			EnumPins->Release();
			return false;
		}
		piPin->Release();
	}
	
	EnumPins->Release();
	return true;
}

void CMediaPlayer::ShowErrorMessage(const std::string& showMessage)
{
	m_Lock.Unlock(); // manually Unlock
	SallyAPI::GUI::SendMessage::CParameterString parameterString(showMessage);
	m_pParent->SendMessageToParent(NULL, 0, GUI_APP_SHOW_ERROR_MESSAGE, &parameterString);
}

void CMediaPlayer::OnDeviceLost()
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	m_rPlayPostion = GetCurrentPosition();
	m_oafPlayState = GetState();
}

bool CMediaPlayer::ShouldResume()
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	if (m_rPlayPostion != -1)
		return true;
	return false;
}

int CMediaPlayer::GetVideoWidth()
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	if (m_pAllocator == NULL)
		return 0;
	return m_pAllocator->GetVideoWidth();
}

int CMediaPlayer::GetVideoHeight()
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	if (m_pAllocator == NULL)
		return 0;
	return m_pAllocator->GetVideoHeight();
}

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