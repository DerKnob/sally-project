////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	ImageBrowser\MainForm.h
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
#include <sallyAPI\sallyAPI.h>
#include <algorithm>
#include "LoadImageScreensaver.h"
#include "LoadImage.h"
#include "ScreensaverForm.h"
#include "ImageDatabase.h"
#include "GetFromDatabase.h"
#include "Box2DObject.h"

class CMainForm
	: public SallyAPI::GUI::CForm
{
private:
	b2World*		m_pb2World;
	CBox2DObject*	m_pb2Object;
	int				m_iOldPosition;

	SallyAPI::GUI::CApplicationWindow* m_pApplicationWindow;

	CScreensaverForm*			m_pScreensaverForm;
	
	SallyAPI::GUI::CForm*		m_pZoomForm;
	SallyAPI::GUI::CImageBox*	m_pZoomBackground;
	std::map<int, int>			m_mListViewPages;

	// InfoForm
	bool						m_bInfoOn;
	SallyAPI::GUI::CGroupBox*	m_pInfoView;
	SallyAPI::GUI::CButton*		m_pInfoViewShowButton;
	SallyAPI::GUI::CImageBox*	m_pInfoViewSmallImage;
	SallyAPI::GUI::CLabel*		m_pInfoViewTitle;
	SallyAPI::GUI::CLabel*		m_pInfoViewLabel[4];
	SallyAPI::GUI::CLabelBox*	m_pInfoViewDesciption[4];

	CLoadImageScreensaver	m_tImageLoaderZoom;
	CLoadImage				m_tImageLoader;
	int						m_iStartPicture;
	std::string				m_strCurrentFolder;
	int						m_iFolderDeep;
	int						m_iZoomImageNumber;
	int						m_iZoomImageControlId;
	bool					m_bZoomAction;
	RECT					m_rZoomBig;
	RECT					m_rZoomSmall;
	bool					m_bZoomDirectionX;
	bool					m_bZoomDirectionY;
	int						m_iCols;
	int						m_iRows;
	int						m_iBorderTop;
	int						m_iBorderLeft;

	std::map<int, std::vector<SallyAPI::GUI::CImageBox*>>	m_mImage;
	std::map<int, std::vector<SallyAPI::GUI::CLabel*>>		m_mImageName;
	std::map<std::string, int>								m_mDriveImages;

	SallyAPI::GUI::CForm*				m_pMainForm;
	SallyAPI::GUI::CButtonBar*			m_pMenuBar;
	SallyAPI::GUI::CButtonBarButton*	m_pFolderUp;
	SallyAPI::GUI::CButtonBarButton*	m_pRefreshFolder;
	SallyAPI::GUI::CButtonBarButton*	m_pStartDiashow;
	SallyAPI::GUI::CButtonBarButton*	m_pIncludeSubFolders;
	SallyAPI::GUI::CImageBox*			m_pImageBoxBig;
	SallyAPI::GUI::CImageBox*			m_pImageBoxBigOld;
	SallyAPI::GUI::CPicture*			m_pPictureBig;
	SallyAPI::GUI::CPicture*			m_pPictureBigOld;
	SallyAPI::GUI::CForm*				m_pSmoothMoveForm;
	
	SallyAPI::GUI::CBreadcrumb*			m_pBreadcrumb;

	SallyAPI::GUI::CLabelBox*			m_pLabelDatabaseInfo;
	SallyAPI::GUI::CLabel*				m_pLabelFilter;
	SallyAPI::GUI::CLabel*				m_pLabelYear;
	SallyAPI::GUI::CDropDown*			m_pDropDownYear;
	SallyAPI::GUI::CLabel*				m_pLabelMonth;
	SallyAPI::GUI::CDropDown*			m_pDropDownMonth;
	
	SallyAPI::GUI::CButtonBar*			m_pMenuBarBottom;
	SallyAPI::GUI::CButtonBarButton*	m_pShowFolders;
	SallyAPI::GUI::CButtonBarButton*	m_pShowDate;

	SallyAPI::System::CCritSection					m_LockFolderPictureVector;
	std::vector<std::string>						m_vFolderPictureVector;
	std::vector<std::string>						m_vImageOnlyVector;
	std::vector<std::string>						m_vFilenameVectorForScreensaver;
	std::map<std::string, SallyAPI::GUI::CPicture*>	m_mPictureMap;

	CGetFromDatabase								m_tGetFromDatabase;

	void	OnCommandZoomRight();
	void	OnCommandZoomLeft();

	void	OnCommandPreviousImage();
	void	OnCommandNextImage();
	void	OnCommandFormClicked();

	void	AddFolderToScreensaverVector(std::string& folder);
	void	OnCommandChangeFolder(std::string& folder);
	void	OnCommandOpenFolder(std::string& folder);
	void	OnCommandResetFolder();
	void	OnCommandProcessClicked(int reporterId);
	void	OnCommandNextFolder(const std::string& nextFolder);

	void	OnCommandShowDate();
	void	OnCommandShowFolders();
	void	OnCommandControlPositionChanged(SallyAPI::GUI::CGUIBaseObject* reporter);
	void	OnCommandInfoView();

	bool	ResetBox2Object();

	void	UpdateImages(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);
	void	UpdateImages();

	void	SetPictureToBox(SallyAPI::GUI::CPicture* picture, SallyAPI::GUI::CImageBox* imageBox, SallyAPI::GUI::CLabel* labelBox);
	void	ResetImages();
	void	BigPicLoadedZoom();

	void	SetInfoViewPicture();
	void	BigPicLoadedSwitch();
	void	DatabaseLoadDone();

	void	UpdateFromDatabase();

	void	OnCommandStartDiaShow();

	void	UpdatePictureVector();
	void	DeleteLoadedImages();

	void	OnCommandMouseMove(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);

	virtual bool CheckProcessMouseDown(int x, int y);
public:
	CMainForm(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int height, CScreensaverForm* screensaverForm);
	virtual ~CMainForm();

	virtual void SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
	virtual void SendMessageToChilds(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
	virtual void Timer(float timeDelta);

	void LoadConfig();
	bool GetZoom();
	bool SpecialKeyPressed(int key);
	void OnCommandRefreshFolder();
};
