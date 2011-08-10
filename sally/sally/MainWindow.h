////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\MainWindow.h
///
/// \brief	Declares the main Windows Form. 
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
#include <PowrProf.h>
#include "VoiceInput.h"
#include "Menu.h"
#include "Keyboard.h"
#include "VolumePopUp.h"
#include "WindowLoading.h"
#include "WorkingWindow.h"
#include "OnScreenMenu.h"
#include "DropDownPopUp.h"
#include "ConfigWindow.h"
#include "FirstStartWizard.h"
#include "ShutdownPopUp.h"
#include "InfoPopup.h"
#include "FacebookConfig.h"
#include "AlarmPopUp.h"
#include "LoadThemeImage.h"
#include "ScreenLock.h"
#include "UnloadControl.h"
#include "FacebookOff.h"

enum POPUP_ANI {POPUP_ANI_NONE, POPUP_ANI_SHOW, POPUP_ANI_HIDE};

class CMenu;
class SallyAPI::GUI::CApplicationWindow;

using namespace SallyAPI::GUI;

class CMainWindow :
	public CWindow
{
private:
	int										m_iMuteSound;
	int										m_iGraphicId;
	
	CWindowLoading*							m_pGUILoading;

	CMenu*									m_pMenuView;
	CApplicationWindow*						m_pCurrentWindow;
	SallyAPI::GUI::CDropDown*				m_pCurrentDropDown;
	CApplicationWindow*						m_pScreensaverWindow;
	CApplicationWindow*						m_pFacebookOffWindow;
	std::map<int, CApplicationWindow*>		m_mAppWindows;
	std::list<CControl*>					m_lGUIControlPopUp;
	std::vector<HINSTANCE>					m_vPluginDlls;
	std::vector<HINSTANCE>					m_vPluginExtDlls;
	CVoiceInput*							m_pVoiceInput;
	SallyAPI::GUI::CGUIBaseObject*			m_pKeyboardReporter;
	CConfigWindow*							m_pConfigWindow;
	float									m_fOnScreenMenuDeltaStart;
	CFacebookOff*							m_pFacebookOff;

	//////////////////////////////////////////////////////////////////////////
	std::vector<SallyAPI::GUI::CPopUpWindow*>	m_vPopUpWindowsList;
	SallyAPI::GUI::CPopUpWindow*				m_pCurrentPopUpWindow;
	
	CInputBox*					m_pPopUpInputBox;
	CMessageBox*				m_pPopUpMessageBox;
	CQuestionBox*				m_pPopUpQuestionBox;
	COpenDialog*				m_pPopUpOpenDialog;
	CFirstStartWizard*			m_pPopUpFirstStartWizard;
	CDropDownPopUp*				m_pPopUpDropDown;
	CShutdownPopUp*				m_pPopUpShutdown;
	CFacebookConfig*			m_pPopUpFacebookConfig;
	CKeyboard*					m_pPopUpKeyboard;
	CVolumePopUp*				m_pPopUpVolume;
	CWorkingWindow*				m_pPopUpWorkingWindow;
	CAlarmPopUp*				m_pPopUpAlarm;
	CScreenLock*				m_pPopUpLockScreen;

	SallyAPI::System::CCritSection	m_PopUpAnimationCritSection;
	
	//////////////////////////////////////////////////////////////////////////
	COnScreenMenu*						m_pPopUpOnScreenMenu;
	CInfoPopup*							m_pPopUpInfo;
	SallyAPI::System::CSmartThreadPool	m_LoadTheme;

	SallyAPI::System::CSmartThreadPool	m_UnloadControls;

	SallyAPI::GUI::CThreadStarter*	m_pKeyboardRequestWordsThreadStarter;
	std::string						m_strKeyboardRequestWordsParameter;


	SallyAPI::GUI::CTimer*		m_ptFacebookTimerUpdateStatusMessages;
	SallyAPI::GUI::CTimer*		m_ptFacebookUpdateUserInfo;
	SallyAPI::GUI::CTimer*		m_tScreensaverTimer;
	SallyAPI::GUI::CTimer*		m_tSchedulerTimer;

	void LoadTheme();

	void SwitchScreensaver(SallyAPI::GUI::CApplicationWindow* reporter = NULL);
	void KeyPageDown();
	void KeyPageUp();

	void ShowApplicationWindow();

	void OnCommandAddConfigPanel(SallyAPI::GUI::CGUIBaseObject* reporter, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);
	void OnCommandAddWizardPanel(SallyAPI::GUI::CGUIBaseObject* reporter, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);

	void OnCommandFacebookConfigChanged(SallyAPI::GUI::CGUIBaseObject* reporter);

	void StartScreensaverTimer();

	void OnCommandShowAlarmWindow();
	void OnCommandHideAlarmWindow(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);

	void OnCommandShowApplicationSelector();
	void OnCommandGetApplicationInfo(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);

	void OnCommandFacebookUpdateInfo();
	void OnCommandFacebookGetStatusMessages();
	void OnCommandScheduler();

	void OnCommandDropDownClicked(SallyAPI::GUI::CGUIBaseObject* reporter);
	void OnCommandDropDownSelected(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);
	void OnCommandShowSettings();

	void OnCommandShowWorking();
	void OnCommandHideWorking();

	void OnCommandHideKeyboard();
	void OnCommandShowKeyboard(SallyAPI::GUI::CGUIBaseObject* reporter);
	void OnCommandKeyboardRequestWords();

	void OnCommandAddPopUp(SallyAPI::GUI::CGUIBaseObject* reporter);
	void OnCommandHidePopUp(SallyAPI::GUI::CGUIBaseObject* reporter);
	void OnCommandShowPopUp(SallyAPI::GUI::CGUIBaseObject* reporter);

	void OnCommandControlBlended(SallyAPI::GUI::CGUIBaseObject* reporter);

	void OnCommandStopScreensaver();
	void OnCommandStartScreensaver(bool checkPopUp);
	void OnCommandStartScreensaver(SallyAPI::GUI::CGUIBaseObject* reporter);

	void OnCommandHideVolume();
	void OnCommandShowVolumne();

	void OnCommandLockWindow();
	void OnCommandUnlockWindow();

	void OnCommandShowFacebookConfig();

	void OnCommandShowShutdown();
	void OnCommandShowFirstStartWizard();

	void OnCommandShowInputBox(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);
	void OnCommandShowMessageBox(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);
	void OnCommandShowQuestionBox(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);
	void OnCommandHideDialogBox(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);
	
	void OnCommandShowOpenDialog(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);
	void OnCommandHideOpenDialog(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);

	void OnCommandVoiceCommand(bool release = false, CApplicationWindow* set = NULL);
	void OnCommandChangeApp(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);
	void OnCommandSallyConfigChanged();
	void OnCommandSallyThemeChanged();
	void OnCommandScreenMenu(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);
	void OnCommandDeleteInfoPopup(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);
	void OnCommandShowInfoPopup(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);

	void OnCommandMuteSound();
	void OnCommandUnMuteSound();

	void LoadPlugins();
	void LoadPlugin(const std::string& dllName, const std::string& pluginPath);

	virtual bool	ProcessMouseDown(int x, int y);
	virtual void	RenderControl();
	virtual void	SaveConfig();
public:
	CMainWindow(CWindowLoading* loadingWindow);
	virtual ~CMainWindow();

	virtual bool	KeyDown(int c);
	virtual bool	CharInputPressed(char c);
	virtual void	Timer(float timeDelta);
	virtual void	SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
};