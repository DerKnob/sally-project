////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\VolumeNotification.h
///
/// \brief	Declares the volume notification class. 
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
#include <endpointvolume.h>

namespace SallyAPI
{
	namespace Sound
	{
		namespace Volume
		{
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \class	CVolumeNotification
			///
			/// \brief	Volume notification. 
			///
			/// \author	Christian Knobloch
			/// \date	05.05.2010
			////////////////////////////////////////////////////////////////////////////////////////////////////

			class DLL_API_SALLY CVolumeNotification
				: public IAudioEndpointVolumeCallback 
			{
			private:
				LONG	m_RefCount; 
			public: 
				CVolumeNotification();
				~CVolumeNotification();

				STDMETHODIMP_(ULONG)	AddRef();
				STDMETHODIMP_(ULONG)	Release();
				STDMETHODIMP QueryInterface(REFIID IID, void **ReturnValue);
				STDMETHODIMP OnNotify(PAUDIO_VOLUME_NOTIFICATION_DATA NotificationData);
			};
		}
	}
}