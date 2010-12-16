////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\ScreenLock.cpp
///
/// \brief	Implements the screen lock class. 
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

#include "ScreenLock.h"

#define HIDE_TIMER		8000

CScreenLock::CScreenLock(SallyAPI::GUI::CGUIBaseObject* parent)
	: SallyAPI::GUI::CPopUpWindow(parent, 0, "")
{
	m_pButtonClose->Visible(false);
	m_pBackground->Visible(false);

	m_pInput = new SallyAPI::GUI::CEdit(this, 0, 0, 0);
	m_pInput->SetNumberOnly(true);
	m_pInput->SetPassword(true);
	m_pInput->SetMaxCharacterInputLength(6);

	m_pHideTimer = new SallyAPI::GUI::CTimer(30, this, 0, HIDE_TIMER);
}

CScreenLock::~CScreenLock()
{
}

bool CScreenLock::ProcessMouseUp(int x, int y)
{
	m_pParent->SendMessageToParent(m_pInput, 0, MS_SALLY_SHOW_KEYBOARD);

	m_pHideTimer->Reset();
	m_pHideTimer->Start();
	return true;
}

void CScreenLock::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	switch (messageId)
	{
	case HIDE_TIMER:
		m_pParent->SendMessageToParent(this, m_iControlId, MS_SALLY_HIDE_KEYBOARD_CANCEL);
		m_pHideTimer->Stop();
		return;
	case GUI_EDIT_CHANGED:
		if (m_pInput->GetText().compare(option->GetPropertyString("lock", "password", "")) == 0)
		{
			m_pParent->SendMessageToParent(m_pInput, 0, MS_SALLY_UNLOCK_SCREEN);
		}
		else
		{
			SallyAPI::GUI::SendMessage::CParameterMessageBox sendMessageParameterMessageBox(0, this,
				"The PIN was wrong!\nPlease Try again.", SallyAPI::GUI::MESSAGEBOX_ICON_ERROR);
			m_pParent->SendMessageToParent(this, m_iControlId, MS_SALLY_SHOW_MESSAGEBOX, &sendMessageParameterMessageBox);
		}
		return;;
	}
	SallyAPI::GUI::CPopUpWindow::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}