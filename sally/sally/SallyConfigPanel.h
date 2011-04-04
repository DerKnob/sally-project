////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\SallyConfigPanel.h
///
/// \brief	Declares the sally configuration panel class. 
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
#include "UpdateHelper.h"
#include "UpdateProcessWatcher.h"

class CSallyConfigPanel :
	public SallyAPI::GUI::CConfigPanel
{
private:
	SallyAPI::GUI::CTabcontrol*		m_pTabControl;
	SallyAPI::GUI::CTabcontrolItem*	m_pTabInfo;
	SallyAPI::GUI::CTabcontrolItem*	m_pTabProperties_1;
	SallyAPI::GUI::CTabcontrolItem*	m_pTabScreensaver;
	SallyAPI::GUI::CTabcontrolItem*	m_pTabFacebook;
	SallyAPI::GUI::CTabcontrolItem*	m_pTabNetwork;
	SallyAPI::GUI::CTabcontrolItem*	m_pTabLock;
	SallyAPI::GUI::CTabcontrolItem*	m_pTabUpdate;
	SallyAPI::GUI::CTabcontrolItem*	m_pTabAbout;

	SallyAPI::GUI::CLabel*			m_pTabInfoTextDescription[6];
	SallyAPI::GUI::CLabel*			m_pTabInfoText[6];

	SallyAPI::GUI::CLabel*			m_pTabPropertiesTextDescription[9];

	SallyAPI::GUI::CDropDown*		m_pGUILanguage;
	SallyAPI::GUI::CDropDown*		m_pSpeechInput;
	SallyAPI::GUI::CDropDown*		m_pSpeechOutput;
	SallyAPI::GUI::CDropDown*		m_pKeyboardLayout;
	SallyAPI::GUI::CDropDown*		m_pTheme;
	SallyAPI::GUI::CDropDown*		m_pStartUp;
	SallyAPI::GUI::CCheckbox*		m_pShowPopUpInfos;
	SallyAPI::GUI::CCheckbox*		m_pFontAntialasing;
	SallyAPI::GUI::CRadioButton*	m_pVolumeWindows;
	SallyAPI::GUI::CRadioButton*	m_pVolumeApp;
	SallyAPI::GUI::CButton*			m_pFirstStartUpWizard;

	SallyAPI::GUI::CDropDown*		m_pScreensaver;
	SallyAPI::GUI::CLabel*			m_pTabScreensaverTextDescription[2];
	SallyAPI::GUI::CLabel*			m_pWaitTimeLabel;
	SallyAPI::GUI::CNumberSelector*	m_pWaitTime;

	SallyAPI::GUI::CEditBox*		m_pLoadedPlugins;
	SallyAPI::GUI::CEditBox*		m_pAbout;

	SallyAPI::GUI::CLabelBox*		m_pFullscreenLeftRight;
	SallyAPI::GUI::CRadioButton*	m_pFullscreenLeftRight_1;
	SallyAPI::GUI::CRadioButton*	m_pFullscreenLeftRight_2;

	SallyAPI::GUI::CLabelBox*	m_pFacebookLabel;
	SallyAPI::GUI::CButton*		m_pShowFacebookConfig;
	SallyAPI::GUI::CCheckbox*	m_pShowFacebookPopUpInfos;

	SallyAPI::GUI::CCheckbox*	m_pUseInternetExplorerProxy;
	SallyAPI::GUI::CLabel*		m_pLabelServer;
	SallyAPI::GUI::CEdit*		m_pEditServer;
	SallyAPI::GUI::CLabel*		m_pLabelBypass;
	SallyAPI::GUI::CEdit*		m_pEditBypass;

	SallyAPI::GUI::CLabelBox*	m_pLabelLock;
	SallyAPI::GUI::CLabel*		m_pLabelLock1;
	SallyAPI::GUI::CLabel*		m_pLabelLock2;
	SallyAPI::GUI::CEdit*		m_pEditLock1;
	SallyAPI::GUI::CEdit*		m_pEditLock2;

	SallyAPI::GUI::CCheckbox*	m_pCheckboxAutoUpdate;
	SallyAPI::GUI::CButton*		m_pButtonCheckForUpdate;
	SallyAPI::GUI::CButton*		m_pUpdateAvailable;
	SallyAPI::GUI::CLabelBox*	m_pUpdateInfo;

	CUpdateProcessWatcher*		m_pUpdateProcessWatcher;

	void OnCommandCheckUpdate(bool userAction);
	void OnCommandShowUpdateMessage();
	void OnCommandGetUpdate();

	void FillThemes();
	void FillGUILanguages();
	void FillInputLanguages();
	void FillOutputLanguages();
	void FillKeyboardLayouts();
public:
	CSallyConfigPanel(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, const std::string& pluginPath);
	virtual ~CSallyConfigPanel();

	void	SetLoadedPlugins(std::map<int, SallyAPI::GUI::CApplicationWindow*>* applicationWindowList);

	virtual void LoadConfig();
	virtual void SaveConfig();

	virtual void SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
};
