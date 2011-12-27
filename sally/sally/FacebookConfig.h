////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\FacebookConfig.h
///
/// \brief	Declares the facebook configuration class. 
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
#include <shellapi.h>

class CFacebookConfig
	: public SallyAPI::GUI::CPopUpWindow
{
private:
	SallyAPI::GUI::CLabel*			m_pLabelHeader;
	SallyAPI::GUI::CImageBox*		m_pImageLogo;
	SallyAPI::GUI::CGroupBox*		m_pGroupBox;
	
	SallyAPI::GUI::CLabelBox*		m_pLabelBoxInfoMessage;

	SallyAPI::GUI::CForm*			m_pInfoForm;
	SallyAPI::GUI::CLabel*			m_pLabelBoxInfo[6];

	SallyAPI::GUI::CButton*			m_pFacebookEnable;

	SallyAPI::GUI::CForm*			m_pLoginForm;
	SallyAPI::GUI::CWorking*		m_pWorking;
	SallyAPI::GUI::CButton*			m_pCancel;
	SallyAPI::GUI::CLabelBox*		m_pFacebookStatus;

	SallyAPI::GUI::CThreadStarter*	m_pFacebookGetKeyThread;
	SallyAPI::GUI::CTimer*			m_pFacebookGetKeyTimer;

	void OnCommandEnableFacebookChanged();
	void OnCommandGetSallyKey();
	void OnCommandCancelLogin();
	void StartLogin();
	void StopLogin();
	void LoadSettings();
	void UpdateAndShowUserInfo();
public:
	CFacebookConfig(SallyAPI::GUI::CGUIBaseObject* parent);
	virtual ~CFacebookConfig();

	virtual void SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
};
