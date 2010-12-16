////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\ShutdownPopUp.cpp
///
/// \brief	Implements the shutdown pop up class. 
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

#include "ShutdownPopUp.h"

CShutdownPopUp::CShutdownPopUp(SallyAPI::GUI::CGUIBaseObject* parent)
	: SallyAPI::GUI::CPopUpWindow(parent, 0, "")
{
	m_pExit = new SallyAPI::GUI::CButton(this, (WINDOW_WIDTH - 200) / 2, (WINDOW_HEIGHT / 2) - 80, 200, 40, GUIM_EXIT);
	m_pExit->SetImageId(GUI_THEME_SALLY_ICON_SYSTEM_EXIT);
	m_pExit->SetText("Exit Sally");
	this->AddChild(m_pExit);

	m_pHibernate = new SallyAPI::GUI::CButton(this, (WINDOW_WIDTH - 200) / 2, (WINDOW_HEIGHT / 2) - 20, 200, 40, GUIM_HIBERNATE);
	m_pHibernate->SetImageId(GUI_THEME_SALLY_ICON_SYSTEM_BATTERY);
	m_pHibernate->SetText("Hibernate");
	this->AddChild(m_pHibernate);

	m_pShutdown = new SallyAPI::GUI::CButton(this, (WINDOW_WIDTH - 200) / 2, (WINDOW_HEIGHT / 2) + 40, 200, 40, GUIM_SHUTDOWN);
	m_pShutdown->SetImageId(GUI_THEME_SALLY_ICON_SYSTEM_SHUTDOWN);
	m_pShutdown->SetText("Shutdown PC");
	this->AddChild(m_pShutdown);
}

CShutdownPopUp::~CShutdownPopUp()
{
}

void CShutdownPopUp::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_BUTTON_CLICKED:
		switch (reporterId)
		{
		case GUIM_EXIT:
			m_pParent->SendMessageToParent(m_pParent, 0, GUIM_EXIT, messageParameter);
			return;
		case GUIM_SHUTDOWN:
			m_pParent->SendMessageToParent(m_pParent, 0, GUIM_SHUTDOWN, messageParameter);
			return;
		case GUIM_HIBERNATE:
			m_pParent->SendMessageToParent(this, 0, MS_SALLY_HIDE_POPUP_VIEW);
			m_pParent->SendMessageToParent(m_pParent, 0, GUIM_HIBERNATE, messageParameter);
			return;
		}
		break;
	}
	SallyAPI::GUI::CPopUpWindow::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}