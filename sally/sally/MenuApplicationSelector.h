////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\MenuApplicationSelector.h
///
/// \brief	Declares the menu application selector class. 
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

class CMenuApplicationSelector :
	public SallyAPI::GUI::CPopUpWindow
{
private:
	SallyAPI::GUI::CLabel*			m_pListInfo;
	SallyAPI::GUI::CScrollList*		m_pList;
	SallyAPI::GUI::CButton*			m_pSelectedAppIcon;
	SallyAPI::GUI::CLabel*			m_pSelectedAppName;

	SallyAPI::GUI::CButton*		m_pLogout;
	SallyAPI::GUI::CButton*		m_pScreensaver;
	SallyAPI::GUI::CButton*		m_pSettings;
	SallyAPI::GUI::CButton*		m_pWindows;
	SallyAPI::GUI::CButton*		m_pLockScreen;
	SallyAPI::GUI::CButton*		m_pUpdateAvailable;

	void	OnCommandApplicationChanged(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);
	void	OnCommandGetUpdate();
public:
	CMenuApplicationSelector(SallyAPI::GUI::CGUIBaseObject* parent, std::map<int, SallyAPI::GUI::CApplicationWindow*>* appWindows);
	virtual ~CMenuApplicationSelector();

	void	ApplicationClicked(int applicationId, const std::string& name);

	virtual void	SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
};
