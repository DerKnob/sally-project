////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	ImageBrowser\MyConfigPanel.h
///
/// \brief	Declares my configuration panel class. 
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
#include <shlobj.h>
#include "CreateNewDatabase.h"

class CMyConfigPanel
	: public SallyAPI::GUI::CConfigPanel
{
private:
	SallyAPI::GUI::CTabcontrol*		m_pTabControl;
	SallyAPI::GUI::CTabcontrolItem*	m_pTabFolders;
	SallyAPI::GUI::CTabcontrolItem*	m_pTabScreensaver;
	SallyAPI::GUI::CTabcontrolItem*	m_pTabDatabase;

	SallyAPI::GUI::CLabel*		m_pLabel[2];
	SallyAPI::GUI::CEdit*		m_pTimerScreensaver;
	SallyAPI::GUI::CCheckbox*	m_pTimerShuffle;
	SallyAPI::GUI::CDropDown*	m_pDropDownChangeType;

	SallyAPI::GUI::CCheckbox*	m_pShowAlwaysHarddiscs;
	SallyAPI::GUI::CButton*		m_pSelectFolder[12];
	SallyAPI::GUI::CButton*		m_pEmptySelection[12];
	SallyAPI::GUI::CEdit*		m_pSelection[12];
	SallyAPI::GUI::CCheckbox*	m_pIncludeInScreensaver[12];

	SallyAPI::GUI::CLabel*		m_pUpdateDBLastRunLabel;
	SallyAPI::GUI::CLabel*		m_pUpdateDBLastRunInfo;
	SallyAPI::GUI::CButton*		m_pButtonUpdateDB;
	SallyAPI::GUI::CButton*		m_pButtonUpdateDBCancel;
	SallyAPI::GUI::CProcessbar*	m_pProcessbarUpdateDB;
	SallyAPI::GUI::CButton*		m_pButtonDeleteDB;
	SallyAPI::GUI::CButton*		m_pButtonResetLastBuildDB;

	SallyAPI::GUI::CLabel*		m_pInfo;

	CCreateNewDatabase			m_tCreateNewDatabase;

	bool CheckIfChanged();

	void OnCommandResetLastBuildDatabase();

	void OnCommandUpdateDBScheduler();
	void OnCommandUpdateDBDone();

	void OnCommandUpdateDB(bool showPopUp);
	void OnCommandUpdateDBCancel();
	void OnCommandAskToDeleteDB();
	void OnCommandConfirmDeleteDB();

	void OnCommandSelectFolder(int reporterId);
	void OnCommandEmptySelection(int reporterId);
	void OnCommandFolderSelected(int reporterId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);
public:
	CMyConfigPanel(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, const std::string& pluginPath);
	virtual ~CMyConfigPanel();

	virtual void LoadConfig();
	virtual void SaveConfig();

	void	WizardDBCreate();

	virtual void SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
};
