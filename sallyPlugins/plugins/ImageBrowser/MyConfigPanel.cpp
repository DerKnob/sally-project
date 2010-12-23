////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	ImageBrowser\MyConfigPanel.cpp
///
/// \brief	Implements my configuration panel class. 
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

	m_pTabScreensaver = new SallyAPI::GUI::CTabcontrolItem(m_pTabControl, "Screensaver", GUI_THEME_SALLY_SCREENSAVER);
	m_pTabControl->AddTabItem(m_pTabScreensaver);

	m_pTabDatabase = new SallyAPI::GUI::CTabcontrolItem(m_pTabControl, "Database", GUI_THEME_SALLY_ICON_DATABASE);
	m_pTabControl->AddTabItem(m_pTabDatabase);

	//////////////////////////////////////////////////////////////////////////

	// Folder Tab
	m_pShowAlwaysHarddiscs = new SallyAPI::GUI::CCheckbox(m_pTabFolders->GetForm(), WINDOW_BORDER_H, WINDOW_BORDER_V, 600, GUI_APP_ALWAYS_SHOW_HDS);
	m_pShowAlwaysHarddiscs->SetText("Show all hard disks");
	m_pTabFolders->GetForm()->AddChild(m_pShowAlwaysHarddiscs);

	m_pInfo = new SallyAPI::GUI::CLabel(m_pTabFolders->GetForm(), WINDOW_BORDER_H, WINDOW_BORDER_V + 40, 600);
	m_pInfo->SetText("Please select your picture folders:");
	m_pTabFolders->GetForm()->AddChild(m_pInfo);

	for (int i = 0; i < 12; ++i)
	{
		m_pSelection[i] = new SallyAPI::GUI::CEdit(m_pTabFolders->GetForm(),
			WINDOW_BORDER_H,
			WINDOW_BORDER_V + (i * (CONTROL_HEIGHT + 10)) + (WINDOW_BORDER_V + 50),
			width - 400 - 40, i);
		m_pSelection[i]->SetInfoText("Image Folder");
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

		m_pIncludeInScreensaver[i] = new SallyAPI::GUI::CCheckbox(m_pTabFolders->GetForm(),
			width - 210 - 10,
			WINDOW_BORDER_V + (i * (CONTROL_HEIGHT + 10)) + (WINDOW_BORDER_V + 50),
			210, i);
		m_pIncludeInScreensaver[i]->SetText("Include in Screensaver");
		m_pTabFolders->GetForm()->AddChild(m_pIncludeInScreensaver[i]);
	}

	// hide fileds if the screen is to small
	int folderCount = 12;
	if (WINDOW_HEIGHT < 768)
		folderCount = 8;

	for (int i = folderCount; i < 12; ++i) {
		m_pSelection[i]->Visible(false);
		m_pSelectFolder[i]->Visible(false);
		m_pEmptySelection[i]->Visible(false);
		m_pIncludeInScreensaver[i]->Visible(false);
	}

	// Screensaver Tab
	m_pLabel[0] = new SallyAPI::GUI::CLabel(m_pTabScreensaver->GetForm(), WINDOW_BORDER_H, WINDOW_BORDER_V, 180);
	m_pLabel[0]->SetText("Slideshow Timer:");
	m_pTabScreensaver->GetForm()->AddChild(m_pLabel[0]);

	m_pTimerScreensaver = new SallyAPI::GUI::CEdit(m_pTabScreensaver->GetForm(), WINDOW_BORDER_H + 190, WINDOW_BORDER_V, 50);
	m_pTimerScreensaver->SetLocalised(false);
	m_pTimerScreensaver->SetNumberOnly(true);
	m_pTabScreensaver->GetForm()->AddChild(m_pTimerScreensaver);

	m_pLabel[1] = new SallyAPI::GUI::CLabel(m_pTabScreensaver->GetForm(), WINDOW_BORDER_H + 250, WINDOW_BORDER_V, 50);
	m_pLabel[1]->SetText("sec.");
	m_pTabScreensaver->GetForm()->AddChild(m_pLabel[1]);

	m_pTimerShuffle = new SallyAPI::GUI::CCheckbox(m_pTabScreensaver->GetForm(), WINDOW_BORDER_H, WINDOW_BORDER_V + CONTROL_HEIGHT + 10, 250);
	m_pTimerShuffle->SetText("Screensaver Shuffle");
	m_pTabScreensaver->GetForm()->AddChild(m_pTimerShuffle);

	m_pDropDownChangeType = new SallyAPI::GUI::CDropDown(m_pTabScreensaver->GetForm(), WINDOW_BORDER_H, WINDOW_BORDER_V + (CONTROL_HEIGHT + 10) * 2, 120);
	m_pTabScreensaver->GetForm()->AddChild(m_pDropDownChangeType);

	SallyAPI::GUI::CDropDownItem itemSlideIn("SlideIn", "Slide In");
	m_pDropDownChangeType->AddItem(itemSlideIn);

	SallyAPI::GUI::CDropDownItem itemSlideZoom("Zoom", "Zoom");
	m_pDropDownChangeType->AddItem(itemSlideZoom);

// 	SallyAPI::GUI::CDropDownItem itemSlideCollage("Collage", "Collage");
// 	m_pDropDownChangeType->AddItem(itemSlideCollage);

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



	m_pButtonResetLastBuildDB = new SallyAPI::GUI::CButton(m_pTabDatabase->GetForm(), WINDOW_BORDER_H,
		WINDOW_BORDER_V + CONTROL_HEIGHT + 10 + CONTROL_HEIGHT + 10, 200, CONTROL_HEIGHT,
		GUI_APP_RESET_LAST_BUILD_DATABASE, SallyAPI::GUI::BUTTON_TYPE_NORMAL);
	m_pButtonResetLastBuildDB->SetImageId(GUI_THEME_SALLY_ICON_RELOAD);
	m_pButtonResetLastBuildDB->SetText("Rebuild Database");
	m_pTabDatabase->GetForm()->AddChild(m_pButtonResetLastBuildDB);

	m_pButtonDeleteDB = new SallyAPI::GUI::CButton(m_pTabDatabase->GetForm(), WINDOW_BORDER_H + 210,
		WINDOW_BORDER_V + CONTROL_HEIGHT + 10 + CONTROL_HEIGHT + 10, 200, CONTROL_HEIGHT,
		GUI_APP_DELETE_DATABASE, SallyAPI::GUI::BUTTON_TYPE_NORMAL);
	m_pButtonDeleteDB->SetImageId(GUI_THEME_SALLY_ICON_DELETE);
	m_pButtonDeleteDB->SetText("Clear Database");
	m_pTabDatabase->GetForm()->AddChild(m_pButtonDeleteDB);

	// Scheduler
	SallyAPI::Scheduler::CSchedulerManager* schedulerManager = SallyAPI::Scheduler::CSchedulerManager::GetInstance();

	SallyAPI::Scheduler::CScheduler scheduler(this, 0, GUI_APP_SCHEDULER_CREATE_DB, "dbcreator", 10);
	schedulerManager->AddScheduler(scheduler);
}

CMyConfigPanel::~CMyConfigPanel()
{
	m_tCreateNewDatabase.WaitForStop();
}

void CMyConfigPanel::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_APP_UPDATE_DB_DONE:
		OnCommandUpdateDBDone();
		return;
	case GUI_APP_SCHEDULER_CREATE_DB:
		OnCommandUpdateDBScheduler();
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
			m_pIncludeInScreensaver[reporterId]->Visible(true);
		}
		else
		{
			m_pEmptySelection[reporterId]->Visible(false);
			m_pIncludeInScreensaver[reporterId]->Visible(false);
		}
		return;
	case GUI_BUTTON_CLICKED:
		switch (reporterId)
		{
		case GUI_APP_UPDATE_DB_CANCEL:
			OnCommandUpdateDBCancel();
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
	SallyAPI::Scheduler::CSchedulerManager* schedulerManager = SallyAPI::Scheduler::CSchedulerManager::GetInstance();
	schedulerManager->ResetScheduler(this, "dbcreator");

	m_pUpdateDBLastRunInfo->SetText(schedulerManager->GetLastSchedulerRunAsString(this, "dbcreator"));

	SendMessageToParent(m_pButtonUpdateDB, GUI_APP_UPDATE_DB, GUI_BUTTON_CLICKED);
}

void CMyConfigPanel::OnCommandFolderSelected(int reporterId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	// now load the new stuff
	SallyAPI::GUI::SendMessage::CParameterString* parameterString = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterString*> (messageParameter);

	m_pSelection[reporterId - GUI_APP_SELECT_FOLDER]->SetText(parameterString->GetString());

	m_pEmptySelection[reporterId - GUI_APP_SELECT_FOLDER]->Visible(true);
	m_pIncludeInScreensaver[reporterId - GUI_APP_SELECT_FOLDER]->Visible(true);
}

void CMyConfigPanel::OnCommandSelectFolder(int reporterId)
{
	SallyAPI::GUI::GUIHelper::SelectFolder(GetGraphicId(), reporterId, GUI_APP_SELECT_THIS_FOLDER, this, "Please select a folder:");
}

void CMyConfigPanel::OnCommandEmptySelection(int reporterId)
{
	int id = reporterId - GUI_APP_EMPTEY_SELECTION;

	m_pSelection[reporterId - GUI_APP_EMPTEY_SELECTION]->SetText("");
	m_pIncludeInScreensaver[reporterId - GUI_APP_EMPTEY_SELECTION]->SetCheckStatus(false);
	m_pEmptySelection[reporterId - GUI_APP_EMPTEY_SELECTION]->Visible(false);
	m_pIncludeInScreensaver[reporterId - GUI_APP_EMPTEY_SELECTION]->Visible(false);
}

void CMyConfigPanel::LoadConfig()
{
	int screensaverTimeout = GetPropertyInt("screensaverTimeout", 10);
	m_pTimerScreensaver->SetText(SallyAPI::String::StringHelper::ConvertToString(screensaverTimeout));

	m_pTimerShuffle->SetCheckStatus(GetPropertyBool("screensaverShuffle", true));

	std::string type = GetPropertyString("screensaverType", "SlideIn");
	m_pDropDownChangeType->SelectItemByIdentifier(type);

	m_pShowAlwaysHarddiscs->SetCheckStatus(GetPropertyBool("alwaysShowHds", true));

	// Scheduler
	SallyAPI::Scheduler::CSchedulerManager* schedulerManager = SallyAPI::Scheduler::CSchedulerManager::GetInstance();
	m_pUpdateDBLastRunInfo->SetText(schedulerManager->GetLastSchedulerRunAsString(this, "dbcreator"));

	for (int i = 0; i < 12; i++)
	{
		std::string dir;

		dir.append("dir");
		dir.append(SallyAPI::String::StringHelper::ConvertToString(i));
		dir.append(".path");

		std::string dirTemp = GetPropertyString(dir);

		m_pSelection[i]->SetText(dirTemp);


		std::string defaultscreensaver;

		defaultscreensaver.append("dir");
		defaultscreensaver.append(SallyAPI::String::StringHelper::ConvertToString(i));
		defaultscreensaver.append(".defaultscreensaver");

		int defaultscreensaverTemp = GetPropertyInt(defaultscreensaver);

		m_pIncludeInScreensaver[i]->SetCheckStatus((defaultscreensaverTemp) ? true : false);

		if (dirTemp.length() == 0)
		{
			m_pEmptySelection[i]->Visible(false);
			m_pIncludeInScreensaver[i]->Visible(false);
		}
	}
}

void CMyConfigPanel::SaveConfig()
{
	bool changed = CheckIfChanged();

	std::string screensaverTimeout = m_pTimerScreensaver->GetText();
	SetPropertyString("screensaverTimeout", screensaverTimeout);

	SetPropertyBool("screensaverShuffle", m_pTimerShuffle->GetCheckStatus());

	SetPropertyBool("alwaysShowHds", m_pShowAlwaysHarddiscs->GetCheckStatus());

	SetPropertyString("screensaverType", m_pDropDownChangeType->GetSelectedIdentifier());

	for (int i = 0; i < 12; i++)
	{
		std::string text = m_pSelection[i]->GetText();

		std::string dir;

		dir.append("dir");
		dir.append(SallyAPI::String::StringHelper::ConvertToString(i));
		dir.append(".path");

		SetPropertyString(dir, text);


		bool defaultscreensaverTemp = m_pIncludeInScreensaver[i]->GetCheckStatus();

		std::string defaultscreensaver;

		defaultscreensaver.append("dir");
		defaultscreensaver.append(SallyAPI::String::StringHelper::ConvertToString(i));
		defaultscreensaver.append(".defaultscreensaver");

		SetPropertyInt(defaultscreensaver, defaultscreensaverTemp);
	}

	if (changed)
	{
		// Scheduler
		SallyAPI::Scheduler::CSchedulerManager* schedulerManager = SallyAPI::Scheduler::CSchedulerManager::GetInstance();
		schedulerManager->ResetScheduler(this, "dbcreator");
	}
}

bool CMyConfigPanel::CheckIfChanged()
{
	for (int i = 0; i < 12; i++)
	{
		std::string dirValue = m_pSelection[i]->GetText();

		std::string dir;

		dir.append("dir");
		dir.append(SallyAPI::String::StringHelper::ConvertToString(i));
		dir.append(".path");

		std::string oldDir = GetPropertyString(dir, "");

		if (oldDir.compare(dirValue) != 0)
		{
			return true;
		}
	}
	return false;
}

void CMyConfigPanel::OnCommandAskToDeleteDB()
{
	SallyAPI::GUI::SendMessage::CParameterQuestionBox questionBox(GUI_APP_DELETE_DATABASE, this, "All your database statistics will be deleted.\nAre you sure you want to delete your image database?");
	m_pParent->SendMessageToParent(this, m_iControlId, MS_SALLY_SHOW_QUESTIONBOX, &questionBox);
}

void CMyConfigPanel::OnCommandConfirmDeleteDB()
{
	std::string mediaDirectory = SallyAPI::System::SallyHelper::GetMediaDirectory(this);
	mediaDirectory.append("image.db");

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
	SallyAPI::Scheduler::CSchedulerManager* schedulerManager = SallyAPI::Scheduler::CSchedulerManager::GetInstance();
	schedulerManager->ResetScheduler(this, "dbcreator");

	m_pUpdateDBLastRunInfo->SetText(schedulerManager->GetLastSchedulerRunAsString(this, "dbcreator"));
}

void CMyConfigPanel::WizardDBCreate()
{
	m_tCreateNewDatabase.WaitForStop();
	OnCommandUpdateDB(false);
}

void CMyConfigPanel::OnCommandUpdateDB(bool showPopUp)
{
	SaveConfig();

	// scheduler
	SallyAPI::Scheduler::CSchedulerManager* schedulerManager = SallyAPI::Scheduler::CSchedulerManager::GetInstance();
	schedulerManager->ExecuteScheduler(this, "dbcreator");

	// show info popup
	if (showPopUp)
	{
		SallyAPI::GUI::SendMessage::CParameterMessageBox sendMessageParameterMessageBox(0, this, "The update of the database can take some minutes.\nBut you can continue using Sally.", SallyAPI::GUI::MESSAGEBOX_ICON_INFO);
		m_pParent->SendMessageToParent(this, m_iControlId, MS_SALLY_SHOW_MESSAGEBOX, &sendMessageParameterMessageBox);
	}
}

void CMyConfigPanel::OnCommandUpdateDBScheduler()
{
	m_pButtonDeleteDB->Enable(false);
	m_pButtonResetLastBuildDB->Enable(false);

	m_pButtonUpdateDB->Visible(false);
	m_pButtonUpdateDBCancel->Visible(true);
	m_pProcessbarUpdateDB->Visible(true);
	m_pUpdateDBLastRunInfo->Visible(false);
	m_pUpdateDBLastRunLabel->Visible(false);

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

	// Scheduler
	SallyAPI::Scheduler::CSchedulerManager* schedulerManager = SallyAPI::Scheduler::CSchedulerManager::GetInstance();
	schedulerManager->SchedulerFinished(this, "dbcreator");

	m_pParent->SendMessageToParent(this, GetGraphicId(), MS_SALLY_APP_CONFIG_CHANGED);

	m_pUpdateDBLastRunInfo->SetText(schedulerManager->GetLastSchedulerRunAsString(this, "dbcreator"));
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

	// Scheduler
	SallyAPI::Scheduler::CSchedulerManager* schedulerManager = SallyAPI::Scheduler::CSchedulerManager::GetInstance();
	schedulerManager->SchedulerCanceled(this, "dbcreator");
}