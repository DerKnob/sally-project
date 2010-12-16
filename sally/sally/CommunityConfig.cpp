////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\CommunityConfig.cpp
///
/// \brief	Implements the community configuration class. 
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

#include "CommunityConfig.h"

#define GUI_ENABLE_COMMUNITY		50001
#define GUI_CHECK_COMMUNITY			50002
#define GUI_TIMER_COMMUNITY_LOGIN	50003
#define GUI_CREATE_ACCOUNT			50004

CCommunityConfig::CCommunityConfig(SallyAPI::GUI::CGUIBaseObject* parent)
	:SallyAPI::GUI::CPopUpWindow(parent, 0, "")
{
	m_pButtonClose->Visible(false);

	m_pGroupBox = new SallyAPI::GUI::CGroupBox(this, (WINDOW_WIDTH - 640) / 2, (WINDOW_HEIGHT  - 550) / 2, 640, 550);
	this->AddChild(m_pGroupBox);

	m_pImageLogo = new SallyAPI::GUI::CImageBox(m_pGroupBox, 20, 20, 32, 32);
	m_pImageLogo->SetImageId(GUI_THEME_SALLY_COMMUNITY);
	m_pGroupBox->AddChild(m_pImageLogo);

	m_pLabelHeader = new SallyAPI::GUI::CLabel(m_pGroupBox, 60, 11, 200);
	m_pLabelHeader->SetText("Community");
	m_pLabelHeader->SetFont("big3.font");
	m_pGroupBox->AddChild(m_pLabelHeader);

	m_pLabelBox1 = new SallyAPI::GUI::CLabelBox(m_pGroupBox, 20, 50, 510, 120);
	m_pLabelBox1->SetText("Why should you activate the community?\n\n" \
		"+ You can see the status of all your friends\n" \
		"+ The recipe book plugin needs the community to be enabled to work");
	m_pLabelBox1->SetLocalised(true);
	m_pGroupBox->AddChild(m_pLabelBox1);

	m_pLabelBox2 = new SallyAPI::GUI::CLabelBox(m_pGroupBox, 20, 50 + 120, 510, 80);
	m_pLabelBox2->SetText("If you have already an account on the Sally community page than use these one to login. " \
		"If not click the 'Create Account' button and you will be redirected to the homepage. " \
		"There you can create a new account.");
	m_pLabelBox2->SetLocalised(true);
	m_pGroupBox->AddChild(m_pLabelBox2);

	m_pCommunityEnable = new SallyAPI::GUI::CCheckbox(m_pGroupBox, 20, 280, 280, GUI_ENABLE_COMMUNITY);
	m_pCommunityEnable->SetText("Enable Community");
	m_pGroupBox->AddChild(m_pCommunityEnable);

	m_pCommunityUser = new SallyAPI::GUI::CEdit(m_pGroupBox, 20, 280 + 40, 280);
	m_pCommunityUser->SetInfoText("Username");
	m_pGroupBox->AddChild(m_pCommunityUser);

	m_pCommunityPassword = new SallyAPI::GUI::CEdit(m_pGroupBox, 20, 280 + 80, 280);
	m_pCommunityPassword->SetInfoText("Password");
	m_pCommunityPassword->SetPassword(true);
	m_pGroupBox->AddChild(m_pCommunityPassword);

	m_pCommunityTest = new SallyAPI::GUI::CButton(m_pGroupBox, 20, 280 + 120, 280, CONTROL_HEIGHT, GUI_CHECK_COMMUNITY);
	m_pCommunityTest->SetText("Test Login");
	m_pCommunityTest->SetImageId(GUI_THEME_SALLY_ICON_COMMUNITY);
	m_pGroupBox->AddChild(m_pCommunityTest);

	m_pCommunityStatus = new SallyAPI::GUI::CLabelBox(m_pGroupBox, 20, 280 + 160, 280, 80);
	m_pCommunityStatus->SetLocalised(true);
	m_pGroupBox->AddChild(m_pCommunityStatus);

	m_pCreateAccount = new SallyAPI::GUI::CButton(m_pGroupBox, 20 + 280 + 40, 280, 280, CONTROL_HEIGHT, GUI_CREATE_ACCOUNT);
	m_pCreateAccount->SetText("create Account");
	m_pCreateAccount->SetImageId(GUI_THEME_SALLY_ICON_COMMUNITY);
	m_pGroupBox->AddChild(m_pCreateAccount);

	m_pSave = new SallyAPI::GUI::CButton(m_pGroupBox, 640 - 200 - 20, 550 - CONTROL_HEIGHT - 20, 200, CONTROL_HEIGHT, GUI_THEME_SALLY_CLOSE);
	m_pSave->SetText("Save");
	m_pSave->SetImageId(GUI_THEME_SALLY_ICON_SAVE);
	m_pSave->SetDefaultButton(true);
	m_pGroupBox->AddChild(m_pSave);

	m_pCommunityTimer = new SallyAPI::GUI::CThreadStarter(this, 0, GUI_TIMER_COMMUNITY_LOGIN);
}

CCommunityConfig::~CCommunityConfig()
{
	m_pCommunityTimer->WaitForStop();
}

void CCommunityConfig::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_TIMER_COMMUNITY_LOGIN:
		OnCommandTryLoign();
		return;
	case GUI_BUTTON_CLICKED:
		switch (reporterId)
		{
		case GUI_CREATE_ACCOUNT:
			OnCommandCreateAccount();
			return;
		case GUI_CHECK_COMMUNITY:
			OnCommandStartLoign();
			return;
		case GUI_THEME_SALLY_CLOSE:
			SaveConfig();
			break;
		}
		break;
	case GUI_CHECKBOX_CLICKED:
		switch (reporterId)
		{
		case GUI_ENABLE_COMMUNITY:
			OnCommandEnableCommunityChanged();
			return;
		}
		return;
	}
	SallyAPI::GUI::CPopUpWindow::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

void CCommunityConfig::OnCommandCreateAccount()
{
	std::string url = "http://";
	url.append(COMMUNITY_SERVER);
	url.append("/");
	url.append(COMMUNITY_URL);
	ShellExecute(0, "open", url.c_str(), 0, 0, SW_NORMAL);
}

void CCommunityConfig::SaveConfig()
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	if (m_pCommunityEnable->GetCheckStatus())
	{
		option->SetPropertyBool("sally", "community", true);
		option->SetPropertyString("sally", "communityUsername", m_pCommunityUser->GetText());
		option->SetPropertyString("sally", "communityPassword", m_pCommunityPassword->GetText());
	}
	else
	{
		option->SetPropertyBool("sally", "community", false);
		option->SetPropertyString("sally", "communityUsername", "");
		option->SetPropertyString("sally", "communityPassword", "");
		m_pCommunityUser->SetText("");
		m_pCommunityPassword->SetText("");
	}

	// reset last update
	option->SetPropertyString("sally", "communityLastUpdate", "");

	// delete database
	std::string communityFile = SallyAPI::Core::CGame::GetMediaFolder();
	communityFile.append("community.db");

	bool continueDelete;
	do
	{
		continueDelete = true;
		DeleteFile(communityFile.c_str());
		int error = GetLastError();
		if ((error != ERROR_ACCESS_DENIED) && (error != ERROR_SHARING_VIOLATION))
			continueDelete = false;
	}
	while(continueDelete);

	m_pParent->SendMessageToParent(this, GetControlId(), MS_SALLY_COMMUNITY_CONFIG_CHANGED);
}

void CCommunityConfig::Visible(bool visible)
{
	SallyAPI::GUI::CPopUpWindow::Visible(visible);

	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	m_pCommunityEnable->SetCheckStatus(option->GetPropertyBool("sally", "community", false));

	if (m_pCommunityEnable->GetCheckStatus())
		EnableCommunityControls(true);
	else
		EnableCommunityControls(false);

	m_pCommunityUser->SetText(option->GetPropertyString("sally", "communityUsername", ""));
	m_pCommunityPassword->SetText(option->GetPropertyString("sally", "communityPassword", ""));
}

void CCommunityConfig::OnCommandStartLoign()
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	option->SetPropertyBool("sally", "community", m_pCommunityEnable->GetCheckStatus());
	option->SetPropertyString("sally", "communityUsername", m_pCommunityUser->GetText());
	option->SetPropertyString("sally", "communityPassword", m_pCommunityPassword->GetText());

	m_pCommunityTimer->Start();
	m_pCommunityStatus->SetFont("");
	m_pCommunityStatus->SetText("Trying to login");
}

void CCommunityConfig::OnCommandTryLoign()
{
	m_pCommunityTimer->Stop();

	SallyAPI::Community::CCommunityManager* communityManager = SallyAPI::Community::CCommunityManager::GetInstance();

	std::string errorMessage;
	if (communityManager->TestLogin(errorMessage))
	{
		m_pCommunityStatus->SetFont("info.font");
		m_pCommunityStatus->SetText("Login sucessfull");
	}
	else
	{
		m_pCommunityStatus->SetFont("attention.font");
		m_pCommunityStatus->SetText(errorMessage);
	}
}

void CCommunityConfig::OnCommandEnableCommunityChanged()
{
	if (m_pCommunityEnable->GetCheckStatus())
	{
		EnableCommunityControls(true);
	}
	else
	{
		EnableCommunityControls(false);
	}
}

void CCommunityConfig::EnableCommunityControls(bool enable)
{
	if (enable)
	{
		m_pCommunityUser->Enable(true);
		m_pCommunityPassword->Enable(true);
		m_pCommunityTest->Enable(true);
	}
	else
	{
		m_pCommunityUser->Enable(false);
		m_pCommunityPassword->Enable(false);
		m_pCommunityTest->Enable(false);
	}
	m_pCommunityStatus->SetText("");
}