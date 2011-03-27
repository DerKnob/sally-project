////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	ImageBrowser\ScreensaverForm.h
///
/// \brief	Declares the screensaver Windows Form. 
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
#include <sallyAPI\sallyAPI.h>
#include "LoadImageScreensaver.h"
#include "InfoPopUp.h"
#include <algorithm>

class CScreensaverForm
	: public SallyAPI::GUI::CForm
{
private:
	SallyAPI::GUI::CApplicationWindow*	m_pApplicationWindow;
	CInfoPopUp*							m_pInfoPopUp;

	SallyAPI::GUI::CTimer*				m_pTimerDiashow;

	SallyAPI::GUI::CGroupBox*			m_pTopMenu;
	SallyAPI::GUI::CVolumeControl*		m_pVolumeControl;
	SallyAPI::GUI::CBottonMenu*			m_pBottomMenu;
	SallyAPI::GUI::CTimer*				m_pTimerHideMenu;

	SallyAPI::GUI::CButtonBar*			m_pMenuBar;
	SallyAPI::GUI::CButtonBarButton*	m_pExitFullscreen;
	SallyAPI::GUI::CButtonBarButton*	m_pShuffle;
	SallyAPI::GUI::CButtonBarButton*	m_pInfo;
	SallyAPI::GUI::CForm*				m_pScreensaverFormNotifier;
	SallyAPI::GUI::CImageBox*			m_pImageCurrent;
	SallyAPI::GUI::CImageBox*			m_pImageOld;
	SallyAPI::GUI::CLabel*				m_pLabelTimer;
	SallyAPI::GUI::CNumberSelector*		m_pEditTimer;
	SallyAPI::GUI::CDropDown*			m_pDropDownChangeType;
	SallyAPI::GUI::CRoundButton*		m_pButtonPlay;
	SallyAPI::GUI::CRoundButton*		m_pButtonNext;
	SallyAPI::GUI::CRoundButton*		m_pButtonPrevious;

	SallyAPI::GUI::CPicture*	m_pPictureCurrent;
	SallyAPI::GUI::CPicture*	m_pPictureOld;

	bool						m_bScreensaverMode;
	std::vector<std::string>	m_vHistoryPictureList;
	std::string					m_stdCurrentPictureScreensaver;
	int							m_iCurrentPictureScreensaver;
	CLoadImageScreensaver		m_tImageLoaderScreensaver;

	bool						m_bAnimationZoom;
	int							m_iCurrentImageX;
	int							m_iCurrentImageY;

	int							m_iPictureLoadDone;

	std::vector<std::string>*	m_vImageListCurrent;
	std::vector<std::string>	m_vImageListSmartShuffle;

	std::vector<std::string>	m_vImageListScreensaver;

	void	OnCommandScreensaverPrevious();
	void	OnCommandScreensaverNext();

	void	OnCommandScreensaverPause();
	void	OnCommandScreensaverPlay();
	void	OnCommandScreensaverUp();
	void	OnCommandScreensaverDown();
	void	OnCommandScreensaverLeft();
	void	OnCommandScreensaverRight();
	void	OnCommandNextImageScreensaver();
	void	OnCommandPreviousImageScreensaver();

	void	OnCommandSwitchShuffle();
	void	OnCommandTimeoutChanged();
	void	OnCommandTpyeChanged();

	void	OnCommandShowBottomMenu();
	void	OnCommandHideBottomMenu();

	void	AddFolderToPictureVector(std::string &folder);

	void	CreateImagesScreensaverList();
	void	OnCommandPictureLoaded();
	
	void	RemoveAsPlayedFromSmartShuffle(const std::string& number);

	void	PictureLoaded();
	void	StartLoadImage(const std::string& file, bool next);
	void	CalculateMoveZoom();
	void	DeleteOldImage();
	
	virtual bool ProcessMouseUp(int x, int y);
	virtual void RenderControl();
public:
	CScreensaverForm(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int height);
	virtual ~CScreensaverForm();

	virtual void SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
	virtual void Timer(float timeDelta);

	void LoadConfig();
	void SetDiashowValues(std::vector<std::string>* imageList, bool screensaverActive, int timeOut, bool shuffle, const std::string& type);
	bool ActivateScreensaver();
	bool DeactivateScreensaver();

	bool SpecialKeyPressed(int key);
};
