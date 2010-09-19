////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\AlarmPopUp.h
///
/// \brief	Declares the alarm pop up class. 
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

class CAlarmPopUp :
	public SallyAPI::GUI::CPopUpWindow
{
private:
	SallyAPI::GUI::CGroupBox*		m_pGroupBox;
	SallyAPI::GUI::CLabelBox*		m_pLabelBox;
	SallyAPI::GUI::CButton*			m_pButtonSleep;
	SallyAPI::GUI::CButton*			m_pButtonDisable;

	void	OnCommandSleep();
	void	OnCommandDisable();
public:
	CAlarmPopUp(SallyAPI::GUI::CGUIBaseObject* parent);
	virtual ~CAlarmPopUp();

	virtual void SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);
};
