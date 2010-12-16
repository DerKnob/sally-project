////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\MyWizardPanel.h
///
/// \brief	Declares my wizard panel class. 
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
#include <sallyAPI\sallyAPI.h>
#include "MyConfigPanel.h"

class CMyWizardPanel :
	public SallyAPI::GUI::CWizardPanel
{
private:
	CMyConfigPanel*						m_pConfigPanel;
	SallyAPI::GUI::CApplicationWindow*	m_pMainWindow;

	SallyAPI::GUI::CLabelBox*	m_pLabelBoxWelcome1;
	SallyAPI::GUI::CLabelBox*	m_pLabelBoxWelcome2;

	SallyAPI::GUI::CButton*		m_pSelectFolder[8];
	SallyAPI::GUI::CButton*		m_pEmptySelection[8];
	SallyAPI::GUI::CEdit*		m_pSelection[8];

	void OnCommandSelectFolder(int reporterId);
	void OnCommandEmptySelection(int reporterId);
	void OnCommandFolderSelected(int reporterId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);

public:
	CMyWizardPanel(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, const std::string& pluginPath, 
		CMyConfigPanel* configPanel, SallyAPI::GUI::CApplicationWindow* mainWindow);
	virtual ~CMyWizardPanel();

	virtual void LoadConfig();
	virtual void SaveConfig();

	virtual void SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
};