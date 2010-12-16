////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\AlarmClock.h
///
/// \brief	Declares the alarm clock class. 
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

#pragma once
#include "Define.h"
#include <shellapi.h>
#include "EditAlarmClock.h"

class CAlarmClock :
	public SallyAPI::GUI::CPopUpWindow
{
private:
	SallyAPI::GUI::CGroupBox*			m_pBackgroundGroupBox;
	SallyAPI::GUI::CButtonBar*			m_pMenu;
	SallyAPI::GUI::CButtonBarButton*	m_pButtonAlarmClock;
	SallyAPI::GUI::CButtonBarButton*	m_pButtonTimer;
	SallyAPI::GUI::CButtonBarButton*	m_pButtonStopWatch;
	SallyAPI::GUI::CForm*				m_pFormAlarmClock;
	SallyAPI::GUI::CForm*				m_pFormTimer;
	SallyAPI::GUI::CForm*				m_pFormStopWatch;

	SallyAPI::GUI::CButton*				m_pAlarmEdit[5];
	SallyAPI::GUI::CCheckbox*			m_pAlarmActivate[5];
	SallyAPI::GUI::CTimer*				m_pAlarmTimer;
	int									m_iDayMinutes[5];
	int									m_iNextCheckSleep[5];
	bool								m_bAlarmIsOn;
	SallyAPI::Sound::CPlaySound*		m_ptAlarmPlayAlarm;
	SallyAPI::Sound::CPlaySound*		m_ptAlarmPlayBeep;
	CEditAlarmClock*					m_pEditAlarmClock;
	int									m_iAlarmRunning;
	int									m_iAlarmNumberEdit;

	SallyAPI::GUI::CLabel*				m_pTimerLabel;
	SallyAPI::GUI::CButton*				m_pTimerStart;
	SallyAPI::GUI::CButton*				m_pTimerPause;
	SallyAPI::GUI::CNumberSelector*		m_pTimerSeconds;
	SallyAPI::GUI::CNumberSelector*		m_pTimerMinutes;
	SallyAPI::GUI::CNumberSelector*		m_pTimerHours;
	SallyAPI::GUI::CLabel*				m_pTimerSecondsLabel;
	SallyAPI::GUI::CLabel*				m_pTimerMinutesLabel;
	SallyAPI::GUI::CLabel*				m_pTimerHoursLabel;
	bool								m_bTimerRunning;
	ULONGLONG							m_pTimerSystemTimeStart;
	ULONGLONG							m_pTimerSystemDiff;
	int									m_iSecondsToProcess;
	SallyAPI::Sound::CPlaySound*		m_ptTimerPlayAlarm;

	SallyAPI::GUI::CLabel*				m_pStopWatchLabel;
	SallyAPI::GUI::CButton*				m_pStopWatchStart;
	SallyAPI::GUI::CButton*				m_pStopWatchPause;
	bool								m_bStopWatchRunning;
	ULONGLONG							m_pStopWatchSystemTimeStart;
	ULONGLONG							m_pStopWatchSystemDiff;


	void		ActivateAlarm(int number);
	void		DeactivateAlarm(int number);
	void		OnCommandAlarmTimer();

	void		OnCommandTimerStart();
	void		OnCommandTimerPause();

	void		OnCommandStopWatchStart();
	void		OnCommandStopWatchPause();

	void		StartAlarm(int number);
	void		OnCommandHideAlarm(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);

	void		ProcessStopWatch();
	void		ProcessTimer();
	void		UpdateAlarmText(int number);
public:
	CAlarmClock(SallyAPI::GUI::CGUIBaseObject* parent);
	virtual ~CAlarmClock();

	virtual void Timer(float timeDelta);
	virtual void SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
};
