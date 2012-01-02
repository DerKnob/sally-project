////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\ScreensaverOverlay.cpp
///
/// \brief	Implements the screensaver overlay class. 
///
/// \author	Christian Knobloch
/// \date	27.12.2011
///
/// This file is part of the Sally Project
/// 
/// Copyright(c) 2008-2011 Sally Project
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

#include "ScreensaverOverlay.h"

#define GUI_CLOSE_SCREENSAVER			50001
#define GUI_SHOW_NEXT_CONTROL			50002

CScreensaverOverlay::CScreensaverOverlay(SallyAPI::GUI::CGUIBaseObject* parent)
	:SallyAPI::GUI::CApplicationWindow(parent, 0, ""), m_bOverlayVisible(false)
{
	m_pTopMenu = new SallyAPI::GUI::CTopMenu(this);
	this->AddChild(m_pTopMenu);

	m_pVolumeControl = new SallyAPI::GUI::CVolumeControl(m_pTopMenu, 10, 15, WINDOW_WIDTH - 200 - 20);
	m_pTopMenu->AddChild(m_pVolumeControl);

	m_pButtonCloseFullscreen = new SallyAPI::GUI::CButton(m_pTopMenu, WINDOW_WIDTH - 200, 10, 190, 40, GUI_CLOSE_SCREENSAVER);
	m_pButtonCloseFullscreen->SetImageId(GUI_THEME_SALLY_ICON_FULLSCREEN);
	m_pButtonCloseFullscreen->SetText("exit Fullscreen");
	m_pTopMenu->AddChild(m_pButtonCloseFullscreen);

	m_pThreadStarter = new SallyAPI::GUI::CThreadStarter(this, 0, GUI_SHOW_NEXT_CONTROL);
}

CScreensaverOverlay::~CScreensaverOverlay()
{
	m_pThreadStarter->WaitForStop();

	SafeDelete(m_pThreadStarter);
}

void CScreensaverOverlay::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, 
											  SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_SHOW_NEXT_CONTROL:
		OnCommandShowNextControl();
		return;
	case MS_SALLY_SCREENSAVER_SHOW_MENU:
		OnCommandShowMenu(reporter, messageParameter);
		return;
	case MS_SALLY_SCREENSAVER_HIDE_MENU:
		OnCommandHideMenu();
		return;
	case GUI_BUTTON_CLICKED:
		switch (reporterId)
		{
		case GUI_CLOSE_SCREENSAVER:
			m_pParent->SendMessageToParent(m_pParent, m_iControlId, MS_SALLY_SCREENSAVER_STOP);
			return;
		}
		break;
	}
	SallyAPI::GUI::CForm::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

void CScreensaverOverlay::Visible(bool visible)
{
	SallyAPI::GUI::CAppBase::Visible(visible);

	m_pTopMenu->Move(0, -MENU_HEIGHT);
}

void CScreensaverOverlay::OnCommandShowNextControl()
{
	do 
	{
		SallyAPI::GUI::CScreensaverControl* control = m_pScreensaverControlListCurrent[m_iThreadStarter];
		{
			// create locking
			SallyAPI::System::CAutoLock lock(&m_Lock);

			// cancel if it is not longer visible
			if (!m_bOverlayVisible)
				return;

			++m_iThreadStarter;

			control->Move(0, (80 * m_iThreadStarter) + 50);
			control->SetAlphaBlending(0);
			control->BlendAnimated(255, 1000, false);
			control->Visible(true);
			control->Enable(true);
		}
		Sleep(200);
	} while ((m_iThreadStarter < m_pScreensaverControlListCurrent.size()) && (m_bOverlayVisible));
}

void CScreensaverOverlay::OnCommandShowMenu(SallyAPI::GUI::CGUIBaseObject* reporter, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	// create lock
	SallyAPI::System::CAutoLock lock(&m_Lock);
	m_bOverlayVisible = true;

	m_pVolumeControl->UpdateView();
	m_pTopMenu->MoveAnimated(m_pTopMenu->GetPositionX(), 0, 400, false);


	m_pScreensaverControlListCurrent.clear();
	m_iThreadStarter = 0;
	SallyAPI::GUI::CScreensaverControl* currentControl = NULL;

	int maxWidth = 0;

	std::vector<SallyAPI::GUI::CScreensaverControl*>::iterator iter = m_pScreensaverControlList.begin();
	while (iter != m_pScreensaverControlList.end())
	{
		SallyAPI::GUI::CScreensaverControl* control = (*iter);
		if (reporter == control->GetApplicationWindow())
		{
			m_pScreensaverControlListCurrent.push_back(control);
			currentControl = control;

			if (control->GetWidth() > maxWidth)
				maxWidth = control->GetWidth();
			break;
		}
		iter++;
	}

	iter = m_pScreensaverControlList.begin();
	while (iter != m_pScreensaverControlList.end())
	{
		SallyAPI::GUI::CScreensaverControl* control = (*iter);
		if ((control->IsShowAlways()) && (control != currentControl))
		{
			m_pScreensaverControlListCurrent.push_back(control);

			if (control->GetWidth() > maxWidth)
				maxWidth = control->GetWidth();
		}
		iter++;
	}

	m_pThreadStarter->Start();

	// return the rect used if possible
	if (messageParameter == NULL)
		return;

	SallyAPI::GUI::SendMessage::CParameterRect* paramterRect = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterRect*>(messageParameter);
	if (paramterRect == NULL)
		return;

	RECT rect;
	rect.left = 0;
	rect.top = 50 + 80;
	rect.right = maxWidth;
	rect.bottom = 80 * m_pScreensaverControlListCurrent.size();

	paramterRect->SetRect(rect);
}

void CScreensaverOverlay::OnCommandHideMenu()
{
	m_pThreadStarter->Stop();

	SallyAPI::System::CAutoLock lock(&m_Lock);
	m_bOverlayVisible = false;

	m_pTopMenu->MoveAnimated(m_pTopMenu->GetPositionX(), -MENU_HEIGHT, 400, false);

	std::vector<SallyAPI::GUI::CScreensaverControl*>::iterator iter = m_pScreensaverControlListCurrent.begin();
	while (iter != m_pScreensaverControlListCurrent.end())
	{
		SallyAPI::GUI::CScreensaverControl* control = (*iter);
		control->BlendAnimated(0, 1000, false);
		iter++;
	}
}

void CScreensaverOverlay::AddScreensaverControl(SallyAPI::GUI::CGUIBaseObject* control)
{
	SallyAPI::GUI::CScreensaverControl* screensaverControl = dynamic_cast<SallyAPI::GUI::CScreensaverControl*> (control);

	if (screensaverControl == NULL)
		return;

	SallyAPI::System::CAutoLock lock(&m_Lock);

	this->AddChild(screensaverControl);
	m_pScreensaverControlList.push_back(screensaverControl);

	screensaverControl->Visible(false);
	screensaverControl->Enable(false);
}