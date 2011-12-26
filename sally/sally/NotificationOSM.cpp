////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\NotificationOSM.cpp
///
/// \brief	Implements the on screen menu class. 
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

#include "NotificationOSM.h"

CNotificationOSM::CNotificationOSM(SallyAPI::GUI::CGUIBaseObject* parent)
	: SallyAPI::GUI::CPopUpWindow(parent, 0, "")
{
	m_pBackground->SetImageId(GUI_NO_IMAGE);
	m_pButtonClose->Visible(false);

	m_pNotificationBox = new SallyAPI::GUI::CNotificationBox(this, (WINDOW_WIDTH - 120) / 2, (WINDOW_HEIGHT - 120) / 2, 120, 120);
	this->AddChild(m_pNotificationBox);

	m_pIcon = new SallyAPI::GUI::CImageBox(m_pNotificationBox, 28, 20, 64, 64);
	m_pNotificationBox->AddChild(m_pIcon);

	m_pText = new SallyAPI::GUI::CLabel(m_pNotificationBox, 00, 90, 120);
	m_pText->SetFont("notification.small.font");
	m_pText->SetAlign(DT_CENTER | DT_VCENTER);
	m_pNotificationBox->AddChild(m_pText);

	m_pProcessbar = new SallyAPI::GUI::CProcessbar(m_pNotificationBox, 10, 90, 100);
	m_pProcessbar->SetMaxPosition(100);
	m_pNotificationBox->AddChild(m_pProcessbar);

	this->Move(0, 0);
}

CNotificationOSM::~CNotificationOSM()
{
}

void CNotificationOSM::Render()
{
	SallyAPI::GUI::CAppBase::Render();
}

void CNotificationOSM::BlendIn()
{
	this->BlendAnimated(255, 800);
}

void CNotificationOSM::BlendOut()
{
	this->BlendAnimated(0, 800);
}

void CNotificationOSM::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_CONTROL_BLENDED:
		if (m_pBackground->GetAlphaBlending() == 0)
			Visible(false);
		return;
	}
	SallyAPI::GUI::CPopUpWindow::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

void CNotificationOSM::SetImageId(int icon)
{
	m_pIcon->SetImageId(icon);
}

void CNotificationOSM::SetText(const std::string& text)
{
	m_pText->Visible(true);
	m_pProcessbar->Visible(false);

	m_pText->SetText(text);
}

void CNotificationOSM::SetProcessbar(int i)
{
	m_pText->Visible(false);
	m_pProcessbar->Visible(true);

	m_pProcessbar->SetPosition(i);
}