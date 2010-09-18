////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\EditAlarmClock.h
///
/// \brief	Declares the edit alarm clock class. 
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

#pragma once
#include "Define.h"

class CEditAlarmClock :
	public SallyAPI::GUI::CForm
{
private:
	SallyAPI::GUI::CGroupBox*			m_pFormAlarmClock;

	SallyAPI::GUI::CNumberSelector*		m_pAlarmMinutes;
	SallyAPI::GUI::CNumberSelector*		m_pAlarmHours;
	SallyAPI::GUI::CButtonBar*			m_pAlarmMenuDays;
	SallyAPI::GUI::CButtonBarButton*	m_pAlarmButtonDays[7];
	SallyAPI::GUI::CRadioButton*		m_pAlarmSound;
	SallyAPI::GUI::CRadioButton*		m_pAlarmSpeak;
	SallyAPI::GUI::CRadioButton*		m_pAlarmBeep;
	SallyAPI::GUI::CEdit*				m_pAlarmSpeakText;
	SallyAPI::GUI::CLabel*				m_pAlarmLabelHour;
	SallyAPI::GUI::CLabel*				m_pAlarmLabelMinute;
	SallyAPI::GUI::CButton*				m_pAlarmSave;
	SallyAPI::GUI::CButton*				m_pAlarmCancel;
	int									m_iNumber;

	void		OnCommandChange();
	void		OnCommandSave();
public:
	CEditAlarmClock(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int height);
	virtual ~CEditAlarmClock();

	virtual void SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);
	void SetValue(int number);
};
