////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\AlarmPopUp.cpp
///
/// \brief	Implements the alarm pop up class. 
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

#include "AlarmPopUp.h"

#define GUI_ALARM_SLEEP		50100
#define GUI_ALARM_DISABLE	50101

CAlarmPopUp::CAlarmPopUp(SallyAPI::GUI::CGUIBaseObject* parent)
	:SallyAPI::GUI::CPopUpWindow(parent, 0, "")
{
	m_pButtonClose->Visible(false);

	m_pGroupBox = new SallyAPI::GUI::CGroupBox(this, (WINDOW_WIDTH - 420) / 2, (WINDOW_HEIGHT - 170) / 2, 420, 170);
	this->AddChild(m_pGroupBox);

	m_pLabelBox = new SallyAPI::GUI::CLabelBox(m_pGroupBox, 20, 20, 420 - 40, 100);
	m_pLabelBox->SetText("Click 'Disable' to shutdown the alarm or 'Sleep' to be notified 10 min later.");
	m_pLabelBox->SetLocalised(true);
	m_pGroupBox->AddChild(m_pLabelBox);

	m_pButtonSleep = new SallyAPI::GUI::CButton(m_pGroupBox, 20, 170 - CONTROL_HEIGHT - 20, 150,
		CONTROL_HEIGHT, GUI_ALARM_SLEEP);
	m_pButtonSleep->SetImageId(GUI_THEME_SALLY_ICON_TIMER);
	m_pButtonSleep->SetText("Sleep");
	m_pGroupBox->AddChild(m_pButtonSleep);

	m_pButtonDisable = new SallyAPI::GUI::CButton(m_pGroupBox, 420 - 150 - 20, 170 - CONTROL_HEIGHT - 20,
		150, CONTROL_HEIGHT, GUI_ALARM_DISABLE);
	m_pButtonDisable->SetImageId(GUI_THEME_SALLY_ICON_ACCEPT);
	m_pButtonDisable->SetText("Disable");
	m_pButtonDisable->SetDefaultButton(true);
	m_pGroupBox->AddChild(m_pButtonDisable);
}

CAlarmPopUp::~CAlarmPopUp()
{
}

void CAlarmPopUp::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_BUTTON_CLICKED:
		switch (reporterId)
		{
		case GUI_ALARM_SLEEP:
			OnCommandSleep();
			return;
		case GUI_ALARM_DISABLE:
			OnCommandDisable();
			return;
		}
		break;
	}
	SallyAPI::GUI::CPopUpWindow::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

void CAlarmPopUp::OnCommandDisable()
{
	SallyAPI::GUI::SendMessage::CParameterString parameterString("Disable");
	m_pParent->SendMessageToParent(this, 0, MS_SALLY_HIDE_ALARM_WINDOW, &parameterString);			
}

void CAlarmPopUp::OnCommandSleep()
{
	SallyAPI::GUI::SendMessage::CParameterString parameterString("Sleep");
	m_pParent->SendMessageToParent(this, 0, MS_SALLY_HIDE_ALARM_WINDOW, &parameterString);			
}