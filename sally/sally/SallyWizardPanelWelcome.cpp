////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\SallyWizardPanelWelcome.cpp
///
/// \brief	Implements the sally wizard panel welcome class. 
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

#include "SallyWizardPanelWelcome.h"

CSallyWizardPanelWelcome::CSallyWizardPanelWelcome(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, const std::string& pluginPath)
	:SallyAPI::GUI::CWizardPanel(parent, graphicId, pluginPath)
{
	m_pImageLogo->SetImageId(GUI_THEME_SALLY_LOGO_BIG);

	m_pLabelHeader->SetText("Welcome to Sally");

	m_pLabelBoxWelcome = new SallyAPI::GUI::CLabelBox(this, 20, 50 + CONTROL_HEIGHT, WIZARD_PANEL_WIDTH - 40, 320);
	m_pLabelBoxWelcome->SetText("Congratulations that you have decided to install Sally.\n\n" \
		"This project is still in an early stage of development. Nevertheless there are many features to be discovered.\n\n" \
		"This is the 'First Start Wizard' which should help you to do your fist steps and to configure Sally.\nThe easiest way to work with Sally is just try to click your way through the application.\n\n\n" \
		"The whole team wishes you a nice time while you are exploring Sally!");
	m_pLabelBoxWelcome->SetLocalised(true);
	this->AddChild(m_pLabelBoxWelcome);

	m_pGUILanguage = new SallyAPI::GUI::CDropDown(this, 20, 50 + CONTROL_HEIGHT + 320 + 10, 150);
	m_pGUILanguage->SetLocalised(false);
	this->AddChild(m_pGUILanguage);
}

CSallyWizardPanelWelcome::~CSallyWizardPanelWelcome()
{
}

void CSallyWizardPanelWelcome::SaveConfig()
{
}

void CSallyWizardPanelWelcome::LoadConfig()
{
	m_pGUILanguage->Clear();

	std::string folder = SallyAPI::System::SystemHelper::GetModulePath();
	folder.append("languages\\");

	std::string searchQuery = folder;
	searchQuery.append("*");

	HANDLE				hFolder;
	WIN32_FIND_DATA		folderInformation;

	hFolder = FindFirstFile(searchQuery.c_str(), &folderInformation);

	if(hFolder != INVALID_HANDLE_VALUE)
	{
		do
		{
			if ((folderInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				&& (strcmp(folderInformation.cFileName, "..") != 0)
				&& (strcmp(folderInformation.cFileName, ".") != 0))
			{
				std::string iniFile = folder;
				iniFile.append(folderInformation.cFileName);
				iniFile.append("\\");
				iniFile.append("manifest.ini");

				std::string name = SallyAPI::System::COption::GetPropertyStringStatic("info", "name", "", iniFile);
				if (name.length() > 0)
				{
					std::string uid = SallyAPI::System::COption::GetPropertyStringStatic("info", "uid", "", iniFile);
					std::string isocode = SallyAPI::System::COption::GetPropertyStringStatic("info", "isocode", "", iniFile);
					int flagId = SallyAPI::System::SallyHelper::GetFlagImageId(isocode);
					
					SallyAPI::GUI::CDropDownItem item(folderInformation.cFileName, name, flagId);
					m_pGUILanguage->AddItem(item);
				}
			}
		} while(FindNextFile(hFolder, &folderInformation) == TRUE);
	}
	FindClose(hFolder);

	// select current language
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	m_pGUILanguage->SelectItemByIdentifier(option->GetPropertyString("sally", "language", "sally-project.language.en"));

	SallyAPI::GUI::CWizardPanel::LoadConfig();
}

void CSallyWizardPanelWelcome::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_DROPDOWN_CHANGED:
		OnCommandDropDownChanged();
		return;
	case LANGUAGE_CHANGED:
		m_pParent->SendMessageToParent(this, GetGraphicId(), MS_SALLY_HIDE_WORKING);
		return;
	}
	SallyAPI::GUI::CWizardPanel::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

void CSallyWizardPanelWelcome::OnCommandDropDownChanged()
{
	m_tLanguageChanger.SetValues(this, m_pGUILanguage->GetSelectedIdentifier());
	m_tLanguageChanger.Start();
	
	m_pParent->SendMessageToParent(this, GetGraphicId(), MS_SALLY_SHOW_WORKING);
}