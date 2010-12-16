////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	ImageBrowser\MyWizardPanel.cpp
///
/// \brief	Implements my wizard panel class. 
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

#include "MyWizardPanel.h"

CMyWizardPanel::CMyWizardPanel(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, const std::string& pluginPath,
							   CMyConfigPanel* configPanel, SallyAPI::GUI::CApplicationWindow* mainWindow)
:SallyAPI::GUI::CWizardPanel(parent, graphicId, pluginPath)
{
	m_pMainWindow = mainWindow;
	m_pConfigPanel = configPanel;

	m_pLabelHeader->SetLocalised(false);
	m_pLabelHeader->SetText("Image Browser");

	m_pLabelBoxWelcome = new SallyAPI::GUI::CLabelBox(this, 20, 50 + CONTROL_HEIGHT, 860, CONTROL_HEIGHT);
	m_pLabelBoxWelcome->SetText("Please select your picture folders:");
	m_pLabelBoxWelcome->SetLocalised(true);
	this->AddChild(m_pLabelBoxWelcome);

	for (int i = 0; i < 8; ++i)
	{
		m_pSelection[i] = new SallyAPI::GUI::CEdit(this,
			50, 20 + CONTROL_HEIGHT + 80 + (i * (CONTROL_HEIGHT + 10)), 500, i);
		m_pSelection[i]->SetInfoText("Image Folder");
		this->AddChild(m_pSelection[i]);

		m_pSelectFolder[i] = new SallyAPI::GUI::CButton(this,
			50 + 500 + 10, 20 + CONTROL_HEIGHT + 80 + (i * (CONTROL_HEIGHT + 10)), 130, CONTROL_HEIGHT,
			GUI_APP_SELECT_FOLDER + i, SallyAPI::GUI::BUTTON_TYPE_NORMAL);
		m_pSelectFolder[i]->SetText("Select");
		m_pSelectFolder[i]->SetImageId(GUI_THEME_SALLY_ICON_FOLDER);
		this->AddChild(m_pSelectFolder[i]);

		m_pEmptySelection[i] = new SallyAPI::GUI::CButton(this,
			50 + 500 + 10 + 10 + 130, 20 + CONTROL_HEIGHT + 80 + (i * (CONTROL_HEIGHT + 10)), CONTROL_HEIGHT, CONTROL_HEIGHT,
			GUI_APP_EMPTEY_SELECTION + i, SallyAPI::GUI::BUTTON_TYPE_NORMAL);
		m_pEmptySelection[i]->SetImageId(GUI_THEME_SALLY_ICON_DELETE);
		this->AddChild(m_pEmptySelection[i]);
	}
}

CMyWizardPanel::~CMyWizardPanel()
{
}

void CMyWizardPanel::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
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
		}
		else
		{
			m_pEmptySelection[reporterId]->Visible(false);
		}
		return;
	case GUI_BUTTON_CLICKED:
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
	SallyAPI::GUI::CWizardPanel::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

void CMyWizardPanel::OnCommandFolderSelected(int reporterId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	// now load the new stuff
	SallyAPI::GUI::SendMessage::CParameterString* parameterString = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterString*> (messageParameter);

	m_pSelection[reporterId - GUI_APP_SELECT_FOLDER]->SetText(parameterString->GetString());

	m_pEmptySelection[reporterId - GUI_APP_SELECT_FOLDER]->Visible(true);
}

void CMyWizardPanel::OnCommandSelectFolder(int reporterId)
{
	SallyAPI::GUI::GUIHelper::SelectFolder(GetGraphicId(), reporterId, GUI_APP_SELECT_THIS_FOLDER, this, "Please select a folder:");
}

void CMyWizardPanel::OnCommandEmptySelection(int reporterId)
{
	int id = reporterId - GUI_APP_EMPTEY_SELECTION;
	m_pSelection[id]->SetText("");
	m_pEmptySelection[id]->Visible(false);
}

void CMyWizardPanel::SaveConfig()
{
	for (int i = 0; i < 8; i++)
	{
		std::string dirValue = m_pSelection[i]->GetText();

		std::string dir;
		std::string defaultScreensaver;

		dir.append("dir");
		dir.append(SallyAPI::String::StringHelper::ConvertToString(i));
		dir.append(".path");

		defaultScreensaver.append("dir");
		defaultScreensaver.append(SallyAPI::String::StringHelper::ConvertToString(i));
		defaultScreensaver.append(".defaultscreensaver");

		SetPropertyInt(defaultScreensaver, 1);
		SetPropertyString(dir, dirValue);
	}

	SallyAPI::GUI::CWizardPanel::SaveConfig();

	m_pConfigPanel->LoadConfig();
	m_pConfigPanel->WizardDBCreate();

	m_pMainWindow->SendMessageToParent(this, GetGraphicId(), MS_SALLY_APP_CONFIG_CHANGED);
}

void CMyWizardPanel::LoadConfig()
{
	for (int i = 0; i < 8; i++)
	{
		std::string dir;

		dir.append("dir");
		dir.append(SallyAPI::String::StringHelper::ConvertToString(i));
		dir.append(".path");

		std::string dirTemp = GetPropertyString(dir);

		m_pSelection[i]->SetText(dirTemp);

		if (dirTemp.length() == 0)
		{
			m_pEmptySelection[i]->Visible(false);
		}
	}

	SallyAPI::GUI::CWizardPanel::LoadConfig();
}