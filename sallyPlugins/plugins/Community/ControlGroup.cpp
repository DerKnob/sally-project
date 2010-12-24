////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	Community\ControlGroup.cpp
///
/// \brief	Implements the control group class. 
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

#include "ControlGroup.h"

CControlGroup::CControlGroup(SallyAPI::GUI::CGUIBaseObject *parent, int x, int y, int width)
	:SallyAPI::GUI::CGroupBox(parent, x, y, width, CONTROL_GROUP_HEIGHT), m_pWindow(NULL)
{
	m_pAvatar = new SallyAPI::GUI::CImageBox(this, 10, 10, 100, 100);
	this->AddChild(m_pAvatar);

	m_pName = new SallyAPI::GUI::CLabel(this, 120, 10, width - 130);
	m_pName->SetLocalised(false);
	m_pName->SetBold(true);
	this->AddChild(m_pName);

	m_pMessage = new SallyAPI::GUI::CLabelBox(this, 120, CONTROL_HEIGHT, width - 130, 40);
	m_pMessage->SetLocalised(false);
	this->AddChild(m_pMessage);

	m_pActionButton = new SallyAPI::GUI::CButton(this, 120, CONTROL_HEIGHT + 50, width - 130, 30);
	this->AddChild(m_pActionButton);
}

CControlGroup::~CControlGroup()
{
}

void CControlGroup::SetPicture(SallyAPI::GUI::CPicture* avatar)
{
	m_pAvatar->SetImageId(GUI_NO_IMAGE);
	m_pAvatar->SetPicture(avatar);

	int x = 0;
	int y = 0;
	int width = 0;
	int height = 0;

	SallyAPI::GUI::GUIHelper::CalculateImageSize(avatar->GetWidth(), avatar->GetHeight(),
		100, 100, x, y, width, height);

	m_pAvatar->Move(10 + x, 10 + y);
	m_pAvatar->Resize(width, height);
}

void CControlGroup::SetImageId(int avatar)
{
	m_pAvatar->Move(10, 10);
	m_pAvatar->Resize(100, 100);
	m_pAvatar->SetPicture(NULL);
	m_pAvatar->SetImageId(avatar);
}

void CControlGroup::SetValue(const std::string& name, const std::string& message, const std::string& action,
							 const std::string& actionName, SallyAPI::GUI::CApplicationWindow* window)
{
	m_pName->SetText(name);
	m_pMessage->SetText(message);
	m_strAction = action;
	m_pWindow = window;

	if (m_pWindow == NULL)
	{
		m_pActionButton->SetImageId(GUI_NO_IMAGE);
		m_pActionButton->Visible(false);
	}
	else
	{
		m_pActionButton->SetImageId(m_pWindow->GetGraphicId());
		m_pActionButton->Visible(true);
		m_pActionButton->SetText(actionName);
	}
}

void CControlGroup::ResetImage()
{
	m_pAvatar->SetPicture(NULL);
	m_pAvatar->SetImageId(GUI_NO_IMAGE);
}

void CControlGroup::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_BUTTON_CLICKED:
		OnCommandSendNotify();
		return;
	}
	SallyAPI::GUI::CGroupBox::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

void CControlGroup::OnCommandSendNotify()
{
	SallyAPI::GUI::SendMessage::CParameterString messageString(m_strAction);
	m_pWindow->SendMessageToParent(this, 0, MS_SALLY_FACEBOOK_NOTIFY, &messageString);

	SallyAPI::GUI::SendMessage::CParameterInteger parameterInteger(m_pWindow->GetGraphicId());
	m_pWindow->SendMessageToParent(this, 0, MS_SALLY_CHANGE_APP, &parameterInteger);
}