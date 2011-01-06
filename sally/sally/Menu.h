////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\Menu.h
///
/// \brief	Declares the menu class. 
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
#include <map>
#include "SystemTime.h"
#include "MainWindow.h"
#include "MenuApplicationSelector.h"
#include "AlarmClock.h"

#define GUI_SHOW_VOLUME_CONTROL			100

class SallyAPI::GUI::CApplicationWindow;
class CMainWindow;

#define MENU_MAX_APPS	20

class CMenu :
	public SallyAPI::GUI::CForm
{
private:
	SallyAPI::GUI::CButton*		m_pVolumeControl;
	SallyAPI::GUI::CButton*		m_pVolumeControlNotifer;
	SallyAPI::GUI::CButton*		m_pAlarmClockNotifer;
	SallyAPI::GUI::CButton*		m_pApplicationsButton;
	SallyAPI::GUI::CLabel*		m_pClock;
	SallyAPI::GUI::CLabel*		m_pDate;
	CSystemTime					m_SystemTime;
	SallyAPI::GUI::CImageBox*	m_pBackgroundMenu;

	CMenuApplicationSelector*	m_pApplicationSelection;
	CAlarmClock*				m_pAlarmClock;

	virtual void	RenderControl();
	void			UpdateClock();
	void			UpdateVolume();
public:
	CMenu(SallyAPI::GUI::CGUIBaseObject* parent, std::map<int, SallyAPI::GUI::CApplicationWindow*>* appWindows);
	virtual ~CMenu();

	void	ApplicationClicked(int applicationId, const std::string& name);

	virtual void	SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
};