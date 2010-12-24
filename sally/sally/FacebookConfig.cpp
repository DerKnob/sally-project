////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\FacebookConfig.cpp
///
/// \brief	Implements the facebook configuration class. 
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

#include "FacebookConfig.h"

#define GUI_ENABLE_FACEBOOK					50001
#define GUI_TIMER_FACEBOOK_GET_KEY_TIMER	50002
#define GUI_TIMER_FACEBOOK_GET_KEY			50003
#define GUI_ENABLE_FACEBOOK_CANCEL			50004

CFacebookConfig::CFacebookConfig(SallyAPI::GUI::CGUIBaseObject* parent)
	:SallyAPI::GUI::CPopUpWindow(parent, 0, "")
{
	m_pGroupBox = new SallyAPI::GUI::CGroupBox(this, (WINDOW_WIDTH - 450) / 2, (WINDOW_HEIGHT  - 400) / 2, 450, 400);
	this->AddChild(m_pGroupBox);

	m_pImageLogo = new SallyAPI::GUI::CImageBox(m_pGroupBox, -20, -20, 64, 64);
	m_pImageLogo->SetImageId(GUI_THEME_SALLY_FACEBOOK);
	m_pGroupBox->AddChild(m_pImageLogo);

	m_pLabelHeader = new SallyAPI::GUI::CLabel(m_pGroupBox, 55, 20, 350);
	m_pLabelHeader->SetText("Facebook");
	m_pLabelHeader->SetFont("big3.font");
	m_pGroupBox->AddChild(m_pLabelHeader);

	m_pLabelBoxInfoMessage = new SallyAPI::GUI::CLabelBox(m_pGroupBox, 20, 50, 410, 180);
	m_pLabelBoxInfoMessage->SetText("");
	m_pLabelBoxInfoMessage->SetLocalised(true);
	m_pGroupBox->AddChild(m_pLabelBoxInfoMessage);

	m_pFacebookEnable = new SallyAPI::GUI::CButton(m_pGroupBox, 75, 200, 300, CONTROL_HEIGHT, GUI_ENABLE_FACEBOOK);
	m_pFacebookEnable->SetImageId(GUI_THEME_SALLY_ICON_FACEBOOK);
	m_pFacebookEnable->SetText("Enable Facebook"); // ToDo
	m_pGroupBox->AddChild(m_pFacebookEnable);

	/************************************************************************/
	/* Login Form                                                           */
	/************************************************************************/
	m_pLoginForm = new SallyAPI::GUI::CForm(m_pGroupBox, 0, 270 + 20, 450, 70);
	m_pLoginForm->Visible(false);
	m_pGroupBox->AddChild(m_pLoginForm);

	m_pWorking = new SallyAPI::GUI::CWorking(m_pLoginForm, 75, 0, SallyAPI::GUI::WORKING_SMALL);
	m_pLoginForm->AddChild(m_pWorking);

	m_pFacebookStatus = new SallyAPI::GUI::CLabelBox(m_pLoginForm, 75 + 30, 0, 270, CONTROL_HEIGHT);
	m_pFacebookStatus->SetLocalised(true);
	m_pLoginForm->AddChild(m_pFacebookStatus);

	m_pCancel = new SallyAPI::GUI::CButton(m_pLoginForm, 75, 40, 300, CONTROL_HEIGHT, GUI_ENABLE_FACEBOOK_CANCEL);
	m_pCancel->SetImageId(GUI_THEME_SALLY_ICON_CANCEL);
	m_pCancel->SetText("Cancel");
	m_pLoginForm->AddChild(m_pCancel);

	/************************************************************************/
	/* Info Form                                                            */
	/************************************************************************/
	m_pInfoForm = new SallyAPI::GUI::CForm(m_pGroupBox, 20, 270, 450 - 20 - 20, 110);
	m_pInfoForm->Visible(false);
	m_pGroupBox->AddChild(m_pInfoForm);

	m_pLabelBoxInfo[0] = new SallyAPI::GUI::CLabel(m_pInfoForm, 0, 0, 100);
	m_pLabelBoxInfo[0]->SetAlign(DT_RIGHT | DT_VCENTER);
	m_pLabelBoxInfo[0]->SetText("Status:");
	m_pInfoForm->AddChild(m_pLabelBoxInfo[0]);

	m_pLabelBoxInfo[1] = new SallyAPI::GUI::CLabel(m_pInfoForm, 0, 40, 100);
	m_pLabelBoxInfo[1]->SetAlign(DT_RIGHT | DT_VCENTER);
	m_pLabelBoxInfo[1]->SetText("Name:");
	m_pInfoForm->AddChild(m_pLabelBoxInfo[1]);

	m_pLabelBoxInfo[2] = new SallyAPI::GUI::CLabel(m_pInfoForm, 0, 80, 100);
	m_pLabelBoxInfo[2]->SetAlign(DT_RIGHT | DT_VCENTER);
	m_pLabelBoxInfo[2]->SetText("ID:");
	m_pInfoForm->AddChild(m_pLabelBoxInfo[2]);

	m_pLabelBoxInfo[3] = new SallyAPI::GUI::CLabel(m_pInfoForm, 110, 0, 230);
	m_pLabelBoxInfo[3]->SetLocalised(false);
	m_pLabelBoxInfo[3]->SetBold(true);
	m_pInfoForm->AddChild(m_pLabelBoxInfo[3]);

	m_pLabelBoxInfo[4] = new SallyAPI::GUI::CLabel(m_pInfoForm, 110, 40, 230);
	m_pLabelBoxInfo[4]->SetLocalised(false);
	m_pLabelBoxInfo[4]->SetBold(true);
	m_pInfoForm->AddChild(m_pLabelBoxInfo[4]);

	m_pLabelBoxInfo[5] = new SallyAPI::GUI::CLabel(m_pInfoForm, 110, 80, 230);
	m_pLabelBoxInfo[5]->SetLocalised(false);
	m_pLabelBoxInfo[5]->SetBold(true);
	m_pInfoForm->AddChild(m_pLabelBoxInfo[5]);

	/************************************************************************/
	/* Timer                                                                */
	/************************************************************************/
	m_pFacebookGetKeyTimer = new SallyAPI::GUI::CTimer(6, this, NULL, GUI_TIMER_FACEBOOK_GET_KEY_TIMER);
	m_pFacebookGetKeyThread = new SallyAPI::GUI::CThreadStarter(this, NULL, GUI_TIMER_FACEBOOK_GET_KEY);	

	LoadSettings();
}

CFacebookConfig::~CFacebookConfig()
{
	m_pFacebookGetKeyTimer->Stop();
	m_pFacebookGetKeyThread->WaitForStop();
}

void CFacebookConfig::LoadSettings()
{
	SallyAPI::Facebook::CFacebookManager* facebookManager = SallyAPI::Facebook::CFacebookManager::GetInstance();
	if (facebookManager->IsEnabled())
	{
		m_pFacebookEnable->SetText("Disable Facebook");

		UpdateAndShowUserInfo();
	}
	else
	{
		m_pFacebookEnable->SetText("Enable Facebook");
	}
}

void CFacebookConfig::UpdateAndShowUserInfo()
{
	SallyAPI::Facebook::CFacebookManager* facebookManager = SallyAPI::Facebook::CFacebookManager::GetInstance();

	m_pLabelBoxInfo[3]->SetText("Activated");
	m_pLabelBoxInfo[4]->SetText(facebookManager->GetFacebookUserName());
	m_pLabelBoxInfo[5]->SetText(facebookManager->GetFacebookUserId());

	m_pInfoForm->Visible(true);
}

void CFacebookConfig::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_TIMER_FACEBOOK_GET_KEY_TIMER:
		m_pFacebookGetKeyThread->Start();
		return;
	case GUI_TIMER_FACEBOOK_GET_KEY:
		OnCommandGetSallyKey();
		return;
	case GUI_BUTTON_CLICKED:
		switch (reporterId)
		{
		case GUI_ENABLE_FACEBOOK:
			OnCommandEnableFacebookChanged();
			return;
		case GUI_ENABLE_FACEBOOK_CANCEL:
			OnCommandCancelLogin();
			return;
		}
		break;
	}
	SallyAPI::GUI::CPopUpWindow::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

void CFacebookConfig::OnCommandCancelLogin()
{
	StopLogin();

	m_pFacebookStatus->SetFont("");
	m_pFacebookStatus->SetText("");
}

void CFacebookConfig::OnCommandEnableFacebookChanged()
{
	SallyAPI::Facebook::CFacebookManager* facebookManager = SallyAPI::Facebook::CFacebookManager::GetInstance();

	if (facebookManager->IsEnabled())
	{
		// disable facebook
		facebookManager->Disable();

		// disable controls
		m_pFacebookEnable->SetText("Enable Facebook");

		m_pFacebookStatus->SetFont("");
		m_pFacebookStatus->SetText("");

		m_pInfoForm->Visible(false);

		// send the message that the facebook config has changed
		SendMessageToParent(this, 0, MS_SALLY_FACEBOOK_CONFIG_CHANGED);
	}
	else
	{
		// enable facebook
		facebookManager->ConnectFacebook();

		StartLogin();
	}
}

void CFacebookConfig::OnCommandGetSallyKey()
{
	SallyAPI::Facebook::CFacebookManager* facebookManager = SallyAPI::Facebook::CFacebookManager::GetInstance();

	if (facebookManager->ActivateFacebook())
	{
		// login done

		// did we have success?
		if (facebookManager->IsEnabled())
		{
			m_pFacebookStatus->SetFont("info.font");
			m_pFacebookStatus->SetText("Login sucessfull. Updating Data...");

			m_pFacebookEnable->SetText("Disable Facebook");

			// update the facebook db and all the informations
			SendMessageToParent(this, 0, MS_SALLY_APP_FACEBOOK_STATUS);
			SendMessageToParent(this, 0, MS_SALLY_APP_FACEBOOK_UPDATE_INFO);

			// updating facebook info
			UpdateAndShowUserInfo();
		}
		else
		{
			m_pFacebookStatus->SetFont("attention.font");
			m_pFacebookStatus->SetText("Error");

			m_pFacebookEnable->SetText("Enable Facebook");
		}

		StopLogin();

		// send the message that the facebook config has changed
		SendMessageToParent(this, 0, MS_SALLY_FACEBOOK_CONFIG_CHANGED);
	}
}

void CFacebookConfig::StartLogin()
{
	m_pFacebookGetKeyTimer->Start();

	m_pFacebookEnable->Enable(false);

	m_pLoginForm->Visible(true);

	// set status message
	m_pFacebookStatus->SetFont("");
	m_pFacebookStatus->SetText("Trying to login");
}

void CFacebookConfig::StopLogin()
{
	m_pFacebookGetKeyTimer->Stop();

	m_pFacebookEnable->Enable(true);

	m_pLoginForm->Visible(false);
}