////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\VolumePopUp.cpp
///
/// \brief	Implements the volume pop up class. 
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

#include "VolumePopUp.h"

#define GUI_MUTED	50001

CVolumePopUp::CVolumePopUp(SallyAPI::GUI::CGUIBaseObject* parent)
	:SallyAPI::GUI::CPopUpWindow(parent, 0, "")
{
	m_pBackground = new SallyAPI::GUI::CGroupBox(this, (WINDOW_WIDTH - 644) / 2, (WINDOW_HEIGHT - 200), 644, 102);
	this->AddChild(m_pBackground);

	m_pVolumeControl = new SallyAPI::GUI::CVolumeControl(this, (WINDOW_WIDTH - 644) / 2 + 20, (WINDOW_HEIGHT - 200) + 20,
		644 - 40);
	this->AddChild(m_pVolumeControl);

// 	m_pVolumeSlider = new SallyAPI::GUI::CSlider(this, (WINDOW_WIDTH - 644) / 2 + 30 + 40, (WINDOW_HEIGHT - 200) + 20, 544);
// 	m_pVolumeSlider->SetMaxPosition(1000);
// 	this->AddChild(m_pVolumeSlider);
// 
// 	m_pVolumeMute = new SallyAPI::GUI::CButton(this, (WINDOW_WIDTH - 644) / 2 + 20, (WINDOW_HEIGHT - 200) + 20, CONTROL_HEIGHT, CONTROL_HEIGHT, GUI_MUTED);
// 	m_pVolumeMute->SetImageId(GUI_THEME_SALLY_AUDIO_MUTED);
// 	this->AddChild(m_pVolumeMute);

	m_pVolumeLow = new SallyAPI::GUI::CImageBox(this, (WINDOW_WIDTH - 644) / 2 + 30 + 40, (WINDOW_HEIGHT - 200) + 65, 22, 22);
	m_pVolumeLow->SetImageId(GUI_THEME_SALLY_AUDIO_LOW);
	this->AddChild(m_pVolumeLow);

	m_pVolumeMedium = new SallyAPI::GUI::CImageBox(this, (WINDOW_WIDTH - 644) / 2 + 291 + 40, (WINDOW_HEIGHT - 200) + 65, 22, 22);
	m_pVolumeMedium->SetImageId(GUI_THEME_SALLY_AUDIO_MEDIUM);
	this->AddChild(m_pVolumeMedium);

	m_pVolumeHigh = new SallyAPI::GUI::CImageBox(this, (WINDOW_WIDTH - 644) / 2 + 552 + 40, (WINDOW_HEIGHT - 200) + 65, 22, 22);
	m_pVolumeHigh->SetImageId(GUI_THEME_SALLY_AUDIO_HIGH);
	this->AddChild(m_pVolumeHigh);
}

CVolumePopUp::~CVolumePopUp()
{
}

void CVolumePopUp::UpdateView()
{
	m_pVolumeControl->UpdateView();
}

void CVolumePopUp::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_BUTTON_CLICKED:
		switch (reporterId)
		{
		case GUI_THEME_SALLY_CLOSE:
			m_pParent->SendMessageToParent(this, 0, MS_SALLY_HIDE_VOLUME);
			return;
		}
		break;
	}
	SallyAPI::GUI::CPopUpWindow::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}