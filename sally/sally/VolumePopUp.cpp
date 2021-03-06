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
#define HIDE_TIMER	8000

CVolumePopUp::CVolumePopUp(SallyAPI::GUI::CGUIBaseObject* parent)
	:SallyAPI::GUI::CContextMenuPopUp(parent, 0, "", 644, 107)
{
	m_pButtonClose->Visible(false);

	m_pVolumeControl = new SallyAPI::GUI::CVolumeControl(m_pContextMenu, 20, 20,
		644 - 40);
	m_pContextMenu->AddChild(m_pVolumeControl);

	m_pVolumeLow = new SallyAPI::GUI::CImageBox(m_pContextMenu, 30 + 40, 65, 22, 22);
	m_pVolumeLow->SetImageId(GUI_THEME_SALLY_AUDIO_LOW);
	m_pContextMenu->AddChild(m_pVolumeLow);

	m_pVolumeMedium = new SallyAPI::GUI::CImageBox(m_pContextMenu, 291 + 40, 65, 22, 22);
	m_pVolumeMedium->SetImageId(GUI_THEME_SALLY_AUDIO_MEDIUM);
	m_pContextMenu->AddChild(m_pVolumeMedium);

	m_pVolumeHigh = new SallyAPI::GUI::CImageBox(m_pContextMenu, 552 + 40, 65, 22, 22);
	m_pVolumeHigh->SetImageId(GUI_THEME_SALLY_AUDIO_HIGH);
	m_pContextMenu->AddChild(m_pVolumeHigh);

	m_pHideTimer = new SallyAPI::GUI::CTimer(15, this, 0, HIDE_TIMER);
}

CVolumePopUp::~CVolumePopUp()
{
	m_pHideTimer->WaitForStop();
	SafeDelete(m_pHideTimer);
}

void CVolumePopUp::UpdateView()
{
	m_pVolumeControl->UpdateView();
	m_pHideTimer->Reset();
	m_pHideTimer->Start();
}

void CVolumePopUp::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case HIDE_TIMER:
		m_pHideTimer->Stop();
		m_pParent->SendMessageToParent(this, 0, MS_SALLY_HIDE_VOLUME);
		return;
	case GUI_BUTTON_CLICKED:
		switch (reporterId)
		{
		case GUI_THEME_SALLY_CLOSE:
			m_pHideTimer->Stop();
			m_pParent->SendMessageToParent(this, 0, MS_SALLY_HIDE_VOLUME);
			return;
		}
		break;
	}
	SallyAPI::GUI::CPopUpWindow::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

bool CVolumePopUp::ProcessMouseDown(int x, int y)
{
	m_pHideTimer->Stop();
	return SallyAPI::GUI::CPopUpWindow::ProcessMouseDown(x, y);
}

bool CVolumePopUp::ProcessMouseUp(int x, int y)
{
	m_pHideTimer->Reset();
	m_pHideTimer->Start();
	return SallyAPI::GUI::CPopUpWindow::ProcessMouseUp(x, y);
}