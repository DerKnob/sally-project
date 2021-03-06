////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	ImageBrowser\ScreensaverForm.cpp
///
/// \brief	Implements the screensaver Windows Form. 
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

#include "ScreensaverForm.h"

#define MAX_HISTORY		40

CScreensaverForm::CScreensaverForm(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int height)
	: SallyAPI::GUI::CForm(parent, x, y, width, height, 0), m_pTimerDiashow(NULL)
{
	SetHandleInputIfItIsChildHandled(false);
	this->SetScrollType(SallyAPI::GUI::SCROLL_TYPE_NORMAL);

	m_pApplicationWindow = (SallyAPI::GUI::CApplicationWindow*) parent;

	m_pInfoPopUp = new CInfoPopUp(m_pApplicationWindow, m_pApplicationWindow->GetGraphicId(), m_pApplicationWindow->GetExplicitAppName());
	m_pParent->SendMessageToParent(m_pInfoPopUp, 0, MS_SALLY_ADD_CHILD, 0);

	m_pPictureCurrent = NULL;
	m_pPictureOld = NULL;
	m_bScreensaverMode = true;
	m_bAnimationZoom = false;
	m_iPictureLoadDone = 0;

	m_pImageCurrent = new SallyAPI::GUI::CImageBox(this, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	this->AddChild(m_pImageCurrent);

	m_pImageOld = new SallyAPI::GUI::CImageBox(this, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	this->AddChild(m_pImageOld);

	// BottomMenu
	m_pFormScreensaverBottomMenu = new SallyAPI::GUI::CBottonMenu(this);
	m_pFormScreensaverBottomMenu->Move(0, WINDOW_HEIGHT);
	this->AddChild(m_pFormScreensaverBottomMenu);

	m_pMenuBar = new SallyAPI::GUI::CButtonBar(m_pFormScreensaverBottomMenu,
		WINDOW_WIDTH - 200 - WINDOW_BORDER_H, (MENU_HEIGHT - CONTROL_HEIGHT) / 2, 200);
	m_pFormScreensaverBottomMenu->AddChild(m_pMenuBar);

	m_pInfo = new SallyAPI::GUI::CButtonBarButton(m_pMenuBar, 100, GUI_APP_SHOW_INFO);
	m_pInfo->SetImageId(GUI_THEME_SALLY_ICON_INFO);
	m_pInfo->SetText("Info");
	m_pMenuBar->AddChild(m_pInfo);

	m_pShuffle = new SallyAPI::GUI::CButtonBarButton(m_pMenuBar, 100, GUI_APP_SHUFFLE);
	m_pShuffle->SetImageId(GUI_THEME_SALLY_ICON_SHUFFLE);
	m_pShuffle->SetText("Shuffle");
	m_pMenuBar->AddChild(m_pShuffle);

	m_pEditTimer = new SallyAPI::GUI::CNumberSelector(m_pFormScreensaverBottomMenu, WINDOW_BORDER_H, (MENU_HEIGHT - CONTROL_HEIGHT) / 2, 120);
	m_pEditTimer->SetMaxValue(86400);
	m_pEditTimer->SetMinValue(5);
	m_pEditTimer->SetSteps(30);
	m_pFormScreensaverBottomMenu->AddChild(m_pEditTimer);

	m_pLabelTimer = new SallyAPI::GUI::CLabel(m_pFormScreensaverBottomMenu, WINDOW_BORDER_H + 120 + 5, (MENU_HEIGHT - CONTROL_HEIGHT) / 2, 50);
	m_pLabelTimer->SetText("sec.");
	m_pLabelTimer->SetDrawBackground(true);
	m_pLabelTimer->SetAlign(DT_CENTER | DT_VCENTER);
	m_pFormScreensaverBottomMenu->AddChild(m_pLabelTimer);

	m_pDropDownChangeType = new SallyAPI::GUI::CDropDown(m_pFormScreensaverBottomMenu, WINDOW_BORDER_H + 120 + 10 + 40 + 10, (MENU_HEIGHT - CONTROL_HEIGHT) / 2, 120);
	m_pFormScreensaverBottomMenu->AddChild(m_pDropDownChangeType);

	SallyAPI::GUI::CDropDownItem itemSlideIn("SlideIn", "Slide In");
	m_pDropDownChangeType->AddItem(itemSlideIn);

	SallyAPI::GUI::CDropDownItem itemSlideZoom("Zoom", "Zoom");
	m_pDropDownChangeType->AddItem(itemSlideZoom);

// 	SallyAPI::GUI::CDropDownItem itemSlideCollage("Collage", "Collage");
// 	m_pDropDownChangeType->AddItem(itemSlideCollage);

	// pressed Notifier
	m_pScreensaverFormNotifier = new SallyAPI::GUI::CForm(this, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	m_pScreensaverFormNotifier->SetHandleInputIfItIsChildHandled(false);
	m_pScreensaverFormNotifier->SetScrollType(SallyAPI::GUI::SCROLL_TYPE_NORMAL);
	this->AddChild(m_pScreensaverFormNotifier);

	// create the timer thread
	m_pTimerDiashow = new SallyAPI::GUI::CTimer(60, this, GUI_APP_NEXT_SCREENSAVER_TIMER, GUI_BUTTON_CLICKED);
	m_pTimerHideMenu = new SallyAPI::GUI::CTimer(10, m_pScreensaverFormNotifier, 0, GUI_FORM_CLICKED);

	m_vImageListCurrent = &m_vImageListScreensaver;

	/************************************************************************/
	/* Helper                                                               */
	/************************************************************************/
	m_pScreensaverControl = new SallyAPI::GUI::CScreensaverControl(this, m_pApplicationWindow);
	m_pParent->SendMessageToParent(m_pScreensaverControl, 0, MS_SALLY_ADD_SCREENSAVER_CONTROL);

	SallyAPI::GUI::CScreensaverControlButton* screensaverControl = new SallyAPI::GUI::CScreensaverControlButton(m_pScreensaverControl, GUI_APP_PREVIOUS_SCREENSAVER);
	screensaverControl->SetImageId(GUI_THEME_SALLY_ICON_MEDIA_SKIP_BACKWARD);
	m_pScreensaverControl->AddChild(screensaverControl);

	m_pScreensaverControlButtonPlay = new SallyAPI::GUI::CScreensaverControlButton(m_pScreensaverControl, GUI_APP_PLAY);
	m_pScreensaverControlButtonPlay->SetImageId(GUI_THEME_SALLY_ICON_MEDIA_PLAY);
	m_pScreensaverControl->AddChild(m_pScreensaverControlButtonPlay);

	screensaverControl = new SallyAPI::GUI::CScreensaverControlButton(m_pScreensaverControl, GUI_APP_NEXT_SCREENSAVER);
	screensaverControl->SetImageId(GUI_THEME_SALLY_ICON_MEDIA_SKIP_FORWARD);
	m_pScreensaverControl->AddChild(screensaverControl);
}

CScreensaverForm::~CScreensaverForm()
{
	m_pTimerHideMenu->WaitForStop();
	m_pTimerDiashow->WaitForStop();
	SafeDelete(m_pTimerHideMenu);
	SafeDelete(m_pTimerDiashow);
}

/************************************************************************/
/* Screensaver start helper                                             */
/************************************************************************/
void CScreensaverForm::SetDiashowValues(std::vector<std::string>* imageList, bool screensaverActive, int timeOut,
										bool shuffle, const std::string& type)
{
	m_vImageListCurrent = imageList;
	m_bScreensaverMode = screensaverActive;

	m_pEditTimer->SetValue(timeOut);

	m_pShuffle->SetCheckStatus(shuffle);

	m_pDropDownChangeType->SelectItemByIdentifier(type);
	OnCommandTpyeChanged();
}

bool CScreensaverForm::ActivateScreensaver()
{
	m_vImageListScreensaver.clear();
	m_vImageListSmartShuffle.clear();
	m_iCurrentPictureScreensaver = -1;
	m_stdCurrentPictureScreensaver = "";

	if (m_vImageListCurrent->size() == 0)
	{
		// first time screensaver is started
		CreateImagesScreensaverList();

		if (m_vImageListScreensaver.size() == 0)
			return false;
	}

	m_pTimerDiashow->Reset();
	m_pTimerDiashow->Start();
	m_pScreensaverControlButtonPlay->SetImageId(GUI_THEME_SALLY_ICON_MEDIA_PAUSE);

	// diashow is started
	OnCommandNextImageScreensaver();
	return true;
}

bool CScreensaverForm::DeactivateScreensaver()
{
	m_pTimerDiashow->Stop();

	EnterRenderLock();
	m_pImageCurrent->SetPicture(NULL);
	m_pImageOld->SetPicture(NULL);
	LeaveRenderLock();

	SafeDelete(m_pPictureCurrent);
	SafeDelete(m_pPictureOld);

	// reset to default
	m_bScreensaverMode = true;

	m_pShuffle->SetCheckStatus(m_pApplicationWindow->GetPropertyBool("screensaverShuffle", true));
	
	int timeOut = m_pApplicationWindow->GetPropertyInt("screensaverTimeout", 30);
	m_pEditTimer->SetValue(timeOut);

	std::string type = m_pApplicationWindow->GetPropertyString("screensaverType", "SlideIn");
	m_pDropDownChangeType->SelectItemByIdentifier(type);
	OnCommandTpyeChanged();
	
	m_vImageListCurrent = &m_vImageListScreensaver;

	// stop the menu hide timer
	m_pTimerHideMenu->Stop();

	// hide screensaver menus
	OnCommandHideMenu();
	return true;
}

/************************************************************************/
/* OnCommand                                                            */
/************************************************************************/
void CScreensaverForm::OnCommandScreensaverPause()
{
	SallyAPI::GUI::SendMessage::CParameterNotificationOSM messageOnScreenMenu(GUI_THEME_SALLY_OSM_PAUSE, "Pause");
	m_pParent->SendMessageToParent(this, 0, MS_SALLY_NOTIFICATION_OSM, &messageOnScreenMenu);
	m_pTimerDiashow->Stop();

	m_pScreensaverControlButtonPlay->SetImageId(GUI_THEME_SALLY_ICON_MEDIA_PLAY);
}

void CScreensaverForm::OnCommandScreensaverPlay()
{
	SallyAPI::GUI::SendMessage::CParameterNotificationOSM messageOnScreenMenu(GUI_THEME_SALLY_OSM_PLAY, "Play");
	m_pParent->SendMessageToParent(this, 0, MS_SALLY_NOTIFICATION_OSM, &messageOnScreenMenu);

	m_pTimerDiashow->Reset();
	m_pTimerDiashow->Start();

	m_pScreensaverControlButtonPlay->SetImageId(GUI_THEME_SALLY_ICON_MEDIA_PAUSE);	
}

void CScreensaverForm::OnCommandShowMenu()
{
	m_pFormScreensaverBottomMenu->MoveAnimated(0, WINDOW_HEIGHT - MENU_HEIGHT, 400, false);

	SallyAPI::GUI::SendMessage::CParameterRect paramterRect;
	m_pParent->SendMessageToParent(m_pApplicationWindow, 0, MS_SALLY_SCREENSAVER_SHOW_MENU, &paramterRect);

	RECT rect = paramterRect.GetRect();

	m_pTimerHideMenu->Reset();
	m_pTimerHideMenu->Start();

	m_pScreensaverFormNotifier->Move(rect.left + rect.right, MENU_HEIGHT);
	m_pScreensaverFormNotifier->Resize(WINDOW_WIDTH - (rect.left + rect.right), WINDOW_HEIGHT - m_pFormScreensaverBottomMenu->GetHeight() - MENU_HEIGHT);
}

void CScreensaverForm::OnCommandHideMenu()
{
	m_pFormScreensaverBottomMenu->MoveAnimated(0, WINDOW_HEIGHT, 400, false);
	m_pParent->SendMessageToParent(m_pApplicationWindow, 0, MS_SALLY_SCREENSAVER_HIDE_MENU);
	
	m_pTimerHideMenu->Stop();

	m_pScreensaverFormNotifier->Move(0, 0);
	m_pScreensaverFormNotifier->Resize(WINDOW_WIDTH, WINDOW_HEIGHT);
}

void CScreensaverForm::OnCommandScreensaverUp()
{
	SendMessageToParent(this, GUI_APP_SHUFFLE, GUI_BUTTON_CLICKED);
	m_pTimerDiashow->Reset();
}

void CScreensaverForm::OnCommandScreensaverDown()
{
	m_pApplicationWindow->SendMessageToParent(m_pApplicationWindow, m_iControlId, MS_SALLY_SCREENSAVER_STOP);
}

void CScreensaverForm::OnCommandScreensaverLeft()
{
	if ((m_tImageLoaderScreensaver.GetStatus() == SallyAPI::System::THREAD_RUNNING) || (m_pPictureOld != NULL))
		return;

	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	bool result = option->GetPropertyBool("sally", "leftEQprevious", true);

	if (result)
	{
		OnCommandScreensaverPrevious();
	}
	else
	{
		OnCommandScreensaverNext();
	}
	m_pTimerDiashow->Reset();
}

void CScreensaverForm::OnCommandScreensaverRight()
{
	if ((m_tImageLoaderScreensaver.GetStatus() == SallyAPI::System::THREAD_RUNNING) || (m_pPictureOld != NULL))
		return;

	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	bool result = option->GetPropertyBool("sally", "leftEQprevious", true);

	if (result)
	{
		OnCommandScreensaverNext();
	}
	else
	{
		OnCommandScreensaverPrevious();
	}
	m_pTimerDiashow->Reset();
}

void CScreensaverForm::OnCommandScreensaverPrevious()
{
	OnCommandPreviousImageScreensaver();

	SallyAPI::GUI::SendMessage::CParameterNotificationOSM messageOnScreenMenu(GUI_THEME_SALLY_OSM_PREVIOUS, "Previous");
	m_pParent->SendMessageToParent(this, 0, MS_SALLY_NOTIFICATION_OSM, &messageOnScreenMenu);
}

void CScreensaverForm::OnCommandScreensaverNext()
{
	OnCommandNextImageScreensaver();

	SallyAPI::GUI::SendMessage::CParameterNotificationOSM messageOnScreenMenu(GUI_THEME_SALLY_OSM_NEXT, "Next");
	m_pParent->SendMessageToParent(this, 0, MS_SALLY_NOTIFICATION_OSM, &messageOnScreenMenu);
}

void CScreensaverForm::OnCommandNextImageScreensaver()
{
	if (m_vImageListCurrent->size() == 0)
		return;

	// for the history if not already in
	std::vector<std::string>::iterator historySearch = std::find(m_vHistoryList.begin(), m_vHistoryList.end(), m_stdCurrentPictureScreensaver);
	if (historySearch != m_vHistoryList.end())
		m_vHistoryList.erase(historySearch);
	
	m_vHistoryList.push_back(m_stdCurrentPictureScreensaver);

	// cleanup the history ... remove old values
	int maxHistory = MAX_HISTORY;
	if (m_vImageListCurrent->size() < maxHistory)
		maxHistory = m_vImageListCurrent->size();

	if (m_vHistoryList.size() > maxHistory)
		m_vHistoryList.erase(m_vHistoryList.begin());

	// Do we have to reset the smart shuffle?
	if (m_vImageListSmartShuffle.size() == 0)
		m_vImageListSmartShuffle = (*m_vImageListCurrent);

	if (m_pShuffle->GetCheckStatus())
	{
		int randTrack = SallyAPI::System::CNumberGenerator::GetNumber(0, m_vImageListSmartShuffle.size() - 1);

		m_stdCurrentPictureScreensaver = m_vImageListSmartShuffle.at(randTrack);

		//////////////////////////////////////////////////////////////////////////
		// get the real postion number
		std::vector<std::string>::iterator iter = m_vImageListCurrent->begin();
		for (int i = 0; i < m_vImageListCurrent->size(); ++i)
		{
			if (m_stdCurrentPictureScreensaver.compare(*iter) == 0)
			{
				m_iCurrentPictureScreensaver = i;
				break;
			}
			++iter;
		}		
	}
	else
	{
		//////////////////////////////////////////////////////////////////////////
		// get to the next picture
		m_iCurrentPictureScreensaver++;
		if (m_iCurrentPictureScreensaver > (int) m_vImageListCurrent->size() - 1)
			m_iCurrentPictureScreensaver = 0;

		m_stdCurrentPictureScreensaver = m_vImageListCurrent->at(m_iCurrentPictureScreensaver);
	}

	m_pTimerDiashow->Reset();
	StartLoadImage(m_stdCurrentPictureScreensaver, true);

	// remove from smart shuffle
	RemoveAsPlayedFromSmartShuffle(m_stdCurrentPictureScreensaver);
}

void CScreensaverForm::RemoveAsPlayedFromSmartShuffle(const std::string& number)
{
	std::vector<std::string>::iterator iter = m_vImageListSmartShuffle.begin();
	std::vector<std::string>::iterator iterToDelete = m_vImageListSmartShuffle.end();
	while ((iter != m_vImageListSmartShuffle.end()) && (iterToDelete == m_vImageListSmartShuffle.end()))
	{
		std::string fileName = *iter;

		if (m_stdCurrentPictureScreensaver.compare(fileName) == 0)
		{
			iterToDelete = iter;
		}
		++iter;
	}
	if (iterToDelete != m_vImageListSmartShuffle.end())
		m_vImageListSmartShuffle.erase(iterToDelete);
}

void CScreensaverForm::OnCommandPreviousImageScreensaver()
{
	if (m_vImageListCurrent->size() == 0)
		return;

	// only if we have a history AND shuffle is enabled [SA-00345]
	if ((m_vHistoryList.size() > 0) && (m_pShuffle->GetCheckStatus()))
	{
		int i = m_vHistoryList.size() - 1;
		m_stdCurrentPictureScreensaver = m_vHistoryList.at(i);
		m_vHistoryList.erase(m_vHistoryList.begin() + i);
		std::vector<std::string>::iterator iter = std::find(m_vImageListCurrent->begin(),
			m_vImageListCurrent->end(), m_stdCurrentPictureScreensaver);

		if (iter != m_vImageListCurrent->end()) {
			m_iCurrentPictureScreensaver = iter - m_vImageListCurrent->begin();
		}
	}
	else
	{
		m_iCurrentPictureScreensaver--;
		if (m_iCurrentPictureScreensaver < 0)
			m_iCurrentPictureScreensaver = m_vImageListCurrent->size() - 1;
		
		m_stdCurrentPictureScreensaver = m_vImageListCurrent->at(m_iCurrentPictureScreensaver);
	}

	m_pTimerDiashow->Reset();
	StartLoadImage(m_stdCurrentPictureScreensaver, false);
}

void CScreensaverForm::OnCommandSwitchShuffle()
{
	if (m_pShuffle->GetCheckStatus())
	{
		SallyAPI::GUI::SendMessage::CParameterNotificationOSM messageOnScreenMenu(GUI_THEME_SALLY_OSM_SHUFFLEOFF, "Shuffle Off");
		m_pParent->SendMessageToParent(this, 0, MS_SALLY_NOTIFICATION_OSM, &messageOnScreenMenu);

		m_pShuffle->SetCheckStatus(false);

		if (!m_bScreensaverMode)
			m_pApplicationWindow->SetPropertyBool("diashowShuffle", false);
		else
			m_pApplicationWindow->SetPropertyBool("screensaverShuffle", false);
	}
	else
	{
		SallyAPI::GUI::SendMessage::CParameterNotificationOSM messageOnScreenMenu(GUI_THEME_SALLY_OSM_SHUFFLEON, "Shuffle On");
		m_pParent->SendMessageToParent(this, 0, MS_SALLY_NOTIFICATION_OSM, &messageOnScreenMenu);

		m_pShuffle->SetCheckStatus(true);

		if (!m_bScreensaverMode)
			m_pApplicationWindow->SetPropertyBool("diashowShuffle", true);
		else
			m_pApplicationWindow->SetPropertyBool("screensaverShuffle", true);
	}
}

void CScreensaverForm::OnCommandTimeoutChanged()
{
	m_pTimerDiashow->SetTimeout(m_pEditTimer->GetValue());

	if (!m_bScreensaverMode)
		m_pApplicationWindow->SetPropertyInt("diashowTimeout", m_pEditTimer->GetValue());
	else
		m_pApplicationWindow->SetPropertyInt("screensaverTimeout", m_pEditTimer->GetValue());
}

void CScreensaverForm::OnCommandTpyeChanged()
{
	/************************************************************************/
	/* What shall we do with the drunken sailor                             */
	/************************************************************************/
	std::string type = m_pDropDownChangeType->GetSelectedIdentifier();
	if (type.compare("SlideIn") == 0)
	{
		m_bAnimationZoom = false;

		if (m_pImageCurrent->GetPicture() != NULL)
		{
			int width;
			int height;

			SallyAPI::GUI::GUIHelper::CalculateImageSize(m_pPictureCurrent->GetWidth(), m_pPictureCurrent->GetHeight(),
				WINDOW_WIDTH, WINDOW_HEIGHT, m_iCurrentImageX, m_iCurrentImageY, width, height);

			m_pImageCurrent->Resize(width, height);
			m_pImageCurrent->Move(m_iCurrentImageX, m_iCurrentImageY);
		}
	}
	else if (type.compare("Zoom") == 0)
	{
		m_bAnimationZoom = true;
		CalculateMoveZoom();
	}
	else if (type.compare("Collage") == 0)
	{
	}
	/************************************************************************/

	if (!m_bScreensaverMode)
		m_pApplicationWindow->SetPropertyString("diashowType", type);
	else
		m_pApplicationWindow->SetPropertyString("screensaverType", type);
}

/************************************************************************/
/* Overwritten                                                          */
/************************************************************************/
void CScreensaverForm::LoadConfig()
{
	if (!this->IsVisible())
	{
		// set default parameter
		int timeOut = m_pApplicationWindow->GetPropertyInt("screensaverTimeout", 10);
		m_pEditTimer->SetValue(timeOut);

		m_pShuffle->SetCheckStatus(m_pApplicationWindow->GetPropertyBool("screensaverShuffle", true));

		std::string type = m_pApplicationWindow->GetPropertyString("screensaverType", "SlideIn");
		m_pDropDownChangeType->SelectItemByIdentifier(type);
		OnCommandTpyeChanged();
	}
}

bool CScreensaverForm::ProcessMouseUp(int x, int y)
{
	// reset the timer for the screensaver menu auto hide
	m_pTimerHideMenu->Reset();

	return SallyAPI::GUI::CForm::ProcessMouseUp(x, y);
}

void CScreensaverForm::RenderControl()
{
	DrawImage(GUI_THEME_SALLY_BLACK_BACKGROUND, 0, 0);
	SallyAPI::GUI::CForm::RenderControl();
}

void CScreensaverForm::DeleteOldImage()
{
	EnterRenderLock();
	m_pImageOld->SetPicture(NULL);
	LeaveRenderLock();

	SafeDelete(m_pPictureOld);
}

void CScreensaverForm::Timer(float timeDelta)
{
	SallyAPI::GUI::CForm::Timer(timeDelta);

	/************************************************************************/
	/* What shall we do with the drunken sailor                             */
	/************************************************************************/
	std::string type = m_pDropDownChangeType->GetSelectedIdentifier();
	if (type.compare("SlideIn") == 0)
	{
	}
	else if (type.compare("Zoom") == 0)
	{
		if (m_iPictureLoadDone == 11)
			PictureLoaded();
	}
	else if (type.compare("Collage") == 0)
	{
	}
	/************************************************************************/
}

void CScreensaverForm::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId,
										   SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (reporterId)
	{
	case GUI_APP_NEXT_SCREENSAVER_TIMER:
		OnCommandNextImageScreensaver();
		return;
	}
	switch (messageId)
	{
	case GUI_SCREENSAVER_CONTROL_CLICKED:
		switch (reporterId)
		{
		case GUI_APP_PREVIOUS_SCREENSAVER:
			OnCommandScreensaverPrevious();
			return;
		case GUI_APP_NEXT_SCREENSAVER:
			OnCommandScreensaverNext();
			return;
		case GUI_APP_PLAY:
			if (m_pTimerDiashow->GetStatus() == SallyAPI::System::THREAD_RUNNING)
				OnCommandScreensaverPause();
			else
				OnCommandScreensaverPlay();
			return;
		}
		return;
	case GUI_CONTROL_BLENDED:
		m_iPictureLoadDone = m_iPictureLoadDone | 10;
		return;
	case GUI_CONTROL_MOVED:
		if (reporter == m_pImageOld)
			DeleteOldImage();
		return;
	case GUI_CONTROL_RESIZED:
		if (m_bAnimationZoom)
			CalculateMoveZoom();
		return;
	case GUI_APP_SCREENSAVER_PIC_LOADED:
		OnCommandPictureLoaded();
		return;
	case GUI_NUMBER_SELECTOR_CHANGED:
		OnCommandTimeoutChanged();
		return;
	case GUI_DROPDOWN_CHANGED:
		OnCommandTpyeChanged();
		return;
	case GUI_BUTTON_CLICKED:
		switch (reporterId)
		{
		case GUI_APP_SHOW_INFO:
			OnCommandHideMenu();
			m_pParent->SendMessageToParent(m_pInfoPopUp, 0, MS_SALLY_SHOW_POPUP_VIEW, 0);
			return;
		case GUI_APP_SHUFFLE:
			OnCommandSwitchShuffle();
			return;
		case GUI_APP_PLAY:
			if (m_pTimerDiashow->GetStatus() == SallyAPI::System::THREAD_RUNNING)
				OnCommandScreensaverPause();
			else
				OnCommandScreensaverPlay();
			return;
		case GUI_APP_NEXT_SCREENSAVER:
			OnCommandScreensaverNext();
			return;
		case GUI_APP_PREVIOUS_SCREENSAVER:
			OnCommandScreensaverPrevious();
			return;
		}
		break;
	}
	if (reporter == m_pScreensaverFormNotifier)
	{
		switch(messageId)
		{
		case GUI_FORM_CLICKED:
			if (m_pScreensaverFormNotifier->GetPositionY() == 0)
				OnCommandShowMenu();
			else
				OnCommandHideMenu();
			return;
		}
	}
	if (reporter == this)
	{
		SallyAPI::GUI::SendMessage::CParameterPoint* parameter = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterPoint*> (messageParameter);

		switch(messageId)
		{
		case GUI_FORM_DOUBLECLICKED:
			if (m_pTimerDiashow->GetStatus() == SallyAPI::System::THREAD_RUNNING)
				OnCommandScreensaverPause();
			else
				OnCommandScreensaverPlay();
			return;
		case GUI_MOUSEMOVE_LEFT_FAST:
		case GUI_MOUSEMOVE_LEFT:
			if ((parameter->GetY() > MENU_HEIGHT) && (parameter->GetY() < WINDOW_HEIGHT - MENU_HEIGHT))
				OnCommandScreensaverLeft();
			return;
		case GUI_MOUSEMOVE_RIGHT_FAST:
		case GUI_MOUSEMOVE_RIGHT:
			if ((parameter->GetY() > MENU_HEIGHT) && (parameter->GetY() < WINDOW_HEIGHT - MENU_HEIGHT))
				OnCommandScreensaverRight();
			return;
		case GUI_MOUSEMOVE_DOWN_FAST:
		case GUI_MOUSEMOVE_DOWN:
			if ((parameter->GetY() > MENU_HEIGHT) && (parameter->GetY() < WINDOW_HEIGHT - MENU_HEIGHT))
				OnCommandScreensaverDown();
			return;
		case GUI_MOUSEMOVE_UP_FAST:
		case GUI_MOUSEMOVE_UP:
			if ((parameter->GetY() > MENU_HEIGHT) && (parameter->GetY() < WINDOW_HEIGHT - MENU_HEIGHT))
				OnCommandScreensaverUp();
			return;
		}
	}
	SallyAPI::GUI::CForm::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

void CScreensaverForm::OnCommandPictureLoaded()
{
	/************************************************************************/
	/* What shall we do with the drunken sailor                             */
	/************************************************************************/
	std::string type = m_pDropDownChangeType->GetSelectedIdentifier();
	if (type.compare("SlideIn") == 0)
	{
		PictureLoaded();
	}
	else if (type.compare("Zoom") == 0)
	{
		m_iPictureLoadDone = m_iPictureLoadDone | 1;
	}
	else if (type.compare("Collage") == 0)
	{
	}
	/************************************************************************/
}

/************************************************************************/
/* Helper                                                               */
/************************************************************************/

void CScreensaverForm::AddFolderToPictureVector(std::string& folder)
{
	HANDLE				hFile;
	WIN32_FIND_DATA		FileInformation;

	std::string firstFile;

	folder = SallyAPI::String::PathHelper::CorrectPath(folder);

	firstFile.append(folder);
	firstFile.append("*");

	hFile = FindFirstFile(firstFile.c_str(), &FileInformation);

	if(hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			if ((strcmp(FileInformation.cFileName, ".") != NULL) &&
				(strcmp(FileInformation.cFileName, "..") != NULL))
			{
				std::string filename;
				filename.append(folder);
				filename.append(FileInformation.cFileName);

				if (FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					AddFolderToPictureVector(filename);
				}
				else  if (SallyAPI::String::StringHelper::StringEndsWith(filename, ".jpg")
					|| SallyAPI::String::StringHelper::StringEndsWith(filename, ".bmp")
					|| SallyAPI::String::StringHelper::StringEndsWith(filename, ".png")
					|| SallyAPI::String::StringHelper::StringEndsWith(filename, ".jpeg"))
				{
					m_vImageListScreensaver.push_back(filename);
				}
			}
		} while(FindNextFile(hFile, &FileInformation) == TRUE);
	}
	FindClose(hFile);
	return;
}

void CScreensaverForm::PictureLoaded()
{
	m_iPictureLoadDone = 0;

	EnterRenderLock();

	m_pImageCurrent->SetPicture(NULL);
	if (m_pPictureCurrent == NULL)
		return;


	/************************************************************************/
	/* What shall we do with the drunken sailor                             */
	/************************************************************************/
	std::string type = m_pDropDownChangeType->GetSelectedIdentifier();
	if (type.compare("SlideIn") == 0)
	{
	}
	else if (type.compare("Zoom") == 0)
	{
 		m_pImageCurrent->SetAlphaBlending(0);
	}
	else if (type.compare("Collage") == 0)
	{
	}
	/************************************************************************/
	
	int width = 0;
	int height = 0;

	SallyAPI::GUI::GUIHelper::CalculateImageSize(m_pPictureCurrent->GetWidth(), m_pPictureCurrent->GetHeight(),
		WINDOW_WIDTH, WINDOW_HEIGHT, m_iCurrentImageX, m_iCurrentImageY, width, height);

	m_pImageCurrent->Resize(width, height);
	m_pImageCurrent->SetPicture(m_pPictureCurrent);

	m_pInfoPopUp->Update(m_stdCurrentPictureScreensaver, m_pPictureCurrent);

	LeaveRenderLock();

	/************************************************************************/
	/* What shall we do with the drunken sailor                             */
	/************************************************************************/
	if (type.compare("SlideIn") == 0)
	{
		m_pImageCurrent->MoveAnimated(m_iCurrentImageX, m_iCurrentImageY, 5000);
	}
	else if (type.compare("Zoom") == 0)
	{
		m_pImageCurrent->Move(m_iCurrentImageX, m_iCurrentImageY);
		DeleteOldImage();

		CalculateMoveZoom();

		m_pImageCurrent->BlendAnimated(255, 400, false);
	}
	else if (type.compare("Collage") == 0)
	{
	}
	/************************************************************************/
}

void CScreensaverForm::CalculateMoveZoom()
{
	if (m_pPictureCurrent == NULL)
		return;
	if (m_pPictureCurrent->GetTexture() == NULL)
		return;

	// calculate x and y positions
	int xTempSource = 0;
	int yTempSource = 0;
	int widthTempSource = 0;
	int heightTempSource = 0;

	SallyAPI::GUI::GUIHelper::CalculateImageSize(m_pPictureCurrent->GetWidth(), m_pPictureCurrent->GetHeight(), WINDOW_WIDTH, WINDOW_HEIGHT,
		xTempSource, yTempSource, widthTempSource, heightTempSource);

	int xTemp = xTempSource;
	int yTemp = yTempSource;
	int widthTemp = widthTempSource;
	int heightTemp = heightTempSource;

	int width = m_pPictureCurrent->GetTexture()->GetWidth();
	int height = m_pPictureCurrent->GetTexture()->GetHeight();

	int widthAnimated = SallyAPI::System::CNumberGenerator::GetNumber(0, widthTemp - 1) + widthTemp;

	int heightAnimated = (int)(height * ((float) ((float) widthAnimated / (float)width)));

	xTemp += width - widthAnimated;
	yTemp += height - heightAnimated;

	int xAnimated = 0;
	int yAnimated = 0;
	if (xTemp != 0)
		xAnimated = ((SallyAPI::System::CNumberGenerator::GetNumber(0, xTemp - 1) - xTemp) / 2) * -1;
	if (yTemp != 0)
		yAnimated = ((SallyAPI::System::CNumberGenerator::GetNumber(0, yTemp - 1) - yTemp) / 2) * -1;

	if (widthAnimated > WINDOW_WIDTH)
	{
		if (xAnimated > 0)
			xAnimated *= -1;
	}
	else if (widthAnimated < WINDOW_WIDTH)
	{
		if (xAnimated < 0)
			xAnimated *= -1;
	}

	if (heightAnimated > WINDOW_HEIGHT)
	{
		if (yAnimated > 0)
			yAnimated *= -1;
	}
	else if (heightAnimated < WINDOW_HEIGHT)
	{
		if (yAnimated < 0)
			yAnimated *= -1;
	}

	int speedResizeWidth = SallyAPI::System::CNumberGenerator::GetNumber(0, 20 - 1) + 20;
	int speedResizeHeight = speedResizeWidth * (float) ((float) height / (float) width);

	//m_pImageCurrent->Move(m_pImageCurrent->GetPositionX(), m_pImageCurrent->GetPositionY());

	// correct values
	if ((xAnimated < 0) && (xAnimated + widthAnimated < WINDOW_WIDTH))
		xAnimated = 0;

	if ((xAnimated > 0) && (xAnimated + widthAnimated > WINDOW_WIDTH))
		xAnimated = WINDOW_WIDTH - widthAnimated;

	if ((yAnimated < 0) && (yAnimated + heightAnimated < WINDOW_HEIGHT))
		yAnimated = 0;

	if ((yAnimated > 0) && (yAnimated + heightAnimated > WINDOW_HEIGHT))
		xAnimated = WINDOW_HEIGHT - heightAnimated;

	// select the slower one for the move
	if (speedResizeWidth > speedResizeHeight)
		m_pImageCurrent->MoveAnimated(xAnimated, yAnimated, speedResizeHeight, false);
	else
		m_pImageCurrent->MoveAnimated(xAnimated, yAnimated, speedResizeWidth, false);

	m_pImageCurrent->ResizeAnimated(widthAnimated, heightAnimated, speedResizeWidth, speedResizeHeight, false);
}

void CScreensaverForm::StartLoadImage(const std::string& file, bool next)
{
	SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();
	logger->Debug(file);

	m_pPictureOld = m_pPictureCurrent;

	EnterRenderLock();
	m_pImageCurrent->SetImageId(GUI_NO_IMAGE);
	m_pImageOld->SetPicture(NULL);
	LeaveRenderLock();

	m_iPictureLoadDone = 10;

	if (m_pPictureOld != NULL)
	{
		m_pImageOld->Move(m_pImageCurrent->GetPositionX(), m_pImageCurrent->GetPositionY());
		m_pImageOld->Resize(m_pImageCurrent->GetWidth(), m_pImageCurrent->GetHeight());
		EnterRenderLock();
		m_pImageOld->SetPicture(m_pPictureOld);
		LeaveRenderLock();

		// always set the alpha to 255
		// we can have switched the presentation type before
		m_pImageOld->SetAlphaBlending(255);

		m_iPictureLoadDone = 0;
	}
	m_pPictureCurrent = NULL;

	m_iCurrentImageX = -1;
	m_iCurrentImageY = -1;

	m_tImageLoaderScreensaver.SetValues(this, &m_pPictureCurrent, file, GUI_APP_SCREENSAVER_PIC_LOADED);
	m_tImageLoaderScreensaver.Start();

	/************************************************************************/
	/* What shall we do with the drunken sailor                             */
	/************************************************************************/
	std::string type = m_pDropDownChangeType->GetSelectedIdentifier();
	if (type.compare("SlideIn") == 0)
	{
		if (next)
		{
			m_pImageCurrent->Move(WINDOW_WIDTH * 2, 0);
			m_pImageOld->MoveAnimated(-WINDOW_WIDTH, m_pImageOld->GetPositionY(), 5000);
		}
		else
		{
			m_pImageCurrent->Move(-WINDOW_WIDTH, 0);
			m_pImageOld->MoveAnimated(WINDOW_WIDTH, m_pImageOld->GetPositionY(), 5000);
		}
	}
	else if (type.compare("Zoom") == 0)
	{
		m_pImageOld->BlendAnimated(0, 400, false);
	}
	else if (type.compare("Collage") == 0)
	{
	}
	/************************************************************************/
}

void CScreensaverForm::CreateImagesScreensaverList()
{
	for (int i = 0; i < 12; i++)
	{
		std::string defaultscreensaver;

		defaultscreensaver.append("dir");
		defaultscreensaver.append(SallyAPI::String::StringHelper::ConvertToString(i));
		defaultscreensaver.append(".defaultscreensaver");

		bool defaultscreensaverTemp = (m_pApplicationWindow->GetPropertyInt(defaultscreensaver)) ? true : false;
		if (defaultscreensaverTemp)
		{
			std::string dir;

			dir.append("dir");
			dir.append(SallyAPI::String::StringHelper::ConvertToString(i));
			dir.append(".path");

			std::string dirTemp = m_pApplicationWindow->GetPropertyString(dir);

			if (dirTemp.length() > 0)
			{
				AddFolderToPictureVector(dirTemp);
			}
		}
	}
}

bool CScreensaverForm::SpecialKeyPressed(int key)
{
	SallyAPI::GUI::CApplicationWindow* applicationWindow = 	dynamic_cast<SallyAPI::GUI::CApplicationWindow*> (m_pParent);

	if (applicationWindow == NULL)
		return false;

	switch (key)
	{
	case SPECIAL_KEY_PLAY:
		SendMessageToParent(this, GUI_APP_PLAY, GUI_BUTTON_CLICKED);
		return true;
	case SPECIAL_KEY_PREVIOUS:
		OnCommandScreensaverPrevious();
		return true;
	case SPECIAL_KEY_NEXT:
		OnCommandScreensaverNext();
		return true;
	case SPECIAL_KEY_ENTER:
		if (applicationWindow->IsEnabled() == true) // if no popup is open
		{
			SendMessageToParent(this, GUI_APP_SHOW_INFO, GUI_BUTTON_CLICKED);
			return true;
		}
		return false;
	case SPECIAL_KEY_SHUFFLE:
		SendMessageToParent(this, GUI_APP_SHUFFLE, GUI_BUTTON_CLICKED);
		return true;
	case SPECIAL_KEY_INFO:
		if (applicationWindow->IsEnabled() == true) // if no popup is open
		{
			SendMessageToParent(m_pScreensaverFormNotifier, NULL, GUI_FORM_CLICKED);
			return true;
		}
		return false;
	}
	return false;
}