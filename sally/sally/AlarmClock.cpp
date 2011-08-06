////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\AlarmClock.cpp
///
/// \brief	Implements the alarm clock class. 
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

#include "AlarmClock.h"

#define GUI_ALARM_CLOCK			50000
#define GUI_ALARM_TIMER			50001
#define GUI_STOP_WATCH			50002

#define GUI_ALARM_SAVE			50007
#define GUI_ALARM_CANCEL		50008

#define GUI_STOP_WATCH_START	50010
#define GUI_STOP_WATCH_PAUSE	50011

#define GUI_TIMER_START			50012
#define GUI_TIMER_PAUSE			50013

#define GUI_ALARM_TIMER_CHECK	50050
#define GUI_ALARM_EDIT			50060
#define GUI_ALARM_ACTIVATE		50070


CAlarmClock::CAlarmClock(SallyAPI::GUI::CGUIBaseObject* parent)
	:SallyAPI::GUI::CPopUpWindow(parent, 0, ""), m_bStopWatchRunning(false), m_bTimerRunning(false), 
	m_bAlarmIsOn(false), m_iAlarmRunning(-1)
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	SetCloseOnClick(true);

	m_pBackgroundGroupBox = new SallyAPI::GUI::CGroupBox(this, (WINDOW_WIDTH - 520) / 2, (WINDOW_HEIGHT - 320) / 2, 520, 320);
	m_pBackgroundGroupBox->SetAlwaysHandleInput(true);
	this->AddChild(m_pBackgroundGroupBox);

	m_pMenu = new SallyAPI::GUI::CButtonBar(m_pBackgroundGroupBox, 20, 20, 480);
	m_pBackgroundGroupBox->AddChild(m_pMenu);

	m_pButtonAlarmClock = new SallyAPI::GUI::CButtonBarButton(m_pMenu, 160, GUI_ALARM_CLOCK);
	m_pButtonAlarmClock->SetImageId(GUI_THEME_SALLY_ICON_DATE);
	m_pButtonAlarmClock->SetText("Alarm Clock");
	m_pButtonAlarmClock->SetCheckStatus(true);
	m_pMenu->AddChild(m_pButtonAlarmClock);

	m_pButtonTimer = new SallyAPI::GUI::CButtonBarButton(m_pMenu, 160, GUI_ALARM_TIMER);
	m_pButtonTimer->SetImageId(GUI_THEME_SALLY_ICON_TIMER);
	m_pButtonTimer->SetText("Timer");
	m_pMenu->AddChild(m_pButtonTimer);

	m_pButtonStopWatch = new SallyAPI::GUI::CButtonBarButton(m_pMenu, 160, GUI_STOP_WATCH);
	m_pButtonStopWatch->SetImageId(GUI_THEME_SALLY_ICON_TIMER);
	m_pButtonStopWatch->SetText("Stop Watch");
	m_pMenu->AddChild(m_pButtonStopWatch);

	// forms
	m_pFormAlarmClock = new SallyAPI::GUI::CForm(m_pBackgroundGroupBox, 20, 60, 520 - 20, 320 - 60);
	m_pBackgroundGroupBox->AddChild(m_pFormAlarmClock);

	m_pFormTimer = new SallyAPI::GUI::CForm(m_pBackgroundGroupBox, 20, 60, 520 - 20, 320 - 60);
	m_pFormTimer->Visible(false);
	m_pBackgroundGroupBox->AddChild(m_pFormTimer);

	m_pFormStopWatch = new SallyAPI::GUI::CForm(m_pBackgroundGroupBox, 20, 60, 520 - 20, 320 - 60);
	m_pFormStopWatch->Visible(false);
	m_pBackgroundGroupBox->AddChild(m_pFormStopWatch);


	std::string timerAlarm = SallyAPI::System::SystemHelper::GetModulePath();
	timerAlarm.append("sounds\\timer.wav");

	std::string alarmAlarm = SallyAPI::System::SystemHelper::GetModulePath();
	alarmAlarm.append("sounds\\alarm.wav");

	m_ptTimerPlayAlarm = new SallyAPI::Sound::CPlaySound(this, timerAlarm);
	m_ptAlarmPlayAlarm = new SallyAPI::Sound::CPlaySound(this, alarmAlarm);
	m_ptAlarmPlayBeep = new SallyAPI::Sound::CPlaySound(this, 100, 500, 3, 100);

	/************************************************************************/
	/* Alarm Clock                                                          */
	/************************************************************************/

	m_pAlarmTimer = new SallyAPI::GUI::CTimer(5, this, 0, GUI_ALARM_TIMER_CHECK);
	m_pAlarmTimer->Start();

	for (int i = 0; i < 5; ++i)
	{
		m_pAlarmEdit[i] = new SallyAPI::GUI::CButton(m_pFormAlarmClock, 0, 30 + ((CONTROL_HEIGHT + 10) * i),
			250, CONTROL_HEIGHT, GUI_ALARM_EDIT + i);
		m_pAlarmEdit[i]->SetLocalised(false);
		m_pFormAlarmClock->AddChild(m_pAlarmEdit[i]);

		m_pAlarmActivate[i] = new SallyAPI::GUI::CCheckbox(m_pFormAlarmClock, 260, 30 + ((CONTROL_HEIGHT + 10) * i),
			240, GUI_ALARM_ACTIVATE + i);
		m_pAlarmActivate[i]->SetText("Activate Alarm");
		m_pFormAlarmClock->AddChild(m_pAlarmActivate[i]);

		m_iNextCheckSleep[i] = -1;
		m_iDayMinutes[i] = 0;

		std::string alarmNumber = "alarm";
		alarmNumber.append(SallyAPI::String::StringHelper::ConvertFloatToString(i));
		alarmNumber.append(".active");

		if (option->GetPropertyBool("alarm", alarmNumber, false))
			m_pAlarmActivate[i]->SetCheckStatus(true);

		UpdateAlarmText(i);
	}

	m_pEditAlarmClock = new CEditAlarmClock(this, (WINDOW_WIDTH - 520) / 2, (WINDOW_HEIGHT - 320) / 2, 520, 320);
	m_pEditAlarmClock->Visible(false);
	this->AddChild(m_pEditAlarmClock);

	/************************************************************************/
	/* Timer                                                                */
	/************************************************************************/
	m_pTimerHoursLabel = new SallyAPI::GUI::CLabel(m_pFormTimer, 0 + 80, 50, 100);
	m_pTimerHoursLabel->SetAlign(DT_CENTER | DT_VCENTER);
	m_pTimerHoursLabel->SetText("Hours");
	m_pFormTimer->AddChild(m_pTimerHoursLabel);

	m_pTimerMinutesLabel = new SallyAPI::GUI::CLabel(m_pFormTimer, 110 + 80, 50, 100);
	m_pTimerMinutesLabel->SetAlign(DT_CENTER | DT_VCENTER);
	m_pTimerMinutesLabel->SetText("Minutes");
	m_pFormTimer->AddChild(m_pTimerMinutesLabel);

	m_pTimerSecondsLabel = new SallyAPI::GUI::CLabel(m_pFormTimer, 220 + 80, 50, 100);
	m_pTimerSecondsLabel->SetAlign(DT_CENTER | DT_VCENTER);
	m_pTimerSecondsLabel->SetText("Seconds");
	m_pFormTimer->AddChild(m_pTimerSecondsLabel);

	m_pTimerHours = new SallyAPI::GUI::CNumberSelector(m_pFormTimer, 0 + 80, 80, 100);
	m_pTimerHours->SetMinValue(0);
	m_pTimerHours->SetMaxValue(96);
	m_pTimerHours->SetSteps(1);
	m_pTimerHours->SetValue(option->GetPropertyInt("alarm", "timerHours", 0));
	m_pFormTimer->AddChild(m_pTimerHours);

	m_pTimerMinutes = new SallyAPI::GUI::CNumberSelector(m_pFormTimer, 110 + 80, 80, 100);
	m_pTimerMinutes->SetMinValue(0);
	m_pTimerMinutes->SetMaxValue(59);
	m_pTimerMinutes->SetSteps(10);
	m_pTimerMinutes->SetValue(option->GetPropertyInt("alarm", "timerMinutes", 10));
	m_pFormTimer->AddChild(m_pTimerMinutes);

	m_pTimerSeconds = new SallyAPI::GUI::CNumberSelector(m_pFormTimer, 220 + 80, 80, 100);
	m_pTimerSeconds->SetMinValue(0);
	m_pTimerSeconds->SetMaxValue(59);
	m_pTimerSeconds->SetSteps(10);
	m_pTimerSeconds->SetValue(option->GetPropertyInt("alarm", "timerSeconds", 0));
	m_pFormTimer->AddChild(m_pTimerSeconds);

	m_pTimerLabel = new SallyAPI::GUI::CLabel(m_pFormTimer, 0, 130, 500 - 20);
	m_pTimerLabel->SetLocalised(false);
	m_pTimerLabel->SetFont("big3.font");
	m_pTimerLabel->SetAlign(DT_CENTER | DT_VCENTER);
	m_pTimerLabel->SetText("0:00:00");
	m_pFormTimer->AddChild(m_pTimerLabel);

	m_pTimerStart = new SallyAPI::GUI::CButton(m_pFormTimer, 500 - 20 - 150, 300 - 60 - CONTROL_HEIGHT, 150, CONTROL_HEIGHT, GUI_TIMER_START);
	m_pTimerStart->SetText("Start");
	m_pTimerStart->SetImageId(GUI_THEME_SALLY_ICON_MEDIA_PLAY);
	m_pFormTimer->AddChild(m_pTimerStart);

	m_pTimerPause = new SallyAPI::GUI::CButton(m_pFormTimer, 500 - 20 - 310, 300 - 60 - CONTROL_HEIGHT, 150, CONTROL_HEIGHT, GUI_TIMER_PAUSE);
	m_pTimerPause->SetText("Pause");
	m_pTimerPause->SetImageId(GUI_THEME_SALLY_ICON_MEDIA_PAUSE);
	m_pTimerPause->Enable(false);
	m_pFormTimer->AddChild(m_pTimerPause);

	/************************************************************************/
	/* Stop Watch                                                           */
	/************************************************************************/
	m_pStopWatchLabel = new SallyAPI::GUI::CLabel(m_pFormStopWatch, 0, 80, 500 - 20);
	m_pStopWatchLabel->SetLocalised(false);
	m_pStopWatchLabel->SetFont("big3.font");
	m_pStopWatchLabel->SetAlign(DT_CENTER | DT_VCENTER);
	m_pStopWatchLabel->SetText("0:00:00.000");
	m_pFormStopWatch->AddChild(m_pStopWatchLabel);

	m_pStopWatchStart = new SallyAPI::GUI::CButton(m_pFormStopWatch, 500 - 20 - 150, 300 - 60 - CONTROL_HEIGHT, 150, CONTROL_HEIGHT, GUI_STOP_WATCH_START);
	m_pStopWatchStart->SetText("Start");
	m_pStopWatchStart->SetImageId(GUI_THEME_SALLY_ICON_MEDIA_PLAY);
	m_pFormStopWatch->AddChild(m_pStopWatchStart);

	m_pStopWatchPause = new SallyAPI::GUI::CButton(m_pFormStopWatch, 500 - 20 - 310, 300 - 60 - CONTROL_HEIGHT, 150, CONTROL_HEIGHT, GUI_STOP_WATCH_PAUSE);
	m_pStopWatchPause->SetText("Pause");
	m_pStopWatchPause->SetImageId(GUI_THEME_SALLY_ICON_MEDIA_PAUSE);
	m_pStopWatchPause->Enable(false);
	m_pFormStopWatch->AddChild(m_pStopWatchPause);
}

CAlarmClock::~CAlarmClock()
{
	m_ptTimerPlayAlarm->WaitForStop();
	SafeDelete(m_ptTimerPlayAlarm);

	m_ptAlarmPlayAlarm->WaitForStop();
	SafeDelete(m_ptAlarmPlayAlarm);

	m_pAlarmTimer->WaitForStop();
	SafeDelete(m_pAlarmTimer);
}

void CAlarmClock::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	switch (messageId)
	{
	case GUI_ALARM_TIMER_CHECK:
		OnCommandAlarmTimer();
		return;
	case GUI_CHECKBOX_CLICKED:
		if ((reporterId >= GUI_ALARM_ACTIVATE) && (reporterId <= GUI_ALARM_ACTIVATE + 4))
		{
			if (m_pAlarmActivate[reporterId - GUI_ALARM_ACTIVATE]->GetCheckStatus())
				ActivateAlarm(reporterId - GUI_ALARM_ACTIVATE);
			else
				DeactivateAlarm(reporterId - GUI_ALARM_ACTIVATE);
			return;
		}
		break;
	case MS_SALLY_SALLY_CONFIG_CHANGED:
		for (int i = 0; i < 5; ++i)
		{
			UpdateAlarmText(i);
		}
		return;
	case GUI_BUTTON_CLICKED:
		switch (reporterId)
		{
		case GUI_ALARM_SAVE:
			UpdateAlarmText(m_iAlarmNumberEdit);
		case GUI_ALARM_CANCEL:
			m_pBackgroundGroupBox->Visible(true);
			m_pEditAlarmClock->Visible(false);
			m_pButtonClose->Visible(true);
			SetCloseOnClick(true);
			return;
		case GUI_TIMER_START:
			OnCommandTimerStart();
			return;
		case GUI_TIMER_PAUSE:
			OnCommandTimerPause();
			return;
		case GUI_STOP_WATCH_START:
			OnCommandStopWatchStart();
			return;
		case GUI_STOP_WATCH_PAUSE:
			OnCommandStopWatchPause();
			return;
		case GUI_ALARM_CLOCK:
			m_pButtonAlarmClock->SetCheckStatus(true);
			m_pButtonTimer->SetCheckStatus(false);
			m_pButtonStopWatch->SetCheckStatus(false);

			m_pFormAlarmClock->Visible(true);
			m_pFormTimer->Visible(false);
			m_pFormStopWatch->Visible(false);
			return;
		case GUI_ALARM_TIMER:
			m_pButtonAlarmClock->SetCheckStatus(false);
			m_pButtonTimer->SetCheckStatus(true);
			m_pButtonStopWatch->SetCheckStatus(false);

			m_pFormAlarmClock->Visible(false);
			m_pFormTimer->Visible(true);
			m_pFormStopWatch->Visible(false);
			return;
		case GUI_STOP_WATCH:
			m_pButtonAlarmClock->SetCheckStatus(false);
			m_pButtonTimer->SetCheckStatus(false);
			m_pButtonStopWatch->SetCheckStatus(true);

			m_pFormAlarmClock->Visible(false);
			m_pFormTimer->Visible(false);
			m_pFormStopWatch->Visible(true);
			return;
		}
		
		if ((reporterId >= GUI_ALARM_EDIT) && (reporterId <= GUI_ALARM_EDIT + 5))
		{
			m_iAlarmNumberEdit = reporterId - GUI_ALARM_EDIT;
			m_pBackgroundGroupBox->Visible(false);
			m_pEditAlarmClock->SetValue(reporterId - GUI_ALARM_EDIT);
			m_pEditAlarmClock->Visible(true);
			m_pButtonClose->Visible(false);
			SetCloseOnClick(false);
			return;
		}
		break;
	case MS_SALLY_HIDE_ALARM_WINDOW:
		OnCommandHideAlarm(messageParameter);
		return;
	}
	SallyAPI::GUI::CPopUpWindow::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

void CAlarmClock::UpdateAlarmText(int number)
{
	std::string info;

	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();
	SallyAPI::Config::CLanguageManager* lang = config->GetLanguageLocalization();

	std::string alarmNumber = "alarm";
	alarmNumber.append(SallyAPI::String::StringHelper::ConvertFloatToString(number));
	alarmNumber.append(".");

	std::string hour = alarmNumber;
	hour.append("hour");
	std::string minute = alarmNumber;
	minute.append("minute");
	std::string day = alarmNumber;
	day.append("day");

	if (option->GetPropertyString("alarm", hour, "0").length() < 2)
		info.append("0");
	info.append(option->GetPropertyString("alarm", hour, "0"));
	info.append(":");
	if (option->GetPropertyString("alarm", minute, "0").length() < 2)
		info.append("0");
	info.append(option->GetPropertyString("alarm", minute, "0"));
	info.append(" ");

	bool first = true;

	for (int i = 0; i < 7; ++i)
	{
		std::string propertyValueName = day;
		propertyValueName.append(SallyAPI::String::StringHelper::ConvertFloatToString(i));

		if (option->GetPropertyBool("alarm", propertyValueName, false))
		{
			if (!first)
				info.append(", ");

			switch (i)
			{
			case 0:
				info.append(lang->GetString("Mo"));
				break;
			case 1:
				info.append(lang->GetString("Tu"));
				break;
			case 2:
				info.append(lang->GetString("We"));
				break;
			case 3:
				info.append(lang->GetString("Th"));
				break;
			case 4:
				info.append(lang->GetString("Fr"));
				break;
			case 5:
				info.append(lang->GetString("Sa"));
				break;
			case 6:
				info.append(lang->GetString("So"));
				break;
			}
			first = false;
		}
	}

	if (first)
		info.append(lang->GetString("click to edit"));

	m_pAlarmEdit[number]->SetText(info);
}

void CAlarmClock::Timer(float timeDelta)
{
	SallyAPI::GUI::CPopUpWindow::Timer(timeDelta);

	if (m_bStopWatchRunning)
		ProcessStopWatch();

	if (m_bTimerRunning)
		ProcessTimer();
}

void CAlarmClock::OnCommandHideAlarm(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	SallyAPI::GUI::SendMessage::CParameterString* messageParameterString = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterString*> (messageParameter);

	if (messageParameterString == NULL)
		return;

	if (messageParameterString->GetString().compare("Sleep") == 0)
	{
		SYSTEMTIME currentTime;
		GetLocalTime(&currentTime);

		int dayMinutes = currentTime.wMinute;
		dayMinutes += currentTime.wHour * 60;

		m_iNextCheckSleep[m_iAlarmRunning] = dayMinutes + 10;
	}
	else
	{
		m_iNextCheckSleep[m_iAlarmRunning] = -1;
	}
	m_iAlarmRunning = -1;
	m_bAlarmIsOn = false;
}

void CAlarmClock::OnCommandAlarmTimer()
{
	if (m_bAlarmIsOn)
	{
		StartAlarm(m_iAlarmRunning);
		return;
	}

	SYSTEMTIME currentTime;
	GetLocalTime(&currentTime);

	// Specifies the current day of the week; Sunday = 0, Monday = 1, and so on
	int dayId = currentTime.wDayOfWeek - 1;

	// correct the sunday
	if (dayId < 0)
		dayId = 6;

	int currentMinutes = currentTime.wMinute;
	currentMinutes += currentTime.wHour * 60;

	for (int i = 0; i < 5; ++i)
	{
		SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
		SallyAPI::System::COption* option = config->GetOption();

		std::string alarmNumber = "alarm";
		alarmNumber.append(SallyAPI::String::StringHelper::ConvertFloatToString(i));
		alarmNumber.append(".");

		std::string activate = alarmNumber;
		activate.append("active");

		if (option->GetPropertyBool("alarm", activate, false))
		{
			std::string hour = alarmNumber;
			hour.append("hour");
			std::string minute = alarmNumber;
			minute.append("minute");
			std::string day = alarmNumber;
			day.append("day");
			day.append(SallyAPI::String::StringHelper::ConvertFloatToString(dayId));

			// can we check today
			if (option->GetPropertyBool("alarm", day, false))
			{
				int alarmMinutes = option->GetPropertyInt("alarm", minute, 0);
				alarmMinutes += option->GetPropertyInt("alarm", hour, 0) * 60;

				if ((alarmMinutes == currentMinutes) && (m_iDayMinutes[i] != currentMinutes))
				{
					StartAlarm(i);

					m_bAlarmIsOn = true;
					m_iDayMinutes[i] = currentMinutes;
					m_iAlarmRunning = i;
					SendMessageToParent(this, 0, MS_SALLY_SHOW_ALARM_WINDOW);
					return;
				}
			}
			if ((m_iNextCheckSleep[i] == currentMinutes) && (m_iDayMinutes[i] != m_iNextCheckSleep[i]))
			{
				StartAlarm(i);

				m_bAlarmIsOn = true;
				m_iDayMinutes[i] = currentMinutes;
				m_iAlarmRunning = i;
				SendMessageToParent(this, 0, MS_SALLY_SHOW_ALARM_WINDOW);
				return;
			}
		}
	}
	return;
}

void CAlarmClock::StartAlarm(int number)
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	std::string alarmNumber = "alarm";
	alarmNumber.append(SallyAPI::String::StringHelper::ConvertFloatToString(number));
	alarmNumber.append(".");

	std::string type = alarmNumber;
	type.append("type");
	std::string speak = alarmNumber;
	speak.append("speak");

	if (option->GetPropertyInt("alarm", type, 0) == 0)
	{
		m_ptAlarmPlayAlarm->Start();
	}
	else if (option->GetPropertyInt("alarm", type, 0) == 1)
	{
		SallyAPI::Speech::CSpeechOutputManager* speechOutputManager = SallyAPI::Speech::CSpeechOutputManager::GetInstance();
		speechOutputManager->Speak(option->GetPropertyString("alarm", speak, ""), true);
	}
	else
	{
		m_ptAlarmPlayBeep->Start();
	}
}

void CAlarmClock::DeactivateAlarm(int number)
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	std::string alarmNumber = "alarm";
	alarmNumber.append(SallyAPI::String::StringHelper::ConvertFloatToString(number));
	alarmNumber.append(".active");

	option->SetPropertyBool("alarm", alarmNumber, false);

	m_iNextCheckSleep[number] = -1;
}

void CAlarmClock::ActivateAlarm(int number)
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	std::string alarmNumber = "alarm";
	alarmNumber.append(SallyAPI::String::StringHelper::ConvertFloatToString(number));
	alarmNumber.append(".active");

	option->SetPropertyBool("alarm", alarmNumber, true);
}

void CAlarmClock::ProcessStopWatch()
{
	ULONGLONG currentTime = SallyAPI::Date::DateHelper::GetMilliseconds();
	ULONGLONG diff = currentTime - m_pStopWatchSystemTimeStart + m_pStopWatchSystemDiff;

	std::string info;
	std::string milliseconds;
	std::string seconds;
	std::string minutes;
	std::string hours;

	if ((int) diff % 1000 < 10)
		milliseconds.append("0");
	if ((int) diff % 1000 < 100)
		milliseconds.append("0");
	milliseconds.append(SallyAPI::String::StringHelper::ConvertToString((int) diff % 1000));

	diff = diff / 1000;

	if ((int) diff % 60 < 10)
		seconds.append("0");
	seconds.append(SallyAPI::String::StringHelper::ConvertToString((int) diff % 60));

	diff = diff / 60;

	if ((int) diff % 60 < 10)
		minutes.append("0");
	minutes.append(SallyAPI::String::StringHelper::ConvertToString((int) diff % 60));

	diff = diff / 60;
	hours = SallyAPI::String::StringHelper::ConvertToString((int) diff);

	info.append(hours);
	info.append(":");
	info.append(minutes);
	info.append(":");
	info.append(seconds);
	info.append(".");
	info.append(milliseconds);

	m_pStopWatchLabel->SetText(info);
}

void CAlarmClock::ProcessTimer()
{
	ULONGLONG currentTime = SallyAPI::Date::DateHelper::GetMilliseconds();
	ULONGLONG diffTemp = currentTime - m_pTimerSystemTimeStart + m_pTimerSystemDiff;
	int diff = m_iSecondsToProcess - (diffTemp / 1000);

	if (diff <= 0)
	{
		m_pTimerLabel->SetText("0:00:00");

		SendMessageToParent(this, GUI_TIMER_START, GUI_BUTTON_CLICKED);
		m_ptTimerPlayAlarm->Start();
		return;
	}

	std::string info;
	std::string seconds;
	std::string minutes;
	std::string hours;

	if ((int) diff % 60 < 10)
		seconds.append("0");
	seconds.append(SallyAPI::String::StringHelper::ConvertToString((int) diff % 60));

	diff = diff / 60;

	if ((int) diff % 60 < 10)
		minutes.append("0");
	minutes.append(SallyAPI::String::StringHelper::ConvertToString((int) diff % 60));

	diff = diff / 60;
	hours = SallyAPI::String::StringHelper::ConvertToString((int) diff);

	info.append(hours);
	info.append(":");
	info.append(minutes);
	info.append(":");
	info.append(seconds);

	m_pTimerLabel->SetText(info);
}

void CAlarmClock::OnCommandTimerStart()
{
	if (m_pTimerPause->IsEnabled())
	{
		m_bTimerRunning = false;
		m_pTimerPause->Enable(false);
		m_pTimerStart->SetText("Start");
		m_pTimerStart->SetImageId(GUI_THEME_SALLY_ICON_MEDIA_PLAY);

		m_pTimerHours->Enable(true);
		m_pTimerMinutes->Enable(true);
		m_pTimerSeconds->Enable(true);

		m_pTimerPause->SetText("Pause");
		m_pTimerPause->SetImageId(GUI_THEME_SALLY_ICON_MEDIA_PAUSE);
	}
	else
	{
		SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
		SallyAPI::System::COption* option = config->GetOption();

		m_pTimerSystemDiff = 0;
		m_pTimerSystemTimeStart = SallyAPI::Date::DateHelper::GetMilliseconds();
		m_bTimerRunning = true;
		m_pTimerPause->Enable(true);
		m_pTimerStart->SetText("Stop");
		m_pTimerStart->SetImageId(GUI_THEME_SALLY_ICON_MEDIA_STOP);

		m_iSecondsToProcess = 0;
		m_iSecondsToProcess += m_pTimerSeconds->GetValue();
		m_iSecondsToProcess += m_pTimerMinutes->GetValue() * 60;
		m_iSecondsToProcess += m_pTimerHours->GetValue() * 60 * 60;

		m_pTimerHours->Enable(false);
		m_pTimerMinutes->Enable(false);
		m_pTimerSeconds->Enable(false);

		option->SetPropertyInt("alarm", "timerHours", m_pTimerHours->GetValue());
		option->SetPropertyInt("alarm", "timerMinutes", m_pTimerMinutes->GetValue());
		option->SetPropertyInt("alarm", "timerSeconds", m_pTimerSeconds->GetValue());
	}
}

void CAlarmClock::OnCommandTimerPause()
{
	if (m_bTimerRunning)
	{
		m_bTimerRunning = false;
		ULONGLONG currentTime = SallyAPI::Date::DateHelper::GetMilliseconds();
		m_pTimerSystemDiff += currentTime - m_pTimerSystemTimeStart;

		m_pTimerPause->SetText("Continue");
		m_pTimerPause->SetImageId(GUI_THEME_SALLY_ICON_MEDIA_PLAY);
	}
	else
	{
		m_bTimerRunning = true;
		m_pTimerSystemTimeStart = SallyAPI::Date::DateHelper::GetMilliseconds();

		m_pTimerPause->SetText("Pause");
		m_pTimerPause->SetImageId(GUI_THEME_SALLY_ICON_MEDIA_PAUSE);
	}
}

void CAlarmClock::OnCommandStopWatchStart()
{
	if (m_pStopWatchPause->IsEnabled())
	{
		m_bStopWatchRunning = false;
		m_pStopWatchPause->Enable(false);
		m_pStopWatchStart->SetText("Start");
		m_pStopWatchStart->SetImageId(GUI_THEME_SALLY_ICON_MEDIA_PLAY);

		m_pStopWatchPause->SetText("Pause");
		m_pStopWatchPause->SetImageId(GUI_THEME_SALLY_ICON_MEDIA_PAUSE);
	}
	else
	{
		m_pStopWatchSystemDiff = 0;
		m_pStopWatchSystemTimeStart = SallyAPI::Date::DateHelper::GetMilliseconds();
		m_bStopWatchRunning = true;
		m_pStopWatchPause->Enable(true);
		m_pStopWatchStart->SetText("Stop");
		m_pStopWatchStart->SetImageId(GUI_THEME_SALLY_ICON_MEDIA_STOP);
	}
}

void CAlarmClock::OnCommandStopWatchPause()
{
	if (m_bStopWatchRunning)
	{
		m_bStopWatchRunning = false;
		ULONGLONG currentTime = SallyAPI::Date::DateHelper::GetMilliseconds();
		m_pStopWatchSystemDiff += currentTime - m_pStopWatchSystemTimeStart;

		m_pStopWatchPause->SetText("Continue");
		m_pStopWatchPause->SetImageId(GUI_THEME_SALLY_ICON_MEDIA_PLAY);
	}
	else
	{
		m_bStopWatchRunning = true;
		m_pStopWatchSystemTimeStart = SallyAPI::Date::DateHelper::GetMilliseconds();

		m_pStopWatchPause->SetText("Pause");
		m_pStopWatchPause->SetImageId(GUI_THEME_SALLY_ICON_MEDIA_PAUSE);
	}
}