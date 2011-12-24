////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\MyConfigPanel.h
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
#include "GetCovers.h"

class CMyConfigPanel
	: public SallyAPI::GUI::CConfigPanel
{
private:
	SallyAPI::GUI::CTabcontrol*		m_pTabControl;
	SallyAPI::GUI::CTabcontrolItem*	m_pTabFolders;
	SallyAPI::GUI::CTabcontrolItem*	m_pTabDatabase;
	SallyAPI::GUI::CTabcontrolItem*	m_pTabProperties;

	SallyAPI::GUI::CCheckbox*	m_pShowAlwaysHarddiscs;
	SallyAPI::GUI::CLabel*		m_pInfo;
	SallyAPI::GUI::CButton*		m_pSelectFolder[12];
	SallyAPI::GUI::CButton*		m_pEmptySelection[12];
	SallyAPI::GUI::CEdit*		m_pSelection[12];
	SallyAPI::GUI::CCheckbox*	m_pIncludeInDatabase[12];

	SallyAPI::GUI::CLabel*		m_pUpdateDBLastRunLabel;
	SallyAPI::GUI::CLabel*		m_pUpdateDBLastRunInfo;
	SallyAPI::GUI::CLabel*		m_pUpdateCoversLastRunLabel;
	SallyAPI::GUI::CLabel*		m_pUpdateCoversLastRunInfo;
	SallyAPI::GUI::CCheckbox*	m_pAutoUpdateDB;
	SallyAPI::GUI::CButton*		m_pButtonUpdateDB;
	SallyAPI::GUI::CButton*		m_pButtonUpdateDBCancel;
	SallyAPI::GUI::CProcessbar*	m_pProcessbarUpdateDB;
	SallyAPI::GUI::CCheckbox*	m_pAutoUpdateCovers;
	SallyAPI::GUI::CButton*		m_pButtonUpdateCovers;
	SallyAPI::GUI::CButton*		m_pButtonUpdateCoversCancel;
	SallyAPI::GUI::CProcessbar*	m_pProcessbarUpdateCovers;
	SallyAPI::GUI::CButton*		m_pButtonDeleteDB;
	SallyAPI::GUI::CButton*		m_pButtonResetLastBuildDB;
	CCreateNewDatabase			m_tCreateNewDatabase;
	CGetCovers					m_tGetCovers;

	SallyAPI::GUI::CCheckbox*	m_pAutostartPlay;
	SallyAPI::GUI::CCheckbox*	m_pMusicBlendInOut;
	SallyAPI::GUI::CCheckbox*	m_pFullAutomaticPlaylistHistory;
	SallyAPI::GUI::CCheckbox*	m_pPreventDuclicatesInPlaylist;

	bool CheckIfChanged();

	void OnCommandResetLastBuildDatabase();

	void OnCommandUpdateDBScheduler();
	void OnCommandUpdateCoversScheduler();
	void OnCommandUpdateDBDone();
	void OnCommandUpdateCoverDone();

	void OnCommandUpdateDB(bool showPopUp);
	void OnCommandUpdateDBCancel();
	void OnCommandUpdateCovers(bool showPopUp);
	void OnCommandGetCoversCancel();
	void OnCommandAskToDeleteDB();
	void OnCommandConfirmDeleteDB();

	void OnCommandSelectFolder(int reporterId);
	void OnCommandEmptySelection(int reporterId);
	void OnCommandFolderSelected(int reporterId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);
public:
	CMyConfigPanel(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, const std::string& pluginPath);
	virtual ~CMyConfigPanel();

	virtual void				LoadConfig();
	virtual std::vector<int>	SaveConfig();

	void	WizardDBCreate();

	virtual void SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
};
