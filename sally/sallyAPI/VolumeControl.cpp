////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\VolumeControl.cpp
///
/// \brief	Implements the volume control class. 
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

#include "VolumeControl.h"
#include "VolumeManager.h"

using namespace SallyAPI::GUI;

#define GUI_MUTED	50001

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CVolumeControl::CVolumeControl(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y,
/// int width, int controlId) :SallyAPI::GUI::CForm(parent, x, y, width, CONTROL_HEIGHT,
/// controlId)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	parent	If non-null, the parent. 
/// \param	x				The x coordinate. 
/// \param	y				The y coordinate. 
/// \param	width			The width. 
/// \param	controlId		Identifier for the control. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CVolumeControl::CVolumeControl(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int controlId)
	:SallyAPI::GUI::CForm(parent, x, y, width, CONTROL_HEIGHT, controlId)
{
	m_pVolumeSlider = new SallyAPI::GUI::CSlider(this, CONTROL_HEIGHT + 20, 0, m_iWidth - CONTROL_HEIGHT - 30);
	m_pVolumeSlider->SetMaxPosition(1000);
	this->AddChild(m_pVolumeSlider);

	m_pVolumeMute = new SallyAPI::GUI::CButton(this, 0, 0, CONTROL_HEIGHT, CONTROL_HEIGHT, GUI_MUTED);
	m_pVolumeMute->SetImageId(GUI_THEME_SALLY_AUDIO_MUTED);
	this->AddChild(m_pVolumeMute);

	SallyAPI::Sound::Volume::CVolumeManager* volumeManger = SallyAPI::Sound::Volume::CVolumeManager::GetInstance();
	volumeManger->RegisterListener(this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CVolumeControl::~CVolumeControl()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CVolumeControl::~CVolumeControl()
{
	SallyAPI::Sound::Volume::CVolumeManager* volumeManger = SallyAPI::Sound::Volume::CVolumeManager::GetInstance();
	volumeManger->UnregisterListener(this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CVolumeControl::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter,
/// int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
///
/// \brief	Send message to parent. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	reporter			If non-null, the reporter. 
/// \param	reporterId					Identifier for the reporter. 
/// \param	messageId					Identifier for the message. 
/// \param [in,out]	messageParameter	If non-null, the message parameter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CVolumeControl::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_BUTTON_CLICKED:
		switch (reporterId)
		{
		case GUI_MUTED:
			OnCommandMuted();
			return;
		}
		break;
	case GUI_SLIDER_MOVED:
	case GUI_SLIDER_CLICKED:
		OnCommandChangeVolume(messageParameter);
		return;
	}
	SallyAPI::GUI::CForm::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CVolumeControl::UpdateView()
///
/// \brief	Updates this object. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CVolumeControl::UpdateView()
{
	SallyAPI::Sound::Volume::CVolumeManager* volumeManger = SallyAPI::Sound::Volume::CVolumeManager::GetInstance();

	m_pVolumeSlider->SetPosition(volumeManger->GetVolume());
	m_pVolumeMute->SetCheckStatus(volumeManger->GetMuted());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CVolumeControl::OnCommandMuted()
///
/// \brief	Executes the command muted action. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CVolumeControl::OnCommandMuted()
{
	SallyAPI::Sound::Volume::CVolumeManager* volumeManger = SallyAPI::Sound::Volume::CVolumeManager::GetInstance();

	bool muted = false;

	if (m_pVolumeMute->GetCheckStatus())
	{
		m_pVolumeMute->SetCheckStatus(false);
	}
	else
	{
		muted = true;
		m_pVolumeMute->SetCheckStatus(true);
	}

	volumeManger->SetMuted(muted);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CVolumeControl::OnCommandChangeVolume(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
///
/// \brief	Executes the command change volume action. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	messageParameter	If non-null, the message parameter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CVolumeControl::OnCommandChangeVolume(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	SallyAPI::GUI::SendMessage::CParameterInteger* parameterInteger = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterInteger*> (messageParameter);
	if (parameterInteger == NULL)
		return;

	SallyAPI::Sound::Volume::CVolumeManager* volumeManger = SallyAPI::Sound::Volume::CVolumeManager::GetInstance();

	m_pVolumeSlider->SetPosition(parameterInteger->GetInteger());

	volumeManger->SetVolume(parameterInteger->GetInteger());
}