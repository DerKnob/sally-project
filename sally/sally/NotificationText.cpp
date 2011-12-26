////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\NotificationText.cpp
///
/// \brief	Implements the notification text class. 
///
/// \author	Christian Knobloch
/// \date	26.12.2011
///
/// This file is part of the Sally Project
/// 
/// Copyright(c) 2008-2011 Sally Project
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

#include "NotificationText.h"

CNotificationText::CNotificationText(SallyAPI::GUI::CGUIBaseObject* parent)
	: SallyAPI::GUI::CPopUpWindow(parent, 0, "")
{
	m_pBackground->SetImageId(GUI_NO_IMAGE);
	m_pButtonClose->Visible(false);

	m_pNotificationBox = new SallyAPI::GUI::CNotificationBox(this, 0, 0, 500, 0);
	this->AddChild(m_pNotificationBox);

	m_pDescription = new SallyAPI::GUI::CLabelBox(m_pNotificationBox, 10, 10, 500, 0);
	m_pDescription->SetFont("notification.small.font");
	m_pDescription->SetAutoResize(true);
	m_pDescription->SetLocalised(false);
	m_pDescription->SetAlign(DT_CENTER);
	m_pNotificationBox->AddChild(m_pDescription);

	this->Move(0, 0);
}

CNotificationText::~CNotificationText()
{
}

void CNotificationText::Render()
{
	SallyAPI::GUI::CAppBase::Render();
}

void CNotificationText::BlendIn()
{
	this->BlendAnimated(255, 800);
}

void CNotificationText::BlendOut()
{
	this->BlendAnimated(0, 800);
}

void CNotificationText::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
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

void CNotificationText::SendMessageToChilds(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case MS_SALLY_SALLY_THEME_CHANGED:
		SallyAPI::GUI::CPopUpWindow::SendMessageToChilds(reporter, reporterId, messageId, messageParameter);
		ResizeGroupBox();
		return;
	}
	SallyAPI::GUI::CPopUpWindow::SendMessageToChilds(reporter, reporterId, messageId, messageParameter);
}

void CNotificationText::SetText(const std::string& text)
{
	m_pDescription->SetText(text);

	ResizeGroupBox();
}

void CNotificationText::ResizeGroupBox()
{
	// resize groupbox
	int width = 10 + m_pDescription->GetWidth() + 10;
	int height = 10 + m_pDescription->GetHeight() + 10;

	m_pNotificationBox->Resize(width, height);
	m_pNotificationBox->Move((WINDOW_WIDTH - width) / 2, WINDOW_HEIGHT - height - WINDOW_BORDER_H);
}