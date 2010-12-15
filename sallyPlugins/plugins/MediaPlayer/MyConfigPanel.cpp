////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\MyConfigPanel.cpp
///
/// \brief	Implements my configuration panel class. 
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

#include "MyConfigPanel.h"

CMyConfigPanel::CMyConfigPanel(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, const std::string& pluginPath)
	:SallyAPI::GUI::CConfigPanel(parent, graphicId, pluginPath)
{
	m_pTabControl = new SallyAPI::GUI::CTabcontrol(this, WINDOW_BORDER_H, WINDOW_BORDER_V,
		this->GetWidth() - (2 * WINDOW_BORDER_H), this->GetHeight() - (2 * WINDOW_BORDER_V));
	this->AddChild(m_pTabControl);

	int height = m_pTabControl->GetFormHeight();
	int width = m_pTabControl->GetFormWidth();

	m_pTabFolders = new SallyAPI::GUI::CTabcontrolItem(m_pTabControl, "Folders", GUI_THEME_SALLY_ICON_FOLDER);
	m_pTabControl->AddTabItem(m_pTabFolders);

	m_pTabProperties = new SallyAPI::GUI::CTabcontrolItem(m_pTabControl, "Properties", GUI_THEME_SALLY_ICON_PROPERTIES);
	m_pTabControl->AddTabItem(m_pTabProperties);

	m_pTabDatabase = new SallyAPI::GUI::CTabcontrolItem(m_pTabControl, "Database", GUI_THEME_SALLY_ICON_DATABASE);
	m_pTabControl->AddTabItem(m_pTabDatabase);

	//////////////////////////////////////////////////////////////////////////

	// Folder Tab
	m_pShowAlwaysHarddiscs = new SallyAPI::GUI::CCheckbox(m_pTabFolders->GetForm(), WINDOW_BORDER_H, WINDOW_BORDER_V, 600, GUI_APP_ALWAYS_SHOW_HDS);
	m_pShowAlwaysHarddiscs->SetText("Show all hard disks");
	m_pTabFolders->GetForm()->AddChild(m_pShowAlwaysHarddiscs);

	m_pInfo = new SallyAPI::GUI::CLabel(m_pTabFolders->GetForm(), WINDOW_BORDER_H, WINDOW_BORDER_V + 40, 600);
	m_pInfo->SetText("Please select your mp3 and video folders:");
	m_pTabFolders->GetForm()->AddChild(m_pInfo);

	for (int i = 0; i < 12; ++i)
	{
		m_pSelection[i] = new SallyAPI::GUI::CEdit(m_pTabFolders->GetForm(),
			WINDOW_BORDER_H,
			WINDOW_BORDER_V + (i * (CONTROL_HEIGHT + 10)) + (WINDOW_BORDER_V + 50),
			width - 400 - 40, i);
		m_pSelection[i]->SetInfoText("Media Folder");
		m_pTabFolders->GetForm()->AddChild(m_pSelection[i]);

		m_pSelectFolder[i] = new SallyAPI::GUI::CButton(m_pTabFolders->GetForm(),
			width - 210 - 10 - CONTROL_HEIGHT - 10 - 130 - 10,
			WINDOW_BORDER_V + (i * (CONTROL_HEIGHT + 10)) + (WINDOW_BORDER_V + 50),
			130,
			CONTROL_HEIGHT,
			GUI_APP_SELECT_FOLDER + i, SallyAPI::GUI::BUTTON_TYPE_NORMAL);
		m_pSelectFolder[i]->SetText("Select");
		m_pSelectFolder[i]->SetImageId(GUI_THEME_SALLY_ICON_FOLDER);
		m_pTabFolders->GetForm()->AddChild(m_pSelectFolder[i]);

		m_pEmptySelection[i] = new SallyAPI::GUI::CButton(m_pTabFolders->GetForm(),
			width - 210 - 10 - CONTROL_HEIGHT - 10,
			WINDOW_BORDER_V + (i * (CONTROL_HEIGHT + 10)) + (WINDOW_BORDER_V + 50),
			CONTROL_HEIGHT,
			CONTROL_HEIGHT,
			GUI_APP_EMPTEY_SELECTION + i, SallyAPI::GUI::BUTTON_TYPE_NORMAL);
		m_pEmptySelection[i]->SetImageId(GUI_THEME_SALLY_ICON_DELETE);
		m_pTabFolders->GetForm()->AddChild(m_pEmptySelection[i]);

		m_pIncludeInDatabase[i] = new SallyAPI::GUI::CCheckbox(m_pTabFolders->GetForm(),
			width - 210 - 10,
			WINDOW_BORDER_V + (i * (CONTROL_HEIGHT + 10)) + (WINDOW_BORDER_V + 50),
			210, i);
		m_pIncludeInDatabase[i]->SetText("In Database");
		m_pTabFolders->GetForm()->AddChild(m_pIncludeInDatabase[i]);
	}

	// hide fileds if the screen is to small
	int folderCount = 12;
	if (WINDOW_HEIGHT < 768)
		folderCount = 8;

	for (int i = folderCount; i < 12; ++i) {
		m_pSelection[i]->Visible(false);
		m_pSelectFolder[i]->Visible(false);
		m_pEmptySelection[i]->Visible(false);
		m_pIncludeInDatabase[i]->Visible(false);
	}


	// Database Tab
	m_pButtonUpdateDB = new SallyAPI::GUI::CButton(m_pTabDatabase->GetForm(), WINDOW_BORDER_H, WINDOW_BORDER_V, 200, CONTROL_HEIGHT,
		GUI_APP_UPDATE_DB, SallyAPI::GUI::BUTTON_TYPE_NORMAL);
	m_pButtonUpdateDB->SetImageId(GUI_THEME_SALLY_ICON_DATABASE);
	m_pButtonUpdateDB->SetText("Update Database");
	m_pTabDatabase->GetForm()->AddChild(m_pButtonUpdateDB);

	m_pButtonUpdateDBCancel = new SallyAPI::GUI::CButton(m_pTabDatabase->GetForm(), WINDOW_BORDER_H, WINDOW_BORDER_V, 200, CONTROL_HEIGHT,
		GUI_APP_UPDATE_DB_CANCEL, SallyAPI::GUI::BUTTON_TYPE_NORMAL);
	m_pButtonUpdateDBCancel->SetImageId(GUI_THEME_SALLY_ICON_MEDIA_STOP);
	m_pButtonUpdateDBCancel->SetText("Cancel");
	m_pButtonUpdateDBCancel->Visible(false);
	m_pTabDatabase->GetForm()->AddChild(m_pButtonUpdateDBCancel);

	m_pUpdateDBLastRunLabel = new SallyAPI::GUI::CLabel(m_pTabDatabase->GetForm(), WINDOW_BORDER_H + 210,
		WINDOW_BORDER_V, 100);
	m_pUpdateDBLastRunLabel->SetText("Last run:");
	m_pUpdateDBLastRunLabel->SetBold(true);
	m_pTabDatabase->GetForm()->AddChild(m_pUpdateDBLastRunLabel);

	m_pUpdateDBLastRunInfo = new SallyAPI::GUI::CLabel(m_pTabDatabase->GetForm(), WINDOW_BORDER_H + 310,
		WINDOW_BORDER_V, 200);
	m_pUpdateDBLastRunInfo->SetLocalised(false);
	m_pTabDatabase->GetForm()->AddChild(m_pUpdateDBLastRunInfo);

	m_pProcessbarUpdateDB = new SallyAPI::GUI::CProcessbar(m_pTabDatabase->GetForm(), WINDOW_BORDER_H + 210,
		WINDOW_BORDER_V, width - (WINDOW_BORDER_H + WINDOW_BORDER_H + 210));
	m_pProcessbarUpdateDB->Visible(false);
	m_pTabDatabase->GetForm()->AddChild(m_pProcessbarUpdateDB);



	m_pButtonUpdateCovers = new SallyAPI::GUI::CButton(m_pTabDatabase->GetForm(), WINDOW_BORDER_H, WINDOW_BORDER_V + CONTROL_HEIGHT + 10, 200, CONTROL_HEIGHT,
		GUI_APP_GET_COVERS, SallyAPI::GUI::BUTTON_TYPE_NORMAL);
	m_pButtonUpdateCovers->SetImageId(GUI_THEME_SALLY_ICON_CD);
	m_pButtonUpdateCovers->SetText("Get Missing Covers");
	m_pTabDatabase->GetForm()->AddChild(m_pButtonUpdateCovers);

	m_pButtonUpdateCoversCancel = new SallyAPI::GUI::CButton(m_pTabDatabase->GetForm(), WINDOW_BORDER_H, WINDOW_BORDER_V + CONTROL_HEIGHT + 10, 200, CONTROL_HEIGHT,
		GUI_APP_UPDATE_COVERS_CANCEL, SallyAPI::GUI::BUTTON_TYPE_NORMAL);
	m_pButtonUpdateCoversCancel->SetImageId(GUI_THEME_SALLY_ICON_MEDIA_STOP);
	m_pButtonUpdateCoversCancel->SetText("Cancel");
	m_pButtonUpdateCoversCancel->Visible(false);
	m_pTabDatabase->GetForm()->AddChild(m_pButtonUpdateCoversCancel);

	m_pUpdateCoversLastRunLabel = new SallyAPI::GUI::CLabel(m_pTabDatabase->GetForm(), WINDOW_BORDER_H + 210,
		WINDOW_BORDER_V + CONTROL_HEIGHT + 10, 100);
	m_pUpdateCoversLastRunLabel->SetText("Last run:");
	m_pUpdateCoversLastRunLabel->SetBold(true);
	m_pTabDatabase->GetForm()->AddChild(m_pUpdateCoversLastRunLabel);

	m_pUpdateCoversLastRunInfo = new SallyAPI::GUI::CLabel(m_pTabDatabase->GetForm(), WINDOW_BORDER_H + 310,
		WINDOW_BORDER_V + CONTROL_HEIGHT + 10, 200);
	m_pUpdateCoversLastRunInfo->SetLocalised(false);
	m_pTabDatabase->GetForm()->AddChild(m_pUpdateCoversLastRunInfo);

	m_pProcessbarUpdateCovers = new SallyAPI::GUI::CProcessbar(m_pTabDatabase->GetForm(), WINDOW_BORDER_H + 210,
		WINDOW_BORDER_V + CONTROL_HEIGHT + 10, width - (WINDOW_BORDER_H + WINDOW_BORDER_H + 210));
	m_pProcessbarUpdateCovers->Visible(false);
	m_pTabDatabase->GetForm()->AddChild(m_pProcessbarUpdateCovers);



	m_pButtonResetLastBuildDB = new SallyAPI::GUI::CButton(m_pTabDatabase->GetForm(), WINDOW_BORDER_H,
		WINDOW_BORDER_V + CONTROL_HEIGHT + 10 + CONTROL_HEIGHT + 10 + CONTROL_HEIGHT + 10, 200, CONTROL_HEIGHT,
		GUI_APP_RESET_LAST_BUILD_DATABASE, SallyAPI::GUI::BUTTON_TYPE_NORMAL);
	m_pButtonResetLastBuildDB->SetImageId(GUI_THEME_SALLY_ICON_RELOAD);
	m_pButtonResetLastBuildDB->SetText("Rebuild Database");
	m_pTabDatabase->GetForm()->AddChild(m_pButtonResetLastBuildDB);

	m_pButtonDeleteDB = new SallyAPI::GUI::CButton(m_pTabDatabase->GetForm(), WINDOW_BORDER_H + 210,
		WINDOW_BORDER_V + CONTROL_HEIGHT + 10 + CONTROL_HEIGHT + 10 + CONTROL_HEIGHT + 10, 200, CONTROL_HEIGHT,
		GUI_APP_DELETE_DATABASE, SallyAPI::GUI::BUTTON_TYPE_NORMAL);
	m_pButtonDeleteDB->SetImageId(GUI_THEME_SALLY_ICON_DELETE);
	m_pButtonDeleteDB->SetText("Clear Database");
	m_pTabDatabase->GetForm()->AddChild(m_pButtonDeleteDB);

	// Properties Tab
	m_pAutostartPlay = new SallyAPI::GUI::CCheckbox(m_pTabProperties->GetForm(), WINDOW_BORDER_H, WINDOW_BORDER_V, 400,
		GUI_APP_AUTOSTART_PLAY);
	m_pAutostartPlay->SetText("Start Play on Sally startup");
	m_pTabProperties->GetForm()->AddChild(m_pAutostartPlay);

	m_pMusicBlendInOut = new SallyAPI::GUI::CCheckbox(m_pTabProperties->GetForm(), WINDOW_BORDER_H, WINDOW_BORDER_V + CONTROL_HEIGHT + 10, 400,
		GUI_APP_MUSIC_BLEND_IN_OUT);
	m_pMusicBlendInOut->SetText("Music blend in / out");
	m_pTabProperties->GetForm()->AddChild(m_pMusicBlendInOut);

	m_pFullAutomaticPlaylistHistory = new SallyAPI::GUI::CCheckbox(m_pTabProperties->GetForm(), WINDOW_BORDER_H, WINDOW_BORDER_V + CONTROL_HEIGHT + 10 + CONTROL_HEIGHT + 10, 400,
		GUI_APP_FULL_AUTOMATIC_PLAYLIST_HISTORY);
	m_pFullAutomaticPlaylistHistory->SetText("Full automatic playlist history");
	m_pTabProperties->GetForm()->AddChild(m_pFullAutomaticPlaylistHistory);

	m_pPreventDuclicatesInPlaylist = new SallyAPI::GUI::CCheckbox(m_pTabProperties->GetForm(), WINDOW_BORDER_H, WINDOW_BORDER_V + CONTROL_HEIGHT + 10 + CONTROL_HEIGHT + 10 + CONTROL_HEIGHT + 10, 400,
		GUI_APP_FULL_PREVENT_DUPLICATES);
	m_pPreventDuclicatesInPlaylist->SetText("Prevent duplicates in playlist");
	m_pTabProperties->GetForm()->AddChild(m_pPreventDuclicatesInPlaylist);

	// Scheduler
	SallyAPI::Scheduler::CSchedulerManager* schedulerManger = SallyAPI::Scheduler::CSchedulerManager::GetInstance();

	SallyAPI::Scheduler::CScheduler scheduler1(this, 0, GUI_APP_SCHEDULER_CREATE_DB, "dbcreator", 10);
	schedulerManger->AddScheduler(scheduler1);

	SallyAPI::Scheduler::CScheduler scheduler2(this, 0, GUI_APP_SCHEDULER_DOWNLOAD_COVERS, "downloadcovers", 10);
	schedulerManger->AddScheduler(scheduler2);
}

CMyConfigPanel::~CMyConfigPanel(void)
{
	m_tCreateNewDatabase.WaitForStop();
	m_tGetCovers.WaitForStop();
}

void CMyConfigPanel::OnCommandUpdateDBDone()
{
	m_pButtonDeleteDB->Enable(true);
	m_pButtonResetLastBuildDB->Enable(true);

	m_pButtonUpdateDB->Visible(true);
	m_pButtonUpdateDBCancel->Visible(false);
	m_pProcessbarUpdateDB->Visible(false);
	m_pUpdateDBLastRunInfo->Visible(true);
	m_pUpdateDBLastRunLabel->Visible(true);

	m_pButtonUpdateCovers->Enable(true);

	m_pParent->SendMessageToParent(this, GetGraphicId(), MS_SALLY_APP_CONFIG_CHANGED);

	// Scheduler
	SallyAPI::Scheduler::CSchedulerManager* schedulerManger = SallyAPI::Scheduler::CSchedulerManager::GetInstance();
	schedulerManger->SchedulerFinished(this, "dbcreator");

	m_pUpdateDBLastRunInfo->SetText(schedulerManger->GetLastSchedulerRunAsString(this, "dbcreator"));

	m_pParent->SendMessageToParent(this, GetGraphicId(), GUI_APP_MENU_DBUPDATE_FINISHED);
}

void CMyConfigPanel::OnCommandUpdateDBCancel()
{
	m_tCreateNewDatabase.Stop();

	m_pButtonDeleteDB->Enable(true);
	m_pButtonResetLastBuildDB->Enable(true);

	m_pButtonUpdateDB->Visible(true);
	m_pButtonUpdateDBCancel->Visible(false);
	m_pProcessbarUpdateDB->Visible(false);
	m_pUpdateDBLastRunInfo->Visible(true);
	m_pUpdateDBLastRunLabel->Visible(true);

	m_pButtonUpdateCovers->Enable(true);

	// Scheduler
	SallyAPI::Scheduler::CSchedulerManager* schedulerManger = SallyAPI::Scheduler::CSchedulerManager::GetInstance();
	schedulerManger->SchedulerCanceled(this, "dbcreator");

	m_pParent->SendMessageToParent(this, GetGraphicId(), GUI_APP_MENU_DBUPDATE_FINISHED);
}

void CMyConfigPanel::OnCommandUpdateCoverDone()
{
	m_pButtonDeleteDB->Enable(true);
	m_pButtonResetLastBuildDB->Enable(true);

	m_pButtonUpdateCovers->Visible(true);
	m_pButtonUpdateCoversCancel->Visible(false);
	m_pProcessbarUpdateCovers->Visible(false);
	m_pUpdateCoversLastRunInfo->Visible(true);
	m_pUpdateCoversLastRunLabel->Visible(true);

	m_pButtonUpdateDB->Enable(true);

	m_pParent->SendMessageToParent(this, GetGraphicId(), MS_SALLY_APP_CONFIG_CHANGED);

	// Scheduler
	SallyAPI::Scheduler::CSchedulerManager* schedulerManger = SallyAPI::Scheduler::CSchedulerManager::GetInstance();
	schedulerManger->SchedulerFinished(this, "downloadcovers");

	m_pUpdateCoversLastRunInfo->SetText(schedulerManger->GetLastSchedulerRunAsString(this, "downloadcovers"));

	m_pParent->SendMessageToParent(this, GetGraphicId(), GUI_APP_MENU_DBUPDATE_FINISHED);
}

void CMyConfigPanel::OnCommandGetCoversCancel()
{
	m_tGetCovers.Stop();

	m_pButtonDeleteDB->Enable(true);
	m_pButtonResetLastBuildDB->Enable(true);

	m_pButtonUpdateCovers->Visible(true);
	m_pButtonUpdateCoversCancel->Visible(false);
	m_pProcessbarUpdateCovers->Visible(false);
	m_pUpdateCoversLastRunInfo->Visible(true);
	m_pUpdateCoversLastRunLabel->Visible(true);

	m_pButtonUpdateDB->Enable(true);

	// Scheduler
	SallyAPI::Scheduler::CSchedulerManager* schedulerManger = SallyAPI::Scheduler::CSchedulerManager::GetInstance();
	schedulerManger->SchedulerCanceled(this, "downloadcovers");

	m_pParent->SendMessageToParent(this, GetGraphicId(), GUI_APP_MENU_DBUPDATE_FINISHED);
}

void CMyConfigPanel::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_APP_UPDATE_DB_DONE:
		OnCommandUpdateDBDone();
		return;
	case GUI_APP_GET_COVERS_DONE:
		OnCommandUpdateCoverDone();
		return;
	}
	
	switch (messageId)
	{
	case GUI_APP_SCHEDULER_CREATE_DB:
		OnCommandUpdateDBScheduler();
		return;
	case GUI_APP_SCHEDULER_DOWNLOAD_COVERS:
		OnCommandUpdateCoversScheduler();
		return;
	case MS_DIALOG_YES:
		switch (reporterId)
		{
		case GUI_APP_DELETE_DATABASE:
			OnCommandConfirmDeleteDB();
			return;
		}
		break;
	case MS_SALLY_OK_OPEN_FILE:
		if ((reporterId >= GUI_APP_SELECT_FOLDER) && (reporterId <= GUI_APP_SELECT_FOLDER + 12))
		{
			OnCommandFolderSelected(reporterId, messageParameter);
			return;
		}
		break;
	case GUI_EDIT_CHANGED:
		if (m_pSelection[reporterId]->GetText().length() > 0)
		{
			m_pEmptySelection[reporterId]->Visible(true);
			m_pIncludeInDatabase[reporterId]->Visible(true);
		}
		else
		{
			m_pEmptySelection[reporterId]->Visible(false);
			m_pIncludeInDatabase[reporterId]->Visible(false);
		}
		return;
	case GUI_BUTTON_CLICKED:
		switch (reporterId)
		{
		case GUI_APP_UPDATE_DB_CANCEL:
			OnCommandUpdateDBCancel();
			return;
		case GUI_APP_UPDATE_COVERS_CANCEL:
			OnCommandGetCoversCancel();
			return;
		case GUI_APP_DELETE_DATABASE:
			OnCommandAskToDeleteDB();
			return;
		case GUI_APP_RESET_LAST_BUILD_DATABASE:
			OnCommandResetLastBuildDatabase();
			return;
		case GUI_APP_UPDATE_DB:
			OnCommandUpdateDB(true);
			return;
		case GUI_APP_GET_COVERS:
			OnCommandUpdateCovers(true);
			return;
		}
		if ((reporterId >= GUI_APP_EMPTEY_SELECTION) && (reporterId <= GUI_APP_EMPTEY_SELECTION + 12))
		{
			OnCommandEmptySelection(reporterId);
			return;
		}
		if ((reporterId >= GUI_APP_SELECT_FOLDER) && (reporterId <= GUI_APP_SELECT_FOLDER + 12))
		{
			OnCommandSelectFolder(reporterId);
			return;
		}
		break;
	}
	SallyAPI::GUI::CConfigPanel::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

void CMyConfigPanel::OnCommandResetLastBuildDatabase()
{
	// Scheduler
	SallyAPI::Scheduler::CSchedulerManager* schedulerManger = SallyAPI::Scheduler::CSchedulerManager::GetInstance();
	schedulerManger->ResetScheduler(this, "dbcreator");

	m_pUpdateDBLastRunInfo->SetText(schedulerManger->GetLastSchedulerRunAsString(this, "dbcreator"));

	SendMessageToParent(m_pButtonUpdateDB, GUI_APP_UPDATE_DB, GUI_BUTTON_CLICKED);
}

void CMyConfigPanel::OnCommandFolderSelected(int reporterId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	// now load the new stuff
	SallyAPI::GUI::SendMessage::CParameterString* parameterString = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterString*> (messageParameter);

	m_pSelection[reporterId - GUI_APP_SELECT_FOLDER]->SetText(parameterString->GetString());

	m_pEmptySelection[reporterId - GUI_APP_SELECT_FOLDER]->Visible(true);
	m_pIncludeInDatabase[reporterId - GUI_APP_SELECT_FOLDER]->Visible(true);
}


void CMyConfigPanel::OnCommandAskToDeleteDB()
{
	SallyAPI::GUI::SendMessage::CParameterQuestionBox questionBox(GUI_APP_DELETE_DATABASE, this, "All your database statistics will be deleted.\nAre you sure you want to delete your media database?");
	m_pParent->SendMessageToParent(this, m_iControlId, MS_SALLY_SHOW_QUESTIONBOX, &questionBox);
}

void CMyConfigPanel::OnCommandConfirmDeleteDB()
{
	std::string mediaDirectory = SallyAPI::System::SallyHelper::GetMediaDirectory(this);
	mediaDirectory.append("media.db");

	bool continueDelete;
	do
	{
		continueDelete = true;
		DeleteFile(mediaDirectory.c_str());
		int error = GetLastError();
		if ((error != ERROR_ACCESS_DENIED) && (error != ERROR_SHARING_VIOLATION))
			continueDelete = false;
	}
	while(continueDelete);

	// Scheduler
	SallyAPI::Scheduler::CSchedulerManager* schedulerManger = SallyAPI::Scheduler::CSchedulerManager::GetInstance();
	schedulerManger->ResetScheduler(this, "dbcreator");
	schedulerManger->ResetScheduler(this, "downloadcovers");

	m_pUpdateDBLastRunInfo->SetText(schedulerManger->GetLastSchedulerRunAsString(this, "dbcreator"));
	m_pUpdateCoversLastRunInfo->SetText(schedulerManger->GetLastSchedulerRunAsString(this, "downloadcovers"));
}

void CMyConfigPanel::OnCommandSelectFolder(int reporterId)
{
	SallyAPI::GUI::GUIHelper::SelectFolder(GetGraphicId(), reporterId, GUI_APP_SELECT_THIS_FOLDER, this, "Please select a folder:");
}

void CMyConfigPanel::OnCommandEmptySelection(int reporterId)
{
	int id = reporterId - GUI_APP_EMPTEY_SELECTION;
	m_pSelection[id]->SetText("");
	m_pIncludeInDatabase[id]->SetCheckStatus(false);
	m_pEmptySelection[id]->Visible(false);
	m_pIncludeInDatabase[id]->Visible(false);
}

void CMyConfigPanel::LoadConfig()
{
	m_pMusicBlendInOut->SetCheckStatus(GetPropertyBool("musicBlendInOut", false));
	m_pAutostartPlay->SetCheckStatus(GetPropertyBool("autoplayOnStartup", false));
	m_pFullAutomaticPlaylistHistory->SetCheckStatus(GetPropertyBool("fullautomaticplaylisthistory", true));
	m_pPreventDuclicatesInPlaylist->SetCheckStatus(GetPropertyBool("preventduplicatesinplaylist", false));
	m_pShowAlwaysHarddiscs->SetCheckStatus(GetPropertyBool("alwaysShowHds", true));

	SallyAPI::Scheduler::CSchedulerManager* schedulerManger = SallyAPI::Scheduler::CSchedulerManager::GetInstance();
	m_pUpdateDBLastRunInfo->SetText(schedulerManger->GetLastSchedulerRunAsString(this, "dbcreator"));
	m_pUpdateCoversLastRunInfo->SetText(schedulerManger->GetLastSchedulerRunAsString(this, "downloadcovers"));

	for (int i = 0; i < 12; i++)
	{
		std::string dir;

		dir.append("dir");
		dir.append(SallyAPI::String::StringHelper::ConvertToString(i));
		dir.append(".path");

		std::string dirTemp = GetPropertyString(dir);

		m_pSelection[i]->SetText(dirTemp);


		std::string includeInDatabase;

		includeInDatabase.append("dir");
		includeInDatabase.append(SallyAPI::String::StringHelper::ConvertToString(i));
		includeInDatabase.append(".db");

		int includeInDatabaseTemp = GetPropertyInt(includeInDatabase);

		m_pIncludeInDatabase[i]->SetCheckStatus((includeInDatabaseTemp) ? true : false);

		if (dirTemp.length() == 0)
		{
			m_pEmptySelection[i]->Visible(false);
			m_pIncludeInDatabase[i]->Visible(false);
		}
	}
}

bool CMyConfigPanel::CheckIfChanged()
{
	for (int i = 0; i < 12; i++)
	{
		std::string dirValue = m_pSelection[i]->GetText();
		bool includeInDatabaseValue = m_pIncludeInDatabase[i]->GetCheckStatus();

		std::string dir;
		std::string includeInDatabase;

		dir.append("dir");
		dir.append(SallyAPI::String::StringHelper::ConvertToString(i));
		dir.append(".path");

		includeInDatabase.append("dir");
		includeInDatabase.append(SallyAPI::String::StringHelper::ConvertToString(i));
		includeInDatabase.append(".db");

		std::string oldDir = GetPropertyString(dir, "");
		bool oldIncludeInDatabaseValue = GetPropertyBool(includeInDatabase, false);

		if ((oldDir.compare(dirValue) != 0) || (includeInDatabaseValue != oldIncludeInDatabaseValue))
		{
			return true;
		}
	}
	return false;
}

void CMyConfigPanel::SaveConfig()
{
	bool changed = CheckIfChanged();

	SetPropertyBool("autoplayOnStartup", m_pAutostartPlay->GetCheckStatus());
	SetPropertyBool("musicBlendInOut", m_pMusicBlendInOut->GetCheckStatus());
	SetPropertyBool("fullautomaticplaylisthistory", m_pFullAutomaticPlaylistHistory->GetCheckStatus());
	SetPropertyBool("preventduplicatesinplaylist", m_pPreventDuclicatesInPlaylist->GetCheckStatus());
	SetPropertyBool("alwaysShowHds", m_pShowAlwaysHarddiscs->GetCheckStatus());

	m_pMusicBlendInOut->SetCheckStatus(GetPropertyBool("musicBlendInOut", false));

	for (int i = 0; i < 12; i++)
	{
		std::string dirValue = m_pSelection[i]->GetText();
		bool includeInDatabaseValue = m_pIncludeInDatabase[i]->GetCheckStatus();

		std::string dir;
		std::string includeInDatabase;

		dir.append("dir");
		dir.append(SallyAPI::String::StringHelper::ConvertToString(i));
		dir.append(".path");

		includeInDatabase.append("dir");
		includeInDatabase.append(SallyAPI::String::StringHelper::ConvertToString(i));
		includeInDatabase.append(".db");

		SetPropertyInt(includeInDatabase, includeInDatabaseValue);
		SetPropertyString(dir, dirValue);
	}

	if (changed)
	{
		// Scheduler
		SallyAPI::Scheduler::CSchedulerManager* schedulerManger = SallyAPI::Scheduler::CSchedulerManager::GetInstance();
		schedulerManger->ResetScheduler(this, "dbcreator");
	}
}

void CMyConfigPanel::OnCommandUpdateCovers(bool showPopUp)
{
	SaveConfig();

	// scheduler
	SallyAPI::Scheduler::CSchedulerManager* schedulerManger = SallyAPI::Scheduler::CSchedulerManager::GetInstance();
	schedulerManger->ExecuteScheduler(this, "downloadcovers");

	if (showPopUp)
	{
		SallyAPI::GUI::SendMessage::CParameterMessageBox sendMessageParameterMessageBox(0, this, "The downloading of the CD Covers can take some minutes.\nBut you can continue using Sally.", SallyAPI::GUI::MESSAGEBOX_ICON_INFO);
		m_pParent->SendMessageToParent(this, m_iControlId, MS_SALLY_SHOW_MESSAGEBOX, &sendMessageParameterMessageBox);
	}
}

void CMyConfigPanel::OnCommandUpdateCoversScheduler()
{
	// start only if the create database thread is not running
	if (m_tCreateNewDatabase.GetStatus() == SallyAPI::System::THREAD_RUNNING)
	{
		// Scheduler
		SallyAPI::Scheduler::CSchedulerManager* schedulerManger = SallyAPI::Scheduler::CSchedulerManager::GetInstance();
		schedulerManger->SchedulerCanceled(this, "downloadcovers");
		return;
	}

	m_pButtonDeleteDB->Enable(false);
	m_pButtonResetLastBuildDB->Enable(false);

	m_pButtonUpdateCovers->Visible(false);
	m_pButtonUpdateCoversCancel->Visible(true);
	m_pProcessbarUpdateCovers->Visible(true);
	m_pUpdateCoversLastRunInfo->Visible(false);
	m_pUpdateCoversLastRunLabel->Visible(false);

	m_pButtonUpdateDB->Enable(false);

	m_pProcessbarUpdateCovers->SetPosition(0);

	m_tGetCovers.SetValues(GUI_APP_GET_COVERS_DONE, this, m_pProcessbarUpdateCovers);
	m_tGetCovers.Start();

	m_pParent->SendMessageToParent(this, GetGraphicId(), GUI_APP_MENU_DBUPDATE_STARTED);
}

void CMyConfigPanel::OnCommandUpdateDB(bool showPopUp)
{
	SaveConfig();

	// scheduler
	SallyAPI::Scheduler::CSchedulerManager* schedulerManger = SallyAPI::Scheduler::CSchedulerManager::GetInstance();
	schedulerManger->ExecuteScheduler(this, "dbcreator");

	if (showPopUp)
	{
		SallyAPI::GUI::SendMessage::CParameterMessageBox sendMessageParameterMessageBox(0, this, "The update of the database can take up to one hour.\nBut you can continue using Sally.", SallyAPI::GUI::MESSAGEBOX_ICON_INFO);
		m_pParent->SendMessageToParent(this, m_iControlId, MS_SALLY_SHOW_MESSAGEBOX, &sendMessageParameterMessageBox);
	}
}

void CMyConfigPanel::OnCommandUpdateDBScheduler()
{
	// start only if the cover dowonload thread is not running
	if (m_tGetCovers.GetStatus() == SallyAPI::System::THREAD_RUNNING)
	{
		// Scheduler
		SallyAPI::Scheduler::CSchedulerManager* schedulerManger = SallyAPI::Scheduler::CSchedulerManager::GetInstance();
		schedulerManger->SchedulerCanceled(this, "dbcreator");
		return;
	}

	m_pButtonDeleteDB->Enable(false);
	m_pButtonResetLastBuildDB->Enable(false);

	m_pButtonUpdateDB->Visible(false);
	m_pButtonUpdateDBCancel->Visible(true);
	m_pProcessbarUpdateDB->Visible(true);
	m_pUpdateDBLastRunInfo->Visible(false);
	m_pUpdateDBLastRunLabel->Visible(false);

	m_pButtonUpdateCovers->Enable(false);

	std::vector<std::string> folders;

	for (int i = 0; i < 12; i++)
	{
		std::string dirDB;

		dirDB.append("dir");
		dirDB.append(SallyAPI::String::StringHelper::ConvertToString(i));
		dirDB.append(".db");

		int db = GetPropertyInt(dirDB);

		if (db)
		{
			std::string dirPath;

			dirPath.append("dir");
			dirPath.append(SallyAPI::String::StringHelper::ConvertToString(i));
			dirPath.append(".path");

			std::string path = GetPropertyString(dirPath);

			if (path.length() > 0)
				folders.push_back(path);
		}
	}

	m_pProcessbarUpdateDB->SetPosition(0);

	std::string mediaDirectory = SallyAPI::System::SallyHelper::GetMediaDirectory(this);
	m_tCreateNewDatabase.SetValues(folders, mediaDirectory, GUI_APP_UPDATE_DB_DONE, this, m_pProcessbarUpdateDB);

	m_tCreateNewDatabase.Start();

	m_pParent->SendMessageToParent(this, GetGraphicId(), GUI_APP_MENU_DBUPDATE_STARTED);
}

void CMyConfigPanel::WizardDBCreate()
{
	m_tCreateNewDatabase.WaitForStop();
	m_tGetCovers.WaitForStop();

	OnCommandUpdateDB(false);
}