////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\MainWindow.cpp
///
/// \brief	Implements the main Windows Form. 
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

#include "Mainwindow.h"

typedef SallyAPI::GUI::CApplicationWindow* (* CREATEAPPLICATION)(SallyAPI::GUI::CGUIBaseObject *,
																 int, const std::string&);

CMainWindow::CMainWindow(CWindowLoading* loadingWindow)
	:m_pCurrentWindow(0), m_pGUILoading(loadingWindow), m_iMuteSound(0), m_fOnScreenMenuDeltaStart(-1),
	m_pPopUpInfo(NULL), m_pPopUpInputBox(NULL), m_pPopUpMessageBox(NULL),
	m_pPopUpQuestionBox(NULL), m_pPopUpOpenDialog(NULL), m_pPopUpFirstStartWizard(NULL), m_pPopUpDropDown(NULL),
	m_pPopUpShutdown(NULL), m_pPopUpFacebookConfig(NULL), m_pPopUpKeyboard(NULL), m_pPopUpVolume(NULL),
	m_pPopUpWorkingWindow(NULL), m_pPopUpAlarm(NULL), m_pPopUpLockScreen(NULL), m_pPopUpOnScreenMenu(NULL)
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance(); 
	SallyAPI::System::COption* option = config->GetOption();
	SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();

	/**************************/

	m_iGraphicId = APP_GRAPHIC_ID;

	m_pVoiceInput = NULL;
	/*
	m_pVoiceInput = new CVoiceInput(config->GetOption()->GetPropertyString("sally", "lang"), 
	SallyAPI::Core::CGame::GetHWnd(), MS_SALLY_NEW_VOICE_COMMAND);
	m_pVoiceInput->Init();
	m_pVoiceInput->Start();
	*/

	SallyAPI::Speech::CSpeechOutputManager::SetValues(this);
	SallyAPI::Speech::CSpeechOutputManager::Activate();
	
	m_pKeyboardReporter = NULL;
	m_pCurrentDropDown = NULL;

	LoadTheme();

	// Init Database Engine
	SallyAPI::Database::CDatabaseConnection::Init();

	m_pConfigWindow = new CConfigWindow(this);
	m_pConfigWindow->Visible(false);
	this->AddChild(m_pConfigWindow);

	m_pPopUpFirstStartWizard = new CFirstStartWizard(this);
	m_pPopUpFirstStartWizard->Visible(false);

	// Load Plugins
	LoadPlugins();

	// Create FacebookOff
	m_pFacebookOff = new CFacebookOff(this);
	m_pFacebookOff->Visible(false);
	this->AddChild(m_pFacebookOff);

	// send load of apps complete
	std::map<int, CApplicationWindow*>::iterator it;
	for (it = m_mAppWindows.begin(); it != m_mAppWindows.end(); ++it)
	{
		CApplicationWindow* appWindow = it->second;

		appWindow->SendMessageToParent(this, 0, MS_SALLY_APP_LOAD_COMPLETE);
	}

	m_pConfigWindow->SetLoadedPlugins(&m_mAppWindows);

	if (m_mAppWindows.size() > 0)
	{
		m_pCurrentWindow = m_mAppWindows[APP_GRAPHIC_ID];

		// reset the facebookOffWindow
		m_pFacebookOffWindow = NULL;
		ShowApplicationWindow();
	}
	else
	{
		m_pCurrentWindow = m_pConfigWindow;
		m_pConfigWindow->Visible(true);
	}

	// Create Menu
	m_pMenuView = new CMenu(this, &m_mAppWindows);
	this->AddChild(m_pMenuView);

	// Add PopUps now
	std::list<CControl*>::iterator iter = m_lGUIControlPopUp.begin();
	while (iter !=  m_lGUIControlPopUp.end())
	{
		CControl* control = *iter;
		this->AddChild(control);
		++iter;
	}

	this->AddChild(m_pPopUpFirstStartWizard);
	OnCommandAddPopUp(m_pPopUpFirstStartWizard);

	// FacebookConfig PopUp
	m_pPopUpFacebookConfig = new CFacebookConfig(this);
	m_pPopUpFacebookConfig->Visible(false);
	this->AddChild(m_pPopUpFacebookConfig);
	OnCommandAddPopUp(m_pPopUpFacebookConfig);

	// DropDown PopUp
	m_pPopUpDropDown = new CDropDownPopUp(this);
	m_pPopUpDropDown->Visible(false);
	this->AddChild(m_pPopUpDropDown);
	OnCommandAddPopUp(m_pPopUpDropDown);

	// Input Box
	m_pPopUpInputBox = new CInputBox(this);
	m_pPopUpInputBox->Visible(false);
	this->AddChild(m_pPopUpInputBox);
	OnCommandAddPopUp(m_pPopUpInputBox);

	// Question Box
	m_pPopUpQuestionBox = new CQuestionBox(this);
	m_pPopUpQuestionBox->Visible(false);
	this->AddChild(m_pPopUpQuestionBox);
	OnCommandAddPopUp(m_pPopUpQuestionBox);

	// Open Dialog
	m_pPopUpOpenDialog = new COpenDialog(this);
	m_pPopUpOpenDialog->Visible(false);
	this->AddChild(m_pPopUpOpenDialog);
	OnCommandAddPopUp(m_pPopUpOpenDialog);

	// Lock Screen
	m_pPopUpLockScreen = new CScreenLock(this);
	m_pPopUpLockScreen->Visible(false);
	this->AddChild(m_pPopUpLockScreen);
	OnCommandAddPopUp(m_pPopUpLockScreen);

	// Keyboard
	m_pPopUpKeyboard = new CKeyboard(this);
	m_pPopUpKeyboard->Visible(false);
	m_pPopUpKeyboard->SetBlendIn(SallyAPI::GUI::POPUP_BLEND_IN_BOTTOM);
	this->AddChild(m_pPopUpKeyboard);
	OnCommandAddPopUp(m_pPopUpKeyboard);

	// Message Box
	m_pPopUpMessageBox = new CMessageBox(this);
	m_pPopUpMessageBox->Visible(false);
	this->AddChild(m_pPopUpMessageBox);
	OnCommandAddPopUp(m_pPopUpMessageBox);

	// Volumen Control
	m_pPopUpVolume = new CVolumePopUp(this);
	m_pPopUpVolume->Visible(false);
	this->AddChild(m_pPopUpVolume);
	OnCommandAddPopUp(m_pPopUpVolume);

	// Working
	m_pPopUpWorkingWindow = new CWorkingWindow(this);
	m_pPopUpWorkingWindow->Visible(false);
	this->AddChild(m_pPopUpWorkingWindow);
	OnCommandAddPopUp(m_pPopUpWorkingWindow);

	// OnScreenMenu
	m_pPopUpOnScreenMenu = new COnScreenMenu(this);
	m_pPopUpOnScreenMenu->Visible(false);
	m_pPopUpOnScreenMenu->Enable(false);
	this->AddChild(m_pPopUpOnScreenMenu);
	//OnCommandAddPopUp(m_pOnScreenMenu); // don't add this

	// AlarmPopUp
	m_pPopUpAlarm = new CAlarmPopUp(this);
	m_pPopUpAlarm->Visible(false);
	this->AddChild(m_pPopUpAlarm);
	OnCommandAddPopUp(m_pPopUpAlarm);

	// InfoPopup
	m_pPopUpInfo = new CInfoPopup(this);
	m_pPopUpInfo->Visible(false);
	m_pPopUpInfo->Enable(false);
	this->AddChild(m_pPopUpInfo);
	//OnCommandAddPopUp(m_pInfoPopup); // don't add this

	// m_pShutdownPopUp
	m_pPopUpShutdown = new CShutdownPopUp(this);
	this->AddChild(m_pPopUpShutdown);
	OnCommandAddPopUp(m_pPopUpShutdown);

	m_pCurrentPopUpWindow = NULL;
	m_pScreensaverWindow = NULL;
	m_pFacebookOffWindow = NULL;
	
	m_pGUILoading->UpdateProcessbar();

	// FacebookTimer
	m_ptFacebookTimerUpdateStatusMessages = new SallyAPI::GUI::CTimer(60 * 2, this, 0, MS_SALLY_APP_FACEBOOK_STATUS);
	m_ptFacebookTimerUpdateStatusMessages->Start();

	m_ptFacebookUpdateUserInfo = new SallyAPI::GUI::CTimer(60 * 60, this, 0, MS_SALLY_APP_FACEBOOK_UPDATE_INFO);
	m_ptFacebookUpdateUserInfo->Start();

	// ScreensaverTimer
	m_tScreensaverTimer = new SallyAPI::GUI::CTimer(60, this, 0, MS_SALLY_APP_START_SCREENSAVER);
	StartScreensaverTimer();

	// SchedulerTimer
	m_tSchedulerTimer = new SallyAPI::GUI::CTimer(60 * 10, this, 0, MS_SALLY_SCHEDULER);
	m_tSchedulerTimer->Start();

	if (m_pPopUpFirstStartWizard->HasNewApps())
	{
		m_pPopUpFirstStartWizard->SetNewPanelsStart();
		SendMessageToParent(m_pPopUpFirstStartWizard, 0, MS_SALLY_SHOW_POPUP_VIEW);
	}
	else
	{
		SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance(); 
		SallyAPI::System::COption* option = config->GetOption();

		std::string startupapp = option->GetPropertyString("sally", "startupApp", "");
		bool found = false;

		std::map<int, CApplicationWindow*>::iterator it;
		for (it = m_mAppWindows.begin(); it != m_mAppWindows.end(); ++it)
		{
			CApplicationWindow* appWindow = it->second;

			if (appWindow->GetExplicitAppName().compare(startupapp) == 0)
			{
				// hide old window
				m_pCurrentWindow->Visible(false);

				// show new window
				m_pCurrentWindow = appWindow;

				// reset the facebookOffWindow
				m_pFacebookOffWindow = NULL;
				ShowApplicationWindow();

				m_pMenuView->SendMessageToParent(this, m_pCurrentWindow->GetGraphicId(), MS_SALLY_CHANGE_APP);
				found = true;
				break;
			}
		}

		if (!found)
		{
			OnCommandShowApplicationSelector();
		}
	}
	m_pMenuView->ApplicationClicked(m_pCurrentWindow->GetGraphicId(), m_pCurrentWindow->GetAppName());

	// load facebook images
	SallyAPI::Facebook::CFacebookManager* facebookManager = SallyAPI::Facebook::CFacebookManager::GetInstance();
	facebookManager->ReloadAllFacebookUserImages();
}

CMainWindow::~CMainWindow()
{
	// set the pointers to null
	// delete will be done when delting the items from the m_GUIControlList list
	m_pPopUpInputBox = NULL;
	m_pPopUpMessageBox = NULL;
	m_pPopUpQuestionBox = NULL;
	m_pPopUpOpenDialog = NULL;
	m_pPopUpFirstStartWizard = NULL;
	m_pPopUpDropDown = NULL;
	m_pPopUpShutdown = NULL;
	m_pPopUpFacebookConfig = NULL;
	m_pPopUpKeyboard = NULL;
	m_pPopUpVolume = NULL;
	m_pPopUpWorkingWindow = NULL;
	m_pPopUpAlarm = NULL;
	m_pPopUpLockScreen = NULL;
	m_pPopUpOnScreenMenu = NULL;
	m_pPopUpInfo = NULL;

	// stop voice input
	if (m_pVoiceInput != NULL)
	{
		m_pVoiceInput->Stop();
	}
	SafeDelete(m_pVoiceInput);

	m_tScreensaverTimer->WaitForStop();
	m_tSchedulerTimer->WaitForStop();
	m_ptFacebookTimerUpdateStatusMessages->WaitForStop();
	m_ptFacebookUpdateUserInfo->WaitForStop();

	// Unload Apps
	std::list<CControl*>::iterator itrApps = m_GUIControlList.begin();

	while (itrApps != m_GUIControlList.end())
	{
		CControl* control = *itrApps;

		// delete the controls as a thread pool
		m_UnloadControls.AddThread(new CUnloadControl(control));
 		m_GUIControlList.erase(itrApps);
 
 		itrApps = m_GUIControlList.begin();
	}
	m_UnloadControls.Start();

	// whait until all threads are ended
	while (m_UnloadControls.GetStatus() == SallyAPI::System::THREAD_RUNNING)
	{
		Sleep(1);
	}

	HINSTANCE hDll;
	std::vector<HINSTANCE>::iterator itr = m_vPluginDlls.begin();

	// Unload plugins
	while (itr != m_vPluginDlls.end()) {
		hDll = *itr;

		FreeLibrary(hDll);
		++itr;
	}

	HINSTANCE hDllExt;
	std::vector<HINSTANCE>::iterator itrExt = m_vPluginExtDlls.begin();

	// Unload the plugin ext
	while (itrExt != m_vPluginExtDlls.end()) {
		hDllExt = *itrExt;

		FreeLibrary(hDllExt);
		++itrExt;
	}
}

void CMainWindow::SaveConfig()
{
	std::map<int, CApplicationWindow*>::iterator it = m_mAppWindows.begin();

	while (it != m_mAppWindows.end())
	{
		CApplicationWindow* appWindow = it->second;

		appWindow->SendMessageToParent(this, 0, MS_SALLY_APP_SAVE_CONFIG);

		++it;
	}
}

void CMainWindow::LoadTheme()
{
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\backgrounds\\backgroundblack.png", GUI_THEME_SALLY_BLACK_BACKGROUND));
	
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\backgrounds\\background.png", GUI_THEME_SALLY_BACKGROUND));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\backgrounds\\popup_normal.png", GUI_THEME_SALLY_POPUP_NORMAL));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\backgrounds\\popup_appselector.png", GUI_THEME_SALLY_POPUP_APPSELECTOR));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\backgrounds\\popup_working.png", GUI_THEME_SALLY_POPUP_WORKING));
	
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\backgrounds\\popup_onscreenmenu.png", GUI_THEME_SALLY_POPUP_ONSCREENMENU));
	
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\backgrounds\\popup_info.png", GUI_THEME_SALLY_INFO_POPUP));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\backgrounds\\settings_top.png", GUI_THEME_SALLY_SETTINGS_TOP_BORDER));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\backgrounds\\settings_bottom.png", GUI_THEME_SALLY_SETTINGS_BOTTOM_BORDER));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\processbar\\left.png", GUI_THEME_PROCESSBAR_LEFT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\processbar\\center.png", GUI_THEME_PROCESSBAR));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\processbar\\right.png", GUI_THEME_PROCESSBAR_RIGHT));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\processbar\\bar_left.png", GUI_THEME_PROCESSBAR_2LEFT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\processbar\\bar_center.png", GUI_THEME_PROCESSBAR_2));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\processbar\\bar_right.png", GUI_THEME_PROCESSBAR_2RIGHT));

	/************************************************************************/
	/* GUI Buttons                                                          */
	/************************************************************************/
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\seperator\\h_left.png", GUI_THEME_SEPERATOR_H_LEFT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\seperator\\h_center.png", GUI_THEME_SEPERATOR_H_CENTER));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\seperator\\h_right.png", GUI_THEME_SEPERATOR_H_RIGHT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\seperator\\v_left.png", GUI_THEME_SEPERATOR_V_TOP));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\seperator\\v_center.png", GUI_THEME_SEPERATOR_V_CENTER));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\seperator\\v_right.png", GUI_THEME_SEPERATOR_V_BOTTOM));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\button\\n_left.png", GUI_THEME_BUTTON_NORMAL_LEFT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\button\\n_center.png", GUI_THEME_BUTTON_NORMAL_CENTER));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\button\\n_right.png", GUI_THEME_BUTTON_NORMAL_RIGHT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\button\\n_left_top.png", GUI_THEME_BUTTON_NORMAL_LEFT_TOP));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\button\\n_center_top.png", GUI_THEME_BUTTON_NORMAL_CENTER_TOP));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\button\\n_right_top.png", GUI_THEME_BUTTON_NORMAL_RIGHT_TOP));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\button\\n_left_bottom.png", GUI_THEME_BUTTON_NORMAL_LEFT_BOTTOM));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\button\\n_center_bottom.png", GUI_THEME_BUTTON_NORMAL_CENTER_BOTTOM));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\button\\n_right_bottom.png", GUI_THEME_BUTTON_NORMAL_RIGHT_BOTTOM));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\button\\p_left.png", GUI_THEME_BUTTON_SELECTED_LEFT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\button\\p_center.png", GUI_THEME_BUTTON_SELECTED_CENTER));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\button\\p_right.png", GUI_THEME_BUTTON_SELECTED_RIGHT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\button\\p_left_top.png", GUI_THEME_BUTTON_SELECTED_LEFT_TOP));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\button\\p_center_top.png", GUI_THEME_BUTTON_SELECTED_CENTER_TOP));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\button\\p_right_top.png", GUI_THEME_BUTTON_SELECTED_RIGHT_TOP));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\button\\p_left_bottom.png", GUI_THEME_BUTTON_SELECTED_LEFT_BOTTOM));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\button\\p_center_bottom.png", GUI_THEME_BUTTON_SELECTED_CENTER_BOTTOM));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\button\\p_right_bottom.png", GUI_THEME_BUTTON_SELECTED_RIGHT_BOTTOM));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\button\\d_left.png", GUI_THEME_BUTTON_DISABLED_LEFT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\button\\d_center.png", GUI_THEME_BUTTON_DISABLED_CENTER));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\button\\d_right.png", GUI_THEME_BUTTON_DISABLED_RIGHT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\button\\d_left_top.png", GUI_THEME_BUTTON_DISABLED_LEFT_TOP));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\button\\d_center_top.png", GUI_THEME_BUTTON_DISABLED_CENTER_TOP));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\button\\d_right_top.png", GUI_THEME_BUTTON_DISABLED_RIGHT_TOP));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\button\\d_left_bottom.png", GUI_THEME_BUTTON_DISABLED_LEFT_BOTTOM));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\button\\d_center_bottom.png", GUI_THEME_BUTTON_DISABLED_CENTER_BOTTOM));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\button\\d_right_bottom.png", GUI_THEME_BUTTON_DISABLED_RIGHT_BOTTOM));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\button\\db_left.png", GUI_THEME_BUTTON_DEFAULT_LEFT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\button\\db_center.png", GUI_THEME_BUTTON_DEFAULT_CENTER));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\button\\db_right.png", GUI_THEME_BUTTON_DEFAULT_RIGHT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\button\\db_left_top.png", GUI_THEME_BUTTON_DEFAULT_LEFT_TOP));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\button\\db_center_top.png", GUI_THEME_BUTTON_DEFAULT_CENTER_TOP));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\button\\db_right_top.png", GUI_THEME_BUTTON_DEFAULT_RIGHT_TOP));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\button\\db_left_bottom.png", GUI_THEME_BUTTON_DEFAULT_LEFT_BOTTOM));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\button\\db_center_bottom.png", GUI_THEME_BUTTON_DEFAULT_CENTER_BOTTOM));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\button\\db_right_bottom.png", GUI_THEME_BUTTON_DEFAULT_RIGHT_BOTTOM));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\slider\\left.png", GUI_THEME_SLIDER_LEFT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\slider\\center.png", GUI_THEME_SLIDER));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\slider\\right.png", GUI_THEME_SLIDER_RIGHT));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\slider\\bar_left.png", GUI_THEME_SLIDER_2LEFT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\slider\\bar_center.png", GUI_THEME_SLIDER_2));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\slider\\bar_right.png", GUI_THEME_SLIDER_2RIGHT));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\slider\\knob.png", GUI_THEME_SLIDER_KNOB));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\edit\\left.png", GUI_THEME_EDIT_LEFT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\edit\\center.png", GUI_THEME_EDIT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\edit\\right.png", GUI_THEME_EDIT_RIGHT));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\label\\left.png", GUI_THEME_LABEL_LEFT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\label\\center.png", GUI_THEME_LABEL));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\label\\right.png", GUI_THEME_LABEL_RIGHT));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\labelbig\\left.png", GUI_THEME_LABEL_BIG_LEFT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\labelbig\\center.png", GUI_THEME_LABEL_BIG));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\labelbig\\right.png", GUI_THEME_LABEL_BIG_RIGHT));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\checkbox\\left.png", GUI_THEME_CHECKBOX_LEFT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\checkbox\\center.png", GUI_THEME_CHECKBOX));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\checkbox\\right.png", GUI_THEME_CHECKBOX_RIGHT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\checkbox\\checked.png", GUI_THEME_CHECKBOX_CHECKED));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\radiobutton\\left.png", GUI_THEME_RADIO_LEFT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\radiobutton\\center.png", GUI_THEME_RADIO));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\radiobutton\\right.png", GUI_THEME_RADIO_RIGHT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\radiobutton\\checked.png", GUI_THEME_RADIO_CHECKED));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\groupbox\\corner_left_top.png", GUI_THEME_GROUPBOX_LEFT_TOP));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\groupbox\\top.png", GUI_THEME_GROUPBOX_TOP));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\groupbox\\corner_right_top.png", GUI_THEME_GROUPBOX_RIGHT_TOP));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\groupbox\\left.png", GUI_THEME_GROUPBOX_LEFT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\groupbox\\center.png", GUI_THEME_GROUPBOX_CENTER));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\groupbox\\right.png", GUI_THEME_GROUPBOX_RIGHT));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\groupbox\\corner_left_bottom.png", GUI_THEME_GROUPBOX_LEFT_BOTTOM));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\groupbox\\bottom.png", GUI_THEME_GROUPBOX_BOTTOM));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\groupbox\\corner_right_bottom.png", GUI_THEME_GROUPBOX_RIGHT_BOTTOM));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\groupbox\\arrow_top.png", GUI_THEME_GROUPBOX_ARROW_TOP));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\groupbox\\arrow_bottom.png", GUI_THEME_GROUPBOX_ARROW_BOTTOM));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\editbox\\corner_left_top.png", GUI_THEME_EDITBOX_LEFT_TOP));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\editbox\\top.png", GUI_THEME_EDITBOX_TOP));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\editbox\\corner_right_top.png", GUI_THEME_EDITBOX_RIGHT_TOP));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\editbox\\left.png", GUI_THEME_EDITBOX_LEFT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\editbox\\center.png", GUI_THEME_EDITBOX_CENTER));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\editbox\\right.png", GUI_THEME_EDITBOX_RIGHT));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\editbox\\corner_left_bottom.png", GUI_THEME_EDITBOX_LEFT_BOTTOM));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\editbox\\bottom.png", GUI_THEME_EDITBOX_BOTTOM));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\editbox\\corner_right_bottom.png", GUI_THEME_EDITBOX_RIGHT_BOTTOM));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\labelbox\\corner_left_top.png", GUI_THEME_LABELBOX_LEFT_TOP));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\labelbox\\top.png", GUI_THEME_LABELBOX_TOP));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\labelbox\\corner_right_top.png", GUI_THEME_LABELBOX_RIGHT_TOP));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\labelbox\\left.png", GUI_THEME_LABELBOX_LEFT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\labelbox\\center.png", GUI_THEME_LABELBOX_CENTER));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\labelbox\\right.png", GUI_THEME_LABELBOX_RIGHT));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\labelbox\\corner_left_bottom.png", GUI_THEME_LABELBOX_LEFT_BOTTOM));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\labelbox\\bottom.png", GUI_THEME_LABELBOX_BOTTOM));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\labelbox\\corner_right_bottom.png", GUI_THEME_LABELBOX_RIGHT_BOTTOM));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\buttonbar\\n_left.png", GUI_THEME_BUTTONBAR_NORMAL_LEFT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\buttonbar\\n_center.png", GUI_THEME_BUTTONBAR_NORMAL));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\buttonbar\\n_right.png", GUI_THEME_BUTTONBAR_NORMAL_RIGHT));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\buttonbar\\p_left.png", GUI_THEME_BUTTONBAR_SELECTED_LEFT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\buttonbar\\p_center.png", GUI_THEME_BUTTONBAR_SELECTED));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\buttonbar\\p_right.png", GUI_THEME_BUTTONBAR_SELECTED_RIGHT));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\buttonbar\\d_left.png", GUI_THEME_BUTTONBAR_DISABLED_LEFT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\buttonbar\\d_center.png", GUI_THEME_BUTTONBAR_DISABLED));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\buttonbar\\d_right.png", GUI_THEME_BUTTONBAR_DISABLED_RIGHT));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\buttonbar\\n_seperator_left.png", GUI_THEME_BUTTONBAR_SEPERATOR_NORMAL_LEFT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\buttonbar\\n_seperator_right.png", GUI_THEME_BUTTONBAR_SEPERATOR_NORMAL_RIGHT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\buttonbar\\p_seperator_left.png", GUI_THEME_BUTTONBAR_SEPERATOR_SELECTED_LEFT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\buttonbar\\p_seperator_right.png", GUI_THEME_BUTTONBAR_SEPERATOR_SELECTED_RIGHT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\buttonbar\\d_seperator_left.png", GUI_THEME_BUTTONBAR_SEPERATOR_DISABLED_LEFT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\buttonbar\\d_seperator_right.png", GUI_THEME_BUTTONBAR_SEPERATOR_DISABLED_RIGHT));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\tab\\corner_left_top.png", GUI_THEME_TABCONTROL_LEFT_TOP));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\tab\\top.png", GUI_THEME_TABCONTROL_TOP));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\tab\\corner_right_top.png", GUI_THEME_TABCONTROL_RIGHT_TOP));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\tab\\left.png", GUI_THEME_TABCONTROL_LEFT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\tab\\center.png", GUI_THEME_TABCONTROL_CENTER));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\tab\\right.png", GUI_THEME_TABCONTROL_RIGHT));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\tab\\corner_left_bottom.png", GUI_THEME_TABCONTROL_LEFT_BOTTOM));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\tab\\bottom.png", GUI_THEME_TABCONTROL_BOTTOM));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\tab\\corner_right_bottom.png", GUI_THEME_TABCONTROL_RIGHT_BOTTOM));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\tab\\n_left.png", GUI_THEME_TABCONTROL_BUTTON_NORMAL_LEFT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\tab\\n_center.png", GUI_THEME_TABCONTROL_BUTTON_NORMAL));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\tab\\n_right.png", GUI_THEME_TABCONTROL_BUTTON_NORMAL_RIGHT));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\tab\\p_left.png", GUI_THEME_TABCONTROL_BUTTON_SELECTED_LEFT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\tab\\p_center.png", GUI_THEME_TABCONTROL_BUTTON_SELECTED));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\tab\\p_right.png", GUI_THEME_TABCONTROL_BUTTON_SELECTED_RIGHT));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\breadcrumb\\center.png", GUI_THEME_BREADCRUMB));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\breadcrumb\\left.png", GUI_THEME_BREADCRUMB_LEFT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\breadcrumb\\right.png", GUI_THEME_BREADCRUMB_RIGHT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\breadcrumb\\seperator.png", GUI_THEME_BREADCRUMB_SEPERATOR));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\dropdown\\n_left.png", GUI_THEME_DROPDOWN_NORMAL_LEFT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\dropdown\\n_right.png", GUI_THEME_DROPDOWN_NORMAL_RIGHT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\dropdown\\n_center.png", GUI_THEME_DROPDOWN_NORMAL));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\dropdown\\p_left.png", GUI_THEME_DROPDOWN_SELECTED_LEFT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\dropdown\\p_right.png", GUI_THEME_DROPDOWN_SELECTED_RIGHT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\dropdown\\p_center.png", GUI_THEME_DROPDOWN_SELECTED));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\dropdown\\d_left.png", GUI_THEME_DROPDOWN_DISABLED_LEFT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\dropdown\\d_right.png", GUI_THEME_DROPDOWN_DISABLED_RIGHT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\dropdown\\d_center.png", GUI_THEME_DROPDOWN_DISABLED));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\roundbutton\\d_small.png", GUI_THEME_ROUNDBUTTON_SMALL_DISABLED));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\roundbutton\\p_small.png", GUI_THEME_ROUNDBUTTON_SMALL_SELECTED));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\roundbutton\\n_small.png", GUI_THEME_ROUNDBUTTON_SMALL_NORMAL));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\roundbutton\\d_big.png", GUI_THEME_ROUNDBUTTON_BIG_DISABLED));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\roundbutton\\p_big.png", GUI_THEME_ROUNDBUTTON_BIG_SELECTED));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\roundbutton\\n_big.png", GUI_THEME_ROUNDBUTTON_BIG_NORMAL));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\listview\\n_line1_left.png", GUI_THEME_LISTVIEWBUTTON_NORMAL1_LEFT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\listview\\n_line1_right.png", GUI_THEME_LISTVIEWBUTTON_NORMAL1_RIGHT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\listview\\n_line1_center.png", GUI_THEME_LISTVIEWBUTTON_NORMAL1));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\listview\\n_line1_seperator_left.png", GUI_THEME_LISTVIEWBUTTON_NORMAL1_SEPERATOR_LEFT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\listview\\n_line1_seperator_right.png", GUI_THEME_LISTVIEWBUTTON_NORMAL1_SEPERATOR_RIGHT));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\listview\\n_line2_left.png", GUI_THEME_LISTVIEWBUTTON_NORMAL2_LEFT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\listview\\n_line2_right.png", GUI_THEME_LISTVIEWBUTTON_NORMAL2_RIGHT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\listview\\n_line2_center.png", GUI_THEME_LISTVIEWBUTTON_NORMAL2));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\listview\\n_line2_seperator_left.png", GUI_THEME_LISTVIEWBUTTON_NORMAL2_SEPERATOR_LEFT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\listview\\n_line2_seperator_right.png", GUI_THEME_LISTVIEWBUTTON_NORMAL2_SEPERATOR_RIGHT));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\listview\\p_left.png", GUI_THEME_LISTVIEWBUTTON_SELECTED_LEFT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\listview\\p_right.png", GUI_THEME_LISTVIEWBUTTON_SELECTED_RIGHT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\listview\\p_center.png", GUI_THEME_LISTVIEWBUTTON_SELECTED));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\listview\\p_seperator_left.png", GUI_THEME_LISTVIEWBUTTON_SELECTED_SEPERATOR_LEFT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\listview\\p_seperator_right.png", GUI_THEME_LISTVIEWBUTTON_SELECTED_SEPERATOR_RIGHT));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\sidemenu\\background.png", GUI_THEME_SIDE_MENU_BACKGROUND));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\sidemenu\\seperator.png", GUI_THEME_SIDE_MENU_SEPERATOR));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\sidemenu\\n_left.png", GUI_THEME_SIDE_MENU_BUTTON_NORMAL_LEFT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\sidemenu\\n_center.png", GUI_THEME_SIDE_MENU_BUTTON_NORMAL));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\sidemenu\\n_right.png", GUI_THEME_SIDE_MENU_BUTTON_NORMAL_RIGHT));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\sidemenu\\p_left.png", GUI_THEME_SIDE_MENU_BUTTON_SELECTED_LEFT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\sidemenu\\p_center.png", GUI_THEME_SIDE_MENU_BUTTON_SELECTED));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\sidemenu\\p_right.png", GUI_THEME_SIDE_MENU_BUTTON_SELECTED_RIGHT));

	// scrollbar
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\scrollbar\\horinzontal\\n_bar_left.png", GUI_THEME_SCROLLBAR_H_BAR_NORMAL_LEFT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\scrollbar\\horinzontal\\n_bar_center.png", GUI_THEME_SCROLLBAR_H_BAR_NORMAL_CENTER));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\scrollbar\\horinzontal\\n_bar_right.png", GUI_THEME_SCROLLBAR_H_BAR_NORMAL_RIGHT));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\scrollbar\\horinzontal\\n_knob_left.png", GUI_THEME_SCROLLBAR_H_KNOB_NORMAL_LEFT));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\scrollbar\\horinzontal\\n_knob_center.png", GUI_THEME_SCROLLBAR_H_KNOB_NORMAL_CENTER));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\scrollbar\\horinzontal\\n_knob_right.png", GUI_THEME_SCROLLBAR_H_KNOB_NORMAL_RIGHT));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\scrollbar\\vertical\\n_bar_top.png", GUI_THEME_SCROLLBAR_V_BAR_NORMAL_TOP));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\scrollbar\\vertical\\n_bar_center.png", GUI_THEME_SCROLLBAR_V_BAR_NORMAL_CENTER));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\scrollbar\\vertical\\n_bar_bottom.png", GUI_THEME_SCROLLBAR_V_BAR_NORMAL_BOTTOM));

	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\scrollbar\\vertical\\n_knob_top.png", GUI_THEME_SCROLLBAR_V_KNOB_NORMAL_TOP));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\scrollbar\\vertical\\n_knob_center.png", GUI_THEME_SCROLLBAR_V_KNOB_NORMAL_CENTER));
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\scrollbar\\vertical\\n_knob_bottom.png", GUI_THEME_SCROLLBAR_V_KNOB_NORMAL_BOTTOM));

	// misc	
	m_LoadTheme.AddThread(new CLoadThemeImage("gui\\menu.png", GUI_THEME_SALLY_MENU));

	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\close.png", GUI_THEME_SALLY_CLOSE));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\logout.png", GUI_THEME_SALLY_LOGOUT));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\settings.png", GUI_THEME_SALLY_SETTINGS));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\screensaver.png", GUI_THEME_SALLY_SCREENSAVER));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\logo_small.png", GUI_THEME_SALLY_LOGO_SMALL));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\logo_big.png", GUI_THEME_SALLY_LOGO_BIG));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\facebook.png", GUI_THEME_SALLY_FACEBOOK));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\minimize.png", GUI_THEME_SALLY_MINIMIZE));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\working_small.png", GUI_THEME_SALLY_WORKING_SMALL));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\working_big.png", GUI_THEME_SALLY_WORKING_BIG));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\lock.png", GUI_THEME_SALLY_LOCK));

	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\audio\\high.png", GUI_THEME_SALLY_AUDIO_HIGH));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\audio\\low.png", GUI_THEME_SALLY_AUDIO_LOW));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\audio\\medium.png", GUI_THEME_SALLY_AUDIO_MEDIUM));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\audio\\muted.png", GUI_THEME_SALLY_AUDIO_MUTED));

	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\keyboard\\caps.png", GUI_THEME_SALLY_KEYBOARD_CAPS));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\keyboard\\capslock.png", GUI_THEME_SALLY_KEYBOARD_CAPSLOCK));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\keyboard\\clear.png", GUI_THEME_SALLY_KEYBOARD_CLEAR));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\keyboard\\backspace.png", GUI_THEME_SALLY_KEYBOARD_BACKSPACE));

	/************************************************************************/
	/* Icons                                                                */
	/************************************************************************/
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\action\\add.png", GUI_THEME_SALLY_ICON_ADD));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\action\\delete.png", GUI_THEME_SALLY_ICON_DELETE));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\action\\remove.png", GUI_THEME_SALLY_ICON_REMOVE));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\action\\search.png", GUI_THEME_SALLY_ICON_SEARCH));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\action\\cancel.png", GUI_THEME_SALLY_ICON_CANCEL));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\action\\save.png", GUI_THEME_SALLY_ICON_SAVE));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\action\\edit.png", GUI_THEME_SALLY_ICON_EDIT));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\action\\accept.png", GUI_THEME_SALLY_ICON_ACCEPT));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\action\\next.png", GUI_THEME_SALLY_ICON_NEXT));

	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\action\\previous.png", GUI_THEME_SALLY_ICON_PREVIOUS));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\action\\up.png", GUI_THEME_SALLY_ICON_UP));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\action\\down.png", GUI_THEME_SALLY_ICON_DOWN));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\action\\reload.png", GUI_THEME_SALLY_ICON_RELOAD));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\action\\shuffle.png", GUI_THEME_SALLY_ICON_SHUFFLE));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\action\\timer.png", GUI_THEME_SALLY_ICON_TIMER));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\action\\database.png", GUI_THEME_SALLY_ICON_DATABASE));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\action\\font.png", GUI_THEME_SALLY_ICON_FONT));	
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\action\\fullscreen.png", GUI_THEME_SALLY_ICON_FULLSCREEN));	
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\action\\properties.png", GUI_THEME_SALLY_ICON_PROPERTIES));	
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\action\\about.png", GUI_THEME_SALLY_ICON_ABOUT));	
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\action\\facebook.png", GUI_THEME_SALLY_ICON_FACEBOOK));	
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\action\\date.png", GUI_THEME_SALLY_ICON_DATE));	

	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\status\\info.png", GUI_THEME_SALLY_ICON_INFO));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\status\\rating_off.png", GUI_THEME_SALLY_ICON_RATING_OFF));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\status\\rating_on.png", GUI_THEME_SALLY_ICON_RATING_ON));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\status\\system_exit.png", GUI_THEME_SALLY_ICON_SYSTEM_EXIT));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\status\\system_shutdown.png", GUI_THEME_SALLY_ICON_SYSTEM_SHUTDOWN));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\status\\system_lock.png", GUI_THEME_SALLY_ICON_SYSTEM_LOCK));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\status\\system_battery.png", GUI_THEME_SALLY_ICON_SYSTEM_BATTERY));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\status\\screensaver.png", GUI_THEME_SALLY_SCREENSAVER_SMALL));

	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\device\\small\\usb.png", GUI_THEME_SALLY_ICON_USB));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\device\\small\\hd.png", GUI_THEME_SALLY_ICON_HD));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\device\\small\\cd.png", GUI_THEME_SALLY_ICON_CD));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\device\\small\\dvd.png", GUI_THEME_SALLY_ICON_DVD));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\device\\small\\network.png", GUI_THEME_SALLY_ICON_NETWORK));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\device\\small\\folder.png", GUI_THEME_SALLY_ICON_FOLDER));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\device\\small\\desktop.png", GUI_THEME_SALLY_ICON_DESKTOP));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\device\\small\\home.png", GUI_THEME_SALLY_ICON_HOME));

	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\device\\big\\usb.png", GUI_THEME_SALLY_ICON_BIG_USB));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\device\\big\\hd.png", GUI_THEME_SALLY_ICON_BIG_HD));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\device\\big\\cd.png", GUI_THEME_SALLY_ICON_BIG_CD));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\device\\big\\dvd.png", GUI_THEME_SALLY_ICON_BIG_DVD));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\device\\big\\network.png", GUI_THEME_SALLY_ICON_BIG_NETWORK));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\device\\big\\folder.png", GUI_THEME_SALLY_ICON_BIG_FOLDER));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\device\\big\\desktop.png", GUI_THEME_SALLY_ICON_BIG_DESKTOP));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\device\\big\\home.png", GUI_THEME_SALLY_ICON_BIG_HOME));

	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\device\\folder_up.png", GUI_THEME_SALLY_ICON_FOLDER_UP));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\device\\keyboard.png", GUI_THEME_SALLY_ICON_KEYBOARD));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\device\\mouse.png", GUI_THEME_SALLY_ICON_MOUSE));

	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\dialog\\error.png", GUI_THEME_SALLY_ICON_MB_ERROR));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\dialog\\warning.png", GUI_THEME_SALLY_ICON_MB_WARNING));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\dialog\\info.png", GUI_THEME_SALLY_ICON_MB_INFO));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\dialog\\question.png", GUI_THEME_SALLY_ICON_MB_QUESTION));

	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\media\\stop.png", GUI_THEME_SALLY_ICON_MEDIA_STOP));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\media\\pause.png", GUI_THEME_SALLY_ICON_MEDIA_PAUSE));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\media\\play.png", GUI_THEME_SALLY_ICON_MEDIA_PLAY));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\media\\eject.png", GUI_THEME_SALLY_ICON_MEDIA_EJECT));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\media\\seek-backward.png", GUI_THEME_SALLY_ICON_MEDIA_SEEK_BACKWARD));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\media\\seek-forward.png", GUI_THEME_SALLY_ICON_MEDIA_SEEK_FORWARD));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\media\\skip-backward.png", GUI_THEME_SALLY_ICON_MEDIA_SKIP_BACKWARD));
	m_LoadTheme.AddThread(new CLoadThemeImage("icons\\media\\skip-forward.png", GUI_THEME_SALLY_ICON_MEDIA_SKIP_FORWARD));
	
	/************************************************************************/
	/* Mimetypes                                                            */
	/************************************************************************/
	m_LoadTheme.AddThread(new CLoadThemeImage("mimetypes\\small\\html.png", GUI_THEME_SALLY_ICON_MIMETYPE_HTML));
	m_LoadTheme.AddThread(new CLoadThemeImage("mimetypes\\small\\text.png", GUI_THEME_SALLY_ICON_MIMETYPE_TEXT));
	m_LoadTheme.AddThread(new CLoadThemeImage("mimetypes\\small\\mp3.png", GUI_THEME_SALLY_ICON_MIMETYPE_MP3));
	m_LoadTheme.AddThread(new CLoadThemeImage("mimetypes\\small\\video.png", GUI_THEME_SALLY_ICON_MIMETYPE_VIDEO));
	m_LoadTheme.AddThread(new CLoadThemeImage("mimetypes\\small\\image.png", GUI_THEME_SALLY_ICON_MIMETYPE_IMAGE));

	m_LoadTheme.AddThread(new CLoadThemeImage("mimetypes\\big\\html.png", GUI_THEME_SALLY_ICON_BIG_MIMETYPE_HTML));
	m_LoadTheme.AddThread(new CLoadThemeImage("mimetypes\\big\\text.png", GUI_THEME_SALLY_ICON_BIG_MIMETYPE_TEXT));
	m_LoadTheme.AddThread(new CLoadThemeImage("mimetypes\\big\\mp3.png", GUI_THEME_SALLY_ICON_BIG_MIMETYPE_MP3));
	m_LoadTheme.AddThread(new CLoadThemeImage("mimetypes\\big\\video.png", GUI_THEME_SALLY_ICON_BIG_MIMETYPE_VIDEO));
	m_LoadTheme.AddThread(new CLoadThemeImage("mimetypes\\big\\image.png", GUI_THEME_SALLY_ICON_BIG_MIMETYPE_IMAGE));

	/************************************************************************/
	/* OnScreenMenu                                                         */
	/************************************************************************/
	m_LoadTheme.AddThread(new CLoadThemeImage("onscreen\\stop.png", GUI_THEME_SALLY_OSM_STOP));
	m_LoadTheme.AddThread(new CLoadThemeImage("onscreen\\pause.png", GUI_THEME_SALLY_OSM_PAUSE));
	m_LoadTheme.AddThread(new CLoadThemeImage("onscreen\\play.png", GUI_THEME_SALLY_OSM_PLAY));
	m_LoadTheme.AddThread(new CLoadThemeImage("onscreen\\next.png", GUI_THEME_SALLY_OSM_NEXT));
	m_LoadTheme.AddThread(new CLoadThemeImage("onscreen\\previous.png", GUI_THEME_SALLY_OSM_PREVIOUS));
	m_LoadTheme.AddThread(new CLoadThemeImage("onscreen\\shuffleon.png", GUI_THEME_SALLY_OSM_SHUFFLEON));
	m_LoadTheme.AddThread(new CLoadThemeImage("onscreen\\shuffleoff.png", GUI_THEME_SALLY_OSM_SHUFFLEOFF));
	m_LoadTheme.AddThread(new CLoadThemeImage("onscreen\\save.png", GUI_THEME_SALLY_OSM_SAVE));
	m_LoadTheme.AddThread(new CLoadThemeImage("onscreen\\add.png", GUI_THEME_SALLY_OSM_ADD));

	/************************************************************************/
	/* Flags                                                                */
	/************************************************************************/
	m_LoadTheme.AddThread(new CLoadThemeImage("flags\\flag_germany.png", GUI_THEME_SALLY_FLAG_DE));
	m_LoadTheme.AddThread(new CLoadThemeImage("flags\\flag_usa.png", GUI_THEME_SALLY_FLAG_EN));
	m_LoadTheme.AddThread(new CLoadThemeImage("flags\\flag_france.png", GUI_THEME_SALLY_FLAG_FR));
	m_LoadTheme.AddThread(new CLoadThemeImage("flags\\flag_spain.png", GUI_THEME_SALLY_FLAG_ES));
	m_LoadTheme.AddThread(new CLoadThemeImage("flags\\flag_italy.png", GUI_THEME_SALLY_FLAG_IT));
	m_LoadTheme.AddThread(new CLoadThemeImage("flags\\flag_romania.png", GUI_THEME_SALLY_FLAG_RO));
	m_LoadTheme.AddThread(new CLoadThemeImage("flags\\flag_austria.png", GUI_THEME_SALLY_FLAG_AT));
	m_LoadTheme.AddThread(new CLoadThemeImage("flags\\flag_canada.png", GUI_THEME_SALLY_FLAG_CA));
	m_LoadTheme.AddThread(new CLoadThemeImage("flags\\flag_switzerland.png", GUI_THEME_SALLY_FLAG_CH));

	m_LoadTheme.Start();
}

void CMainWindow::LoadPlugin(const std::string& dllName, const std::string& pluginPath)
{
	SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();

	// check version
	std::string fileInfo = pluginPath;
	fileInfo.append("manifest.ini");

	std::string pluginName = pluginPath;
	pluginName.erase(pluginName.length() - 1, 1);
	pluginName = SallyAPI::String::PathHelper::GetFileFromPath(pluginName);

	SallyAPI::Core::COptionManager* optionManager = SallyAPI::Core::COptionManager::GetInstance();
	SallyAPI::System::COption* option = optionManager->GetOption(pluginName);

	bool disabled = option->GetPropertyBool("info", "disabled", false);

	if (disabled)
	{
		std::string info;
		info.append("Plugin disabled");
		logger->Info(info);
		return;
	}

	std::string apiVersion = SallyAPI::System::COption::GetPropertyStringStatic("info", "apiVersion", "", fileInfo);

	if (apiVersion.compare(SALLY_API_VERSION) != 0)
	{
		std::string info;
		info.append("Wrong API Version: '");
		info.append(apiVersion);
		info.append("' Accepting:  ");
		info.append(SALLY_API_VERSION);
		logger->Info(info);
		return;
	}

	// load the ext dlls for this plugin
	std::string dllPath = pluginPath;
	dllPath.append("dlls\\");

	for (int i = 1; i < 100; ++i)
	{
		std::string extDllName = SallyAPI::System::COption::GetPropertyStringStatic("dll", SallyAPI::String::StringHelper::ConvertToString(i), "", fileInfo);

		if (extDllName.length() > 0)
		{
			std::string fullDLLPath = dllPath;
			fullDLLPath.append(extDllName);

			HINSTANCE hDll;
			if(NULL != (hDll = LoadLibrary(fullDLLPath.c_str())))
			{
				std::string info;
				info.append("Loaded Dll: '");
				info.append(extDllName);
				info.append("' for plugin: '");
				info.append(pluginPath);
				info.append(dllName);
				info.append("'");
				logger->Info(info);

				m_vPluginExtDlls.push_back(hDll);
			}
			else
			{
				std::string error;
				error.append("Error while loading Dll: '");
				error.append(extDllName);
				error.append("' for plugin: '");
				error.append(pluginPath);
				error.append(dllName);
				error.append("'");
				logger->Warning(error);
			}
		}
		else
		{
			// not found ... so end now
			i = 100;
		}
	}

	// load the plugin
	static int loadedPlugins = 0;
	CREATEAPPLICATION	PA_CreateApplication;
	HINSTANCE			hDll;
	std::string			version;

	if(NULL != (hDll = LoadLibrary(dllName.c_str())))
	{
		if(NULL != (PA_CreateApplication = (CREATEAPPLICATION)GetProcAddress(hDll, "CreateApplication")))
		{
			m_pGUILoading->UpdateProcessbar();

			SallyAPI::GUI::CApplicationWindow* temp = PA_CreateApplication(this, m_iGraphicId, pluginPath);
			version = temp->GetAPIVersion();

			if (version.compare(SALLY_API_VERSION) == 0)
			{
				m_mAppWindows[m_iGraphicId] = temp;
				m_mAppWindows[m_iGraphicId]->Visible(false);
				this->AddChild(temp);

				m_vPluginDlls.push_back(hDll);
				m_iGraphicId += 1000;

				temp->SendMessageToParent(0, 0, MS_SALLY_APP_CONFIG_CHANGED);

				std::string info;
				info.append("Plugin '");
				info.append(dllName);
				info.append("' loaded. AppName: ");
				info.append(temp->GetAppName());
				info.append(" Version: ");
				info.append(temp->GetVersion());
				logger->Info(info);
			}
			else
			{
				std::string error;
				error.append("The plugin '");
				error.append(dllName);
				error.append("' has the wrong API Version. Expected: ");
				error.append(SALLY_API_VERSION);
				error.append(" Got: ");
				error.append(version);
				logger->Warning(error);

				delete temp;
				FreeLibrary(hDll);
			}
		}
		else
		{
			std::string error;
			error.append("Error while loading plugin: '");
			error.append(dllName);
			error.append("'. Function CreateApplication not found.");
			logger->Warning(error);

			FreeLibrary(hDll);
		}
	}
	else
	{
		std::string error;
		error.append("Error while loading plugin: '");
		error.append(dllName);
		error.append("' GetLastError: ");
		error.append(SallyAPI::String::StringHelper::ConvertToString(GetLastError()));
		logger->Warning(error);
	}
	++loadedPlugins;
	return;
}

void CMainWindow::LoadPlugins()
{
	// Add Plugins
	std::string installDir = SallyAPI::System::SystemHelper::GetModulePath();
	std::string searchPlugins = installDir;
	searchPlugins.append("applications\\*");

	HANDLE				hFolder, hDll;
	WIN32_FIND_DATA		pluginFolderInformation, pluginDllInformation;

	hFolder = FindFirstFile(searchPlugins.c_str(), &pluginFolderInformation);

	if(hFolder != INVALID_HANDLE_VALUE)
	{
		do
		{
			if ((pluginFolderInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				&& (strcmp(pluginFolderInformation.cFileName, "..") != 0)
				&& (strcmp(pluginFolderInformation.cFileName, ".") != 0))
			{
				std::string pluginFolderSearch = installDir;
				pluginFolderSearch.append("applications\\");

				pluginFolderSearch.append(pluginFolderInformation.cFileName);
				pluginFolderSearch.append("\\");
				pluginFolderSearch.append("*.dll");
				hDll = FindFirstFile(pluginFolderSearch.c_str(), &pluginDllInformation);

				if(hDll != INVALID_HANDLE_VALUE)
				{
					do
					{
						// Now Try to load the DLL
						std::string pluginDll = installDir;
						pluginDll.append("applications\\");

						pluginDll.append(pluginFolderInformation.cFileName);
						pluginDll.append("\\");
						pluginDll.append(pluginDllInformation.cFileName);

						std::string pluginFolder = installDir;
						pluginFolder.append("applications\\");
						pluginFolder.append(pluginFolderInformation.cFileName);
						pluginFolder.append("\\");

						LoadPlugin(pluginDll, pluginFolder);
					} while(FindNextFile(hDll, &pluginDllInformation) == TRUE);
				}
				FindClose(hDll);
			}
		} while(FindNextFile(hFolder, &pluginFolderInformation) == TRUE);
	}
	FindClose(hFolder);	
}

void CMainWindow::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUIM_EXIT:
		if (reporter == this)
			SallyAPI::GUI::CWindow::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
		return;
	case GUIM_UPDATE:
		if (reporter == this)
			SallyAPI::GUI::CWindow::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
		return;
	case GUIM_SHUTDOWN:
		if (reporter == this)
			SallyAPI::GUI::CWindow::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
		return;
	case GUIM_HIBERNATE:
		if (reporter == this)
		{
			SetSuspendState(FALSE, TRUE, FALSE);
		}
		return;
	case GUIM_LOADING_DONE:
	case GUIM_START_LOADING:
		if (reporter == m_pGUILoading)
			SallyAPI::GUI::CWindow::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
		return;
	case MS_SALLY_SALLY_NEW_UPDATE_AVAIL:
		m_pMenuView->SendMessageToParent(reporter, reporterId, messageId);
		return;
	case MS_SALLY_SHOW_APPLICATION_SELECTOR:
		OnCommandShowApplicationSelector();
		return;
	case MS_SALLY_SCHEDULER:
		OnCommandScheduler();
		return;
	case MS_SALLY_MUTE_SOUND:
		OnCommandMuteSound();
		return;
	case MS_SALLY_UNMUTE_SOUND:
		OnCommandUnMuteSound();
		return;
	case GUI_DROPDOWN_CLICKED:
		OnCommandDropDownClicked(reporter);
		return;
	case GUI_DROPDOWN_SELECTED:
		OnCommandDropDownSelected(messageParameter);
		return;
	case GUI_DROPDOWN_CANCELED:
		OnCommandDropDownSelected(NULL);
		return;
	case MS_SALLY_ADD_CHILD:
		OnCommandAddPopUp(reporter);
		return;
	case MS_SALLY_ADD_CONFIG_PANEL:
		OnCommandAddConfigPanel(reporter, messageParameter);
		return;
	case MS_SALLY_ADD_WIZARD_PANEL:
		OnCommandAddWizardPanel(reporter, messageParameter);
		return;
	case MS_SALLY_CHANGE_APP:
		OnCommandChangeApp(messageParameter);
		return;
	case MS_SALLY_NEW_VOICE_COMMAND:
		OnCommandVoiceCommand();
		return;
	case MS_SALLY_SHOW_KEYBOARD:
		OnCommandShowKeyboard(reporter);
		return;
	case MS_SALLY_HIDE_KEYBOARD:
		if (m_pKeyboardReporter != NULL)
			m_pKeyboardReporter->SetText(m_pPopUpKeyboard->GetText());
	case MS_SALLY_HIDE_KEYBOARD_CANCEL:
		OnCommandHideKeyboard();
		return;
	case MS_SALLY_LOCK_SCREEN:
		OnCommandLockWindow();
		return;
	case MS_SALLY_UNLOCK_SCREEN:
		OnCommandUnlockWindow();
		return;
	case MS_SALLY_SHOW_VOLUME:
		OnCommandShowVolumne();
		return;
	case MS_SALLY_HIDE_VOLUME:
		OnCommandHideVolume();
		return;
	case MS_SALLY_SHOW_POPUP_VIEW:
		OnCommandShowPopUp(reporter);
		return;
	case MS_SALLY_POPUP_BLENDED:
		OnCommandControlBlended(reporter);
		return;
	case MS_SALLY_HIDE_POPUP_VIEW:
		OnCommandHidePopUp(reporter);
		return;
	case MS_SALLY_APP_FACEBOOK_UPDATE_INFO:
		OnCommandFacebookUpdateInfo();
		return;
	case MS_SALLY_APP_FACEBOOK_STATUS:
		OnCommandFacebookGetStatusMessages();
		return;
	case MS_SALLY_APP_START_SCREENSAVER:
		if (reporter == NULL)
			OnCommandStartScreensaver(false);
		else if (reporter == this)
			OnCommandStartScreensaver(true);
		else
			OnCommandStartScreensaver(reporter);
		return;
	case MS_SALLY_APP_STOP_SCREENSAVER:
		OnCommandStopScreensaver();
		return;
	case MS_SALLY_VOICE_RELEASE_FOCUS:
		OnCommandVoiceCommand(true);
		return;
	case MS_SALLY_VOICE_SET_FOCUS:
		OnCommandVoiceCommand(false, (CApplicationWindow*) reporter);
		return;
	case MS_SALLY_ON_SCREEN_MENU:
		OnCommandScreenMenu(messageParameter);
		return;
	case MS_SALLY_SHOW_INFO_POPUP:
		OnCommandShowInfoPopup(messageParameter);
		return;
	case MS_SALLY_DELETE_INFO_POPUP:
		OnCommandDeleteInfoPopup(messageParameter);
		return;
	case MS_SALLY_SHOW_INPUTBOX:
		OnCommandShowInputBox(messageParameter);
		return;
	case MS_SALLY_SHOW_MESSAGEBOX:
		OnCommandShowMessageBox(messageParameter);
		return;
	case MS_SALLY_SHOW_QUESTIONBOX:
		OnCommandShowQuestionBox(messageParameter);
		return;
	case MS_SALLY_SHOW_OPEN_FILE:
		OnCommandShowOpenDialog(messageParameter);
		return;
	case MS_SALLY_CANCEL_OPEN_FILE:
	case MS_SALLY_OK_OPEN_FILE:
		OnCommandHideOpenDialog(reporter, reporterId, messageId, messageParameter);
		return;
	case MS_SALLY_SHOW_FACEBOOK_CONFIG:
		OnCommandShowFacebookConfig();
		return;
	case MS_SALLY_FACEBOOK_CONFIG_CHANGED:
		OnCommandFacebookConfigChanged(reporter);
		return;
	case MS_SALLY_SHOW_WORKING:
		OnCommandShowWorking();
		return;
	case MS_SALLY_HIDE_WORKING:
		OnCommandHideWorking();
		return;
	case MS_SALLY_APP_SHOW_SHUTDOWN:
		OnCommandShowShutdown();
		return;
	case MS_SALLY_GET_APPLICATION_INFO:
		OnCommandGetApplicationInfo(messageParameter);
		return;
	case MS_SALLY_SHOW_ALARM_WINDOW:
		OnCommandShowAlarmWindow();
		return;
	case MS_SALLY_HIDE_ALARM_WINDOW:
		OnCommandHideAlarmWindow(messageParameter);
		return;
	case MS_DIALOG_CANCEL:
	case MS_DIALOG_NO:
	case MS_DIALOG_OK:
	case MS_DIALOG_YES:
		OnCommandHideDialogBox(reporter, reporterId, messageId, messageParameter);
		return;
	case MS_SALLY_SHOW_SETTINGS:
		OnCommandShowSettings();
		break;
	case MS_SALLY_HIDE_SETTINGS:
		break;
	case MS_SALLY_SALLY_CONFIG_CHANGED:
		if  (reporter != this)
			OnCommandSallyConfigChanged();
		return;
	case MS_SALLY_SHOW_FIRST_START_WIZARD:
		OnCommandShowFirstStartWizard();
		return;
	case MS_SALLY_DEVICE_RESTORE_END:
		m_tScreensaverTimer->Reset();
		return;
	}
	return;
}

void CMainWindow::OnCommandAddConfigPanel(SallyAPI::GUI::CGUIBaseObject* reporter, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	SallyAPI::GUI::CApplicationWindow* applicationWindow = dynamic_cast<SallyAPI::GUI::CApplicationWindow*> (reporter);
	if (applicationWindow == NULL)
		return;

	SallyAPI::GUI::SendMessage::CParameterControl* parameter = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterControl*> (messageParameter);
	if (parameter == NULL)
		return;

	SallyAPI::GUI::CConfigPanel* controlPanel = dynamic_cast<SallyAPI::GUI::CConfigPanel*> (parameter->GetControl());
	if (controlPanel == NULL)
		return;

	// Load Config Panel
	m_pConfigWindow->AddConfigPanel(applicationWindow, controlPanel);
}

void CMainWindow::OnCommandAddWizardPanel(SallyAPI::GUI::CGUIBaseObject* reporter, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	SallyAPI::GUI::CApplicationWindow* applicationWindow = dynamic_cast<SallyAPI::GUI::CApplicationWindow*> (reporter);
	if (applicationWindow == NULL)
		return;

	SallyAPI::GUI::SendMessage::CParameterControl* parameter = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterControl*> (messageParameter);
	if (parameter == NULL)
		return;

	SallyAPI::GUI::CWizardPanel* wizardPanel = dynamic_cast<SallyAPI::GUI::CWizardPanel*> (parameter->GetControl());
	if (wizardPanel == NULL)
		return;

	// Load Wizard Panel
	m_pPopUpFirstStartWizard->AddWizardPanel(applicationWindow, wizardPanel);
}

void CMainWindow::OnCommandFacebookConfigChanged(SallyAPI::GUI::CGUIBaseObject* reporter)
{
	if (reporter != m_pPopUpFacebookConfig)
		return;
	
	std::map<int, CApplicationWindow*>::iterator it = m_mAppWindows.begin();

	while (it != m_mAppWindows.end())
	{
		CApplicationWindow* appWindow = it->second;

		appWindow->SendMessageToParent(this, 0, MS_SALLY_FACEBOOK_CONFIG_CHANGED);

		++it;
	}

	if (m_pCurrentWindow != NULL)
	{
		ShowApplicationWindow();
	}
}

void CMainWindow::OnCommandAddPopUp(SallyAPI::GUI::CGUIBaseObject* reporter)
{
	SallyAPI::GUI::CControl* control = dynamic_cast<SallyAPI::GUI::CControl*> (reporter);

	if (control == NULL)
		return;

	m_lGUIControlPopUp.push_back(control);
}

void CMainWindow::OnCommandShowApplicationSelector()
{
	m_pMenuView->SendMessageToParent(0, MS_SALLY_SHOW_APPLICATION_SELECTOR, GUI_BUTTON_CLICKED);
}

void CMainWindow::OnCommandGetApplicationInfo(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	SallyAPI::GUI::SendMessage::CParameterApplicationInfo* applicationInfo = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterApplicationInfo*> (messageParameter);

	if (applicationInfo == NULL)
		return;

	std::string explicidAppName = applicationInfo->GetExplicidAppName();

	std::map<int, CApplicationWindow*>::iterator it = m_mAppWindows.begin();

	while (it != m_mAppWindows.end())
	{
		CApplicationWindow* appWindow = it->second;

		if (explicidAppName.compare(appWindow->GetExplicitAppName()) == 0)
		{
			applicationInfo->SetWindow(appWindow);
			return;
		}
		++it;
	}
}

void CMainWindow::OnCommandShowFirstStartWizard()
{
	m_pPopUpFirstStartWizard->SetAllPanelsStart();
	OnCommandShowPopUp(m_pPopUpFirstStartWizard);
}

void CMainWindow::OnCommandShowShutdown()
{
	OnCommandShowPopUp(m_pPopUpShutdown);
}

void CMainWindow::OnCommandMuteSound()
{
	++m_iMuteSound;

	if (m_iMuteSound == 1)
	{
		std::map<int, CApplicationWindow*>::iterator	it;

		for (it = m_mAppWindows.begin(); it != m_mAppWindows.end(); ++it)
		{
			CApplicationWindow* appWindow = it->second;

			appWindow->SendMessageToParent(this, 0, MS_SALLY_APP_MUTE_SOUND);
		}
	}
}

void CMainWindow::OnCommandUnMuteSound()
{
	--m_iMuteSound;

	if (m_iMuteSound == 0)
	{
		std::map<int, CApplicationWindow*>::iterator	it;

		for (it = m_mAppWindows.begin(); it != m_mAppWindows.end(); ++it)
		{
			CApplicationWindow* appWindow = it->second;

			appWindow->SendMessageToParent(this, 0, MS_SALLY_APP_UNMUTE_SOUND);
		}
	}

	if (m_iMuteSound < 0)
		m_iMuteSound = 0;
}

void CMainWindow::OnCommandSallyConfigChanged()
{
	StartScreensaverTimer();

	std::map<int, CApplicationWindow*>::iterator	it;

	for (it = m_mAppWindows.begin(); it != m_mAppWindows.end(); ++it)
	{
		CApplicationWindow* appWindow = it->second;

		appWindow->SendMessageToParent(this, 0, MS_SALLY_SALLY_CONFIG_CHANGED);
	}

	m_pMenuView->SendMessageToParent(this, 0, MS_SALLY_SALLY_CONFIG_CHANGED);
}

void CMainWindow::OnCommandDropDownClicked(SallyAPI::GUI::CGUIBaseObject* reporter)
{
	if (m_pPopUpDropDown == NULL)
		return;

	SallyAPI::GUI::CDropDown* dropDown = dynamic_cast<SallyAPI::GUI::CDropDown*> (reporter);

	if (dropDown == NULL)
		return;

	if (dropDown->GetItemList().size() == 0)
		return;

	m_pCurrentDropDown = dropDown;
	m_pPopUpDropDown->SetPopUpControlPoint(m_pCurrentDropDown);
	m_pPopUpDropDown->SetList(dropDown->GetItemList(), m_pCurrentDropDown->GetSelectedIdentifier(), m_pCurrentDropDown->IsLocalised());

	OnCommandShowPopUp(m_pPopUpDropDown);
}

void CMainWindow::OnCommandDropDownSelected(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	SallyAPI::GUI::SendMessage::CParameterString* messageParameterString = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterString*> (messageParameter);

	if ((messageParameterString != NULL) && (m_pCurrentDropDown != NULL))
	{
		m_pCurrentDropDown->SelectItemByIdentifier(messageParameterString->GetString());
		m_pCurrentDropDown->SendMessageToParent(m_pCurrentDropDown, m_pCurrentDropDown->GetControlId(), GUI_DROPDOWN_CHANGED);
	}

	m_pCurrentDropDown = NULL;

	OnCommandHidePopUp(m_pPopUpDropDown);
}

void CMainWindow::OnCommandShowSettings()
{
	if (m_pCurrentWindow != NULL)
		m_pCurrentWindow->Visible(false);
	m_pConfigWindow->Visible(true);
	m_pCurrentWindow = m_pConfigWindow;
	m_pMenuView->SendMessageToParent(this, GUI_THEME_SALLY_SETTINGS, GUI_THEME_SALLY_SETTINGS, 0);

	m_pMenuView->ApplicationClicked(GUI_THEME_SALLY_SETTINGS, "Settings");
}

void CMainWindow::OnCommandShowWorking()
{
	OnCommandShowPopUp(m_pPopUpWorkingWindow);
}

void CMainWindow::OnCommandHideWorking()
{
	OnCommandHidePopUp(m_pPopUpWorkingWindow);
}

void CMainWindow::OnCommandShowFacebookConfig()
{
	OnCommandShowPopUp(m_pPopUpFacebookConfig);
}

void CMainWindow::OnCommandShowAlarmWindow()
{
	OnCommandShowPopUp(m_pPopUpAlarm);
}

void CMainWindow::OnCommandHideAlarmWindow(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	m_pMenuView->SendMessageToParent(this, 0, MS_SALLY_HIDE_ALARM_WINDOW, messageParameter);

	OnCommandHidePopUp(m_pPopUpAlarm);
}

void CMainWindow::OnCommandShowOpenDialog(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	if (m_pPopUpOpenDialog == NULL)
		return;

	SallyAPI::GUI::SendMessage::CParameterOpenDialog* parameter = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterOpenDialog*> (messageParameter);

	if (parameter == NULL)
		return;

	m_pPopUpOpenDialog->SetValues(*parameter);

	OnCommandShowPopUp(m_pPopUpOpenDialog);
}

void CMainWindow::OnCommandHideOpenDialog(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	if (reporter != this)
		reporter->SendMessageToParent(this, reporterId, messageId, messageParameter);

	OnCommandHidePopUp(m_pPopUpOpenDialog);
}

void CMainWindow::OnCommandShowInputBox(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	if (m_pPopUpInputBox == NULL)
		return;

	SallyAPI::GUI::SendMessage::CParameterInputBox* parameter = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterInputBox*> (messageParameter);

	if (parameter == NULL)
		return;

	m_pPopUpInputBox->SetValues(*parameter);

	OnCommandShowPopUp(m_pPopUpInputBox);
}

void CMainWindow::OnCommandShowMessageBox(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	if (m_pPopUpMessageBox == NULL)
		return;

	SallyAPI::GUI::SendMessage::CParameterMessageBox* parameter = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterMessageBox*> (messageParameter);

	if (parameter == NULL)
		return;

	m_pPopUpMessageBox->SetValues(*parameter);

	OnCommandShowPopUp(m_pPopUpMessageBox);
}

void CMainWindow::OnCommandShowQuestionBox(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	if (m_pPopUpQuestionBox == NULL)
		return;

	SallyAPI::GUI::SendMessage::CParameterQuestionBox* parameter = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterQuestionBox*> (messageParameter);

	if (parameter == NULL)
		return;

	m_pPopUpQuestionBox->SetValues(*parameter);

	OnCommandShowPopUp(m_pPopUpQuestionBox);
}

void CMainWindow::OnCommandHideDialogBox(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	if (reporter == this)
		return;

	OnCommandHidePopUp(reporter);
}

void CMainWindow::RenderControl()
{
	int x = (WINDOW_WIDTH - MAX_WIDTH) / 2;
	int y = (WINDOW_HEIGHT - MAX_HEIGHT) / 2;
	DrawImage(GUI_THEME_SALLY_BACKGROUND, x, y);

	SallyAPI::GUI::CWindow::RenderControl();
}

void CMainWindow::OnCommandDeleteInfoPopup(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	if (m_pPopUpInfo == NULL)
		return;

	SallyAPI::GUI::SendMessage::CParameterInteger* parameter = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterInteger*> (messageParameter);

	if (parameter == NULL)
		return;

	m_pPopUpInfo->RemoveItem(parameter->GetInteger());
}

void CMainWindow::OnCommandShowInfoPopup(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	if (m_pPopUpInfo == NULL)
		return;

	SallyAPI::GUI::SendMessage::CParameterInfoPopup* parameter = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterInfoPopup*> (messageParameter);

	if (parameter == NULL)
		return;

	int id = m_pPopUpInfo->AddItem(*parameter);
	parameter->SetId(id);
}

void CMainWindow::OnCommandScreenMenu(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	if (m_pPopUpOnScreenMenu == NULL)
		return;

	SallyAPI::GUI::SendMessage::CParameterOnScreenMenu* parameter = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterOnScreenMenu*> (messageParameter);

	if (parameter == NULL)
		return;

	m_pPopUpOnScreenMenu->SetAlphaBlending(0);
	m_pPopUpOnScreenMenu->BlendIn();
	m_pPopUpOnScreenMenu->Visible(true);

	m_fOnScreenMenuDeltaStart = 0;
	m_pPopUpOnScreenMenu->SetImageId(parameter->GetIcon());
	m_pPopUpOnScreenMenu->SetText(parameter->GetText());
}

void CMainWindow::OnCommandChangeApp(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	SallyAPI::GUI::SendMessage::CParameterInteger* parameterInteger = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterInteger*> (messageParameter);

	if (parameterInteger == NULL)
		return;

	if (m_pCurrentWindow != NULL)
		m_pCurrentWindow->Visible(false);

	std::map<int, CApplicationWindow*>::iterator	it;

	for (it = m_mAppWindows.begin(); it != m_mAppWindows.end(); ++it)
	{
		CApplicationWindow* appWindow = it->second;

		if (appWindow->GetGraphicId() == parameterInteger->GetInteger())
		{
			m_pCurrentWindow = appWindow;

			// reset the facebookOffWindow
			m_pFacebookOffWindow = NULL;
			ShowApplicationWindow();
			m_pCurrentWindow->Enable(true);

			m_pMenuView->SendMessageToParent(this, parameterInteger->GetInteger(), MS_SALLY_CHANGE_APP, 0);
			m_pMenuView->ApplicationClicked(m_pCurrentWindow->GetGraphicId(), m_pCurrentWindow->GetAppName());
			break;
		}
	}
}

void CMainWindow::ShowApplicationWindow()
{
	// we are already at the facebook off window
	if (m_pCurrentWindow == m_pFacebookOff)
	{
		m_pCurrentWindow->Visible(true);
		return;
	}

	// no facebook needed
	if (!m_pCurrentWindow->IsFacebookNeeded())
	{
		m_pCurrentWindow->Visible(true);
		m_pFacebookOff->Visible(false);

		return;
	}

	SallyAPI::Facebook::CFacebookManager* facebookManager = SallyAPI::Facebook::CFacebookManager::GetInstance();

	// facebook on
	if (facebookManager->IsEnabled())
	{
		if (m_pFacebookOffWindow != NULL)
		{
			m_pCurrentWindow = m_pFacebookOffWindow; // restore window
			m_pFacebookOffWindow = NULL;
		}

		m_pCurrentWindow->Visible(true);
		m_pFacebookOff->Visible(false);
		
		return;
	}

	// facebook off
	if (m_pFacebookOffWindow == NULL)
	{
		m_pFacebookOffWindow = m_pCurrentWindow;
	}

	m_pCurrentWindow->Visible(false);
	m_pFacebookOff->Visible(true);

	m_pCurrentWindow = m_pFacebookOff;
}

bool CMainWindow::CharInputPressed(char c)
{
	if (m_pPopUpKeyboard == NULL)
		return false;

	m_tScreensaverTimer->Reset();

	if (!m_pPopUpKeyboard->IsVisible())
		return false;

	m_pPopUpKeyboard->CharInputPressed(c);
	return true;
}

bool CMainWindow::KeyDown(int c)
{
	m_tScreensaverTimer->Reset();

	// the keyboard is on
	if (m_pKeyboardReporter != NULL)
		return false;

	if (c == 17)
		return false;
	if (c == 16)
		return false;

	switch (c)
	{
	case SPECIAL_KEY_SWITCH_SCREENSAVER_1:
		if (m_pPopUpLockScreen->IsVisible())
			break;
		if ((m_pCurrentWindow != NULL) && (m_pCurrentWindow->HasScreensaver()))
			SwitchScreensaver(m_pCurrentWindow);
		else
			SwitchScreensaver();
		break;
	case SPECIAL_KEY_SWITCH_SCREENSAVER_2:
		if (m_pPopUpLockScreen->IsVisible())
			break;
		SwitchScreensaver();
		break;
	case SPECIAL_KEY_PAGE_UP: // Page Up
		if (m_pPopUpLockScreen->IsVisible())
			break;
		KeyPageUp();
		break;
	case SPECIAL_KEY_PAGE_DOWN: // Page Down
		if (m_pPopUpLockScreen->IsVisible())
			break;
		KeyPageDown();
		break;
	case SPECIAL_KEY_RECORD:
	case SPECIAL_KEY_PLAY:
	case SPECIAL_KEY_STOP:
	case SPECIAL_KEY_NEXT:
	case SPECIAL_KEY_PREVIOUS:
	case SPECIAL_KEY_SEEK_FORWARD:
	case SPECIAL_KEY_SEEK_BACKWARD:
	case SPECIAL_KEY_ENTER:
	case SPECIAL_KEY_SHUFFLE:
	case SPECIAL_KEY_INFO:
	case SPECIAL_KEY_ARROW_UP:
	case SPECIAL_KEY_ARROW_DOWN:
	case SPECIAL_KEY_ARROW_LEFT:
	case SPECIAL_KEY_ARROW_RIGHT:
		m_pCurrentWindow->SpecialKeyPressed(c);
		break;
	default:
		return false;
	}

	return true;
}

void CMainWindow::SwitchScreensaver(SallyAPI::GUI::CApplicationWindow* reporter)
{
	if (m_pScreensaverWindow != NULL)
		OnCommandStopScreensaver(); // Stop the Screensaver
	else if (reporter != NULL)
		OnCommandStartScreensaver(reporter);
	else
		OnCommandStartScreensaver(true);
}

void CMainWindow::KeyPageUp()
{
	if (m_pScreensaverWindow != NULL)
		return;
	if (!m_pCurrentWindow->IsEnabled())
		return;

	bool enabled = false;
	std::map<int, CApplicationWindow*>::iterator	it;

	for (it = m_mAppWindows.begin(); it != m_mAppWindows.end(); ++it)
	{
		CApplicationWindow* appWindow = it->second;

		if ((appWindow == m_pCurrentWindow) || (appWindow == m_pFacebookOffWindow))
		{
			if (m_pCurrentWindow != NULL)
			{
				m_pCurrentWindow->Visible(false);
				enabled = m_pCurrentWindow->IsEnabled();
			}


			++it;
			if (it == m_mAppWindows.end())
				m_pCurrentWindow = m_mAppWindows.begin()->second;
			else
				m_pCurrentWindow = it->second;

			// reset the facebookOffWindow
			m_pFacebookOffWindow = NULL;
			ShowApplicationWindow();
			m_pCurrentWindow->Enable(enabled);

			m_pMenuView->SendMessageToParent(this, m_pCurrentWindow->GetGraphicId(), MS_SALLY_CHANGE_APP);
			m_pMenuView->ApplicationClicked(m_pCurrentWindow->GetGraphicId(), m_pCurrentWindow->GetAppName());
			return;
		}
	}
	if (m_mAppWindows.size() > 0)
	{
		SallyAPI::GUI::SendMessage::CParameterInteger parameterInteger(APP_GRAPHIC_ID);
		OnCommandChangeApp(&parameterInteger);
	}
}

void CMainWindow::KeyPageDown()
{
	if (m_pScreensaverWindow != NULL)
		return;
	if (!m_pCurrentWindow->IsEnabled())
		return;

	bool enabled = false;
	std::map<int, CApplicationWindow*>::iterator	it;
	CApplicationWindow* last = NULL;

	for (it = m_mAppWindows.begin(); it != m_mAppWindows.end(); ++it)
	{
		CApplicationWindow* appWindow = it->second;

		if ((appWindow == m_pCurrentWindow) || (appWindow == m_pFacebookOffWindow))
		{
			// we are at the first entry?
			if (last == NULL)
			{
				// than go to the end of the days
				for (it = m_mAppWindows.begin(); it != m_mAppWindows.end(); ++it)
				{
					last = it->second;
				}
			}
			if (m_pCurrentWindow != NULL)
			{
				m_pCurrentWindow->Visible(false);
				enabled = m_pCurrentWindow->IsEnabled();
			}

			m_pCurrentWindow = last;

			// reset the facebookOffWindow
			m_pFacebookOffWindow = NULL;
			ShowApplicationWindow();
			m_pCurrentWindow->Enable(enabled);

			m_pMenuView->SendMessageToParent(this, m_pCurrentWindow->GetGraphicId(), MS_SALLY_CHANGE_APP);
			m_pMenuView->ApplicationClicked(m_pCurrentWindow->GetGraphicId(), m_pCurrentWindow->GetAppName());
			return;
		}
		last = appWindow;
	}
	if (m_mAppWindows.size() > 0)
	{
		SallyAPI::GUI::SendMessage::CParameterInteger parameterInteger(APP_GRAPHIC_ID);
		OnCommandChangeApp(&parameterInteger);
	}
}

void CMainWindow::Timer(float timeDelta)
{
	SallyAPI::GUI::CWindow::Timer(timeDelta);

	if (m_pPopUpOnScreenMenu == NULL)
		return;

	// OnScreenMenu Animation
	if ((m_pPopUpOnScreenMenu->IsVisible()) && (m_fOnScreenMenuDeltaStart != -1))
	{
		if (m_fOnScreenMenuDeltaStart == 0)
			m_fOnScreenMenuDeltaStart = m_fTimeDelta;

		if (m_fTimeDelta - m_fOnScreenMenuDeltaStart > 1.0)
		{
			m_pPopUpOnScreenMenu->BlendOut();
			m_fOnScreenMenuDeltaStart = -1;
		}
	}
}

void CMainWindow::OnCommandVoiceCommand(bool release, CApplicationWindow* set)
{
	static bool whaitForCommand = false;
	static CApplicationWindow*	voiceFocus = NULL;

	// release exclusive access
	if (release)
	{
		voiceFocus = NULL;
		return;
	}
	// set exclusive access
	if (set != NULL)
	{
		voiceFocus = set;
		return;
	}

	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance(); 
	SallyAPI::Config::CLanguageManager* lang = config->GetSpeechInputLocalization();

	std::string command = m_pVoiceInput->GetCommand();
	std::string commandCompare = SallyAPI::String::StringHelper::StringToUpper(command);

	if (voiceFocus != NULL)
	{
		if (!voiceFocus->VoiceCommandFocus(command))
		{
			voiceFocus = NULL;
		}
		return;
	}

	std::string listen = SallyAPI::String::StringHelper::StringToUpper(lang->GetString("listen"));
	if (commandCompare.compare(listen) == 0)
	{
		Beep(1000, 100);
		whaitForCommand = true;
		return;
	}
	else if (whaitForCommand == false)
	{
		return;
	}

	whaitForCommand = false;
	if (m_pCurrentWindow->VoiceCommandActive(command))
		return;

	std::map<int, CApplicationWindow*>::iterator	it;

	for (it = m_mAppWindows.begin(); it != m_mAppWindows.end(); ++it)
	{
		CApplicationWindow* appWindow = it->second;

		if (appWindow->VoiceCommand(command))
			return;
	}
	return;
}

void CMainWindow::OnCommandShowKeyboard(SallyAPI::GUI::CGUIBaseObject* reporter)
{
	if (m_pPopUpKeyboard == NULL)
		return;

	// do we have already a keyboard open?
	if (m_pKeyboardReporter != NULL)
		return;

	SallyAPI::GUI::CEdit* editControl = dynamic_cast<SallyAPI::GUI::CEdit*> (reporter);
	if (editControl == NULL)
		return;

	m_pPopUpKeyboard->SetData(editControl);
	m_pKeyboardReporter = reporter;

	// now show the keyboard
	OnCommandShowPopUp(m_pPopUpKeyboard);
}

void CMainWindow::OnCommandHideKeyboard()
{
	if (m_pKeyboardReporter == NULL)
		return;

	m_pKeyboardReporter->SendMessageToParent(m_pKeyboardReporter, 0, MS_SALLY_KEYBOARD_CLOSED);
	m_pKeyboardReporter = NULL;

	OnCommandHidePopUp(m_pPopUpKeyboard);
}

void CMainWindow::OnCommandLockWindow()
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	if (option->GetPropertyString("lock", "password", "").length() == 0)
	{
		SallyAPI::GUI::SendMessage::CParameterMessageBox sendMessageParameterMessageBox(0, this,
			"No PIN was configured in the options.\nPlease go to the settings and set a PIN there and than try again.", SallyAPI::GUI::MESSAGEBOX_ICON_WARNING);
		SendMessageToParent(this, 0, MS_SALLY_SHOW_MESSAGEBOX, &sendMessageParameterMessageBox);
		return;
	}

	OnCommandShowPopUp(m_pPopUpLockScreen);
	m_pCurrentWindow->Enable(false);

	if ((m_pCurrentWindow != NULL) && (m_pCurrentWindow->HasScreensaver()))
		SwitchScreensaver(m_pCurrentWindow);
	else
		SwitchScreensaver();
}

void CMainWindow::OnCommandUnlockWindow()
{
	OnCommandHidePopUp(m_pPopUpLockScreen);
	m_pCurrentWindow->Enable(true);
}

void CMainWindow::OnCommandShowVolumne()
{
	if (m_pPopUpVolume == NULL)
		return;

	m_pPopUpVolume->SetPopUpPoint(WINDOW_WIDTH - 98, 38);
	m_pPopUpVolume->UpdateView();

	OnCommandShowPopUp(m_pPopUpVolume);
}

void CMainWindow::OnCommandHideVolume()
{
	OnCommandHidePopUp(m_pPopUpVolume);
}

void CMainWindow::OnCommandControlBlended(SallyAPI::GUI::CGUIBaseObject* reporter)
{
	SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();

	SallyAPI::GUI::CPopUpWindow* popUpWindow = dynamic_cast<SallyAPI::GUI::CPopUpWindow*> (reporter);

	if (popUpWindow == NULL)
		return;

	// check if it is in the list
	bool found = false;
	std::list<SallyAPI::GUI::CControl*>::iterator iter = m_lGUIControlPopUp.begin();
	while (iter !=  m_lGUIControlPopUp.end())
	{
		SallyAPI::GUI::CControl* control = *iter;

		if (popUpWindow == control)
		{
			iter = m_lGUIControlPopUp.end();
			found = true;
		}
		else
		{
			++iter;
		}
	}

	if (!found)
		return;

	if (popUpWindow->GetPositionX() != 0)
		return;

	SallyAPI::System::CAutoLock lock(&m_PopUpAnimationCritSection);

	// blend in
	if (popUpWindow->GetAlphaBlending() == 255)
	{
// 		std::string info = "### PopUp Window Moved - Blend In - ";
// 		info.append(SallyAPI::String::StringHelper::ConvertToString(m_vPopUpWindowsList.size()));
// 		logger->Debug(info);

		if (popUpWindow == m_pCurrentPopUpWindow)
		{
			popUpWindow->Enable(true);
		}
		return;
	}
	else if (popUpWindow->GetAlphaBlending() == 0)
	{
		popUpWindow->Visible(false);

// 		// blend out
// 		std::string info = "### PopUp Window Moved - Blend Out - ";
// 		info.append(SallyAPI::String::StringHelper::ConvertToString(m_vPopUpWindowsList.size()));
// 		logger->Debug(info);

		if (m_vPopUpWindowsList.size() > 0)
		{
			int position = 1;
			int oldSize = m_vPopUpWindowsList.size();
			std::vector<SallyAPI::GUI::CPopUpWindow*>::iterator iter = m_vPopUpWindowsList.begin();
			while (iter !=  m_vPopUpWindowsList.end())
			{
				SallyAPI::GUI::CPopUpWindow* control = *iter;

				if (popUpWindow == control)
				{
					m_vPopUpWindowsList.erase(iter);
					iter = m_vPopUpWindowsList.end();
				}
				else
				{
					++position;
					++iter;
				}
			}

			if ((m_vPopUpWindowsList.size() > 0) && (position == oldSize))
			{
				m_pCurrentPopUpWindow = m_vPopUpWindowsList[m_vPopUpWindowsList.size() - 1];
				m_pCurrentPopUpWindow->Enable(true);
			}
		}
		if (m_vPopUpWindowsList.size() == 0)
		{
			m_pMenuView->Enable(true);
			m_pCurrentWindow->Enable(true);
		}
		return;
	}
	//logger->Debug("### FUCK ???!!!!");
	return;
}

void CMainWindow::OnCommandShowPopUp(SallyAPI::GUI::CGUIBaseObject* reporter)
{
	if (reporter == NULL)
		return;

	SallyAPI::GUI::CPopUpWindow* popUpWindow = dynamic_cast<SallyAPI::GUI::CPopUpWindow*> (reporter);

	if (popUpWindow == NULL)
		return;

	SallyAPI::System::CAutoLock lock(&m_PopUpAnimationCritSection);

	if (m_pCurrentPopUpWindow != NULL)
		m_pCurrentPopUpWindow->Enable(false);

	m_pCurrentPopUpWindow = popUpWindow;

	m_vPopUpWindowsList.push_back(m_pCurrentPopUpWindow);

	m_pMenuView->Enable(false);
	m_pCurrentWindow->Enable(false);
	m_pCurrentPopUpWindow->Enable(false);
	m_pCurrentPopUpWindow->Visible(true);

	m_pCurrentPopUpWindow->BlendIn();
}

void CMainWindow::OnCommandHidePopUp(SallyAPI::GUI::CGUIBaseObject* reporter)
{
	if (reporter == NULL)
		return;

	SallyAPI::GUI::CPopUpWindow* popUpWindow = dynamic_cast<SallyAPI::GUI::CPopUpWindow*> (reporter);

	if (popUpWindow == NULL)
		return;

	SallyAPI::System::CAutoLock lock(&m_PopUpAnimationCritSection);

	popUpWindow->Enable(false);
	popUpWindow->BlendOut();
}

void CMainWindow::OnCommandFacebookUpdateInfo()
{
	SallyAPI::Facebook::CFacebookManager* facebookManager = SallyAPI::Facebook::CFacebookManager::GetInstance();

	if ((facebookManager->UpdateFacebookUserInfo() == false) && (facebookManager->IsEnabled()))
		facebookManager->ShowErrorMessage(this);
}

void CMainWindow::OnCommandFacebookGetStatusMessages()
{
	SallyAPI::Facebook::CFacebookManager* facebookManager = SallyAPI::Facebook::CFacebookManager::GetInstance();
	SallyAPI::Facebook::CFacebookDB* facebookDB = SallyAPI::Facebook::CFacebookDB::GetInstance();

	if ((facebookDB->UpdateStatusMessages(this) == false) && (facebookManager->IsEnabled()))
		facebookManager->ShowErrorMessage(this);
}

void CMainWindow::OnCommandStartScreensaver(bool checkPopUp)
{
	// is the screensaver already running?
	if (m_pScreensaverWindow != NULL)
		return;

	// do we have a popup open?
	if (checkPopUp)
	{
		if ((m_pMenuView->IsEnabled() == false) && (m_pPopUpLockScreen->IsVisible() == false))
			return;
	}

	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	std::string screensaverName = option->GetPropertyString("sally", "screensaver");

	if (screensaverName.length() == 0)
	{
		if ((!checkPopUp) && (m_pPopUpLockScreen->IsVisible() == false))
		{
			SallyAPI::GUI::SendMessage::CParameterMessageBox sendMessageParameterMessageBox(0, this,
				"The screensaver is disabled.\nPlease go to the settings to select a screensaver.", SallyAPI::GUI::MESSAGEBOX_ICON_WARNING);
			SendMessageToParent(this, 0, MS_SALLY_SHOW_MESSAGEBOX, &sendMessageParameterMessageBox);
			return;
		}
	}

	std::map<int, CApplicationWindow*>::iterator iter;

	iter = m_mAppWindows.begin();

	while (iter != m_mAppWindows.end())
	{
		CApplicationWindow* temp = iter->second;

		if (temp->GetExplicitAppName().compare(screensaverName) == 0)
		{
			if (temp->ActivateScreensaver() == false)
			{
				SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();

				logger->Debug("Screensaver start denied");

				if ((!checkPopUp) && (m_pPopUpLockScreen->IsVisible() == false))
				{
					SallyAPI::GUI::SendMessage::CParameterMessageBox sendMessageParameterMessageBox(0, this,
						"The selected screensaver could not start.\nPlease go to the config and check the settings for the selected screensaver.", SallyAPI::GUI::MESSAGEBOX_ICON_WARNING);
					SendMessageToParent(this, 0, MS_SALLY_SHOW_MESSAGEBOX, &sendMessageParameterMessageBox);
				}
				return;
			}

			m_pMenuView->MoveAnimated(m_pMenuView->GetPositionX(), -MENU_HEIGHT, 600);

			m_pCurrentWindow->Visible(false);
			m_pCurrentWindow->Enable(false);

			m_pScreensaverWindow = m_pCurrentWindow;

			m_pCurrentWindow = temp;

			// reset the facebookOffWindow
			m_pFacebookOffWindow = NULL;
			ShowApplicationWindow();

			if (m_pPopUpLockScreen->IsVisible())
				m_pCurrentWindow->Enable(false);
			else
				m_pCurrentWindow->Enable(true);
			return;
		}
		++iter;
	}

	if ((!checkPopUp) && (m_pPopUpLockScreen->IsVisible() == false))
	{
		SallyAPI::GUI::SendMessage::CParameterMessageBox sendMessageParameterMessageBox(0, this,
			"The selected screensaver could not be found.\nPlease go to the settings to select a screensaver.", SallyAPI::GUI::MESSAGEBOX_ICON_WARNING);
		SendMessageToParent(this, 0, MS_SALLY_SHOW_MESSAGEBOX, &sendMessageParameterMessageBox);
	}

	// stop the timer
	m_tScreensaverTimer->Stop();
}

void CMainWindow::OnCommandStartScreensaver(SallyAPI::GUI::CGUIBaseObject* reporter)
{
	// is the screensaver already running?
	if (m_pScreensaverWindow != NULL)
		return;

	// do we have a popup open?
	if ((m_pMenuView->IsEnabled() == false) && (m_pPopUpLockScreen->IsVisible() == false))
		return;

	std::map<int, CApplicationWindow*>::iterator iter;

	iter = m_mAppWindows.begin();

	while (iter != m_mAppWindows.end())
	{
		CApplicationWindow* temp = iter->second;

		if (temp == reporter)
		{
			if (temp->ActivateScreensaver() == false)
			{
				SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();

				logger->Info("Screensaver start denied");
				return;
			}

			m_pMenuView->MoveAnimated(m_pMenuView->GetPositionX(), -MENU_HEIGHT, 600);

			m_pCurrentWindow->Visible(false);
			m_pCurrentWindow->Enable(false);

			m_pScreensaverWindow = m_pCurrentWindow;

			m_pCurrentWindow = temp;

			// reset the facebookOffWindow
			m_pFacebookOffWindow = NULL;
			ShowApplicationWindow();

			if (m_pPopUpLockScreen->IsVisible())
				m_pCurrentWindow->Enable(false);
			else
				m_pCurrentWindow->Enable(true);
			return;
		}
		++iter;
	}
	// stop the timer
	m_tScreensaverTimer->Stop();
}

void CMainWindow::OnCommandStopScreensaver()
{
	if (m_pScreensaverWindow == NULL)
		return;

	m_fOnScreenMenuDeltaStart = -1;
	m_pPopUpOnScreenMenu->Visible(false);

	m_pMenuView->MoveAnimated(m_pMenuView->GetPositionX(), 0, 600);

	m_pCurrentWindow->DeactivateScreensaver();
	m_pCurrentWindow->Visible(false);
	m_pCurrentWindow->Enable(false);

	m_pCurrentWindow = m_pScreensaverWindow;
	m_pScreensaverWindow = NULL;

	// reset the facebookOffWindow
	m_pFacebookOffWindow = NULL;
	ShowApplicationWindow();
	m_pCurrentWindow->Enable(true);

	// start the screensaver timer again
	StartScreensaverTimer();
}

bool CMainWindow::ProcessMouseDown(int x, int y)
{
	m_tScreensaverTimer->Reset();

	return SallyAPI::GUI::CWindow::ProcessMouseDown(x, y);
}

void CMainWindow::StartScreensaverTimer()
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	m_tScreensaverTimer->SetTimeout(option->GetPropertyInt("sally", "waitTime", 10) * 60);

	if (option->GetPropertyString("sally", "screensaver").compare("") == 0)
		return;

	m_tScreensaverTimer->Start();
}

void CMainWindow::OnCommandScheduler()
{
	SallyAPI::Scheduler::CSchedulerManager* schedulerManager = SallyAPI::Scheduler::CSchedulerManager::GetInstance(); 
	schedulerManager->CheckScheduler();
}