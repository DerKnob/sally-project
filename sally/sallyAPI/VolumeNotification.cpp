////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\VolumeNotification.cpp
///
/// \brief	Implements the volume notification class. 
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

#include "VolumeNotification.h"
#include "VolumeManager.h"

using namespace SallyAPI::Sound::Volume;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CVolumeNotification::CVolumeNotification()
///
/// \brief	Default constructor. 
///
/// \author	Christian Knobloch
/// \date	05.05.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CVolumeNotification::CVolumeNotification()
	:m_RefCount(1)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CVolumeNotification::~CVolumeNotification()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	05.05.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CVolumeNotification::~CVolumeNotification()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	STDMETHODIMP_(ULONG) CVolumeNotification::AddRef()
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	05.05.2010
///
/// \param		The. 
////////////////////////////////////////////////////////////////////////////////////////////////////

STDMETHODIMP_(ULONG) CVolumeNotification::AddRef()
{
	return InterlockedIncrement(&m_RefCount);
} 

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	STDMETHODIMP_(ULONG) CVolumeNotification::Release()
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	05.05.2010
///
/// \param		The. 
////////////////////////////////////////////////////////////////////////////////////////////////////

STDMETHODIMP_(ULONG) CVolumeNotification::Release()  
{
	LONG ref = InterlockedDecrement(&m_RefCount);  
	if (ref == 0) 
		delete this; 
	return ref; 
} 

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	STDMETHODIMP CVolumeNotification::QueryInterface(REFIID IID, void **ReturnValue)
///
/// \brief	Queries an interface. 
///
/// \author	Christian Knobloch
/// \date	05.05.2010
///
/// \param	IID					The iid. 
/// \param [in,out]	ReturnValue	If non-null, the return value. 
///
/// \return	The interface. 
////////////////////////////////////////////////////////////////////////////////////////////////////

STDMETHODIMP CVolumeNotification::QueryInterface(REFIID IID, void **ReturnValue) 
{ 
	if (IID == IID_IUnknown || IID== __uuidof(IAudioEndpointVolumeCallback))  
	{ 
		*ReturnValue = static_cast<IUnknown*>(this); 
		AddRef(); 
		return S_OK; 
	} 
	*ReturnValue = NULL; 
	return E_NOINTERFACE; 
} 

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	STDMETHODIMP CVolumeNotification::OnNotify(PAUDIO_VOLUME_NOTIFICATION_DATA NotificationData)
///
/// \brief	Executes the notify action. 
///
/// \author	Christian Knobloch
/// \date	05.05.2010
///
/// \param	NotificationData	Information describing the notification. 
///
/// \return	. 
////////////////////////////////////////////////////////////////////////////////////////////////////

STDMETHODIMP CVolumeNotification::OnNotify(PAUDIO_VOLUME_NOTIFICATION_DATA NotificationData) 
{ 
	SallyAPI::Sound::Volume::CVolumeManager* volumeManager = SallyAPI::Sound::Volume::CVolumeManager::GetInstance();
	volumeManager->OnVolumeChange();
	return S_OK; 
} 