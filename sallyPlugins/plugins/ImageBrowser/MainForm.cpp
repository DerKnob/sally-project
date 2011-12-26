////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	ImageBrowser\MainForm.cpp
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

#include "MainForm.h"

CMainForm::CMainForm(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int height, CScreensaverForm* screensaverForm)
	: SallyAPI::GUI::CForm(parent, x, y, width, height, 0)
{
	m_pScreensaverForm = screensaverForm;
	m_pApplicationWindow = (SallyAPI::GUI::CApplicationWindow*) parent;

	m_iStartPicture = 0;
	m_iZoomImageControlId = -1;
	m_iZoomImageNumber = -1;
	m_iFolderDeep = 0;
	m_bZoomAction = false;
	m_pPictureBig = NULL;
	m_pPictureBigOld = NULL;
	m_bInfoOn = false;

	m_pMainForm = new SallyAPI::GUI::CForm(this, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	this->AddChild(m_pMainForm);

	m_pMenuBar = new SallyAPI::GUI::CButtonBar(m_pMainForm, WINDOW_BORDER_H, 15, 560);
	m_pMainForm->AddChild(m_pMenuBar);

	m_pFolderUp = new SallyAPI::GUI::CButtonBarButton(m_pMenuBar, 120, GUI_APP_FOLDER_UP);
	m_pFolderUp->SetImageId(GUI_THEME_SALLY_ICON_FOLDER_UP);
	m_pFolderUp->SetText("Folder Up");
	m_pMenuBar->AddChild(m_pFolderUp);

	m_pRefreshFolder = new SallyAPI::GUI::CButtonBarButton(m_pMenuBar, 120, GUI_APP_REFRESH_FOLDER);
	m_pRefreshFolder->SetImageId(GUI_THEME_SALLY_ICON_RELOAD);
	m_pRefreshFolder->SetText("Reload");
	m_pMenuBar->AddChild(m_pRefreshFolder);

	m_pStartDiashow = new SallyAPI::GUI::CButtonBarButton(m_pMenuBar, 160, GUI_APP_START_DIASHOW);
	m_pStartDiashow->SetImageId(GUI_THEME_SALLY_ICON_TIMER);
	m_pStartDiashow->SetText("Start Slideshow");
	m_pMenuBar->AddChild(m_pStartDiashow);

	m_pIncludeSubFolders = new SallyAPI::GUI::CButtonBarButton(m_pMenuBar, 160, GUI_APP_INCLUDE_SUBFOLDERS);
	m_pIncludeSubFolders->SetImageId(GUI_THEME_SALLY_ICON_FOLDER);
	m_pIncludeSubFolders->SetText("Incl. Subfolders");
	m_pMenuBar->AddChild(m_pIncludeSubFolders);

	// Bottom
	m_pLabelDatabaseInfo = new SallyAPI::GUI::CLabelBox(m_pMainForm, (WINDOW_WIDTH - 400) / 2, (WINDOW_HEIGHT - 100) / 2, 400, 100);
	m_pLabelDatabaseInfo->SetText("No images to display found.\n\n(Maybe the database index is not up to date.\nYou can check this by taking a look at the settings.)");
	m_pLabelDatabaseInfo->SetLocalised(true);
	m_pLabelDatabaseInfo->SetAlign(DT_CENTER | DT_VCENTER);
	m_pLabelDatabaseInfo->Visible(false);
	m_pMainForm->AddChild(m_pLabelDatabaseInfo);

	m_pBreadcrumb = new SallyAPI::GUI::CBreadcrumb(m_pMainForm, WINDOW_BORDER_H,
		WINDOW_HEIGHT - (CONTROL_HEIGHT + WINDOW_BORDER_V),
		WINDOW_WIDTH - (WINDOW_BORDER_H * 2) - 10 - (CONTROL_HEIGHT * 2));
	m_pBreadcrumb->SetImageId(GUI_THEME_SALLY_ICON_DESKTOP);
	m_pMainForm->AddChild(m_pBreadcrumb);

	m_pLabelFilter = new SallyAPI::GUI::CLabel(m_pMainForm, WINDOW_BORDER_H, WINDOW_HEIGHT - (CONTROL_HEIGHT + WINDOW_BORDER_V),
		50);
	m_pLabelFilter->SetBold(true);
	m_pLabelFilter->SetText("Filter:");
	m_pLabelFilter->Visible(false);
	m_pMainForm->AddChild(m_pLabelFilter);

	m_pLabelYear = new SallyAPI::GUI::CLabel(m_pMainForm, WINDOW_BORDER_H + 60, WINDOW_HEIGHT - (CONTROL_HEIGHT + WINDOW_BORDER_V),
		50);
	m_pLabelYear->SetText("Year");
	m_pLabelYear->SetAlign(DT_VCENTER | DT_RIGHT);
	m_pLabelYear->Visible(false);
	m_pMainForm->AddChild(m_pLabelYear);

	m_pDropDownYear = new SallyAPI::GUI::CDropDown(m_pMainForm, WINDOW_BORDER_H + 60 + 60,
		WINDOW_HEIGHT - (CONTROL_HEIGHT + WINDOW_BORDER_V), 100);
	m_pDropDownYear->Visible(false);
	m_pMainForm->AddChild(m_pDropDownYear);

	m_pLabelMonth = new SallyAPI::GUI::CLabel(m_pMainForm, WINDOW_BORDER_H + 60 + 60 + 100, WINDOW_HEIGHT - (CONTROL_HEIGHT + WINDOW_BORDER_V),
		50);
	m_pLabelMonth->SetText("Month");
	m_pLabelMonth->SetAlign(DT_VCENTER | DT_RIGHT);
	m_pLabelMonth->Visible(false);
	m_pMainForm->AddChild(m_pLabelMonth);

	m_pDropDownMonth = new SallyAPI::GUI::CDropDown(m_pMainForm, WINDOW_BORDER_H + 60 + 60 + 100 + 60,
		WINDOW_HEIGHT - (CONTROL_HEIGHT + WINDOW_BORDER_V), 100);
	m_pDropDownMonth->Visible(false);
	m_pMainForm->AddChild(m_pDropDownMonth);

	// Bottom Menu
	m_pMenuBarBottom = new SallyAPI::GUI::CButtonBar(m_pMainForm, WINDOW_WIDTH - WINDOW_BORDER_H - (CONTROL_HEIGHT * 2),
		WINDOW_HEIGHT - (CONTROL_HEIGHT + WINDOW_BORDER_V), CONTROL_HEIGHT * 2);
	m_pMainForm->AddChild(m_pMenuBarBottom);

	m_pShowFolders = new SallyAPI::GUI::CButtonBarButton(m_pMenuBarBottom, CONTROL_HEIGHT, GUI_APP_SHOW_FOLDERS);
	m_pShowFolders->SetImageId(GUI_THEME_SALLY_ICON_FOLDER);
	m_pShowFolders->SetCheckStatus(true);
	m_pMenuBarBottom->AddChild(m_pShowFolders);

	m_pShowDate = new SallyAPI::GUI::CButtonBarButton(m_pMenuBarBottom, CONTROL_HEIGHT, GUI_APP_SHOW_DATE);
	m_pShowDate->SetImageId(GUI_THEME_SALLY_ICON_DATE);
	m_pMenuBarBottom->AddChild(m_pShowDate);

	m_pSmoothMoveForm = new SallyAPI::GUI::CForm(m_pMainForm, 0, MENU_HEIGHT + 5,
		WINDOW_WIDTH, WINDOW_HEIGHT - MENU_HEIGHT - CONTROL_HEIGHT - WINDOW_BORDER_V - 10);
	m_pSmoothMoveForm->SetScrollType(SallyAPI::GUI::SCROLL_TYPE_SMOOTH);
	m_pMainForm->AddChild(m_pSmoothMoveForm);

	// cols / rows
	m_iCols = WINDOW_WIDTH / (PICTURE_WIDTH_MAX_SMALL + PICTURE_WIDTH_MAX_SMALL_SPACE) + 1;
	m_iBorderLeft = (WINDOW_WIDTH % (PICTURE_WIDTH_MAX_SMALL + PICTURE_WIDTH_MAX_SMALL_SPACE)) / 2;
	m_iRows = m_pSmoothMoveForm->GetHeight() / (PICTURE_HEIGHT_MAX_SMALL + PICTURE_HEIGHT_MAX_SMALL_SPACE + CONTROL_HEIGHT);
	m_iBorderTop = (m_pSmoothMoveForm->GetHeight() % (PICTURE_HEIGHT_MAX_SMALL + PICTURE_HEIGHT_MAX_SMALL_SPACE + CONTROL_HEIGHT)) / 2;

	for (int k = 0 ; k < m_iCols + 2; ++k)
	{
		std::vector<SallyAPI::GUI::CImageBox*> imageBoxVector;
		std::vector<SallyAPI::GUI::CLabel*> imageNameVector;

		for (int i = 0; i < m_iRows; ++i)
		{
			int id = (k * 100) + i + 1;

			SallyAPI::GUI::CImageBox* imageBox = new SallyAPI::GUI::CImageBox(m_pSmoothMoveForm, 0, 0, 0, 0, id);
			imageBox->SetLocalised(false);
			imageBox->SetDisplayType(SallyAPI::GUI::IMAGEBOX_DISPLAY_TYPE_SCALE);
			imageBoxVector.push_back(imageBox);

			m_pSmoothMoveForm->AddChild(imageBox);

			SallyAPI::GUI::CLabel* imageName = new SallyAPI::GUI::CLabel(m_pSmoothMoveForm, 0, 0, PICTURE_WIDTH_MAX_SMALL);
			imageNameVector.push_back(imageName);
			imageName->SetFont("scrolllist.font");
			imageName->SetLocalised(false);
			imageName->SetAlign(DT_CENTER | DT_VCENTER | DT_WORDBREAK);

			m_pSmoothMoveForm->AddChild(imageName);
		}
		m_mImage[k] = imageBoxVector;
		m_mImageName[k] = imageNameVector;
	}

	/************************************************************************/
	/* Setup Box2D                                                          */
	/************************************************************************/
	b2AABB worldAABB;

	float xBottom = (float) WINDOW_WIDTH * -1;
	float yBottom = (float) (WINDOW_HEIGHT + WINDOW_HEIGHT) * -1;

	float xTop = (float) WINDOW_WIDTH + WINDOW_WIDTH;
	float yTop = (float) WINDOW_HEIGHT;

	worldAABB.lowerBound.Set(xBottom, yBottom);
	worldAABB.upperBound.Set(xTop, yTop);

	// Define the gravity vector.
	b2Vec2 gravity(0.0f, -100.0f);

	m_pb2World = new b2World(worldAABB, gravity, true);


	// Define the ground body.
	b2BodyDef groundBodyDef;
	float groundX = WINDOW_WIDTH  / 2;
	float groundY = ((WINDOW_HEIGHT - CONTROL_HEIGHT - WINDOW_BORDER_V - 5) * -1) - 20.0f;
	groundBodyDef.position.Set(groundX, groundY);

	// Call the body factory which allocates memory for the ground body
	// from a pool and creates the ground box shape (also from a pool).
	// The body is also added to the world.
	b2Body* groundBody = m_pb2World->CreateBody(&groundBodyDef);

	// Define the ground box shape.
	b2PolygonDef groundShapeDef;

	// The extents are the half-widths of the box.
	float groundWidth = WINDOW_WIDTH * 2;
	groundShapeDef.SetAsBox(groundWidth, 15.0f);

	// Add the ground shape to the ground body.
	groundBody->CreateShape(&groundShapeDef);


	// Define the ground body.
	b2BodyDef topBodyDef;
	float topX = WINDOW_WIDTH  / 2;
	float topY = ((MENU_HEIGHT + 5) * -1) + 20.0f;
	topBodyDef.position.Set(topX, topY);

	b2Body* topBody = m_pb2World->CreateBody(&topBodyDef);

	// Define the top box shape.
	b2PolygonDef topShapeDef;

	// The extents are the half-widths of the box.
	float topWidth = WINDOW_WIDTH * 2;
	topShapeDef.SetAsBox(topWidth, 15.0f);

	// Add the top shape to the top body.
	topBody->CreateShape(&topShapeDef);

	m_pb2Object = new CBox2DObject(m_pMainForm, 0, MENU_HEIGHT + 5, WINDOW_WIDTH,
		WINDOW_HEIGHT - MENU_HEIGHT - 5 - CONTROL_HEIGHT - WINDOW_BORDER_V - 5);
	m_pMainForm->AddChild(m_pb2Object);
	
	m_iOldPosition = m_pb2Object->GetPositionX();
	m_pb2Object->CreateBox2DObject(m_pb2World);

	/************************************************************************/
	/* Background                                                           */
	/************************************************************************/
	m_pZoomForm = new SallyAPI::GUI::CForm(this, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GUI_APP_ZOOM_BACKGROUND);
	m_pZoomForm->SetScrollType(SallyAPI::GUI::SCROLL_TYPE_NORMAL);
	m_pZoomForm->Visible(false);
	m_pZoomForm->SetHandleInputIfItIsChildHandled(false);
	this->AddChild(m_pZoomForm);

	m_pZoomBackground = new SallyAPI::GUI::CImageBox(m_pZoomForm, 0, 0, MAX_WIDTH, MAX_HEIGHT);
	m_pZoomBackground->SetImageId(GUI_THEME_SALLY_POPUP_NORMAL);
	m_pZoomBackground->SetAlphaBlending(0);
	m_pZoomForm->AddChild(m_pZoomBackground);

	// big zoom pictures
	m_pImageBoxBig = new SallyAPI::GUI::CImageBox(this, 0, 0, 0, 0);
	m_pImageBoxBig->SetDisplayType(SallyAPI::GUI::IMAGEBOX_DISPLAY_TYPE_SCALE);
	m_pImageBoxBig->Visible(false);
	this->AddChild(m_pImageBoxBig);

	m_pImageBoxBigOld = new SallyAPI::GUI::CImageBox(this, 0, 0, 0, 0);
	m_pImageBoxBigOld->SetDisplayType(SallyAPI::GUI::IMAGEBOX_DISPLAY_TYPE_SCALE);
	m_pImageBoxBigOld->Visible(false);
	this->AddChild(m_pImageBoxBigOld);

	m_pInfoView = new SallyAPI::GUI::CGroupBox(m_pZoomForm, 80, INFO_VIEW_HEIGHT_HIDE, 300, INFO_VIEW_HEIGHT);
	m_pInfoView->SetAlwaysHandleInput(true);
	m_pZoomForm->AddChild(m_pInfoView);

	m_pInfoViewShowButton = new SallyAPI::GUI::CButton(m_pInfoView, 50, INFO_VIEW_HEIGHT - 20 - CONTROL_HEIGHT, 200, CONTROL_HEIGHT, GUI_APP_SHOW_INFO_VIEW);
	m_pInfoViewShowButton->SetImageId(GUI_THEME_SALLY_ICON_DOWN);
	m_pInfoViewShowButton->SetText("Show Infos");
	m_pInfoView->AddChild(m_pInfoViewShowButton);

	m_pInfoViewSmallImage = new SallyAPI::GUI::CImageBox(m_pInfoView, 300 - 80, 40, 100, 100);
	m_pInfoViewSmallImage->SetDisplayType(SallyAPI::GUI::IMAGEBOX_DISPLAY_TYPE_SCALE);
	m_pInfoView->AddChild(m_pInfoViewSmallImage);

	m_pInfoViewTitle = new SallyAPI::GUI::CLabel(m_pInfoView, 20, 40, 150);
	m_pInfoViewTitle->SetBold(true);
	m_pInfoViewTitle->SetLocalised(false);
	m_pInfoView->AddChild(m_pInfoViewTitle);

	m_pInfoViewLabel[0] = new SallyAPI::GUI::CLabel(m_pInfoView, 20, 70, 100);
	m_pInfoViewLabel[0]->SetText("Resolution:");
	m_pInfoViewLabel[0]->SetFont("label.font.small");
	m_pInfoView->AddChild(m_pInfoViewLabel[0]);

	m_pInfoViewLabel[1] = new SallyAPI::GUI::CLabel(m_pInfoView, 20, 140, 100);
	m_pInfoViewLabel[1]->SetText("Size:");
	m_pInfoViewLabel[1]->SetFont("label.font.small");
	m_pInfoView->AddChild(m_pInfoViewLabel[1]);

	m_pInfoViewLabel[2] = new SallyAPI::GUI::CLabel(m_pInfoView, 20, 210, 100);
	m_pInfoViewLabel[2]->SetText("Create Date:");
	m_pInfoViewLabel[2]->SetFont("label.font.small");
	m_pInfoView->AddChild(m_pInfoViewLabel[2]);

	m_pInfoViewLabel[3] = new SallyAPI::GUI::CLabel(m_pInfoView, 20, 280, 100);
	m_pInfoViewLabel[3]->SetText("Filename:");
	m_pInfoViewLabel[3]->SetFont("label.font.small");
	m_pInfoView->AddChild(m_pInfoViewLabel[3]);

	m_pInfoViewDesciption[0] = new SallyAPI::GUI::CLabelBox(m_pInfoView, 20, 70 + CONTROL_HEIGHT, 300 - 60, CONTROL_HEIGHT);
	m_pInfoViewDesciption[0]->SetLocalised(false);
	m_pInfoView->AddChild(m_pInfoViewDesciption[0]);

	m_pInfoViewDesciption[1] = new SallyAPI::GUI::CLabelBox(m_pInfoView, 20, 140 + CONTROL_HEIGHT, 300 - 40, CONTROL_HEIGHT);
	m_pInfoViewDesciption[1]->SetLocalised(false);
	m_pInfoView->AddChild(m_pInfoViewDesciption[1]);

	m_pInfoViewDesciption[2] = new SallyAPI::GUI::CLabelBox(m_pInfoView, 20, 210 + CONTROL_HEIGHT, 300 - 40, CONTROL_HEIGHT);
	m_pInfoViewDesciption[2]->SetLocalised(false);
	m_pInfoView->AddChild(m_pInfoViewDesciption[2]);

	m_pInfoViewDesciption[3] = new SallyAPI::GUI::CLabelBox(m_pInfoView, 20, 280 + CONTROL_HEIGHT, 300 - 40, CONTROL_HEIGHT * 2);
	m_pInfoViewDesciption[3]->SetLocalised(false);
	m_pInfoView->AddChild(m_pInfoViewDesciption[3]);

	// Helper
	m_tImageLoader.SetValues(&m_vFolderPictureVector, &m_mPictureMap, this, &m_iStartPicture, m_iCols, m_iRows);
}

CMainForm::~CMainForm()
{
	SafeDelete(m_pPictureBig);
	SafeDelete(m_pPictureBigOld);
	DeleteLoadedImages();
	SafeDelete(m_pb2World);
}

/************************************************************************/
/* Overwrite                                                            */
/************************************************************************/

void CMainForm::Timer(float timeDelta)
{
	SallyAPI::GUI::CForm::Timer(timeDelta);

	m_pb2World->Step(timeDelta * 4, 20);

	if (!m_bMouseDown)
	{
		int moveValue = m_pb2Object->GetPositionX();
		moveValue -= m_iOldPosition;

		if (moveValue != 0)
		{
			SallyAPI::GUI::SendMessage::CParameterInteger integer(moveValue * 2);
			OnCommandMouseMove(&integer);
		}
	}
	m_iOldPosition = m_pb2Object->GetPositionX();
}

void CMainForm::LoadConfig()
{
	m_tImageLoader.WaitForStop();

	if (m_pApplicationWindow->GetPropertyBool("diashowSubfolder", true))
	{
		m_pIncludeSubFolders->SetCheckStatus(true);
	}

	OnCommandShowFolders();

	CImageDatabase::FillYearFromDatabase(m_pApplicationWindow, m_pDropDownYear);
	CImageDatabase::FillMonthFromDatabase(m_pApplicationWindow, m_pDropDownMonth);

	m_tImageLoader.Start();
}

void CMainForm::SendMessageToChilds(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case MS_SALLY_SYSTEM_DEVICECHANGE:
		if (m_iFolderDeep == 0)
			OnCommandNextFolder("..");
		return;
	}
	SallyAPI::GUI::CForm::SendMessageToChilds(reporter, reporterId, messageId, messageParameter);
}

void CMainForm::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, 
									SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_BREADCRUMB_START_CLICKED:
		ResetBox2Object();
		OnCommandShowFolders();
		return;
	case GUI_FORM_CLICKED:
		if (reporterId == GUI_APP_ZOOM_BACKGROUND)
			OnCommandFormClicked();
		return;
	case GUI_MOUSEMOVE_LEFT:
	case GUI_MOUSEMOVE_LEFT_FAST:
		OnCommandZoomLeft();
		return;
	case GUI_MOUSEMOVE_RIGHT:
	case GUI_MOUSEMOVE_RIGHT_FAST:
		OnCommandZoomRight();
		return;
	//case GUI_CONTROL_RESIZED:
	case GUI_CONTROL_MOVED:
	case GUI_CONTROL_BLENDED:
		OnCommandControlPositionChanged(reporter);
		return;
	case GUI_APP_RENDER_LOCK:
		EnterRenderLock();
		return;
	case GUI_APP_RENDER_RELEASE:
		LeaveRenderLock();
		return;
	case GUI_APP_BIG_PIC_LOADED_ZOOM:
		BigPicLoadedZoom();
		return;
	case GUI_APP_BIG_PIC_LOADED_SWITCH:
		BigPicLoadedSwitch();
		return;
	case GUI_APP_FOLDER_PIC_CHANGED:
		UpdateImages();
		return;
	case GUI_APP_FOLDER_PIC_LOADED:
		UpdateImages(messageParameter);
		return;
	case GUI_APP_DATABASE_LOAD_DONE:
		DatabaseLoadDone();
		return;
	case GUI_DROPDOWN_CHANGED:
		UpdateFromDatabase();
		return;
	case GUI_MOUSEMOVE_SMOOTH_LEFT:
	case GUI_MOUSEMOVE_SMOOTH_RIGHT:
	case GUI_MOUSEMOVE_SMOOTH_UP:
	case GUI_MOUSEMOVE_SMOOTH_DOWN:
		if (reporter == m_pSmoothMoveForm)
		{
			OnCommandMouseMove(messageParameter);
		}
		return;
	case GUI_IMAGEBOX_CLICKED:
		OnCommandProcessClicked(reporterId);
		return;
	case GUI_BUTTON_CLICKED:
		switch (reporterId)
		{
		case GUI_APP_SHOW_INFO_VIEW:
			OnCommandInfoView();
			return;
		case GUI_APP_SHOW_DATE:
			ResetBox2Object();
			OnCommandShowDate();
			return;
		case GUI_APP_SHOW_FOLDERS:
			ResetBox2Object();
			OnCommandShowFolders();
			return;
		case GUI_APP_START_DIASHOW:
			ResetBox2Object();
			OnCommandStartDiaShow();
			return;
		case GUI_APP_INCLUDE_SUBFOLDERS:
			if (m_pIncludeSubFolders->GetCheckStatus())
			{
				m_pIncludeSubFolders->SetCheckStatus(false);
				m_pApplicationWindow->SetPropertyBool("diashowSubfolder", false);
			}
			else
			{
				m_pIncludeSubFolders->SetCheckStatus(true);
				m_pApplicationWindow->SetPropertyBool("diashowSubfolder", true);
			}
			return;
		case GUI_APP_FOLDER_UP:
			ResetBox2Object();
			OnCommandNextFolder("..");
			return;
		case GUI_APP_REFRESH_FOLDER:
			OnCommandRefreshFolder();
			return;
		}
		break;
	}
	SallyAPI::GUI::CForm::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

void CMainForm::OnCommandInfoView()
{
	if (m_pInfoView->GetPositionY() == INFO_VIEW_HEIGHT_SHOW_INFO)
	{
		// info off
		m_pInfoViewShowButton->SetText("Show Infos");
		m_pInfoViewShowButton->SetImageId(GUI_THEME_SALLY_ICON_DOWN);
		m_pInfoViewShowButton->Enable(false);

		m_pInfoView->MoveAnimated(m_pInfoView->GetPositionX(), INFO_VIEW_HEIGHT_SHOW_BUTTON, 800);
		m_pImageBoxBig->SetRotationY(SallyAPI::GUI::CONTROLROTATION_RIGHT);
		m_pImageBoxBig->SetStopAngelY(0);
		m_pImageBoxBig->RotateAnimatedY(2, false);

		m_pImageBoxBig->MoveAnimated(m_pImageBoxBig->GetPositionX() - 300, m_pImageBoxBig->GetPositionY(), 800);
		
		m_bInfoOn = false;
	}
	else
	{
		// info on
		m_pInfoViewShowButton->SetText("Hide Infos");
		m_pInfoViewShowButton->SetImageId(GUI_THEME_SALLY_ICON_UP);
		m_pInfoViewShowButton->Enable(false);

		m_pInfoView->MoveAnimated(m_pInfoView->GetPositionX(), INFO_VIEW_HEIGHT_SHOW_INFO, 800);
		m_pImageBoxBig->SetRotationY(SallyAPI::GUI::CONTROLROTATION_LEFT);
		m_pImageBoxBig->SetStopAngelY(45);
		m_pImageBoxBig->RotateAnimatedY(2, false);

		m_pImageBoxBig->MoveAnimated(m_pImageBoxBig->GetPositionX() + 300, m_pImageBoxBig->GetPositionY(), 800);

		m_bInfoOn = true;
	}
}

void CMainForm::OnCommandControlPositionChanged(SallyAPI::GUI::CGUIBaseObject* reporter)
{
	if (reporter == m_pImageBoxBig)
	{
// 		if ((m_pImageBoxBig->GetPositionX() == m_rZoomSmall.left) && (m_pImageBoxBig->GetPositionY() == m_rZoomSmall.top) &&
// 			(m_pImageBoxBig->GetWidth() == m_rZoomSmall.right) && (m_pImageBoxBig->GetHeight() == m_rZoomSmall.bottom))
		if (m_pImageBoxBig->GetAlphaBlending() == 0)
		{
			int col = m_iZoomImageControlId / 100;
			int row = m_iZoomImageControlId % 100;

			//std::vector<SallyAPI::GUI::CImageBox*> imageBoxVector = m_mImage[col];
			//std::vector<SallyAPI::GUI::CLabel*> imageNameVector = m_mImageName[col];
			//imageBoxVector[row]->Visible(true);
			//imageNameVector[row]->Visible(true);

			// reset zoom image
			m_iZoomImageControlId = -1;
			m_iZoomImageNumber = -1;
			m_bZoomAction = false;

			m_pImageBoxBig->SetAlphaBlending(255);
			m_pImageBoxBig->Visible(false);
			m_pImageBoxBig->SetPicture(NULL);

			// check if the zoom loading thread is finished
			m_tImageLoaderZoom.WaitForStop();
			SafeDelete(m_pPictureBig);

			m_pMainForm->Enable(true);
			m_pZoomForm->Visible(false);
		}
	}
	else if (reporter == m_pInfoView)
	{
		m_pInfoViewShowButton->Enable(true);
	}
}

bool CMainForm::CheckProcessMouseDown(int x, int y)
{
	if (ResetBox2Object())
	{
		m_pb2Object->CheckProcessMouseDown(x, y);
		return true;
	}

	return SallyAPI::GUI::CForm::CheckProcessMouseDown(x, y);
}

bool CMainForm::ResetBox2Object()
{
	bool move = m_pb2Object->MoveAndHold(0, MENU_HEIGHT + 5);
	m_iOldPosition = m_pb2Object->GetPositionX();

	return move;
}

void CMainForm::OnCommandZoomLeft()
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	bool result = option->GetPropertyBool("sally", "leftEQprevious", true);

	if (result)
	{
		OnCommandPreviousImage();
	}
	else
	{
		OnCommandNextImage();
	}
}

void CMainForm::OnCommandZoomRight()
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	bool result = option->GetPropertyBool("sally", "leftEQprevious", true);

	if (result)
	{
		OnCommandNextImage();
	}
	else
	{
		OnCommandPreviousImage();
	}
}

void CMainForm::OnCommandPreviousImage()
{
	if (m_bZoomAction)
		return;

	SallyAPI::GUI::SendMessage::CParameterNotificationOSM messageOnScreenMenu(GUI_THEME_SALLY_OSM_PREVIOUS, "Previous");
	m_pParent->SendMessageToParent(this, 0, MS_SALLY_NOTIFICATION_OSM, &messageOnScreenMenu);

	m_iZoomImageNumber -= 1;

	if (m_iZoomImageNumber < 0)
		m_iZoomImageNumber = m_vImageOnlyVector.size() - 1;

	std::string file;
	if (m_pShowFolders->GetCheckStatus())
	{
		file = m_vImageOnlyVector[m_iZoomImageNumber];
	}
	else
	{
		file = m_vFolderPictureVector[m_iZoomImageNumber];
	}

	m_pInfoViewDesciption[3]->SetText(file);
	m_pInfoViewTitle->SetText(SallyAPI::String::PathHelper::GetFileFromPath(file));
	m_pInfoViewSmallImage->SetPicture(NULL);
	m_pInfoViewDesciption[1]->SetText(SallyAPI::File::FileHelper::GetFormatedFileSize(file));
	m_pInfoViewDesciption[2]->SetText(SallyAPI::File::FileHelper::GetFormatedFileWriteDate(file));

	m_tImageLoaderZoom.SetValues(this, &m_pPictureBig, file, GUI_APP_BIG_PIC_LOADED_SWITCH);
	m_tImageLoaderZoom.Start();

	// prepare for the move
	SafeDelete(m_pPictureBigOld);

	m_pPictureBigOld = m_pImageBoxBig->GetPicture();
	m_pImageBoxBigOld->Move(m_pImageBoxBig->GetPositionX(), m_pImageBoxBig->GetPositionY());
	m_pImageBoxBigOld->Resize(m_pImageBoxBig->GetWidth(), m_pImageBoxBig->GetHeight());
	m_pImageBoxBigOld->SetRotationAngleY(m_pImageBoxBig->GetRotationAngleY());

	m_pImageBoxBigOld->SetPicture(m_pPictureBigOld);
	m_pImageBoxBig->SetPicture(NULL);

	m_pImageBoxBigOld->Visible(true);
	m_pImageBoxBigOld->MoveAnimated(WINDOW_WIDTH, m_pImageBoxBigOld->GetPositionY(), 2000);
	m_pImageBoxBig->Move(-WINDOW_WIDTH, 0);
}

void CMainForm::OnCommandNextImage()
{
	if (m_bZoomAction)
		return;

	SallyAPI::GUI::SendMessage::CParameterNotificationOSM messageOnScreenMenu(GUI_THEME_SALLY_OSM_NEXT, "Next");
	m_pParent->SendMessageToParent(this, 0, MS_SALLY_NOTIFICATION_OSM, &messageOnScreenMenu);

	m_iZoomImageNumber += 1;

	if (m_iZoomImageNumber >= m_vImageOnlyVector.size())
		m_iZoomImageNumber = 0;

	std::string file;
	if (m_pShowFolders->GetCheckStatus())
	{
		file = m_vImageOnlyVector[m_iZoomImageNumber];
	}
	else
	{
		file = m_vFolderPictureVector[m_iZoomImageNumber];
	}

	m_pInfoViewDesciption[3]->SetText(file);
	m_pInfoViewTitle->SetText(SallyAPI::String::PathHelper::GetFileFromPath(file));
	m_pInfoViewSmallImage->SetPicture(NULL);
	m_pInfoViewDesciption[1]->SetText(SallyAPI::File::FileHelper::GetFormatedFileSize(file));
	m_pInfoViewDesciption[2]->SetText(SallyAPI::File::FileHelper::GetFormatedFileWriteDate(file));

	m_tImageLoaderZoom.SetValues(this, &m_pPictureBig, file, GUI_APP_BIG_PIC_LOADED_SWITCH);
	m_tImageLoaderZoom.Start();

	// prepare for the move
	m_pPictureBigOld = m_pImageBoxBig->GetPicture();
	m_pImageBoxBigOld->Move(m_pImageBoxBig->GetPositionX(), m_pImageBoxBig->GetPositionY());
	m_pImageBoxBigOld->Resize(m_pImageBoxBig->GetWidth(), m_pImageBoxBig->GetHeight());
	m_pImageBoxBigOld->SetRotationAngleY(m_pImageBoxBig->GetRotationAngleY());

	m_pImageBoxBigOld->SetPicture(m_pPictureBigOld);
	m_pImageBoxBig->SetPicture(NULL);

	m_pImageBoxBigOld->Visible(true);
	m_pImageBoxBigOld->MoveAnimated(-WINDOW_WIDTH, m_pImageBoxBigOld->GetPositionY(), 2000);
	m_pImageBoxBig->Move(WINDOW_WIDTH, 0);
}

void CMainForm::OnCommandFormClicked()
{
	if ((m_iZoomImageControlId != -1) && (!m_bZoomAction))
	{
		if (m_pImageBoxBig->GetRotationAngleY() != 0)
		{
			// hide menu
			SendMessageToParent(m_pInfoViewShowButton, GUI_APP_SHOW_INFO_VIEW, GUI_BUTTON_CLICKED);
			return;
		}

		// Zoom out
		m_bZoomAction = true;

		int xDistance = m_rZoomSmall.left - m_rZoomBig.left;
		int yDistance = m_rZoomSmall.top - m_rZoomBig.top;
		int widthDistance = m_rZoomBig.right - m_rZoomSmall.right;
		int heightDistance = m_rZoomBig.bottom - m_rZoomSmall.bottom;

		if (xDistance < 0)
			xDistance *= -1;

		if (yDistance < 0)
			yDistance *= -1;

		if (widthDistance < 0)
			widthDistance *= -1;

		if (heightDistance < 0)
			heightDistance *= -1;

		int speedWidth;
		int speedHeight;
		int speedX;
		int speedY;

		if (widthDistance > heightDistance)
		{
			speedWidth = 2000;
			speedHeight = (int) (speedWidth * (float) ((float) heightDistance / (float) widthDistance));
			speedX = (int) (speedWidth * (float) ((float) xDistance / (float) widthDistance));
			speedY = (int) (speedHeight * (float) ((float) yDistance / (float) heightDistance));
		}
		else
		{
			speedHeight = 2000;
			speedWidth = (int) (speedHeight * (float) ((float) widthDistance / (float) heightDistance));
			speedX = (int) (speedWidth * (float) ((float) xDistance / (float) widthDistance));
			speedY = (int) (speedHeight * (float) ((float) yDistance / (float) heightDistance));
		}

		//m_pImageBoxBig->MoveAnimated(m_rZoomSmall.left, m_rZoomSmall.top, speedX, speedY);
		//m_pImageBoxBig->ResizeAnimated(m_rZoomSmall.right, m_rZoomSmall.bottom, speedWidth, speedHeight);
		m_pImageBoxBig->BlendAnimated(0, 800);
		m_pApplicationWindow->SendMessageToParent(m_pApplicationWindow, 0, MS_SALLY_APP_STOP_SCREENSAVER);

		m_pZoomBackground->SetAlphaBlending(255);
		m_pZoomBackground->BlendAnimated(0, 800);

		// reset infoview
		m_bInfoOn = false;
		m_pInfoViewShowButton->SetText("Show Infos");
		m_pInfoViewShowButton->SetImageId(GUI_THEME_SALLY_ICON_DOWN);
		m_pInfoViewShowButton->Enable(false);
		m_pInfoView->MoveAnimated(m_pInfoView->GetPositionX(), INFO_VIEW_HEIGHT_HIDE, 800);

		m_pImageBoxBig->SetRotationY(SallyAPI::GUI::CONTROLROTATION_RIGHT);
		m_pImageBoxBig->SetStopAngelY(0);
		m_pImageBoxBig->RotateAnimatedY(2, false);
	}
}

/************************************************************************/
/* Helper Functions                                                     */
/************************************************************************/

bool CMainForm::GetZoom()
{
	if (m_iZoomImageControlId != -1)
		return true;
	return false;
}

void CMainForm::UpdateFromDatabase()
{
	DeleteLoadedImages();

	m_LockFolderPictureVector.Lock();

	m_vFolderPictureVector.clear();
	m_vImageOnlyVector.clear();

	m_LockFolderPictureVector.Unlock();

	ResetImages();
	UpdateImages();

	m_tGetFromDatabase.SetValues(&m_vFolderPictureVector, m_pApplicationWindow, this,
		m_pDropDownYear->GetSelectedIdentifier(), m_pDropDownMonth->GetSelectedIdentifier());
	m_tGetFromDatabase.Start();
}

void CMainForm::DatabaseLoadDone()
{
	// to make the Diashow working
	m_vImageOnlyVector = m_vFolderPictureVector;

	if (m_vImageOnlyVector.size() > 0)
	{
		m_pStartDiashow->Enable(true);
		m_pLabelDatabaseInfo->Visible(false);
	}
	else
	{
		m_pStartDiashow->Enable(false);
		m_pLabelDatabaseInfo->Visible(true);
	}

	m_tImageLoader.Start();
}

void CMainForm::UpdateImages(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	SallyAPI::GUI::SendMessage::CParameterString* message = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterString*> (messageParameter);
	if (message == NULL)
		return;

	for (int k = 0 ; k < m_iCols + 2; ++k)
	{
		for (int i = 0; i < m_iRows; ++i)
		{
			std::vector<SallyAPI::GUI::CImageBox*> imageBoxVector = m_mImage[k];
			std::vector<SallyAPI::GUI::CLabel*> imageNameVector = m_mImageName[k];
			SallyAPI::GUI::CImageBox* imageBox = imageBoxVector[i];
			SallyAPI::GUI::CLabel* labelBox = imageNameVector[i];

			if (imageBox->GetText().compare(message->GetString()) == 0)
			{
				EnterRenderLock();

				SallyAPI::GUI::CPicture* picture = m_mPictureMap[message->GetString()];

				SetPictureToBox(picture, imageBox, labelBox);

				imageBox->SetAlphaBlending(0);
				imageBox->BlendAnimated(255, 400, true);

				LeaveRenderLock();
				return;
			}
		}
	}
}

void CMainForm::UpdateImages()
{
	EnterRenderLock();

	SallyAPI::System::CAutoLock lock(&m_LockFolderPictureVector);

	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance(); 
	SallyAPI::Config::CTheme* theme = config->GetTheme();

	std::vector<std::string>::iterator iter = m_vFolderPictureVector.begin();

	int k = 0;
	int i = 0;

	if (m_iStartPicture == 0)
	{
		std::vector<SallyAPI::GUI::CImageBox*> imageBoxVector = m_mImage[0];
		std::vector<SallyAPI::GUI::CLabel*> imageNameVector = m_mImageName[0];

		for (int l = 0; l < m_iRows; ++l)
		{
			imageBoxVector[l]->Visible(false);
			imageNameVector[l]->Visible(false);
		}
		k = 1;
	}

	for (int l = 0; (l < (m_iStartPicture - 1) * m_iRows) && (iter != m_vFolderPictureVector.end()); ++l)
	{
		++iter;
	}

	SallyAPI::GUI::CPicture* picture;

	while ((k < m_iCols + 2) && (iter != m_vFolderPictureVector.end()))
	{
		std::vector<SallyAPI::GUI::CImageBox*> imageBoxVector = m_mImage[k];
		std::vector<SallyAPI::GUI::CLabel*> imageNameVector = m_mImageName[k];

		for (; (i < m_iRows) && (iter != m_vFolderPictureVector.end()); i++)
		{
			std::string filename = *iter;

			if (SallyAPI::String::StringHelper::StringEndsWith(filename, ".jpg")
				|| SallyAPI::String::StringHelper::StringEndsWith(filename, ".bmp")
				|| SallyAPI::String::StringHelper::StringEndsWith(filename, ".png")
				|| SallyAPI::String::StringHelper::StringEndsWith(filename, ".jpeg"))
			{
				imageNameVector[i]->SetText(SallyAPI::String::PathHelper::GetFileFromPath(filename));
				imageBoxVector[i]->SetText(filename);
				picture = m_mPictureMap[filename];
			}
			else
			{
				std::string name;
				if (m_iFolderDeep == 0)
				{
					name = filename;

					if (name.length() > 40)
					{
						std::string tempName = name.substr(0, name.find_first_of('\\') + 1);
						tempName.append("...");
						tempName.append(name.substr(name.length() - (40 - tempName.length()), name.length()));

						name = tempName;
					}
				}
				else
				{
					if (filename.compare("..") != 0)
						filename = filename.substr(0,filename.length() - 1);
					name = SallyAPI::String::PathHelper::GetFileFromPath(filename);
				}

				imageNameVector[i]->SetText(name);
				imageBoxVector[i]->SetText("");

				if (m_iFolderDeep == 0)
				{
					int id = m_mDriveImages[name];
					if (id != NULL)
					{
						picture = theme->GetPicture(id);
					}
					else
					{
						picture = theme->GetPicture(GUI_THEME_SALLY_ICON_BIG_FOLDER);
					}
				}
				else
				{
					picture = theme->GetPicture(GUI_THEME_SALLY_ICON_BIG_FOLDER);
				}
			}

			// Not yet loaded... use default pic
			if (picture == NULL)
				picture = theme->GetPicture(GUI_THEME_SALLY_ICON_BIG_MIMETYPE_IMAGE);

			// Now paint
			if (picture != NULL)
			{
				SallyAPI::GUI::CImageBox* imageBox = imageBoxVector[i];
				SallyAPI::GUI::CLabel* labelBox = imageNameVector[i];

				SetPictureToBox(picture, imageBox, labelBox);
			}
			else
			{
				imageBoxVector[i]->SetPicture(NULL);
				imageNameVector[i]->SetText("");
			}
			++iter;
		}
		if (iter != m_vFolderPictureVector.end())
		{
			i = 0;
			k++;
		}
	}

	for (; k < m_iCols + 2; ++k)
	{
		std::vector<SallyAPI::GUI::CImageBox*> imageBoxVector = m_mImage[k];
		std::vector<SallyAPI::GUI::CLabel*> imageNameVector = m_mImageName[k];

		for (; i < m_iRows; ++i)
		{
			imageBoxVector[i]->Visible(false);
			imageNameVector[i]->Visible(false);
			imageNameVector[i]->SetText("");
			imageBoxVector[i]->SetPicture(NULL);
		}
		i = 0;
	}

	LeaveRenderLock();
}

void CMainForm::ResetImages()
{
	for (int k = 0; k < m_iCols + 2; ++k)
	{
		std::vector<SallyAPI::GUI::CImageBox*> imageBoxVector = m_mImage[k];
		std::vector<SallyAPI::GUI::CLabel*> imageNameVector = m_mImageName[k];

		for (int i = 0; i < m_iRows; i++)
		{
			int x = 0;
			int y = 0;
			int width = 0;
			int height = 0;

			x = m_iBorderLeft;
			x += (PICTURE_WIDTH_MAX_SMALL + PICTURE_WIDTH_MAX_SMALL_SPACE) * k;
			x -= (PICTURE_WIDTH_MAX_SMALL + PICTURE_WIDTH_MAX_SMALL_SPACE);

			y = m_iBorderTop;
			y += (PICTURE_HEIGHT_MAX_SMALL + PICTURE_HEIGHT_MAX_SMALL_SPACE + CONTROL_HEIGHT) * i;

			imageBoxVector[i]->SetPicture(NULL);
			imageBoxVector[i]->Resize(PICTURE_WIDTH_MAX_SMALL, PICTURE_HEIGHT_MAX_SMALL);
			imageBoxVector[i]->Move(x, y);

			x = m_iBorderLeft;
			x += (PICTURE_WIDTH_MAX_SMALL + PICTURE_WIDTH_MAX_SMALL_SPACE) * k;
			x -= (PICTURE_WIDTH_MAX_SMALL + PICTURE_WIDTH_MAX_SMALL_SPACE);

			y = m_iBorderTop;
			y += (PICTURE_HEIGHT_MAX_SMALL + PICTURE_HEIGHT_MAX_SMALL_SPACE + CONTROL_HEIGHT) * i;
			y += PICTURE_HEIGHT_MAX_SMALL;
			imageNameVector[i]->Move(x, y + 5);
		}
	}
	m_iStartPicture = m_mListViewPages[m_iFolderDeep];
}

void CMainForm::SetPictureToBox(SallyAPI::GUI::CPicture* picture, SallyAPI::GUI::CImageBox* imageBox, SallyAPI::GUI::CLabel* labelBox) 
{
	imageBox->SetPicture(picture);
	imageBox->Visible(true);

	int xImageBox = 0;
	int yImageBox = 0;
	imageBox->GetAbsolutPosition(&xImageBox, &yImageBox);

	float rotationAngleY = xImageBox - (WINDOW_WIDTH / 2);
	rotationAngleY = rotationAngleY / 1000;

	imageBox->SetRotationAngleY(rotationAngleY);

	labelBox->Visible(true);
}

void CMainForm::DeleteLoadedImages()
{
	m_tImageLoader.Stop();

	// if thread is still running...
	m_tImageLoader.WaitForStop();
	m_tGetFromDatabase.WaitForStop();

	EnterRenderLock();
	for (int k = 0; k < m_iCols; ++k)
	{
		std::vector<SallyAPI::GUI::CImageBox*> imageBoxVector = m_mImage[k];
		for (int i = 0; i < m_iRows; ++i)
		{
			imageBoxVector[i]->SetPicture(NULL);
		}
	}

	// clear the old stuff
	std::map<std::string, SallyAPI::GUI::CPicture*>::iterator iterPicture = m_mPictureMap.begin();

	while(iterPicture != m_mPictureMap.end())
	{
		SallyAPI::GUI::CPicture* picture = iterPicture->second;
		SafeDelete(picture);
		++iterPicture;
	}
	m_mPictureMap.clear();

	m_iStartPicture = 0;
	LeaveRenderLock();
}

void CMainForm::BigPicLoadedSwitch()
{
	BigPicLoadedZoom();

	SallyAPI::GUI::GUIHelper::CalculateImageSize(m_pPictureBig->GetWidth(), m_pPictureBig->GetHeight(), PICTURE_WIDTH_MAX_BIG, PICTURE_HEIGHT_MAX_BIG,
		(int&) m_rZoomBig.left, (int&) m_rZoomBig.top, (int&) m_rZoomBig.right, (int&) m_rZoomBig.bottom);

	m_rZoomBig.left += (WINDOW_WIDTH - PICTURE_WIDTH_MAX_BIG) / 2;
	m_rZoomBig.top += (WINDOW_HEIGHT - PICTURE_HEIGHT_MAX_BIG) / 2 + 35;

	m_pImageBoxBig->BlendAnimated(255, 2);
	m_pImageBoxBig->Move(m_pImageBoxBig->GetPositionX(), m_rZoomBig.top);

	if (m_bInfoOn)
		m_pImageBoxBig->MoveAnimated(m_rZoomBig.left + 300, m_rZoomBig.top, 2000);
	else
		m_pImageBoxBig->MoveAnimated(m_rZoomBig.left, m_rZoomBig.top, 2000);
	m_pImageBoxBig->Resize(m_rZoomBig.right, m_rZoomBig.bottom);
}

void CMainForm::BigPicLoadedZoom()
{
	m_pImageBoxBig->SetPicture(m_pPictureBig);
	SetInfoViewPicture();
}

void CMainForm::SetInfoViewPicture() 
{
	m_pInfoViewSmallImage->SetPicture(m_pPictureBig);
	m_pInfoViewSmallImage->SetAlphaBlending(0);
	m_pInfoViewSmallImage->BlendAnimated(255, 400, true);

	std::string info;
	info.append(SallyAPI::String::StringHelper::ConvertToString(m_pPictureBig->GetWidth()));
	info.append(" x ");
	info.append(SallyAPI::String::StringHelper::ConvertToString(m_pPictureBig->GetHeight()));
	m_pInfoViewDesciption[0]->SetText(info);
}

/************************************************************************/
/* OnCommand                                                            */
/************************************************************************/

void CMainForm::OnCommandMouseMove(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	// remove the mouse down selection
	for (int k = 0; k < m_iCols; ++k)
	{
		for (int k = 0; k < m_iCols + 2; ++k)
		{
			std::vector<SallyAPI::GUI::CImageBox*> imageBoxVector = m_mImage[k];
			std::vector<SallyAPI::GUI::CLabel*> imageNameVector = m_mImageName[k];
			for (int i = 0; i < m_iRows; ++i)
			{
				imageBoxVector[i]->SendMessageToChilds(0, 0, GUI_MESSAGE_CONTROL_SCROLLED, NULL);
				imageNameVector[i]->SendMessageToChilds(0, 0, GUI_MESSAGE_CONTROL_SCROLLED, NULL);
			}
		}
	}

	SallyAPI::GUI::SendMessage::CParameterInteger* interger = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterInteger*> (messageParameter);
	if (interger == NULL)
		return;

	int moveValue = interger->GetInteger();

	// 	std::string ttt = SallyAPI::String::StringHelper::ConvertToString(moveValue);
	// 	ttt.append("\n");
	// 	OutputDebugStr(ttt.c_str());

	std::vector<SallyAPI::GUI::CLabel*> imageNameVectorTest = m_mImageName[0];

	// move to the left
	int xTemp = imageNameVectorTest[0]->GetPositionX();
	while ((xTemp + moveValue >= 0) && (m_iStartPicture > 0))
	{
		moveValue -= PICTURE_WIDTH_MAX_SMALL_COMPLETE;
		m_iStartPicture--;

		// 		std::string ttt;
		// 		ttt.append("--\n");
		// 		OutputDebugStr(ttt.c_str());
	}

	// move to the right
	xTemp = imageNameVectorTest[0]->GetPositionX();
	while ((xTemp + moveValue <= PICTURE_WIDTH_MAX_SMALL_COMPLETE * -1) &&
		(m_iStartPicture + m_iCols - 1 < ((m_vFolderPictureVector.size() - 1) / m_iRows) + 1))
	{
		moveValue += PICTURE_WIDTH_MAX_SMALL_COMPLETE;
		m_iStartPicture++;

		// 		std::string ttt;
		// 		ttt.append("++\n");
		// 		OutputDebugStr(ttt.c_str());
	}

	// can we scroll so far to the left?
	if (m_iStartPicture <= 0)
	{
		std::vector<SallyAPI::GUI::CLabel*> imageNameVectorStart = m_mImageName[1];

		int newX = imageNameVectorStart[0]->GetPositionX() + moveValue;
		if (newX > m_iBorderLeft)
		{
			moveValue = (imageNameVectorStart[0]->GetPositionX() - m_iBorderLeft) * -1;
			ResetBox2Object();
		}
	}

	// can we scroll so far to the right?
	if (m_iStartPicture + m_iCols - 1 >= ((m_vFolderPictureVector.size() - 1) / m_iRows) + 1)
	{
		std::vector<SallyAPI::GUI::CLabel*> imageNameVectorStart = m_mImageName[1];

		int newX = imageNameVectorStart[0]->GetPositionX() + moveValue;
		if (newX < m_iBorderLeft)
		{
			moveValue = m_iBorderLeft - imageNameVectorStart[0]->GetPositionX();
			ResetBox2Object();
		}
	}

	// rock'n'roll
	for (int k = 0; k < m_iCols + 2; ++k)
	{
		std::vector<SallyAPI::GUI::CImageBox*> imageBoxVector = m_mImage[k];
		std::vector<SallyAPI::GUI::CLabel*> imageNameVector = m_mImageName[k];
		for (int i = 0; i < m_iRows; ++i)
		{
			imageBoxVector[i]->Move(imageBoxVector[i]->GetPositionX() + moveValue, imageBoxVector[i]->GetPositionY());
			imageNameVector[i]->Move(imageNameVector[i]->GetPositionX() + moveValue, imageNameVector[i]->GetPositionY());
		}
	}
	UpdateImages();
}

void CMainForm::OnCommandProcessClicked(int reporterId)
{
	if ((reporterId == 0) || (m_iZoomImageControlId != -1))
		return;

	reporterId = reporterId - 1;

	int col = reporterId / 100;
	int row = reporterId % 100;

	std::vector<SallyAPI::GUI::CImageBox*> imageBoxVector = m_mImage[col];
	std::vector<SallyAPI::GUI::CLabel*> imageNameVector = m_mImageName[col];

	if (imageBoxVector[row]->GetText().length() == 0)
	{
		int fileID = (m_iStartPicture * m_iRows) + (reporterId % 100) + (((reporterId - 100) / 100) * m_iRows);
		std::string nextFolder = m_vFolderPictureVector[fileID];

		if (m_iFolderDeep != 0)
		{
			nextFolder = nextFolder.substr(0,nextFolder.length() - 1);
			nextFolder = SallyAPI::String::PathHelper::GetFileFromPath(nextFolder);
		}

		OnCommandNextFolder(nextFolder);
	}
	else
	{
		SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance(); 
		SallyAPI::Config::CTheme* theme = config->GetTheme();

		if (imageBoxVector[row]->GetPicture() == theme->GetPicture(GUI_THEME_SALLY_ICON_BIG_MIMETYPE_IMAGE))
			return;

		m_pMainForm->Enable(false);

		m_rZoomSmall.left = imageBoxVector[row]->GetPositionX() + m_pSmoothMoveForm->GetPositionX();
		m_rZoomSmall.top = imageBoxVector[row]->GetPositionY() + m_pSmoothMoveForm->GetPositionY();
		m_rZoomSmall.right = imageBoxVector[row]->GetWidth();
		m_rZoomSmall.bottom = imageBoxVector[row]->GetHeight();

		//imageBoxVector[row]->Visible(false);
		//imageNameVector[row]->Visible(false);

		SallyAPI::GUI::CPicture* temp = m_pImageBoxBig->GetPicture();
		SafeDelete(temp);
		m_pImageBoxBig->SetPicture(imageBoxVector[row]->GetPicture());

		m_pImageBoxBig->Move(m_rZoomSmall.left, m_rZoomSmall.top);
		m_pImageBoxBig->Resize(m_rZoomSmall.right, m_rZoomSmall.bottom);
		m_pImageBoxBig->Visible(true);

		/*
		SallyAPI::GUI::GUIHelper::CalculateImageSize(m_rZoomSmall.right, m_rZoomSmall.bottom, PICTURE_WIDTH_MAX_BIG, PICTURE_HEIGHT_MAX_BIG,
			(int&) m_rZoomBig.left, (int&) m_rZoomBig.top, (int&) m_rZoomBig.right, (int&) m_rZoomBig.bottom);

		m_rZoomBig.left += (WINDOW_WIDTH - PICTURE_WIDTH_MAX_BIG) / 2;
		m_rZoomBig.top += (WINDOW_HEIGHT - PICTURE_HEIGHT_MAX_BIG) / 2 + 35;
		*/

		m_rZoomBig.left = 25;
		m_rZoomBig.right = PICTURE_WIDTH_MAX_BIG;
		m_rZoomBig.top = MENU_HEIGHT + 25;
		m_rZoomBig.bottom = PICTURE_HEIGHT_MAX_BIG;

		m_iZoomImageControlId = reporterId;

		int xDistance = m_rZoomSmall.left - m_rZoomBig.left;
		int yDistance = m_rZoomSmall.top - m_rZoomBig.top;
		int widthDistance = m_rZoomBig.right - m_rZoomSmall.right;
		int heightDistance = m_rZoomBig.bottom - m_rZoomSmall.bottom;

		if (xDistance < 0)
			xDistance *= -1;

		if (yDistance < 0)
			yDistance *= -1;

		if (widthDistance < 0)
			widthDistance *= -1;

		if (heightDistance < 0)
			heightDistance *= -1;

		int speedWidth;
		int speedHeight;
		int speedX;
		int speedY;

		if (widthDistance > heightDistance)
		{
			speedWidth = 2000;
			speedHeight = (int) (speedWidth * (float) ((float) heightDistance / (float) widthDistance));
			speedX = (int) (speedWidth * (float) ((float) xDistance / (float) widthDistance));
			speedY = (int) (speedWidth  * (float) ((float) yDistance / (float) widthDistance));
		}
		else
		{
			speedHeight = 2000;
			speedWidth = speedHeight * (float) ((float) widthDistance / (float) heightDistance);
			speedX = speedHeight * (float) ((float) xDistance / (float) heightDistance);
			speedY = speedHeight  * (float) ((float) yDistance / (float) heightDistance);
		}

		m_pImageBoxBig->MoveAnimated(m_rZoomBig.left, m_rZoomBig.top, speedX, speedY);
		m_pImageBoxBig->ResizeAnimated(m_rZoomBig.right, m_rZoomBig.bottom, speedWidth, speedHeight);

		m_pApplicationWindow->SendMessageToParent(m_pApplicationWindow, 0, MS_SALLY_APP_START_SCREENSAVER);

		std::string file;
		if (m_pShowFolders->GetCheckStatus())
		{
			file = m_strCurrentFolder;
			file.append(imageNameVector[row]->GetText());

			m_iZoomImageNumber = (m_iStartPicture * m_iRows) + (col * m_iRows) + row - m_iRows;
			m_iZoomImageNumber -= (m_vFolderPictureVector.size() - m_vImageOnlyVector.size());
		}
		else
		{
			int fileID = (m_iStartPicture * m_iRows) + (col * m_iRows) + row - m_iRows;
			file = m_vFolderPictureVector[fileID];

			m_iZoomImageNumber = fileID;
		}

		m_pInfoViewDesciption[3]->SetText(file);
		m_pInfoViewTitle->SetText(SallyAPI::String::PathHelper::GetFileFromPath(file));
		m_pInfoViewSmallImage->SetPicture(NULL);
		m_pInfoViewDesciption[1]->SetText(SallyAPI::File::FileHelper::GetFormatedFileSize(file));
		m_pInfoViewDesciption[2]->SetText(SallyAPI::File::FileHelper::GetFormatedFileWriteDate(file));

		m_tImageLoaderZoom.SetValues(this, &m_pPictureBig, file, GUI_APP_BIG_PIC_LOADED_ZOOM);
		m_tImageLoaderZoom.Start();

		m_pZoomForm->Visible(true);
		m_pZoomBackground->SetAlphaBlending(0);
		m_pZoomBackground->BlendAnimated(255, 800);
		
		// show the infoview
		m_pInfoView->MoveAnimated(m_pInfoView->GetPositionX(), INFO_VIEW_HEIGHT_SHOW_BUTTON, 800);
	}
}

void CMainForm::OnCommandStartDiaShow()
{
	m_vFilenameVectorForScreensaver.clear();
	if (m_pShowDate->GetCheckStatus())
	{
		m_vFilenameVectorForScreensaver = m_vImageOnlyVector;
	}
	else if (m_pIncludeSubFolders->GetCheckStatus())
	{
		AddFolderToScreensaverVector(m_strCurrentFolder);
	}
	else
	{
		m_vFilenameVectorForScreensaver = m_vImageOnlyVector;
	}

	if (m_vFilenameVectorForScreensaver.size() == 0)
	{
		SallyAPI::GUI::SendMessage::CParameterMessageBox sendMessageParameterMessageBox(0, this, "Could not start slideshow, because there are no images to display because there are no images in this folder or in the subfolders.", SallyAPI::GUI::MESSAGEBOX_ICON_INFO);
		m_pParent->SendMessageToParent(this, m_iControlId, MS_SALLY_SHOW_MESSAGEBOX, &sendMessageParameterMessageBox);
		return;
	}

	int timeOut = m_pApplicationWindow->GetPropertyInt("diashowTimeout", 30);
	bool shuffle = m_pApplicationWindow->GetPropertyBool("diashowShuffle", true);
	std::string type = m_pApplicationWindow->GetPropertyString("diashowType", "SlideIn");
	m_pScreensaverForm->SetDiashowValues(&m_vFilenameVectorForScreensaver, false, timeOut, shuffle, type);
	m_pApplicationWindow->SendMessageToParent(m_pApplicationWindow, 0, MS_SALLY_APP_START_SCREENSAVER);
}

void CMainForm::OnCommandNextFolder(const std::string& nextFolder)
{
	if (nextFolder.compare("..") == 0)
	{
		m_mListViewPages[m_iFolderDeep] = 0;

		--m_iFolderDeep;
		if (m_iFolderDeep < 0)
			m_iFolderDeep = 0;

		m_iStartPicture = m_mListViewPages[m_iFolderDeep];

		if (m_iFolderDeep > 0)
			m_strCurrentFolder = SallyAPI::String::PathHelper::GetUpperDirectory(m_strCurrentFolder);
	}
	else
	{
		if (nextFolder.length() > 0)
		{
			m_mListViewPages[m_iFolderDeep] = m_iStartPicture;

			m_strCurrentFolder.append(nextFolder);
			++m_iFolderDeep;
			m_strCurrentFolder = SallyAPI::String::PathHelper::CorrectPath(m_strCurrentFolder);
		}
	}
	if (m_iFolderDeep == 0)
	{
		m_strCurrentFolder = "";
	}
	OnCommandChangeFolder(m_strCurrentFolder);
}

void CMainForm::OnCommandChangeFolder(std::string& folder)
{
	// stop the thread
	DeleteLoadedImages();

	m_LockFolderPictureVector.Lock();

	// now load the new stuff
	m_vFolderPictureVector.clear();
	m_vImageOnlyVector.clear();

	m_LockFolderPictureVector.Unlock();

	if (folder.length() == 0)
	{
		m_iFolderDeep = 0;
		m_strCurrentFolder = "";
		OnCommandResetFolder();
		m_pBreadcrumb->SetText("");
	}
	else
	{
		OnCommandOpenFolder(folder);
	}
	ResetImages();
	UpdateImages();

	m_tImageLoader.Start();
}

void CMainForm::OnCommandResetFolder()
{
	DeleteLoadedImages();

	SallyAPI::System::CAutoLock lock(&m_LockFolderPictureVector);

	for (int i = 0; i < 12; i++)
	{
		std::string dir;

		dir.append("dir");
		dir.append(SallyAPI::String::StringHelper::ConvertToString(i));
		dir.append(".path");

		std::string dirTemp = m_pApplicationWindow->GetPropertyString(dir);

		if (dirTemp.length() > 0)
		{
			dirTemp = SallyAPI::String::PathHelper::CorrectPath(dirTemp);
			m_vFolderPictureVector.push_back(dirTemp);
		}
	}

	// empty drive image map
	m_mDriveImages.clear();

	bool hd = (dynamic_cast<SallyAPI::GUI::CAppBase*> (m_pParent))->GetPropertyBool("alwaysShowHds", true);

	std::map<std::string, SallyAPI::File::DRIVE_TYPE> driveList = SallyAPI::File::FileHelper::GetDriveList();
	std::map<std::string, SallyAPI::File::DRIVE_TYPE>::iterator iter = driveList.begin();

	while (iter != driveList.end())
	{
		SallyAPI::File::DRIVE_TYPE driveType = iter->second;
		std::string driveLetter = iter->first;

		switch (driveType)
		{
		case SallyAPI::File::DRIVE_TYPE_CDROM:
			m_vFolderPictureVector.push_back(driveLetter);
			m_mDriveImages[driveLetter] = GUI_THEME_SALLY_ICON_BIG_CD;
			break;
		case SallyAPI::File::DRIVE_TYPE_DVDROM:
			m_vFolderPictureVector.push_back(driveLetter);
			m_mDriveImages[driveLetter] = GUI_THEME_SALLY_ICON_BIG_DVD;
			break;
		case SallyAPI::File::DRIVE_TYPE_REMOVABLE:
			m_vFolderPictureVector.push_back(driveLetter);
			m_mDriveImages[driveLetter] = GUI_THEME_SALLY_ICON_BIG_USB;
			break;
		default:
			if (hd)
			{
				m_vFolderPictureVector.push_back(driveLetter);
				m_mDriveImages[driveLetter] = GUI_THEME_SALLY_ICON_BIG_HD;
			}
			break;
		}
		++iter;
	}

	m_pStartDiashow->Enable(false);
	m_pIncludeSubFolders->Enable(false);
	m_pFolderUp->Enable(false);
}

void CMainForm::OnCommandOpenFolder(std::string& folder)
{
	HANDLE				hFile;
	WIN32_FIND_DATA		fileInformation;
	std::string			firstFile;

	folder = SallyAPI::String::PathHelper::CorrectPath(folder);

	firstFile.append(folder);
	firstFile.append("*");

	hFile = FindFirstFile(firstFile.c_str(), &fileInformation);

	SallyAPI::System::CAutoLock lock(&m_LockFolderPictureVector);

	if(hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			if ((strcmp(fileInformation.cFileName, ".") != NULL) &&
				(strcmp(fileInformation.cFileName, "..") != NULL))
			{
				std::string filename;
				filename.append(folder);
				filename.append(fileInformation.cFileName);

				if (fileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					filename = SallyAPI::String::PathHelper::CorrectPath(filename);
					m_vFolderPictureVector.push_back(filename);
				}
				else if (SallyAPI::String::StringHelper::StringEndsWith(filename, ".jpg")
					|| SallyAPI::String::StringHelper::StringEndsWith(filename, ".bmp")
					|| SallyAPI::String::StringHelper::StringEndsWith(filename, ".png")
					|| SallyAPI::String::StringHelper::StringEndsWith(filename, ".jpeg"))
				{
					m_vImageOnlyVector.push_back(filename);
				}
			}
		} while(FindNextFile(hFile, &fileInformation) == TRUE);
	}
	FindClose(hFile);

	std::sort(m_vFolderPictureVector.begin(), m_vFolderPictureVector.end(), SallyAPI::String::StringHelper::StringCompareCaseInsensitivity);
	std::sort(m_vImageOnlyVector.begin(), m_vImageOnlyVector.end(), SallyAPI::String::StringHelper::StringCompareCaseInsensitivity);

	m_vFolderPictureVector.insert(m_vFolderPictureVector.end(), m_vImageOnlyVector.begin(), m_vImageOnlyVector.end());

	m_pBreadcrumb->SetText(folder + " (" + SallyAPI::String::StringHelper::ConvertToString((int) m_vImageOnlyVector.size()) + ")");

	m_pStartDiashow->Enable(true);
	m_pIncludeSubFolders->Enable(true);
	m_pFolderUp->Enable(true);
}


void CMainForm::OnCommandShowDate()
{
	m_pShowFolders->SetCheckStatus(false);
	m_pShowDate->SetCheckStatus(true);

	m_pBreadcrumb->Visible(false);
	m_pLabelFilter->Visible(true);
	m_pLabelYear->Visible(true);
	m_pDropDownYear->Visible(true);
	m_pLabelMonth->Visible(true);
	m_pDropDownMonth->Visible(true);

	m_pIncludeSubFolders->Enable(false);
	m_pFolderUp->Enable(false);
	m_pRefreshFolder->Enable(false);

	m_mListViewPages.clear();

	UpdateFromDatabase();
}

void CMainForm::OnCommandShowFolders()
{
	m_mListViewPages.clear();

	m_tGetFromDatabase.WaitForStop();

	m_pShowFolders->SetCheckStatus(true);
	m_pShowDate->SetCheckStatus(false);

	m_pBreadcrumb->Visible(true);
	m_pLabelFilter->Visible(false);
	m_pLabelYear->Visible(false);
	m_pDropDownYear->Visible(false);
	m_pLabelMonth->Visible(false);
	m_pDropDownMonth->Visible(false);

	m_pLabelDatabaseInfo->Visible(false);

	m_pIncludeSubFolders->Enable(true);
	m_pFolderUp->Enable(true);
	m_pRefreshFolder->Enable(true);

	std::string reset = "";
	OnCommandChangeFolder(reset);
}

void CMainForm::AddFolderToScreensaverVector(std::string& folder)
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
					AddFolderToScreensaverVector(filename);
				}
				else  if (SallyAPI::String::StringHelper::StringEndsWith(filename, ".jpg")
					|| SallyAPI::String::StringHelper::StringEndsWith(filename, ".bmp")
					|| SallyAPI::String::StringHelper::StringEndsWith(filename, ".png")
					|| SallyAPI::String::StringHelper::StringEndsWith(filename, ".jpeg"))
				{
					m_vFilenameVectorForScreensaver.push_back(filename);
				}
			}
		} while(FindNextFile(hFile, &FileInformation) == TRUE);
	}
	FindClose(hFile);
	return;
}

bool CMainForm::SpecialKeyPressed(int key)
{
	if (!GetZoom())
		return false;

	switch (key)
	{
	case SPECIAL_KEY_PREVIOUS:
		OnCommandPreviousImage();
		return true;
	case SPECIAL_KEY_NEXT:
		OnCommandNextImage();
		return true;
	}
	return false;
}

void CMainForm::OnCommandRefreshFolder()
{
	int temp = m_iStartPicture;
	int count = m_vFolderPictureVector.size();

	OnCommandChangeFolder(m_strCurrentFolder);

	if (m_vFolderPictureVector.size() >= count)
	{
		m_iStartPicture = temp;
		UpdateImages();
	}
}