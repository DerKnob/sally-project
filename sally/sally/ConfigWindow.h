////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\ConfigWindow.h
///
/// \brief	Declares the configuration Windows Form. 
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
#include "SallyConfigPanel.h"
#include "SaveLoadThread.h"

class CConfigWindow
	: public SallyAPI::GUI::CApplicationWindow
{
private:
	SallyAPI::GUI::CForm*				m_pConfigForm;
	SallyAPI::GUI::CImageBox*			m_pTopBorder;
	SallyAPI::GUI::CImageBox*			m_pBottomBorder;
	SallyAPI::GUI::CWorking*			m_pLogo;
	SallyAPI::GUI::CButton*				m_pReload;
	SallyAPI::GUI::CButton*				m_pSave;
	SallyAPI::GUI::CDropDown*			m_pMenuDropDown;
	CSallyConfigPanel*					m_pSallyConfigPanel;
	SallyAPI::GUI::CConfigPanel*		m_pCurrentConfigPanel;
	CSaveLoadThread						m_tSaveLoadThread;
	std::map<int, SallyAPI::GUI::CConfigPanel*>	m_mConfigPanels;

	void OnCommandConfigPanelClicked();

public:
	CConfigWindow(SallyAPI::GUI::CGUIBaseObject* parent);
	virtual ~CConfigWindow();

	CSallyConfigPanel* GetSallyConfigPanel();

	void	AddConfigPanel(SallyAPI::GUI::CApplicationWindow* applicationWindow, SallyAPI::GUI::CConfigPanel* configPanel);
	void	SetLoadedPlugins(std::map<int, SallyAPI::GUI::CApplicationWindow*>* applicationWindowList);

	virtual void SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
};