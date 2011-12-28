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

CScreensaverOverlay::CScreensaverOverlay(SallyAPI::GUI::CGUIBaseObject* parent)
	:SallyAPI::GUI::CApplicationWindow(parent, 0, "")
{
	m_pTopMenu = new SallyAPI::GUI::CTopMenu(this);
	this->AddChild(m_pTopMenu);

	m_pVolumeControl = new SallyAPI::GUI::CVolumeControl(m_pTopMenu, 10, 15 + 25, WINDOW_WIDTH - 200 - 20);
	m_pTopMenu->AddChild(m_pVolumeControl);

	m_pButtonCloseFullscreen = new SallyAPI::GUI::CButton(m_pTopMenu, WINDOW_WIDTH - 200, 10 + 25, 190, 40, GUI_CLOSE_SCREENSAVER);
	m_pButtonCloseFullscreen->SetImageId(GUI_THEME_SALLY_ICON_FULLSCREEN);
	m_pButtonCloseFullscreen->SetText("exit Fullscreen");
	m_pTopMenu->AddChild(m_pButtonCloseFullscreen);
}

CScreensaverOverlay::~CScreensaverOverlay()
{
}

void CScreensaverOverlay::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, 
											  SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case MS_SALLY_SCREENSAVER_SHOW_MENU:
		OnCommandShowMenu(reporter);
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

	m_pTopMenu->Move(0, -(MENU_HEIGHT + 25));
}

void CScreensaverOverlay::OnCommandShowMenu(SallyAPI::GUI::CGUIBaseObject* reporter)
{
	m_pVolumeControl->UpdateView();
	m_pTopMenu->MoveAnimated(m_pTopMenu->GetPositionX(), -25, 400, false);

	m_pCurrentControls = NULL;
	int added = 0;

	std::vector<SallyAPI::GUI::CScreensaverControls*>::iterator iter = m_pScreensaverControlsList.begin();
	while (iter != m_pScreensaverControlsList.end())
	{
		SallyAPI::GUI::CScreensaverControls* controls = (*iter);
		if (reporter == controls->GetApplicationWindow())
		{
			added++;
			m_pCurrentControls = controls;
			m_pCurrentControls->Move(0, 80 * added + 50);
			m_pCurrentControls->SetAlphaBlending(0);
			m_pCurrentControls->BlendAnimated(255, 800, false);
			m_pCurrentControls->Visible(true);
			m_pCurrentControls->Enable(true);
			break;
		}
		iter++;
	}

	iter = m_pScreensaverControlsList.begin();
	while (iter != m_pScreensaverControlsList.end())
	{
		SallyAPI::GUI::CScreensaverControls* controls = (*iter);
		if ((controls->IsShowAlways()) && (controls != m_pCurrentControls))
		{
			added++;
			controls->Move(0, 80 * added + 50);
			controls->SetAlphaBlending(0);
			controls->BlendAnimated(255, 800, false);
			controls->Visible(true);
			controls->Enable(true);
		}
		iter++;
	}
}

void CScreensaverOverlay::OnCommandHideMenu()
{
	m_pTopMenu->MoveAnimated(m_pTopMenu->GetPositionX(), -(MENU_HEIGHT + 25), 400, false);

	std::vector<SallyAPI::GUI::CScreensaverControls*>::iterator iter = m_pScreensaverControlsList.begin();
	while (iter != m_pScreensaverControlsList.end())
	{
		SallyAPI::GUI::CScreensaverControls* controls = (*iter);
		controls->BlendAnimated(0, 800, false);
		iter++;
	}
}

void CScreensaverOverlay::AddScreensaverControl(SallyAPI::GUI::CGUIBaseObject* control)
{
	SallyAPI::GUI::CScreensaverControls* screensaverControls = dynamic_cast<SallyAPI::GUI::CScreensaverControls*> (control);

	if (screensaverControls == NULL)
		return;

	this->AddChild(screensaverControls);
	m_pScreensaverControlsList.push_back(screensaverControls);

	screensaverControls->Visible(false);
	screensaverControls->Enable(false);
}