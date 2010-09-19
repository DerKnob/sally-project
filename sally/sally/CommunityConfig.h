////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\CommunityConfig.h
///
/// \brief	Declares the community configuration class. 
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

#pragma once
#include "Define.h"
#include "SallyConfigPanel.h"
#include <shellapi.h>

class CCommunityConfig :
	public SallyAPI::GUI::CPopUpWindow
{
private:
	SallyAPI::GUI::CLabel*		m_pLabelHeader;
	SallyAPI::GUI::CImageBox*	m_pImageLogo;
	SallyAPI::GUI::CGroupBox*	m_pGroupBox;
	
	SallyAPI::GUI::CLabelBox*	m_pLabelBox1;
	SallyAPI::GUI::CLabelBox*	m_pLabelBox2;

	SallyAPI::GUI::CCheckbox*		m_pCommunityEnable;
	SallyAPI::GUI::CEdit*			m_pCommunityUser;
	SallyAPI::GUI::CEdit*			m_pCommunityPassword;
	SallyAPI::GUI::CButton*			m_pCommunityTest;
	SallyAPI::GUI::CLabelBox*		m_pCommunityStatus;
	SallyAPI::GUI::CThreadStarter*	m_pCommunityTimer;

	SallyAPI::GUI::CButton*		m_pCreateAccount;
	SallyAPI::GUI::CButton*		m_pSave;

	void SaveConfig();
	void EnableCommunityControls(bool enable);

	void OnCommandCreateAccount();
	void OnCommandStartLoign();
	void OnCommandEnableCommunityChanged();
	void OnCommandTryLoign();
public:
	CCommunityConfig(SallyAPI::GUI::CGUIBaseObject* parent);
	virtual ~CCommunityConfig();

	virtual void SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
	virtual void Visible(bool visible);
};
