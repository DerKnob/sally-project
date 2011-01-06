////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\VolumeManager.cpp
///
/// \brief	Implements the volume manager class. 
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
/// it under the terms of the GNU Lesser General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
/// 
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
/// GNU Lesser General Public License for more details.
/// 
/// You should have received a copy of the GNU Lesser General Public License
/// along with this program. If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "VolumeManager.h"

SallyAPI::Sound::Volume::CVolumeManager*	SallyAPI::Sound::Volume::CVolumeManager::m_pObject = NULL;

using namespace SallyAPI::Sound::Volume;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CVolumeManager::CVolumeManager()
///
/// \brief	Default constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CVolumeManager::CVolumeManager()
{
	m_pVolumeNotification = NULL;
	m_pEndpointVolume = NULL;
	IMMDeviceEnumerator* deviceEnumerator = NULL;
	HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);

	if (deviceEnumerator != NULL)
	{
		IMMDevice* defaultDevice = NULL;

		hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
		deviceEnumerator->Release();
		deviceEnumerator = NULL;

		hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&m_pEndpointVolume);
		defaultDevice->Release();
		defaultDevice = NULL; 

		m_pVolumeNotification = new SallyAPI::Sound::Volume::CVolumeNotification(); 
		m_pEndpointVolume->RegisterControlChangeNotify(m_pVolumeNotification); 
	}


	// get the number of mixer devices present in the system
	int m_nNumMixers = ::mixerGetNumDevs();

	m_hMixer = NULL;
	::ZeroMemory(&m_mxcaps, sizeof(MIXERCAPS));

	m_dwMuteControlID = 0;

	// open the first mixer
	// A "mapper" for audio mixer devices does not currently exist.
	if (m_nNumMixers != 0)
	{
		MMRESULT hr = ::mixerOpen(&m_hMixer, 0, 0, NULL, MIXER_OBJECTF_MIXER);
		if (hr != MMSYSERR_NOERROR)
			return;

		if (mixerGetDevCaps(reinterpret_cast<UINT>(m_hMixer), &m_mxcaps, sizeof(MIXERCAPS)) != MMSYSERR_NOERROR)
			return;

		// get dwLineID
		MIXERLINE mxl;
		mxl.cbStruct = sizeof(MIXERLINE);
		mxl.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
		if (::mixerGetLineInfo(reinterpret_cast<HMIXEROBJ>(m_hMixer),
			&mxl, MIXER_OBJECTF_HMIXER | MIXER_GETLINEINFOF_COMPONENTTYPE)
			!= MMSYSERR_NOERROR)
		{
			return;
		}

		// get dwControlID
		MIXERCONTROL mxc;
		MIXERLINECONTROLS mxlc;
		mxlc.cbStruct = sizeof(MIXERLINECONTROLS);
		mxlc.dwLineID = mxl.dwLineID;
		mxlc.dwControlType = MIXERCONTROL_CONTROLTYPE_MUTE;
		mxlc.cControls = 1;
		mxlc.cbmxctrl = sizeof(MIXERCONTROL);
		mxlc.pamxctrl = &mxc;
		if (::mixerGetLineControls(reinterpret_cast<HMIXEROBJ>(m_hMixer),
			&mxlc, MIXER_OBJECTF_HMIXER | MIXER_GETLINECONTROLSF_ONEBYTYPE)
			!= MMSYSERR_NOERROR)
		{
			return;
		}
		m_dwMuteControlID = mxc.dwControlID;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CVolumeManager::~CVolumeManager()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CVolumeManager::~CVolumeManager()
{
	if (m_hMixer != NULL)
	{
		::mixerClose(m_hMixer);
		m_hMixer = NULL;
	}
	if (m_pEndpointVolume != NULL)
	{
		m_pEndpointVolume->UnregisterControlChangeNotify(m_pVolumeNotification); 
		SafeRelease(m_pEndpointVolume);
		SafeRelease(m_pVolumeNotification);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CVolumeManager::RegisterListener(SallyAPI::GUI::CControl* control)
///
/// \brief	Register listener. 
///
/// \author	Christian Knobloch
/// \date	05.05.2010
///
/// \param [in,out]	control	If non-null, the control. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CVolumeManager::RegisterListener(SallyAPI::GUI::CControl* control)
{
	m_vListeners.push_back(control);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CVolumeManager::UnregisterListener(SallyAPI::GUI::CControl* control)
///
/// \brief	Unregister listener. 
///
/// \author	Christian Knobloch
/// \date	05.05.2010
///
/// \param [in,out]	control	If non-null, the control. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CVolumeManager::UnregisterListener(SallyAPI::GUI::CControl* control)
{
	std::vector<SallyAPI::GUI::CControl*>::iterator iter = m_vListeners.begin();

	while (iter != m_vListeners.end())
	{
		SallyAPI::GUI::CControl* temp = *iter;

		if (temp == control)
		{
			m_vListeners.erase(iter);
			return;
		}

		++iter;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CVolumeManager::OnVolumeChange()
///
/// \brief	Executes the volume change action. 
///
/// \author	Christian Knobloch
/// \date	06.01.2011
////////////////////////////////////////////////////////////////////////////////////////////////////

void CVolumeManager::OnVolumeChange()
{
	std::vector<SallyAPI::GUI::CControl*>::iterator iter = m_vListeners.begin();

	while (iter != m_vListeners.end())
	{
		SallyAPI::GUI::CControl* temp = *iter;

		temp->SendMessageToParent(NULL, 0, MS_SALLY_VOLUME_CHANGED);

		++iter;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::Sound::Volume::CVolumeManager* CVolumeManager::GetInstance()
///
/// \brief	Gets the instance. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	null if it fails, else the instance. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::Sound::Volume::CVolumeManager* CVolumeManager::GetInstance()
{
	if (m_pObject == NULL) {
		m_pObject = new SallyAPI::Sound::Volume::CVolumeManager();
	}
	return m_pObject;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CVolumeManager::DeleteInstance()
///
/// \brief	Deletes an instance. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CVolumeManager::DeleteInstance()
{
	SafeDelete(m_pObject);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CVolumeManager::GetVolume()
///
/// \brief	Gets the volume. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The volume. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CVolumeManager::GetVolume()
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	int volume = 0;

	if ((m_pEndpointVolume == NULL) || (!option->GetPropertyBool("sally", "volumeWindows", true)))
	{
		// Win XP
		DWORD dVolume = 0;
		waveOutGetVolume(0,&dVolume);
		volume = HIWORD(dVolume);
		if (LOWORD(dVolume) > volume)
		{
			volume = LOWORD(dVolume);
		}

		float fVolume = (float) (65535 / 1000);

		volume = (int) ((float) volume / fVolume);
		if (volume > 1000)
			volume = 1000;
		if (volume < 0)
			volume = 0;
	}
	else
	{
		// Vista or higher
		float currentVolume = 0;
		m_pEndpointVolume->GetMasterVolumeLevelScalar(&currentVolume);

		volume = (int) (currentVolume * 1000);
	}
	return volume;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CVolumeManager::GetMuted()
///
/// \brief	Gets the muted. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CVolumeManager::GetMuted()
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	BOOL mute;

	if ((m_pEndpointVolume == NULL) || (!option->GetPropertyBool("sally", "volumeWindows", true)))
	{
		// mute or not?
		MIXERCONTROLDETAILS_BOOLEAN mxcdMute;
		MIXERCONTROLDETAILS mxcd;
		mxcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
		mxcd.dwControlID = m_dwMuteControlID;
		mxcd.cChannels = 1;
		mxcd.cMultipleItems = 0;
		mxcd.cbDetails = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
		mxcd.paDetails = &mxcdMute;
		if (::mixerGetControlDetails(reinterpret_cast<HMIXEROBJ>(m_hMixer),
			&mxcd,
			MIXER_OBJECTF_HMIXER |
			MIXER_GETCONTROLDETAILSF_VALUE)
			!= MMSYSERR_NOERROR)
		{
			return false;
		}
		mute = (BOOL) (mxcdMute.fValue);
	}
	else
	{
		m_pEndpointVolume->GetMute(&mute);
	}
	if (mute == FALSE)
		return false;
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CVolumeManager::SetMuted(bool muted)
///
/// \brief	Sets a muted. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	muted	true to muted. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CVolumeManager::SetMuted(bool muted)
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	if ((m_pEndpointVolume == NULL) || (!option->GetPropertyBool("sally", "volumeWindows", true)))
	{
		// Win XP
		MIXERCONTROLDETAILS_BOOLEAN mxcdMute = { muted };
		MIXERCONTROLDETAILS mxcd;
		mxcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
		mxcd.dwControlID = m_dwMuteControlID;
		mxcd.cChannels = 1;
		mxcd.cMultipleItems = 0;
		mxcd.cbDetails = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
		mxcd.paDetails = &mxcdMute;
		if (::mixerSetControlDetails(reinterpret_cast<HMIXEROBJ>(m_hMixer),
			&mxcd, MIXER_OBJECTF_HMIXER | MIXER_SETCONTROLDETAILSF_VALUE))
		{
			return;
		}
	}
	else
	{
		// Vista or higher
		m_pEndpointVolume->SetMute(muted, NULL);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CVolumeManager::SetVolume(int volume)
///
/// \brief	Sets a volume. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	volume	The volume. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CVolumeManager::SetVolume(int volume)
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	if (volume < 0)
		volume = 0;
	if (volume > 1000)
		volume = 1000;

	if ((m_pEndpointVolume == NULL) || (!option->GetPropertyBool("sally", "volumeWindows", true)))
	{
		// Win XP
		int volLeft;
		int volRight;
		int vol;

		volLeft = volRight = (int) ((float) (65535 / 1000) * ((float) volume));

		vol = ((int)volLeft & 0x0000ffff) | ((int)volRight << 16);
		waveOutSetVolume(0, vol);
	}
	else
	{
		// Vista or higher
		float volumeFloat = ((float) volume) / 1000;

		m_pEndpointVolume->SetMasterVolumeLevelScalar(volumeFloat, NULL);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CVolumeManager::IsWindowsVistaVolumeManagerAvailable()
///
/// \brief	Queries if the windows vista volume manager is available. 
///
/// \author	Christian Knobloch
/// \date	06.01.2011
///
/// \return	true if the windows vista volume manager is available, false if not. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CVolumeManager::IsWindowsVistaVolumeManagerAvailable()
{
	if (m_pEndpointVolume == NULL)
		return false;
	return true;
}