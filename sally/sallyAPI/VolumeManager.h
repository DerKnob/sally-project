////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\VolumeManager.h
///
/// \brief	Declares the volume manager class. 
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

#pragma once
#include "define.h"
#include <ddraw.h>
#include <strmif.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include "Config.h"
#include "VolumeControl.h"
#include "VolumeNotification.h"
#include "ParameterOnScreenMenu.h"

namespace SallyAPI
{
	namespace Sound
	{
		namespace Volume
		{
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \class	CVolumeManager
			///
			/// \brief	Manager for volumes. 
			///
			/// \author	Christian Knobloch
			/// \date	19.04.2010
			////////////////////////////////////////////////////////////////////////////////////////////////////

			class DLL_API_SALLY CVolumeManager
			{
			private:
				static SallyAPI::Sound::Volume::CVolumeManager*	m_pObject;
				
				std::vector<SallyAPI::GUI::CControl*>		m_vListeners;

				// XP
				HMIXER						m_hMixer;
				MIXERCAPS					m_mxcaps;
				DWORD						m_dwMuteControlID;

				// Vista +
				SallyAPI::Sound::Volume::CVolumeNotification*	m_pVolumeNotification;
				IAudioEndpointVolume*							m_pEndpointVolume;

				CVolumeManager();
				~CVolumeManager();
			public:
				static SallyAPI::Sound::Volume::CVolumeManager*	GetInstance();
				static void	DeleteInstance();

				int		GetVolume();
				bool	IsMuted();
				
				void	SetMuted(bool muted);
				void	SetVolume(int volume);

				void	RegisterListener(SallyAPI::GUI::CControl* control);
				void	UnregisterListener(SallyAPI::GUI::CControl* control);

				void	OnVolumeChange();

				bool	IsWindowsVistaVolumeManagerAvailable();
			};
		}
	}
}