////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\AddMusicAlbum.cpp
///
/// \brief	Implements the add music album class. 
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

#include "AddMusicAlbum.h"

CAddMusicAlbum::CAddMusicAlbum(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, CPlaylist* playlist)
	:SallyAPI::GUI::CForm(parent, 0, -WINDOW_HEIGHT, WINDOW_WIDTH - MENU_WIDTH, WINDOW_HEIGHT),
	m_iStartPicture(0), m_pPlaylist(playlist), m_iGraphicId(graphicId)
{
	m_pSmoothMoveForm = new SallyAPI::GUI::CForm(this, 0, MENU_HEIGHT + 5,
		WINDOW_WIDTH, WINDOW_HEIGHT - MENU_HEIGHT - 10);
	m_pSmoothMoveForm->SetScrollType(SallyAPI::GUI::SCROLL_TYPE_SMOOTH);
	this->AddChild(m_pSmoothMoveForm);

	// cols / rows
	m_iCols = WINDOW_WIDTH / (PICTURE_WIDTH_MAX_SMALL + PICTURE_WIDTH_MAX_SMALL_SPACE) + 1;
	m_iBorderLeft = (WINDOW_WIDTH % (PICTURE_WIDTH_MAX_SMALL + PICTURE_WIDTH_MAX_SMALL_SPACE)) / 2;
	m_iRows = (m_pSmoothMoveForm->GetHeight() - 90) / (PICTURE_HEIGHT_MAX_SMALL + PICTURE_HEIGHT_MAX_SMALL_SPACE + 40);
	//m_iBorderTop = (m_pSmoothMoveForm->GetHeight() % (PICTURE_HEIGHT_MAX_SMALL + PICTURE_HEIGHT_MAX_SMALL_SPACE + CONTROL_HEIGHT)) / 2;
	m_iBorderTop = 90;

	int l = (m_pSmoothMoveForm->GetHeight() - 90)  % (m_iRows * (PICTURE_HEIGHT_MAX_SMALL + PICTURE_HEIGHT_MAX_SMALL_SPACE + 40));
	m_iBorderTop += l / 2;

	for (int k = 0 ; k < m_iCols + 2; ++k)
	{
		std::vector<SallyAPI::GUI::CImageBox*>	imageBoxVector;
		std::vector<SallyAPI::GUI::CLabelBox*>	imageNameVector;
		std::vector<SallyAPI::GUI::CButton*>	imageAddAlbumVector;

		for (int i = 0; i < m_iRows; ++i)
		{
			int id = (k * 100) + i + 1;

			SallyAPI::GUI::CImageBox* imageBox = new SallyAPI::GUI::CImageBox(m_pSmoothMoveForm, 0, 0, 0, 0, id);
			imageBox->SetLocalised(false);
			imageBoxVector.push_back(imageBox);

			m_pSmoothMoveForm->AddChild(imageBox);

			SallyAPI::GUI::CLabelBox* imageName = new SallyAPI::GUI::CLabelBox(m_pSmoothMoveForm, 0, 0, PICTURE_WIDTH_MAX_SMALL - 35, 40);
			imageNameVector.push_back(imageName);
			imageName->SetFont("scrolllist.font");
			imageName->SetLocalised(false);
			imageName->SetAlign(DT_LEFT | DT_TOP | DT_WORDBREAK);

			m_pSmoothMoveForm->AddChild(imageName);

			SallyAPI::GUI::CButton* button = new SallyAPI::GUI::CButton(m_pSmoothMoveForm, 0, 0, CONTROL_HEIGHT, CONTROL_HEIGHT, id);
			imageAddAlbumVector.push_back(button);
			button->SetImageId(GUI_THEME_SALLY_ICON_ADD);

			m_pSmoothMoveForm->AddChild(button);
		}
		m_mImage[k] = imageBoxVector;
		m_mImageName[k] = imageNameVector;
		m_mImageAddAlbum[k] = imageAddAlbumVector;
	}

	/************************************************************************/
	/* Filter Controls                                                      */
	/************************************************************************/
	m_pFilterBackground = new SallyAPI::GUI::CGroupBox(this, (m_iWidth - 570) / 2, -10, 570, CONTROL_HEIGHT + 50);
	this->AddChild(m_pFilterBackground);

	m_pArtistFilter = new SallyAPI::GUI::CEdit(m_pFilterBackground, 20, 30, 170, GUI_APP_FILTER_ARTIST_EDIT);
	m_pArtistFilter->SetInfoText("Filter by Artist Name");
	m_pArtistFilter->SetImageId(GUI_THEME_SALLY_ICON_SEARCH);
	m_pFilterBackground->AddChild(m_pArtistFilter);

	m_pAlbumFilter = new SallyAPI::GUI::CEdit(m_pFilterBackground, 20 + 170 + 10, 30, 170, GUI_APP_FILTER_ALBUM_EDIT);
	m_pAlbumFilter->SetInfoText("Filter by Album Name");
	m_pAlbumFilter->SetImageId(GUI_THEME_SALLY_ICON_SEARCH);
	m_pFilterBackground->AddChild(m_pAlbumFilter);

	m_pGenreFilter = new SallyAPI::GUI::CEdit(m_pFilterBackground, 20 + 170 + 10 + 170 + 10, 30, 170, GUI_APP_FILTER_GENRE_EDIT);
	m_pGenreFilter->SetInfoText("Filter by Genre Name");
	m_pGenreFilter->SetImageId(GUI_THEME_SALLY_ICON_SEARCH);
	m_pFilterBackground->AddChild(m_pGenreFilter);

	/************************************************************************/
	/* Zoom                                                                 */
	/************************************************************************/
	m_pZoomBackground = new SallyAPI::GUI::CImageBox(this, 0, 0, MAX_WIDTH, MAX_HEIGHT);
	m_pZoomBackground->SetImageId(GUI_THEME_SALLY_POPUP_NORMAL);
	m_pZoomBackground->Visible(false);
	this->AddChild(m_pZoomBackground);

	m_pImageBoxBig = new SallyAPI::GUI::CImageBox(this, 0, 0, 0, 0);
	m_pImageBoxBig->Visible(false);
	this->AddChild(m_pImageBoxBig);

	// Background
	m_pAlbumBackground = new SallyAPI::GUI::CGroupBox(this, (WINDOW_WIDTH - MENU_WIDTH - 500) / 2, (WINDOW_HEIGHT - 500) / 2, 500, 500);
	m_pAlbumBackground->Visible(false);
	this->AddChild(m_pAlbumBackground);

	m_pAlbumName = new SallyAPI::GUI::CLabel(m_pAlbumBackground, 50, 20, 290);
	m_pAlbumName->SetBold(true);
	m_pAlbumName->SetLocalised(false);
	m_pAlbumName->SetAlign(DT_CENTER | DT_VCENTER);
	m_pAlbumBackground->AddChild(m_pAlbumName);

	std::vector<int>	imageListSearch;
	imageListSearch.push_back(GUI_THEME_SALLY_ICON_MIMETYPE_MP3);
	imageListSearch.push_back(GUI_THEME_SALLY_ICON_MIMETYPE_VIDEO);

	m_pAlbumTitles = new SallyAPI::GUI::CListView(m_pAlbumBackground, 20, 20 + CONTROL_HEIGHT + 10, 500 - 40, 500 - (20 + CONTROL_HEIGHT + 10 + 20), 0, GUI_THEME_SALLY_ICON_ADD, imageListSearch);
	m_pAlbumBackground->AddChild(m_pAlbumTitles);

	m_pAlbumBackButton = new SallyAPI::GUI::CButton(m_pAlbumBackground, 20, 20, CONTROL_HEIGHT, CONTROL_HEIGHT, GUI_APP_ALBUM_BACK, SallyAPI::GUI::BUTTON_TYPE_NORMAL);
	m_pAlbumBackButton->SetImageId(GUI_THEME_SALLY_ICON_PREVIOUS);
	m_pAlbumBackground->AddChild(m_pAlbumBackButton);

	m_pAlbumAddAll = new SallyAPI::GUI::CButton(m_pAlbumBackground, 500 - 160, 20, 140, CONTROL_HEIGHT, GUI_APP_ADD_ALL_ALBUM, SallyAPI::GUI::BUTTON_TYPE_NORMAL);
	m_pAlbumAddAll->SetText("Add All");
	m_pAlbumAddAll->SetImageId(GUI_THEME_SALLY_ICON_ADD);
	m_pAlbumBackground->AddChild(m_pAlbumAddAll);

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

	m_pb2Object = new CBox2DObject(m_pSmoothMoveForm, 0, MENU_HEIGHT + 5, WINDOW_WIDTH,
		WINDOW_HEIGHT - MENU_HEIGHT - 5 - CONTROL_HEIGHT - WINDOW_BORDER_V - 5);
	m_pSmoothMoveForm->AddChild(m_pb2Object);

	m_iOldPosition = m_pb2Object->GetPositionX();
	m_pb2Object->CreateBox2DObject(m_pb2World);

	// set image values
	m_tImageLoader.SetValues(&m_vAlbumList, &m_mPictureMap, this, &m_iStartPicture, m_iCols, m_iRows);
}

CAddMusicAlbum::~CAddMusicAlbum()
{
	DeleteLoadedCovers();
	SafeDelete(m_pb2World);
}

void CAddMusicAlbum::Timer(float timeDelta)
{
	SallyAPI::GUI::CForm::Timer(timeDelta);

	m_pb2World->Step(timeDelta * 4, 10);

	if ((!m_bMouseDown) && (m_bEnabled))
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

bool CAddMusicAlbum::CheckProcessMouseDown(int x, int y)
{
	if (ResetBox2Object())
	{
		m_pb2Object->CheckProcessMouseDown(x, y);
		return true;
	}

	return SallyAPI::GUI::CForm::CheckProcessMouseDown(x, y);
}


bool CAddMusicAlbum::ResetBox2Object()
{
	bool move = m_pb2Object->MoveAndHold(0, MENU_HEIGHT + 5);
	m_iOldPosition = m_pb2Object->GetPositionX();

	return move;
}

void CAddMusicAlbum::AddToPlaylistFromListView(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter, SallyAPI::GUI::CListView	*listView)
{
	SallyAPI::GUI::SendMessage::CParameterInteger* parameterInteger = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterInteger*> (messageParameter);

	SallyAPI::GUI::CListViewItem *listItem = listView->GetItem(parameterInteger->GetInteger());
	SallyAPI::GUI::CListViewItem listItemTemp(listItem->GetIdentifier(), listItem->GetText(),
		listItem->GetImageId());

	if (m_pPlaylist->AddItem(listItemTemp) == false)
		return;

	SallyAPI::GUI::SendMessage::CParameterOnScreenMenu messageOnScreenMenu(GUI_THEME_SALLY_OSM_ADD, "Added");
	m_pParent->SendMessageToParent(this, 0, MS_SALLY_ON_SCREEN_MENU, &messageOnScreenMenu);
	return;
}

void CAddMusicAlbum::AddAllToPlaylistFromListView(SallyAPI::GUI::CListView *listView)
{
	int i = listView->GetListSize();
	for (int k = 0; k < i; k++)
	{
		SallyAPI::GUI::SendMessage::CParameterInteger messageInteger(k);
		AddToPlaylistFromListView(&messageInteger, listView);
	}
	SallyAPI::GUI::SendMessage::CParameterOnScreenMenu messageOnScreenMenu(GUI_THEME_SALLY_OSM_ADD, "Added");
	m_pParent->SendMessageToParent(this, 0, MS_SALLY_ON_SCREEN_MENU, &messageOnScreenMenu);
	return;
}

void CAddMusicAlbum::LoadConfig()
{
	OnCommandUpdateFilter();
}

void CAddMusicAlbum::DeleteLoadedCovers()
{
	// if thread is still running...
	m_tImageLoader.WaitForStop();

	// clear the old stuff
	std::map<std::string, SallyAPI::GUI::CPicture*>::iterator iterPicture = m_mPictureMap.begin();

	while(iterPicture != m_mPictureMap.end())
	{
		SallyAPI::GUI::CPicture* picture = iterPicture->second;

		SafeDelete(picture);
		++iterPicture;
	}
	m_mPictureMap.clear();

	std::vector<CAlbum*>::iterator iterAlbum = m_vAlbumList.begin();
	while (iterAlbum != m_vAlbumList.end())
	{
		SafeDelete(*iterAlbum);
		++iterAlbum;
	}
	m_vAlbumList.clear();
}

void CAddMusicAlbum::OnCommandUpdateFilter()
{
	std::string artist = m_pArtistFilter->GetText();
	std::string album = m_pAlbumFilter->GetText();
	std::string genre = m_pGenreFilter->GetText();

	DeleteLoadedCovers();

	m_pZoomBackground->Visible(false);
	m_pAlbumBackground->Visible(false);

	m_iStartPicture = 0;

	ResetImages();
	UpdateImages();

	CMediaDatabase::GetAlbumsFromDatabase(dynamic_cast<SallyAPI::GUI::CAppBase*> (m_pParent), &m_vAlbumList, artist, album, genre);
	m_tImageLoader.Start();
}

void CAddMusicAlbum::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_APP_RENDER_LOCK:
		EnterRenderLock();
		return;
	case GUI_APP_RENDER_RELEASE:
		LeaveRenderLock();
		return;
	case GUI_CONTROL_RESIZED:
		if (reporter == m_pImageBoxBig)
		{
			m_pAlbumBackground->Visible(true);
			m_pAlbumBackground->SetAlphaBlending(0);
			m_pAlbumBackground->BlendAnimated(255, 800);
		}
		return;
	case GUI_CONTROL_BLENDED:
		if ((m_pAlbumBackground == reporter) && (m_pAlbumBackground->GetAlphaBlending() == 0))
		{
			m_pSmoothMoveForm->Enable(true);
			m_pFilterBackground->Enable(true);
			m_pAlbumBackground->Visible(false);
			m_pZoomBackground->Visible(false);
		}
		else if ((m_pAlbumBackground == reporter) && (m_pAlbumBackground->GetAlphaBlending() == 255))
		{
			m_pImageBoxBig->Visible(false);
			m_pImageBoxBig->SetPicture(NULL);
		}
		return;
	case GUI_MOUSEMOVE_SMOOTH_LEFT:
	case GUI_MOUSEMOVE_SMOOTH_RIGHT:
		if (reporter == m_pSmoothMoveForm)
		{
			OnCommandMouseMove(messageParameter);
		}
		return;
	case GUI_BUTTON_CLICKED:
		switch(reporterId)
		{
		case GUI_APP_ALBUM_BACK:
			OnCommandBackClicked();
			return;
		case GUI_APP_ADD_ALL_ALBUM:
			AddAllToPlaylistFromListView(m_pAlbumTitles);
			return;
		default:
			OnCommandAddAlbum(reporterId);
			break;
		}
		break;
	case GUI_EDIT_CHANGED:
		switch (reporterId)
		{
		case GUI_APP_FILTER_GENRE_EDIT:
		case GUI_APP_FILTER_ALBUM_EDIT:
		case GUI_APP_FILTER_ARTIST_EDIT:
			OnCommandUpdateFilter();
			return;
		}
		break;
	case GUI_LISTVIEW_ITEM_DOUBLECLICKED:
		if (reporter == m_pAlbumTitles)
			OnCommandDoubleClicked(messageParameter);
		return;
	case GUI_LISTVIEW_ITEM_ACTION_CLICKED:
	case GUI_LISTVIEW_ITEM_CLICKED:
		if (reporter == m_pAlbumTitles)
		{
			AddToPlaylistFromListView(messageParameter, m_pAlbumTitles);
			return;
		}
		break;
	case GUI_APP_FOLDER_PIC_CHANGED:
		UpdateImages();
		return;
	case GUI_APP_FOLDER_PIC_LOADED:
		UpdateImages(messageParameter);
		return;
 	case GUI_IMAGEBOX_CLICKED:
		OnCommandProcessClicked(reporterId);
 		break;
	}
	SallyAPI::GUI::CForm::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

void CAddMusicAlbum::OnCommandDoubleClicked(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	SallyAPI::GUI::SendMessage::CParameterInteger* parameterInteger = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterInteger*> (messageParameter);

	SallyAPI::GUI::CListViewItem* listItem = m_pAlbumTitles->GetItem(parameterInteger->GetInteger());

	SallyAPI::GUI::SendMessage::CParameterString playNow(listItem->GetIdentifier());
	m_pParent->SendMessageToParent(this, 0, GUI_APP_PLAY_LAST_ADDED, &playNow);
}

void CAddMusicAlbum::UpdateImages(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	SallyAPI::GUI::SendMessage::CParameterString* message = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterString*> (messageParameter);
	if (message == NULL)
		return;

	for (int k = 0 ; k < m_iCols + 2; ++k)
	{
		for (int i = 0; i < m_iRows; ++i)
		{
			std::vector<SallyAPI::GUI::CImageBox*> imageBoxVector = m_mImage[k];
			std::vector<SallyAPI::GUI::CLabelBox*> imageNameVector = m_mImageName[k];
			std::vector<SallyAPI::GUI::CButton*> imageAddAllVector = m_mImageAddAlbum[k];
			SallyAPI::GUI::CImageBox* imageBox = imageBoxVector[i];
			SallyAPI::GUI::CLabelBox* labelBox = imageNameVector[i];
			SallyAPI::GUI::CButton* button = imageAddAllVector[i];

			if (imageBox->GetText().compare(message->GetString()) == 0)
			{
				EnterRenderLock();

				SallyAPI::GUI::CPicture* picture = m_mPictureMap[message->GetString()];

				imageBox->SetPicture(picture);
				imageBox->Visible(true);

				imageBox->SetAlphaBlending(0);
				imageBox->BlendAnimated(255, 400, true);

				labelBox->Visible(true);
				button->Visible(true);

				LeaveRenderLock();

				// break here
				return;
			}
		}
	}
}

void CAddMusicAlbum::UpdateImages()
{
	EnterRenderLock();

	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance(); 
	SallyAPI::Config::CTheme* theme = config->GetTheme();

	std::vector<CAlbum*>::iterator iter = m_vAlbumList.begin();

	int k = 0;
	int i = 0;

	if (m_iStartPicture == 0)
	{
		std::vector<SallyAPI::GUI::CImageBox*> imageBoxVector = m_mImage[0];
		std::vector<SallyAPI::GUI::CLabelBox*> imageNameVector = m_mImageName[0];
		std::vector<SallyAPI::GUI::CButton*> imageAddAllVector = m_mImageAddAlbum[0];

		for (int l = 0; l < m_iRows; ++l)
		{
			imageBoxVector[l]->Visible(false);
			imageNameVector[l]->Visible(false);
			imageAddAllVector[l]->Visible(false);
		}
		k = 1;
	}

	for (int l = 0; (l < (m_iStartPicture - 1) * m_iRows) && (iter != m_vAlbumList.end()); ++l)
	{
		++iter;
	}

	SallyAPI::GUI::CPicture* picture;

	while ((k < m_iCols + 2) && (iter != m_vAlbumList.end()))
	{
		std::vector<SallyAPI::GUI::CImageBox*> imageBoxVector = m_mImage[k];
		std::vector<SallyAPI::GUI::CLabelBox*> imageNameVector = m_mImageName[k];
		std::vector<SallyAPI::GUI::CButton*> imageAddAllVector = m_mImageAddAlbum[k];

		for (; (i < m_iRows) && (iter != m_vAlbumList.end()); i++)
		{
			CAlbum* album = *iter;

			std::string covername = album->GetCoverName();
			
			std::string filename = SallyAPI::String::PathHelper::GetDirectoryFromPath(album->GetFilename());
			filename = SallyAPI::String::PathHelper::CorrectPath(filename);

			filename.append(covername);

			std::string description = album->GetArtist();
			description.append("\n");
			description.append(album->GetAlbum());

			imageNameVector[i]->SetText(description);
			imageBoxVector[i]->SetText(filename);

			picture = m_mPictureMap[filename];

			// Not yet loaded... use default pic
			if (picture == NULL)
				picture = theme->GetPicture(GUI_APP_DEFAULT_CD + m_iGraphicId);

			// Now paint
			if (picture != NULL)
			{
				imageBoxVector[i]->SetPicture(picture);
				imageBoxVector[i]->Visible(true);

				imageNameVector[i]->Visible(true);

				imageAddAllVector[i]->Visible(true);
			}
			else
			{
				imageBoxVector[i]->SetPicture(NULL);
				imageNameVector[i]->SetText("");
				imageNameVector[i]->Visible(false);
				imageAddAllVector[i]->Visible(false);
			}
			++iter;
		}
		if (iter != m_vAlbumList.end())
		{
			i = 0;
			k++;
		}
	}

	for (; k < m_iCols + 2; ++k)
	{
		std::vector<SallyAPI::GUI::CImageBox*> imageBoxVector = m_mImage[k];
		std::vector<SallyAPI::GUI::CLabelBox*> imageNameVector = m_mImageName[k];
		std::vector<SallyAPI::GUI::CButton*> imageAddAllVector = m_mImageAddAlbum[k];

		for (; i < m_iRows; ++i)
		{
			imageBoxVector[i]->Visible(false);
			imageNameVector[i]->Visible(false);
			imageAddAllVector[i]->Visible(false);
			imageNameVector[i]->SetText("");
			imageBoxVector[i]->SetPicture(NULL);
		}
		i = 0;
	}

	LeaveRenderLock();
}

void CAddMusicAlbum::ResetImages()
{
	for (int k = 0; k < m_iCols + 2; ++k)
	{
		std::vector<SallyAPI::GUI::CImageBox*> imageBoxVector = m_mImage[k];
		std::vector<SallyAPI::GUI::CLabelBox*> imageNameVector = m_mImageName[k];
		std::vector<SallyAPI::GUI::CButton*> imageAddAllVector = m_mImageAddAlbum[k];

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

			imageAddAllVector[i]->Move(x + PICTURE_WIDTH_MAX_SMALL - 30, y + 5);
		}
	}
	m_iStartPicture = 0;
}

void CAddMusicAlbum::OnCommandMouseMove(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	// remove the mouse down selection
	for (int k = 0; k < m_iCols + 2; ++k)
	{
		std::vector<SallyAPI::GUI::CImageBox*> imageBoxVector = m_mImage[k];
		std::vector<SallyAPI::GUI::CLabelBox*> imageNameVector = m_mImageName[k];
		std::vector<SallyAPI::GUI::CButton*> imageAddAllVector = m_mImageAddAlbum[k];
		for (int i = 0; i < m_iRows; ++i)
		{
			imageBoxVector[i]->SendMessageToChilds(0, 0, GUI_MESSAGE_CONTROL_SCROLLED, NULL);
			imageNameVector[i]->SendMessageToChilds(0, 0, GUI_MESSAGE_CONTROL_SCROLLED, NULL);
			imageAddAllVector[i]->SendMessageToChilds(0, 0, GUI_MESSAGE_CONTROL_SCROLLED, NULL);
		}
	}

	SallyAPI::GUI::SendMessage::CParameterInteger* interger = (SallyAPI::GUI::SendMessage::CParameterInteger*) messageParameter;
	if (interger == NULL)
		return;

	int moveValue = interger->GetInteger();

// 	std::string ttt = SallyAPI::String::StringHelper::ConvertToString(moveValue);
// 	ttt.append("\n");
// 	OutputDebugStr(ttt.c_str());

	std::vector<SallyAPI::GUI::CLabelBox*> imageNameVectorTest = m_mImageName[0];

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
		(m_iStartPicture + m_iCols - 2 < ((m_vAlbumList.size() - 1) / m_iRows) + 1))
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
		std::vector<SallyAPI::GUI::CLabelBox*> imageNameVectorStart = m_mImageName[1];

		int newX = imageNameVectorStart[0]->GetPositionX() + moveValue;
		if (newX > m_iBorderLeft)
		{
			moveValue = (imageNameVectorStart[0]->GetPositionX() - m_iBorderLeft) * -1;
			ResetBox2Object();
		}
	}

	// can we scroll so far to the right?
	if (m_iStartPicture + m_iCols - 2 >= ((m_vAlbumList.size() - 1) / m_iRows) + 1)
	{
		std::vector<SallyAPI::GUI::CLabelBox*> imageNameVectorStart = m_mImageName[1];

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
		std::vector<SallyAPI::GUI::CLabelBox*> imageNameVector = m_mImageName[k];
		std::vector<SallyAPI::GUI::CButton*> imageAddAllVector = m_mImageAddAlbum[k];
		for (int i = 0; i < m_iRows; ++i)
		{
			imageBoxVector[i]->Move(imageBoxVector[i]->GetPositionX() + moveValue, imageBoxVector[i]->GetPositionY());
			imageNameVector[i]->Move(imageNameVector[i]->GetPositionX() + moveValue, imageNameVector[i]->GetPositionY());
			imageAddAllVector[i]->Move(imageAddAllVector[i]->GetPositionX() + moveValue, imageAddAllVector[i]->GetPositionY());
		}
	}
	UpdateImages();
}

void CAddMusicAlbum::OnCommandProcessClicked(int reporterId)
{
	if (reporterId == 0)
		return;

	reporterId = reporterId - 1;

	int col = reporterId / 100;
	int row = reporterId % 100;

	std::vector<SallyAPI::GUI::CImageBox*> imageBoxVector = m_mImage[col];
	std::vector<SallyAPI::GUI::CLabelBox*> imageNameVector = m_mImageName[col];

	int fileID = (m_iStartPicture * m_iRows) + (col * m_iRows) + row - m_iRows;
	CAlbum* album = m_vAlbumList[fileID];

	m_tAlbumGetter.SetValues(dynamic_cast<SallyAPI::GUI::CAppBase*> (m_pParent), m_pAlbumTitles, album->GetAlbum(), album->GetArtist());
	m_tAlbumGetter.Start();

	m_pSmoothMoveForm->Enable(false);
	m_pFilterBackground->Enable(false);

	// Set Album und Artist
	std::string temp = album->GetAlbum();
	temp.append("\n");
	temp.append(album->GetArtist());

	m_pAlbumName->SetText(temp);

	// move speed
	int xDistance = imageBoxVector[row]->GetPositionX() - (m_pAlbumBackground->GetPositionX() + 5);
	int yDistance = imageBoxVector[row]->GetPositionY() - (m_pAlbumBackground->GetPositionY() + 5);
	int widthDistance = (500 - 10) - imageBoxVector[row]->GetWidth();
	int heightDistance = (500 - 10) - imageBoxVector[row]->GetHeight();

	if (xDistance < 0)
		xDistance *= -1;

	if (yDistance < 0)
		yDistance *= -1;

	if (widthDistance < 0)
		widthDistance *= -1;

	if (heightDistance < 0)
		heightDistance *= -1;

	int speedX = (int) (600 * (float) ((float) xDistance / (float) widthDistance));
	int speedY = (int) (600 * (float) ((float) yDistance / (float) widthDistance));

	m_pImageBoxBig->Move(imageBoxVector[row]->GetPositionX(), imageBoxVector[row]->GetPositionY());
	m_pImageBoxBig->Resize(imageBoxVector[row]->GetWidth(), imageBoxVector[row]->GetHeight());
	m_pImageBoxBig->MoveAnimated(m_pAlbumBackground->GetPositionX() + 5, m_pAlbumBackground->GetPositionY() + 5, speedX, speedY);
	m_pImageBoxBig->ResizeAnimated(500 - 10, 500 - 10, 600);
	m_pImageBoxBig->SetPicture(imageBoxVector[row]->GetPicture());
	
	m_pImageBoxBig->SetRotationAngleY(0.0f);
	m_pImageBoxBig->SetStopAngelY(180);
	m_pImageBoxBig->RotateAnimatedY(6, false);
	m_pImageBoxBig->Visible(true);

	m_pZoomBackground->Visible(true);
	m_pZoomBackground->SetAlphaBlending(0);
	m_pZoomBackground->BlendAnimated(255, 800);
}

void CAddMusicAlbum::OnCommandAddAlbum(int reporterId)
{
	if (reporterId == 0)
		return;

	reporterId = reporterId - 1;

	int col = reporterId / 100;
	int row = reporterId % 100;

	std::vector<SallyAPI::GUI::CImageBox*> imageBoxVector = m_mImage[col];
	std::vector<SallyAPI::GUI::CLabelBox*> imageNameVector = m_mImageName[col];

	int fileID = (m_iStartPicture * m_iRows) + (col * m_iRows) + row - m_iRows;
	CAlbum* album = m_vAlbumList[fileID];

	CMediaDatabase::GetAlbumTitelsFromDatabase(dynamic_cast<SallyAPI::GUI::CAppBase*> (m_pParent), m_pAlbumTitles, album->GetAlbum(), album->GetArtist());

	AddAllToPlaylistFromListView(m_pAlbumTitles);
}

void CAddMusicAlbum::OnCommandBackClicked()
{
	m_pZoomBackground->SetAlphaBlending(255);
	m_pZoomBackground->BlendAnimated(0, 800);

	m_pAlbumBackground->SetAlphaBlending(255);
	m_pAlbumBackground->BlendAnimated(0, 800);
}