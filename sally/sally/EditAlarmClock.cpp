////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\EditAlarmClock.cpp
///
/// \brief	Implements the edit alarm clock class. 
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

#include "EditAlarmClock.h"

#define GUI_ALARM_TIMER			50001
#define GUI_ALARM_HOURS			50002
#define GUI_ALARM_MINUTES		50003
#define GUI_ALARM_SOUND			50004
#define GUI_ALARM_SPEAK_TEXT	50005
#define GUI_ALARM_SPEAK			50006
#define GUI_ALARM_SAVE			50007
#define GUI_ALARM_CANCEL		50008
#define GUI_ALARM_BEEP			50009
#define GUI_ALARM_DAY			50050

CEditAlarmClock::CEditAlarmClock(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int height)
	:SallyAPI::GUI::CForm(parent, x, y, width, height, 0)
{
	m_pFormAlarmClock = new SallyAPI::GUI::CGroupBox(this, 0, 0, m_iWidth, m_iHeight);
	this->AddChild(m_pFormAlarmClock);

	/************************************************************************/
	/* Alarm Clock                                                          */
	/************************************************************************/
	m_pAlarmLabelHour = new SallyAPI::GUI::CLabel(m_pFormAlarmClock, 90, 20, 60);
	m_pAlarmLabelHour->SetText("Hour:");
	m_pFormAlarmClock->AddChild(m_pAlarmLabelHour);

	m_pAlarmHours = new SallyAPI::GUI::CNumberSelector(m_pFormAlarmClock, 90 + 65, 20, 90, GUI_ALARM_HOURS);
	m_pAlarmHours->SetMinValue(0);
	m_pAlarmHours->SetMaxValue(23);
	m_pAlarmHours->SetSteps(1);
	m_pFormAlarmClock->AddChild(m_pAlarmHours);

	m_pAlarmLabelMinute = new SallyAPI::GUI::CLabel(m_pFormAlarmClock, 90 + 65 + 90 + 10, 20, 60);
	m_pAlarmLabelMinute->SetText("Minute:");
	m_pFormAlarmClock->AddChild(m_pAlarmLabelMinute);

	m_pAlarmMinutes = new SallyAPI::GUI::CNumberSelector(m_pFormAlarmClock, 90 + 65 + 90 + 10 + 65, 20, 90, GUI_ALARM_MINUTES);
	m_pAlarmMinutes->SetMinValue(0);
	m_pAlarmMinutes->SetMaxValue(59);
	m_pAlarmMinutes->SetSteps(10);
	m_pFormAlarmClock->AddChild(m_pAlarmMinutes);

	m_pAlarmMenuDays = new SallyAPI::GUI::CButtonBar(m_pFormAlarmClock, 75, 70, 350);
	m_pFormAlarmClock->AddChild(m_pAlarmMenuDays);

	for (int i = 0; i < 7; ++i)
	{
		m_pAlarmButtonDays[i] = new SallyAPI::GUI::CButtonBarButton(m_pAlarmMenuDays, 50, GUI_ALARM_DAY + i);
		m_pAlarmButtonDays[i]->SetAlign(DT_CENTER | DT_VCENTER);
		m_pAlarmMenuDays->AddChild(m_pAlarmButtonDays[i]);
	}
	m_pAlarmButtonDays[0]->SetText("Mo");
	m_pAlarmButtonDays[1]->SetText("Tu");
	m_pAlarmButtonDays[2]->SetText("We");
	m_pAlarmButtonDays[3]->SetText("Th");
	m_pAlarmButtonDays[4]->SetText("Fr");
	m_pAlarmButtonDays[5]->SetText("Sa");
	m_pAlarmButtonDays[6]->SetText("So");

	m_pAlarmSound = new SallyAPI::GUI::CRadioButton(m_pFormAlarmClock, 20, 120, 150, GUI_ALARM_SOUND);
	m_pAlarmSound->SetText("Play Sound");
	m_pFormAlarmClock->AddChild(m_pAlarmSound);

	m_pAlarmSpeak = new SallyAPI::GUI::CRadioButton(m_pFormAlarmClock, 180, 120, 150, GUI_ALARM_SPEAK);
	m_pAlarmSpeak->SetText("Talk something");
	m_pFormAlarmClock->AddChild(m_pAlarmSpeak);

	m_pAlarmBeep = new SallyAPI::GUI::CRadioButton(m_pFormAlarmClock, 340, 120, 150, GUI_ALARM_BEEP);
	m_pAlarmBeep->SetText("PC Speaker");
	m_pFormAlarmClock->AddChild(m_pAlarmBeep);

	m_pAlarmSpeakText = new SallyAPI::GUI::CEdit(m_pFormAlarmClock, 30, 170, m_iWidth - 60, GUI_ALARM_SPEAK_TEXT);
	m_pAlarmSpeakText->SetInfoText("Type Text To Speak");
	m_pAlarmSpeakText->SetLocalised(false);
	m_pAlarmSpeakText->Visible(false);
	m_pFormAlarmClock->AddChild(m_pAlarmSpeakText);

	m_pAlarmSave = new SallyAPI::GUI::CButton(m_pFormAlarmClock, m_iWidth - 150 - 20, m_iHeight - CONTROL_HEIGHT - 20, 
		150, CONTROL_HEIGHT, GUI_ALARM_SAVE);
	m_pAlarmSave->SetText("Save");
	m_pAlarmSave->SetImageId(GUI_THEME_SALLY_ICON_SAVE);
	m_pAlarmSave->SetDefaultButton(true);
	m_pFormAlarmClock->AddChild(m_pAlarmSave);

	m_pAlarmCancel = new SallyAPI::GUI::CButton(m_pFormAlarmClock, 20, m_iHeight - CONTROL_HEIGHT - 20, 
		150, CONTROL_HEIGHT, GUI_ALARM_CANCEL);
	m_pAlarmCancel->SetText("Cancel");
	m_pAlarmCancel->SetImageId(GUI_THEME_SALLY_ICON_CANCEL);
	m_pFormAlarmClock->AddChild(m_pAlarmCancel);
}

CEditAlarmClock::~CEditAlarmClock()
{
}

void CEditAlarmClock::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_RADIOBUTTON_CLICKED:
		switch (reporterId)
		{
		case GUI_ALARM_SOUND:
			m_pAlarmSpeak->SetCheckStatus(false);
			m_pAlarmBeep->SetCheckStatus(false);
			m_pAlarmSpeakText->Visible(false);
			return;
		case GUI_ALARM_SPEAK:
			m_pAlarmSound->SetCheckStatus(false);
			m_pAlarmBeep->SetCheckStatus(false);
			m_pAlarmSpeakText->Visible(true);
			return;
		case GUI_ALARM_BEEP:
			m_pAlarmSpeak->SetCheckStatus(false);
			m_pAlarmSound->SetCheckStatus(false);
			m_pAlarmSpeakText->Visible(false);
			return;
		}
		break;
	case GUI_BUTTON_CLICKED:
		switch (reporterId)
		{
		case GUI_ALARM_CANCEL:
			OnCommandChange();
			return;
		case GUI_ALARM_SAVE:
			OnCommandSave();
			return;
		}
		if ((reporterId >= GUI_ALARM_DAY) && (reporterId <= GUI_ALARM_DAY + 6))
		{
			m_pAlarmButtonDays[reporterId - GUI_ALARM_DAY]->SetCheckStatus(!m_pAlarmButtonDays[reporterId - GUI_ALARM_DAY]->GetCheckStatus());
			return;
		}
	}
	SallyAPI::GUI::CForm::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

void CEditAlarmClock::OnCommandChange()
{
	m_pParent->SendMessageToParent(this, GUI_ALARM_CANCEL, GUI_BUTTON_CLICKED);
}

void CEditAlarmClock::OnCommandSave()
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	std::string alarmNumber = "alarm";
	alarmNumber.append(SallyAPI::String::StringHelper::ConvertFloatToString(m_iNumber));
	alarmNumber.append(".");

	std::string speak = alarmNumber;
	speak.append("speak");
	std::string type = alarmNumber;
	type.append("type");
	std::string day = alarmNumber;
	day.append("day");
	std::string hour = alarmNumber;
	hour.append("hour");
	std::string minute = alarmNumber;
	minute.append("minute");

	option->SetPropertyInt("alarm", hour, m_pAlarmHours->GetValue());
	option->SetPropertyInt("alarm", minute, m_pAlarmMinutes->GetValue());

	for (int i = 0; i < 7; ++i)
	{
		std::string propertyValueName = day;
		propertyValueName.append(SallyAPI::String::StringHelper::ConvertFloatToString(i));

		option->SetPropertyBool("alarm", propertyValueName, m_pAlarmButtonDays[i]->GetCheckStatus());
	}

	if (m_pAlarmSound->GetCheckStatus())
	{
		option->SetPropertyInt("alarm", type, 0);
	}
	else if (m_pAlarmSpeak->GetCheckStatus())
	{
		option->SetPropertyInt("alarm", type, 1);
	}
	else
	{
		option->SetPropertyInt("alarm", type, 2);
	}
	option->SetPropertyString("alarm", speak, m_pAlarmSpeakText->GetText());

	m_pParent->SendMessageToParent(this, GUI_ALARM_SAVE, GUI_BUTTON_CLICKED);
}

void CEditAlarmClock::SetValue(int number)
{
	m_iNumber = number;

	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	std::string alarmNumber = "alarm";
	alarmNumber.append(SallyAPI::String::StringHelper::ConvertFloatToString(number));
	alarmNumber.append(".");

	std::string speak = alarmNumber;
	speak.append("speak");
	std::string type = alarmNumber;
	type.append("type");
	std::string day = alarmNumber;
	day.append("day");
	std::string hour = alarmNumber;
	hour.append("hour");
	std::string minute = alarmNumber;
	minute.append("minute");

	m_pAlarmHours->SetValue(option->GetPropertyInt("alarm", hour, 0));
	m_pAlarmMinutes->SetValue(option->GetPropertyInt("alarm", minute, 0));

	for (int i = 0; i < 7; ++i)
	{
		std::string propertyValueName = day;
		propertyValueName.append(SallyAPI::String::StringHelper::ConvertFloatToString(i));

		m_pAlarmButtonDays[i]->SetCheckStatus(option->GetPropertyBool("alarm", propertyValueName, false));
	}

	if (option->GetPropertyInt("alarm", type, 0) == 0)
	{
		m_pAlarmSound->SetCheckStatus(true);
		m_pAlarmSpeak->SetCheckStatus(false);
		m_pAlarmBeep->SetCheckStatus(false);
		m_pAlarmSpeakText->Visible(false);
	}
	else if (option->GetPropertyInt("alarm", type, 0) == 1)
	{
		m_pAlarmSound->SetCheckStatus(false);
		m_pAlarmSpeak->SetCheckStatus(true);
		m_pAlarmBeep->SetCheckStatus(false);
		m_pAlarmSpeakText->Visible(true);
	}
	else
	{
		m_pAlarmSound->SetCheckStatus(false);
		m_pAlarmSpeak->SetCheckStatus(false);
		m_pAlarmBeep->SetCheckStatus(true);
		m_pAlarmSpeakText->Visible(false);
	}
	m_pAlarmSpeakText->SetText(option->GetPropertyString("alarm", speak, ""));
}