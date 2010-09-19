////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\AppMediaPlayer.cpp
///
/// \brief	Implements the application media player class. 
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

#include "AppMediaPlayer.h"

using namespace SallyAPI::GUI;

CAppMediaPlayer::CAppMediaPlayer(SallyAPI::GUI::CGUIBaseObject *parent, int graphicId, const std::string& pluginPath)
:SallyAPI::GUI::CApplicationWindow(parent, graphicId, pluginPath)
{
	// check for db update
	int dbVersion = GetPropertyInt("databaseVersion", 1);
	if (dbVersion != 2)
	{
		std::string dbFile = SallyAPI::System::SallyHelper::GetMediaDirectory(this);
		dbFile.append("media.db");
		DeleteFile(dbFile.c_str());
	}

	// Specific Infos
	m_pCurrentFile = NULL;
	m_eScreensaver = SCREENSAVER_STATE_OFF;

	m_lVolumeMax = 0;
	m_iGoNextTries = 0;
	m_fDeltaStartPlay = 0;
	m_iPopUpId = 0;

	m_iAlbumLoadDone = 0;

	// Load Pictures
	LoadApplicationImage("cdcase.png", GUI_APP_DEFAULT_CD);

	m_pInfoPopUp = new CInfoPopUp(this, GetGraphicId(), GetExplicitAppName());
	m_pParent->SendMessageToParent(m_pInfoPopUp, 0, MS_SALLY_ADD_CHILD, 0);

	// AlbumCover
	m_pAlbumCover = NULL;
	m_iCurrentNumber = -1;

	m_pScreensaverAlbumImageContainerBackground = new SallyAPI::GUI::CImageBox(this, -5, -5, WINDOW_WIDTH + 10, WINDOW_WIDTH + 10);
	m_pScreensaverAlbumImageContainerBackground->SetAlphaBlending(0);
	this->AddChild(m_pScreensaverAlbumImageContainerBackground);

	// Create GUI
	m_pSideMenuCurrentPlay = new SallyAPI::GUI::CForm(this, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	this->AddChild(m_pSideMenuCurrentPlay);

	m_pDefaultForm = new SallyAPI::GUI::CForm(m_pSideMenuCurrentPlay, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	m_pSideMenuCurrentPlay->AddChild(m_pDefaultForm);

	// Die Breite
	int iWidth = (int) (WINDOW_WIDTH / 2.5);
	if (iWidth < 300)
		iWidth = 300;

	if (iWidth > 800)
		iWidth = 800;

	if (WINDOW_WIDTH == 800)
	{
		iWidth = 300;
	}

	m_pCoverForm = new SallyAPI::GUI::CForm(m_pDefaultForm,
		WINDOW_BORDER_H, WINDOW_BORDER_V, iWidth, WINDOW_HEIGHT - WINDOW_BORDER_V - WINDOW_BORDER_V);
	m_pDefaultForm->AddChild(m_pCoverForm);

	/************************************************************************/
	/* Playlist                                                             */
	/************************************************************************/
	std::vector<int>	imageList;
	imageList.push_back(GUI_THEME_SALLY_ICON_MIMETYPE_MP3);
	imageList.push_back(GUI_THEME_SALLY_ICON_MIMETYPE_VIDEO);

	m_pListViewPlaylist = new SallyAPI::GUI::CListView(m_pDefaultForm,
		WINDOW_BORDER_H + m_pCoverForm->GetWidth() + WINDOW_BORDER_H,
		WINDOW_BORDER_V,
		WINDOW_WIDTH - (WINDOW_BORDER_H + m_pCoverForm->GetWidth() + WINDOW_BORDER_H + WINDOW_BORDER_H + MENU_WIDTH),
		WINDOW_HEIGHT - (WINDOW_BORDER_V * 2),
		1, GUI_THEME_SALLY_ICON_REMOVE, imageList, GUI_APP_PLAYLIST);
	m_pListViewPlaylist->SetLocalised(false);
	m_pDefaultForm->AddChild(m_pListViewPlaylist);

	m_pPlaylist = new CPlaylist(m_pListViewPlaylist, m_strExplicitAppName);

	// Panels
	m_pConfigPanel = new CMyConfigPanel(this, GetGraphicId(), m_strExplicitAppName);
	SallyAPI::GUI::SendMessage::CParameterControl parameterConfig(m_pConfigPanel);
	m_pParent->SendMessageToParent(this, GetGraphicId(), MS_SALLY_ADD_CONFIG_PANEL, &parameterConfig);

	m_pWizardPanel = new CMyWizardPanel(this, GetGraphicId(), m_strExplicitAppName, (CMyConfigPanel*) m_pConfigPanel, this);
	SallyAPI::GUI::SendMessage::CParameterControl parameter(m_pWizardPanel);
	m_pParent->SendMessageToParent(this, GetGraphicId(), MS_SALLY_ADD_WIZARD_PANEL, &parameter);

	/************************************************************************/
	/* Info Popup                                                           */
	/************************************************************************/
	m_pAddMusicSearch = new CAddMusicSearch(this, GetGraphicId(), m_pPlaylist);
	m_pAddMusicSearch->Visible(false);
	m_pAddMusicSearch->Enable(false);
	this->AddChild(m_pAddMusicSearch);

	m_pAddMusicAlbum = new CAddMusicAlbum(this, GetGraphicId(), m_pPlaylist);
	m_pAddMusicAlbum->Visible(false);
	m_pAddMusicAlbum->Enable(false);
	this->AddChild(m_pAddMusicAlbum);

	m_pAddMusicExplorer = new CAddMusicExplorer(this, GetGraphicId(), m_pPlaylist);
	m_pAddMusicExplorer->Visible(false);
	m_pAddMusicExplorer->Enable(false);
	this->AddChild(m_pAddMusicExplorer);

	m_pAddMusicFavorites = new CAddMusicFavorites(this, GetGraphicId(), m_pPlaylist);
	m_pAddMusicFavorites->Visible(false);
	m_pAddMusicFavorites->Enable(false);
	this->AddChild(m_pAddMusicFavorites);

	m_pPlaylistLoad = new CPlaylistLoad(this, GetGraphicId(), m_pPlaylist);
	m_pPlaylistLoad->Visible(false);
	m_pPlaylistLoad->Enable(false);
	this->AddChild(m_pPlaylistLoad);

	m_pPlaylistDelete = new CPlaylistDelete(this, GetGraphicId(), m_pPlaylist);
	m_pPlaylistDelete->Visible(false);
	m_pPlaylistDelete->Enable(false);
	this->AddChild(m_pPlaylistDelete);

	/************************************************************************/
	/* ScreensaverForm                                                      */
	/************************************************************************/
	m_pScreensaverForm = new SallyAPI::GUI::CForm(m_pSideMenuCurrentPlay, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	m_pScreensaverForm->SetHandleInputIfItIsChildHandled(false);
	m_pScreensaverForm->SetScrollType(SallyAPI::GUI::SCROLL_TYPE_NORMAL);
	m_pScreensaverForm->Visible(false);
	m_pScreensaverForm->SetAlphaBlending(0);
	//m_pSideMenuCurrentPlay->AddChild(m_pScreensaverForm);
	// add this later!!!

	/************************************************************************/
	/* Album Container                                                      */
	/************************************************************************/
	m_iCoverSize = m_pCoverForm->GetHeight() - 320;
	if (m_iCoverSize > 500)
		m_iCoverSize = 500;
	if (m_iCoverSize < 300)
		m_iCoverSize = 300;

	m_pAlbumImageContainer = new SallyAPI::GUI::CImageBox(m_pSideMenuCurrentPlay, -m_iCoverSize, SMALL_PICTURE_Y, m_iCoverSize, m_iCoverSize);
	m_pAlbumImageContainer->SetImageId(GUI_APP_DEFAULT_CD + GetGraphicId());
	//m_pSideMenuCurrentPlay->AddChild(m_pAlbumImageContainer);
	// add this later!!!

	m_pVideoImageContainer = new SallyAPI::GUI::CImageBox(m_pSideMenuCurrentPlay, SMALL_VIDEO_X, SMALL_VIDEO_Y, SMALL_VIDEO_WIDTH, SMALL_VIDEO_HEIGHT);
	m_pVideoImageContainer->Visible(false);
	m_pVideoImageContainer->SetDiyplayType(SallyAPI::GUI::IMAGEBOX_DISPLAY_TYPE_SCALE);
	//m_pSideMenuCurrentPlay->AddChild(m_pVideoImageContainer);
	// add this later!!!

	int playLoadingX = SMALL_PICTURE_X;
	int playLoadingY = SMALL_PICTURE_Y;
	playLoadingX += (m_iCoverSize - 128) / 2;
	playLoadingY += (m_iCoverSize - 158) / 2;

	m_pSideMenuCurrentPlay->AddChild(m_pVideoImageContainer);
	m_pSideMenuCurrentPlay->AddChild(m_pScreensaverForm);
	m_pSideMenuCurrentPlay->AddChild(m_pAlbumImageContainer);

	/************************************************************************/
	/* Play Buttons                                                         */
	/************************************************************************/
	m_pButtonPrevious = new SallyAPI::GUI::CRoundButton(m_pCoverForm,
		((m_pCoverForm->GetWidth() - 300) / 2) + 10,
		m_pCoverForm->GetHeight() - 95,
		GUI_APP_PREVIOUS, SallyAPI::GUI::ROUNDBUTTON_TYPE_SMALL);
	m_pButtonPrevious->SetImageId(GUI_THEME_SALLY_ICON_MEDIA_SKIP_BACKWARD);
	m_pCoverForm->AddChild(m_pButtonPrevious);

	m_pButtonNext = new SallyAPI::GUI::CRoundButton(m_pCoverForm,
		((m_pCoverForm->GetWidth() - 300) / 2) + 235,
		m_pCoverForm->GetHeight() - 95,
		GUI_APP_NEXT, SallyAPI::GUI::ROUNDBUTTON_TYPE_SMALL);
	m_pButtonNext->SetImageId(GUI_THEME_SALLY_ICON_MEDIA_SKIP_FORWARD);
	m_pCoverForm->AddChild(m_pButtonNext);

	m_pButtonPlay = new SallyAPI::GUI::CRoundButton(m_pCoverForm,
		((m_pCoverForm->GetWidth() - 300) / 2) + 75,
		m_pCoverForm->GetHeight() - 110,
		GUI_APP_PLAY, SallyAPI::GUI::ROUNDBUTTON_TYPE_BIG);
	m_pButtonPlay->SetImageId(GUI_THEME_SALLY_ICON_MEDIA_PLAY);
	m_pCoverForm->AddChild(m_pButtonPlay);

	m_pButtonStop = new SallyAPI::GUI::CRoundButton(m_pCoverForm,
		((m_pCoverForm->GetWidth() - 300) / 2) + 155,
		m_pCoverForm->GetHeight() - 110,
		GUI_APP_STOP, SallyAPI::GUI::ROUNDBUTTON_TYPE_BIG);
	m_pButtonStop->SetImageId(GUI_THEME_SALLY_ICON_MEDIA_STOP);
	m_pCoverForm->AddChild(m_pButtonStop);

	m_pPicture = new SallyAPI::GUI::CForm(m_pCoverForm,
		SMALL_VIDEO_X, SMALL_VIDEO_Y, SMALL_VIDEO_WIDTH, SMALL_VIDEO_HEIGHT,
		GUI_APP_PICTURE_PRESSED);
	m_pPicture->Visible(false);
	m_pPicture->SetScrollType(SallyAPI::GUI::SCROLL_TYPE_NORMAL);
	m_pCoverForm->AddChild(m_pPicture);

	m_pSliderTime = new SallyAPI::GUI::CSlider(m_pCoverForm,
		0, m_pCoverForm->GetHeight() - CONTROL_HEIGHT, m_pCoverForm->GetWidth() - 105);
	m_pCoverForm->AddChild(m_pSliderTime);

	m_pTime = new SallyAPI::GUI::CLabel(m_pCoverForm, m_pCoverForm->GetWidth() - 100,
		m_pCoverForm->GetHeight() - CONTROL_HEIGHT, 100);
	m_pTime->SetLocalised(false);
	m_pTime->SetText("00:00 / 00:00");
	m_pTime->SetBold(true);
	m_pTime->SetAlign(DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
	m_pCoverForm->AddChild(m_pTime);

	int iInfoY = m_pCoverForm->GetHeight() - 180;

	if (iInfoY < SMALL_PICTURE_Y + m_iCoverSize - m_pCoverForm->GetPositionX() + 5)
		iInfoY = SMALL_PICTURE_Y + m_iCoverSize - m_pCoverForm->GetPositionX() + 5;


	m_pTrack = new SallyAPI::GUI::CLabel(m_pCoverForm,
		0, iInfoY + 20, m_pCoverForm->GetWidth());
	m_pTrack->SetLocalised(false);
	m_pTrack->SetBold(true);
	m_pTrack->Visible(false);
	m_pCoverForm->AddChild(m_pTrack);

	m_pAlbum = new SallyAPI::GUI::CLabel(m_pCoverForm,
		0, iInfoY, m_pCoverForm->GetWidth());
	m_pAlbum->SetLocalised(false);
	m_pAlbum->SetBold(true);
	m_pAlbum->Visible(false);
	m_pCoverForm->AddChild(m_pAlbum);

	m_pRatingDescription = new SallyAPI::GUI::CLabel(m_pCoverForm,
		0, CONTROL_HEIGHT + 10, m_pCoverForm->GetWidth() - 130);
	m_pRatingDescription->SetText("Rating:");
	m_pRatingDescription->SetBold(true);
	m_pRatingDescription->Visible(false);
	m_pCoverForm->AddChild(m_pRatingDescription);

	// Misc
	m_pRating = new SallyAPI::GUI::CRating(m_pCoverForm, m_pCoverForm->GetWidth() - 150, CONTROL_HEIGHT + 10,
		GUI_APP_DEFAULT_RATING_CHANGED);
	m_pRating->Visible(false);
	m_pCoverForm->AddChild(m_pRating);

	/************************************************************************/
	/* Side Menu                                                            */
	/************************************************************************/
	m_pSideMenu = new SallyAPI::GUI::CSideMenu(this, 0);
	this->AddChild(m_pSideMenu);

	m_pSideMenuNowPlaying = new SallyAPI::GUI::CSideMenuButton(m_pSideMenu,
		SallyAPI::GUI::SIDE_MENUE_BUTTON_TYPE_NORMAL, GUI_APP_MENU_NOW_PLAYING);
	m_pSideMenuNowPlaying->SetImageId(GUI_THEME_SALLY_ICON_MEDIA_PLAY);
	m_pSideMenuNowPlaying->SetText("Now Playing");
	m_pSideMenuNowPlaying->SetActive(true);
	m_pSideMenu->AddChild(m_pSideMenuNowPlaying);

	/************************************************************************/
	m_pSideMenuSeperator1 = new SallyAPI::GUI::CSideMenuButton(m_pSideMenu,
		SallyAPI::GUI::SIDE_MENUE_BUTTON_TYPE_SEPERATOR);
	m_pSideMenu->AddChild(m_pSideMenuSeperator1);

	m_pSideMenuAddMusic = new SallyAPI::GUI::CSideMenuButton(m_pSideMenu, SallyAPI::GUI::SIDE_MENUE_BUTTON_TYPE_HEADING);
	m_pSideMenuAddMusic->SetText("Add Music");
	m_pSideMenu->AddChild(m_pSideMenuAddMusic);

	m_pSideMenuSearch = new SallyAPI::GUI::CSideMenuButton(m_pSideMenu, SallyAPI::GUI::SIDE_MENUE_BUTTON_TYPE_NORMAL,
		GUI_APP_MENU_ADDMUSIC_SEARCH);
	m_pSideMenuSearch->SetImageId(GUI_THEME_SALLY_ICON_SEARCH);
	m_pSideMenuSearch->SetText("Search");
	m_pSideMenu->AddChild(m_pSideMenuSearch);

	m_pSideMenuAlbum = new SallyAPI::GUI::CSideMenuButton(m_pSideMenu, SallyAPI::GUI::SIDE_MENUE_BUTTON_TYPE_NORMAL,
		GUI_APP_MENU_ADDMUSIC_ALBUM);
	m_pSideMenuAlbum->SetImageId(GUI_THEME_SALLY_ICON_CD);
	m_pSideMenuAlbum->SetText("Album Browser");
	m_pSideMenu->AddChild(m_pSideMenuAlbum);

	m_pSideMenuFavorites = new SallyAPI::GUI::CSideMenuButton(m_pSideMenu, SallyAPI::GUI::SIDE_MENUE_BUTTON_TYPE_NORMAL,
		GUI_APP_MENU_ADDMUSIC_FAVORITES);
	m_pSideMenuFavorites->SetImageId(GUI_THEME_SALLY_ICON_RATING_ON);
	m_pSideMenuFavorites->SetText("Favourites");
	m_pSideMenu->AddChild(m_pSideMenuFavorites);

	m_pSideMenuExplorer = new SallyAPI::GUI::CSideMenuButton(m_pSideMenu, SallyAPI::GUI::SIDE_MENUE_BUTTON_TYPE_NORMAL,
		GUI_APP_MENU_ADDMUSIC_EXPLORER);
	m_pSideMenuExplorer->SetImageId(GUI_THEME_SALLY_ICON_HD);
	m_pSideMenuExplorer->SetText("File Browser");
	m_pSideMenu->AddChild(m_pSideMenuExplorer);

	/************************************************************************/
	m_pSideMenuSeperator2 = new SallyAPI::GUI::CSideMenuButton(m_pSideMenu,
		SallyAPI::GUI::SIDE_MENUE_BUTTON_TYPE_SEPERATOR);
	m_pSideMenu->AddChild(m_pSideMenuSeperator2);

	m_pSideMenuPlaylist = new SallyAPI::GUI::CSideMenuButton(m_pSideMenu, SallyAPI::GUI::SIDE_MENUE_BUTTON_TYPE_HEADING);
	m_pSideMenuPlaylist->SetText("Playlist");
	m_pSideMenu->AddChild(m_pSideMenuPlaylist);

	m_pSideMenuSavePlaylist = new SallyAPI::GUI::CSideMenuButton(m_pSideMenu, SallyAPI::GUI::SIDE_MENUE_BUTTON_TYPE_NORMAL,
		GUI_APP_MENU_SAVE_PLAYLIST);
	m_pSideMenuSavePlaylist->SetImageId(GUI_THEME_SALLY_ICON_SAVE);
	m_pSideMenuSavePlaylist->SetText("Save Playlist");
	m_pSideMenu->AddChild(m_pSideMenuSavePlaylist);

	m_pSideMenuLoadPlaylist = new SallyAPI::GUI::CSideMenuButton(m_pSideMenu, SallyAPI::GUI::SIDE_MENUE_BUTTON_TYPE_NORMAL,
		GUI_APP_MENU_LOAD_PLAYLIST);
	m_pSideMenuLoadPlaylist->SetImageId(GUI_THEME_SALLY_ICON_FOLDER);
	m_pSideMenuLoadPlaylist->SetText("Load Playlist");
	m_pSideMenu->AddChild(m_pSideMenuLoadPlaylist);

	m_pSideMenuDeletePlaylist = new SallyAPI::GUI::CSideMenuButton(m_pSideMenu, SallyAPI::GUI::SIDE_MENUE_BUTTON_TYPE_NORMAL,
		GUI_APP_MENU_DELETE_PLAYLIST);
	m_pSideMenuDeletePlaylist->SetImageId(GUI_THEME_SALLY_ICON_DELETE);
	m_pSideMenuDeletePlaylist->SetText("Delete Playlist");
	m_pSideMenu->AddChild(m_pSideMenuDeletePlaylist);

	m_pSideMenuSeperator3 = new SallyAPI::GUI::CSideMenuButton(m_pSideMenu,
		SallyAPI::GUI::SIDE_MENUE_BUTTON_TYPE_SEPERATOR);
	m_pSideMenu->AddChild(m_pSideMenuSeperator3);

	m_pSideMenuOptions = new SallyAPI::GUI::CSideMenuButton(m_pSideMenu, SallyAPI::GUI::SIDE_MENUE_BUTTON_TYPE_HEADING);
	m_pSideMenuOptions->SetText("Options");
	m_pSideMenu->AddChild(m_pSideMenuOptions);

	// Fullscreen Button
	m_pStartFullscreen = new SallyAPI::GUI::CSideMenuButton(m_pSideMenu, SallyAPI::GUI::SIDE_MENUE_BUTTON_TYPE_NORMAL, GUI_APP_START_FULLSCREEN);
	m_pStartFullscreen->SetImageId(GUI_THEME_SALLY_ICON_FULLSCREEN);
	m_pStartFullscreen->SetText("Fullscreen");
	m_pStartFullscreen->Enable(false);
	m_pSideMenu->AddChild(m_pStartFullscreen);

	m_pMenuInfo = new SallyAPI::GUI::CSideMenuButton(m_pSideMenu, SallyAPI::GUI::SIDE_MENUE_BUTTON_TYPE_NORMAL, GUI_APP_SHOW_INFO);
	m_pMenuInfo->SetImageId(GUI_THEME_SALLY_ICON_INFO);
	m_pMenuInfo->SetText("Info");
	m_pMenuInfo->Enable(false);
	m_pSideMenu->AddChild(m_pMenuInfo);

	m_pDBUpdate = new SallyAPI::GUI::CWorking(m_pSideMenu, 0, 0, SallyAPI::GUI::WORKING_SMALL);
	m_pDBUpdate->Visible(false);
	m_pSideMenu->AddChild(m_pDBUpdate);

	m_pDBUpdate->Move(4, m_pSideMenu->GetHeight() - 26);
	m_pDBUpdate->Resize(22, 22);

	m_pDBUpdateLabel = new SallyAPI::GUI::CLabel(m_pSideMenu, 0, 0, 0);
	m_pDBUpdateLabel->SetText("Database Index running");
	m_pDBUpdateLabel->SetFont("sidebutton.font");
	m_pDBUpdateLabel->Visible(false);
	m_pDBUpdateLabel->SetAlign(DT_CENTER | DT_VCENTER);
	m_pSideMenu->AddChild(m_pDBUpdateLabel);

	m_pDBUpdateLabel->Move(30, m_pSideMenu->GetHeight() - 30);
	m_pDBUpdateLabel->Resize(162, CONTROL_HEIGHT);

	/************************************************************************/
	/* Menu                                                                 */
	/************************************************************************/
	m_pMenu = new SallyAPI::GUI::CButtonBar(m_pDefaultForm, WINDOW_BORDER_H, WINDOW_BORDER_V, 300);
	m_pDefaultForm->AddChild(m_pMenu);

	m_pMenuShuffle = new SallyAPI::GUI::CButtonBarButton(m_pMenu, 150, GUI_APP_MENU_SHUFFLE);
	m_pMenuShuffle->SetText("Shuffle");
	m_pMenuShuffle->SetImageId(GUI_THEME_SALLY_ICON_SHUFFLE);
	m_pMenu->AddChild(m_pMenuShuffle);

	m_pMenuClear = new SallyAPI::GUI::CButtonBarButton(m_pMenu, 150, GUI_APP_MENU_CLEAR);
	m_pMenuClear->SetImageId(GUI_THEME_SALLY_ICON_DELETE);
	m_pMenuClear->SetText("Clear List");
	m_pMenu->AddChild(m_pMenuClear);

	m_pVideoPicture = new SallyAPI::GUI::CPicture;
	m_pVideoPicture->SetTexture(new SallyAPI::Core::CTexture());

	/************************************************************************/
	/* Screensaver Form                                                     */
	/************************************************************************/
	// TopMenu
	m_pTopMenu = new SallyAPI::GUI::CGroupBox(m_pScreensaverForm, (WINDOW_WIDTH - 440) / 2, -90, 440, 90);
	m_pTopMenu->SetAlwaysHandleInput(true);
	m_pScreensaverForm->AddChild(m_pTopMenu);

	m_pVolumeControl = new SallyAPI::GUI::CVolumeControl(m_pTopMenu, 20, 40, 400);
	m_pTopMenu->AddChild(m_pVolumeControl);

	// BottomMenu
	m_pBottomMenu = new SallyAPI::GUI::CBottonMenu(m_pScreensaverForm);
	m_pBottomMenu->Move(0, WINDOW_HEIGHT);
	m_pBottomMenu->SetAlwaysHandleInput(true);
	m_pScreensaverForm->AddChild(m_pBottomMenu);

	m_pFullscreenSliderTime = new SallyAPI::GUI::CSlider(m_pBottomMenu,
		WINDOW_BORDER_H, (MENU_HEIGHT - CONTROL_HEIGHT) / 2 + 25, (WINDOW_WIDTH - 70) / 2 - 60 - WINDOW_BORDER_H - 110 - 30);
	m_pBottomMenu->AddChild(m_pFullscreenSliderTime);

	m_pFullscreenTime = new SallyAPI::GUI::CLabel(m_pBottomMenu, (WINDOW_WIDTH - 70) / 2 - 60 - 110 - 20,
		(MENU_HEIGHT - CONTROL_HEIGHT) / 2 + 25, 110);
	m_pFullscreenTime->SetLocalised(false);
	m_pFullscreenTime->SetText("00:00 / 00:00");
	m_pFullscreenTime->SetBold(true);
	m_pFullscreenTime->SetDrawBackground(true);
	m_pFullscreenTime->SetAlign(DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	m_pBottomMenu->AddChild(m_pFullscreenTime);

	m_pMenuBar = new SallyAPI::GUI::CButtonBar(m_pBottomMenu,
		WINDOW_WIDTH - 260 - WINDOW_BORDER_H, (MENU_HEIGHT - CONTROL_HEIGHT) / 2 + 25, 260);
	m_pBottomMenu->AddChild(m_pMenuBar);

	m_pShuffle = new SallyAPI::GUI::CButtonBarButton(m_pMenuBar, 100, GUI_APP_MENU_SHUFFLE_BOTTOM);
	m_pShuffle->SetImageId(GUI_THEME_SALLY_ICON_SHUFFLE);
	m_pShuffle->SetText("Shuffle");
	m_pMenuBar->AddChild(m_pShuffle);

	m_pExitFullscreen = new SallyAPI::GUI::CButtonBarButton(m_pMenuBar, 160, GUI_APP_STOP_FULLSCREEN);
	m_pExitFullscreen->SetImageId(GUI_THEME_SALLY_ICON_FULLSCREEN);
	m_pExitFullscreen->SetText("exit Fullscreen");
	m_pMenuBar->AddChild(m_pExitFullscreen);

	m_pScreensaverButtonPlay = new SallyAPI::GUI::CRoundButton(m_pBottomMenu, (WINDOW_WIDTH - 70) / 2, 0, GUI_APP_SCREENSAVER_PLAY, 
		SallyAPI::GUI::ROUNDBUTTON_TYPE_BIG);
	m_pScreensaverButtonPlay->SetImageId(GUI_THEME_SALLY_ICON_MEDIA_PLAY);
	m_pBottomMenu->AddChild(m_pScreensaverButtonPlay);

	m_pScreensaverButtonPrevious = new SallyAPI::GUI::CRoundButton(m_pBottomMenu, (WINDOW_WIDTH - 70) / 2 - 60, 15, GUI_APP_SCREENSAVER_PREVIOUS, 
		SallyAPI::GUI::ROUNDBUTTON_TYPE_SMALL);
	m_pScreensaverButtonPrevious->SetImageId(GUI_THEME_SALLY_ICON_MEDIA_SKIP_BACKWARD);
	m_pBottomMenu->AddChild(m_pScreensaverButtonPrevious);

	m_pScreensaverButtonNext = new SallyAPI::GUI::CRoundButton(m_pBottomMenu, (WINDOW_WIDTH + 70) / 2 + 5, 15, GUI_APP_SCREENSAVER_NEXT,
		SallyAPI::GUI::ROUNDBUTTON_TYPE_SMALL);
	m_pScreensaverButtonNext->SetImageId(GUI_THEME_SALLY_ICON_MEDIA_SKIP_FORWARD);
	m_pBottomMenu->AddChild(m_pScreensaverButtonNext);

	// pressed Notifier
	m_pTimerHideMenu = new SallyAPI::GUI::CTimer(10, m_pScreensaverForm, 0, GUI_FORM_CLICKED);

	m_pScreensaverMp3Form = new SallyAPI::GUI::CForm(m_pScreensaverForm, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	m_pScreensaverMp3Form->Visible(false);
	m_pScreensaverForm->AddChild(m_pScreensaverMp3Form);

	m_pScreensaverRating = new SallyAPI::GUI::CRating(m_pScreensaverMp3Form, WINDOW_WIDTH / 2,
		(WINDOW_HEIGHT / 2) - 125,  GUI_APP_SCREENSAVER_RATING_CHANGED);
	m_pScreensaverMp3Form->AddChild(m_pScreensaverRating);

	int screensaverWith = (WINDOW_WIDTH / 2) - 50;
	if (screensaverWith > 700)
		screensaverWith = 700;

	SallyAPI::GUI::CForm* m_pScreensaverLabelForm = new SallyAPI::GUI::CForm(m_pScreensaverMp3Form, WINDOW_WIDTH / 2, (WINDOW_HEIGHT / 2) - 125 + 40, screensaverWith, 210);
	m_pScreensaverLabelForm->Enable(false);
	m_pScreensaverMp3Form->AddChild(m_pScreensaverLabelForm);

	for (int i = 0; i < 3; ++i)
	{
		m_pScreensaverStatusLabel[i] = new SallyAPI::GUI::CLabel(m_pScreensaverLabelForm, 0, i * 70, screensaverWith);
		m_pScreensaverStatusLabel[i]->SetLocalised(false);
		m_pScreensaverStatusLabel[i]->SetDrawBackground(true);
		m_pScreensaverStatusLabel[i]->SetBig(true);
		m_pScreensaverLabelForm->AddChild(m_pScreensaverStatusLabel[i]);
	}
	m_pScreensaverStatusLabel[0]->SetBold(true);

	m_tAudioHelper.SetStaticValues(m_pTrack, m_pAlbum, m_pPlaylist, this, &m_mCoverLoaders);
	m_tVideoHelper.SetStaticValues(this);

	m_pThreadPlay = new SallyAPI::GUI::CThreadStarter(this, 0, GUI_APP_THREAD_ON_COMMAND_PLAY);
	m_pMediaPlayer = new CMediaPlayer(m_pVideoPicture, this);

	// load the old playlist
	m_pPlaylist->DisableResolver(false);
	m_pPlaylist->LoadPlaylist(CPlaylistHelper::GetDefaultPlaylist(this));
	m_pPlaylist->DisableResolver(true);
	m_pPlaylist->StartResolver();

	m_pSnapBackTimer = new SallyAPI::GUI::CTimer(60, this, 0, GUI_APP_PLAY_SNAP_BACK);
	m_pSnapBackTimer->Start();

	if (GetPropertyBool("autoplayOnStartup", false))
	{
		OnCommandNext();
	}

	if (GetPropertyBool("shuffle"))
	{
		m_pMenuShuffle->SetCheckStatus(true);
		m_pShuffle->SetCheckStatus(true);
	}
}

CAppMediaPlayer::~CAppMediaPlayer()
{
	m_tUpdateRating.WaitForStop();
	m_tAudioHelper.WaitForStop();
	m_tVideoHelper.WaitForStop();
	m_pSnapBackTimer->WaitForStop();
	m_pPlaylist->Stop();

	while (m_mCoverLoaders.size() > 0)
	{
		Sleep(1);
	}

	// is also calling CleanUp
	OnCommandStop();

	SafeDelete(m_pAlbumCover);
	SafeDelete(m_pCurrentFile);
	SafeDelete(m_pVideoPicture);
	SafeDelete(m_pSnapBackTimer);
	SafeDelete(m_pPlaylist);
}

void CAppMediaPlayer::SaveConfig()
{
	m_pPlaylist->SavePlaylist(CPlaylistHelper::GetDefaultPlaylist(this), true);
}

void CAppMediaPlayer::LoadConfig()
{
	m_pAddMusicExplorer->LoadConfig();
	m_pAddMusicAlbum->LoadConfig();
}

void CAppMediaPlayer::CleanUpMedia()
{
	m_tAudioHelper.Stop();
	m_tVideoHelper.Stop();

	// delete PopUp
	if (m_iPopUpId != 0)
	{
		SallyAPI::GUI::SendMessage::CParameterInteger sendMessageParameterInfoPopup(m_iPopUpId);
		m_pParent->SendMessageToParent(this, m_iControlId, MS_SALLY_DELETE_INFO_POPUP, &sendMessageParameterInfoPopup);

		m_iPopUpId = 0;
	}

	EnterRenderLock();

	m_pVideoImageContainer->SetPicture(NULL);
	m_pVideoImageContainer->Visible(false);

	m_pMediaPlayer->Stop();

	LeaveRenderLock();

	m_tAudioHelper.WaitForStop();
	m_tVideoHelper.WaitForStop();
	SafeDelete(m_pCurrentFile);
}

void CAppMediaPlayer::Timer(float fDelta)
{
	SallyAPI::GUI::CApplicationWindow::Timer(fDelta);

	EnterRenderLock();

	if ((m_pCurrentFile != NULL) && (m_pMediaPlayer->GetState() != State_Stopped))
	{
		REFTIME refDuration = m_pMediaPlayer->GetDuration();
		REFTIME refCurrentPosition = m_pMediaPlayer->GetCurrentPosition();

		std::string playTime;

		playTime.append(CalculateTime(refCurrentPosition));
		playTime.append(" / ");
		playTime.append(CalculateTime(refDuration));

		m_pTime->SetText(playTime);
		m_pSliderTime->SetPosition(((int) refCurrentPosition));
		m_pFullscreenTime->SetText(playTime);
		m_pFullscreenSliderTime->SetPosition(((int) refCurrentPosition));

		// fade in
		if (GetPropertyBool("musicBlendInOut", false) && (refCurrentPosition < 2))
		{
			long newVolume = (refCurrentPosition * 2500) - 5000;
			if (newVolume > m_lVolumeMax)
			{
				newVolume = m_lVolumeMax;
			}

			m_pMediaPlayer->SetVolume(newVolume);
		}
		else
		{
			m_pMediaPlayer->SetVolume(m_lVolumeMax);
		}

		// fade out
		if (GetPropertyBool("musicBlendInOut", false) && (refCurrentPosition + 2 >= refDuration))
		{
			long newVolume = ((refDuration - refCurrentPosition) * 2500) + 5000;
			if (newVolume < -10000)
			{
				newVolume = -10000;
			}
			if (newVolume > m_lVolumeMax)
			{
				newVolume = m_lVolumeMax;
			}

			m_pMediaPlayer->SetVolume(newVolume);
		}
		else
		{
			m_pMediaPlayer->SetVolume(m_lVolumeMax);
		}


		if (refCurrentPosition + 0.001 >= refDuration)
			OnCommandNext();
	}

	LeaveRenderLock();

	// cover load is done and we can now blend in the cover
	if (m_iAlbumLoadDone == 111)
	{
		// wait till the last animation (screensaver on or off) is done
		if ((m_eScreensaver == SCREENSAVER_STATE_OFF) && (m_pAlbumImageContainer->GetPositionX() == -m_iCoverSize)
			&& (m_pMediaPlayer->GetState() == State_Running))
		{
			m_pAlbumImageContainer->MoveAnimated(SMALL_PICTURE_X, SMALL_PICTURE_Y, 2000);
			m_iAlbumLoadDone = 0;
		}
		else if ((m_eScreensaver == SCREENSAVER_STATE_ON) && (m_pAlbumImageContainer->GetPositionX() == -m_iCoverSize)
			&& (m_pMediaPlayer->GetState() == State_Running))
		{
			m_pAlbumImageContainer->MoveAnimated(BIG_PICTURE_X, BIG_PICTURE_Y, 2000);
			m_iAlbumLoadDone = 0;
			m_pScreensaverAlbumImageContainerBackground->BlendAnimated(255, 800);
		}
	}
}


/************************************************************************/
/* Helper                                                               */
/************************************************************************/
std::string CAppMediaPlayer::CalculateTime(REFTIME in)
{
	char		iTemp[12];
	int			iMinute;
	int			iSecond;
	std::string playTime;

	// calculate Minute
	iMinute = ((int) in / 60);
	if (iMinute < 10)
		playTime.append("0");
	_itoa_s(iMinute, iTemp, 10, 10);
	playTime.append(iTemp);

	playTime.append(":");

	// calculate Seconds
	iSecond = ((int) in % 60);
	if (iSecond < 10)
		playTime.append("0");
	_itoa_s(iSecond, iTemp, 10, 10);
	playTime.append(iTemp);

	return playTime;
}

/************************************************************************/
/* On Command                                                           */
/************************************************************************/
void CAppMediaPlayer::OnCommandSavePlaylistDialog(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	SallyAPI::GUI::SendMessage::CParameterString* parameterString = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterString*> (messageParameter);

	m_strPlaylistName = SallyAPI::System::SallyHelper::GetMediaDirectory(m_strExplicitAppName);
	m_strPlaylistName.append("Playlist\\");

	m_strPlaylistName.append(parameterString->GetString());
	m_strPlaylistName.append(".m3u");

	if (SallyAPI::File::FileHelper::FileExists(m_strPlaylistName))
	{
		// File exists - ask if we should overwrite it
		SallyAPI::GUI::SendMessage::CParameterQuestionBox questionBox(GUI_APP_SAVE_PLAYLIST, this, "File already exists.\nDo you want to overwrite it?");
		m_pParent->SendMessageToParent(this, m_iControlId, MS_SALLY_SHOW_QUESTIONBOX, &questionBox);
	}
	else
	{
		// File does not exist - save now
		this->SendMessageToParent(this, GUI_APP_SAVE_PLAYLIST, MS_DIALOG_YES);
	}
}

void CAppMediaPlayer::OnCommandSavePlaylistDialog()
{
	m_pPlaylist->SavePlaylist(m_strPlaylistName, false);

	m_pPlaylistDelete->Reload();
	m_pPlaylistLoad->Reload();
}

void CAppMediaPlayer::OnCommandMenuSavePlaylist()
{
	SallyAPI::GUI::SendMessage::CParameterInputBox inputBox(GUI_APP_SAVE_PLAYLIST, this, "Please enter the name for the playlist:", "");

	m_pParent->SendMessageToParent(this, m_iControlId, MS_SALLY_SHOW_INPUTBOX, &inputBox);
}

void CAppMediaPlayer::OnCommandProcessbarMoved(SallyAPI::GUI::CGUIBaseObject* reporter, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	if (m_pCurrentFile == NULL)
		return;
	if (m_pMediaPlayer == NULL)
		return;

	SallyAPI::GUI::SendMessage::CParameterInteger* parameterInteger = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterInteger*> (messageParameter);
	if (parameterInteger == NULL)
		return;

	m_pMediaPlayer->SetCurrentPosition(parameterInteger->GetInteger());

	// update the other slider (fullscreen / normal)
	if (reporter == m_pFullscreenSliderTime)
		m_pSliderTime->SetPosition(parameterInteger->GetInteger());
	else
		m_pFullscreenSliderTime->SetPosition(parameterInteger->GetInteger());
}

void CAppMediaPlayer::OnCommandPlay(bool startAsThread)
{
	if ((m_pThreadPlay->GetStatus() == SallyAPI::System::THREAD_RUNNING) && (startAsThread))
		return;

	// Are we in the pause mode?
	if (m_pMediaPlayer->GetState() == State_Paused)
	{
		m_pButtonPlay->SetImageId(GUI_THEME_SALLY_ICON_MEDIA_PAUSE);
		m_pScreensaverButtonPlay->SetImageId(GUI_THEME_SALLY_ICON_MEDIA_PAUSE);
		m_pMediaPlayer->Play();
		return;
	}

	// Do we have any files in the playlist
	if (m_pPlaylist->GetListSize() == 0)
	{
		OnCommandStop();
		return;
	}

	if (startAsThread == true)
		m_pThreadPlay->Start();
	else
		OnCommandThreadPlay();
}

void CAppMediaPlayer::OnCommandThreadPlay()
{
	CleanUpMedia();

	SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();

	// check if we have one file to play or if the user has only hit the playbutton
	if (m_iCurrentNumber < 0 || m_iCurrentNumber >= m_pPlaylist->GetListSize())
		m_iCurrentNumber = 0;

	// Get the clicked listview item
	SallyAPI::GUI::CListViewItem* listItem = m_pPlaylist->GetOrginalItem(m_iCurrentNumber);

	// remove from smart shuffle
	RemoveAsPlayedFromSmartShuffle(m_iCurrentNumber);

	logger->Debug(listItem->GetIdentifier());

	if (listItem->GetImageIndex() == 0)
		m_pCurrentFile = new CAudioFile(listItem->GetIdentifier());
	else
		m_pCurrentFile = new CVideoFile(listItem->GetIdentifier());

	// activate the listview
	m_pPlaylist->SetActive(m_iCurrentNumber);

	if (!m_pMediaPlayer->RenderFile(m_pCurrentFile))
		return;

	/************************************************************************/
	/* Set GUI                                                              */
	/************************************************************************/
	// Default controls for the video and the audio
	m_pPicture->Visible(true);
	m_pTrack->Visible(true);
	m_pStartFullscreen->Enable(true);
	m_pMenuInfo->Enable(true);

	m_pButtonPlay->SetImageId(GUI_THEME_SALLY_ICON_MEDIA_PAUSE);
	m_pScreensaverButtonPlay->SetImageId(GUI_THEME_SALLY_ICON_MEDIA_PAUSE);

	// Set Process Bar
	REFTIME refDuration = m_pMediaPlayer->GetDuration();
	m_pSliderTime->SetMaxPosition((int) refDuration);
	m_pFullscreenSliderTime->SetMaxPosition((int) refDuration);

	if (m_pCurrentFile->GetType() == MEDIAFILE_VIDEO)
	{
		m_pScreensaverMp3Form->Visible(false);

		std::string strVideoName = SallyAPI::String::PathHelper::GetFileFromPath(m_pCurrentFile->GetFilename());
		m_pTrack->SetText(strVideoName);

		m_pAlbum->Visible(false);
		m_pAlbumImageContainer->Visible(false);

		CVideoFile* video = (CVideoFile*) m_pCurrentFile;
		m_tVideoHelper.SetValues(video);
		m_tVideoHelper.Start();

		m_pAlbumImageContainer->Move(-m_iCoverSize, m_pAlbumImageContainer->GetPositionY());
	}
	else if (m_pCurrentFile->GetType() == MEDIAFILE_AUDIO)
	{
		m_pScreensaverMp3Form->Visible(true);

		// Start the Mp3Helper
		CAudioFile* mp3 = (CAudioFile*) m_pCurrentFile;
		m_iAlbumLoadDone = 0;
		m_tAudioHelper.SetValues(mp3, listItem);
		m_tAudioHelper.Start();

		m_pAlbum->Visible(true);
		m_pAlbumImageContainer->Visible(true);
	}

	/************************************************************************/
	/* check if the file was found                                          */
	/************************************************************************/
	if (refDuration == 0.0)
	{
		ShowErrorMessage("The file '%s' was not found.");
	}
	m_iGoNextTries = 0;

	/************************************************************************/
	/* Start now!!!                                                         */
	/************************************************************************/
	m_pMediaPlayer->SetVolume(-10000);
	m_pMediaPlayer->Play();

	m_pAlbumImageContainer->MoveAnimated(-m_iCoverSize, m_pAlbumImageContainer->GetPositionY(), 2000);

	// if we play a video than set the height
	if (m_pCurrentFile->GetType() == MEDIAFILE_VIDEO)
	{
		m_pVideoPicture->SetWidth(m_pMediaPlayer->GetVideoWidth());
		m_pVideoPicture->SetHeight(m_pMediaPlayer->GetVideoHeight());

		m_pVideoImageContainer->SetPicture(m_pVideoPicture);
		m_pVideoImageContainer->Visible(true);

		m_pScreensaverAlbumImageContainerBackground->SetImageId(GUI_THEME_SALLY_BLACK_BACKGROUND);
		m_pAlbumImageContainer->SetImageId(GUI_NO_IMAGE);

		if (m_eScreensaver == SCREENSAVER_STATE_ON)
		{
			m_pScreensaverAlbumImageContainerBackground->SetAlphaBlending(255);
		}
		else if (m_eScreensaver == SCREENSAVER_STATE_OFF)
		{
			// fix for video
			m_pVideoImageContainer->Resize(SMALL_VIDEO_WIDTH - 1, SMALL_VIDEO_HEIGHT - 1);
		}
	}
	else
	{
		m_pScreensaverAlbumImageContainerBackground->BlendAnimated(0, 800);
	}
}

void CAppMediaPlayer::ShowErrorMessage(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	SallyAPI::GUI::SendMessage::CParameterString* parameterString = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterString*> (messageParameter);
	if (parameterString == NULL)
		return;

	ShowErrorMessage(parameterString->GetString());
}

void CAppMediaPlayer::ShowErrorMessage(const std::string& showMessage)
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::Config::CLanguageManager* languageManager = config->GetLanguageLocalization();
	SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();

	std::string infoMessage = languageManager->GetString(showMessage, m_pCurrentFile->GetFilename().c_str(), NULL);

	// File Not Found
	SallyAPI::GUI::SendMessage::CParameterInfoPopup sendMessageParameterInfoPopup(GetGraphicId(), GetAppName(), infoMessage);
	m_pParent->SendMessageToParent(this, m_iControlId, MS_SALLY_SHOW_INFO_POPUP, &sendMessageParameterInfoPopup);

	if (m_iGoNextTries < 10)
	{
		std::string triesInfo = "MediaPlyer ShowErrorMessage: ";
		triesInfo.append(SallyAPI::String::StringHelper::ConvertToString(m_iGoNextTries + 1));
		triesInfo.append(" try to go on");
		logger->Debug(triesInfo);

		++m_iGoNextTries;
		OnCommandNext(false);
	}
	else
	{
		std::string triesInfo = "MediaPlyer ShowErrorMessage: ";
		triesInfo.append(" enough tries... we end");
		logger->Debug(triesInfo);

		OnCommandStop();
		m_iGoNextTries = 0;
	}
	return;
}

void CAppMediaPlayer::OnCommandStop()
{
	CleanUpMedia();

	m_pSliderTime->SetMaxPosition(0);
	m_pSliderTime->SetPosition(0);
	m_pFullscreenSliderTime->SetMaxPosition(0);
	m_pFullscreenSliderTime->SetPosition(0);

	m_pButtonPlay->SetImageId(GUI_THEME_SALLY_ICON_MEDIA_PLAY);
	m_pScreensaverButtonPlay->SetImageId(GUI_THEME_SALLY_ICON_MEDIA_PLAY);

	m_pTrack->Visible(false);
	m_pStartFullscreen->Enable(false);
	m_pMenuInfo->Enable(false);
	m_pAlbum->Visible(false);

	m_pPlaylist->SetActive(-1);

	m_pRating->Visible(false);
	m_pRatingDescription->Visible(false);

	m_pTime->SetText("00:00 / 00:00");
	m_pFullscreenTime->SetText("00:00 / 00:00");

	m_pAlbumImageContainer->MoveAnimated(-m_iCoverSize, m_pAlbumImageContainer->GetPositionY(), 2000);
}

void CAppMediaPlayer::OnCommandNext(bool startAsThread)
{
	if (m_pPlaylist->GetListSize() == 0)
	{
		OnCommandStop();
		return;
	}

	if (m_pMediaPlayer->GetState() == State_Paused)
		m_pMediaPlayer->Stop();

	// for the history if not already in
	if (std::find(m_vHistoryPlayList.begin(), m_vHistoryPlayList.end(), m_iCurrentNumber) == m_vHistoryPlayList.end())
		m_vHistoryPlayList.push_back(m_iCurrentNumber);
	if (m_vHistoryPlayList.size() > 10)
		m_vHistoryPlayList.erase(m_vHistoryPlayList.begin());


	// Do we have to reset the smart shuffle?
	if (m_vImageListSmartShuffle.size() == 0)
	{
		int listSize = m_pPlaylist->GetListSize();

		for (int i = 0; i < listSize; ++i)
		{
			m_vImageListSmartShuffle.push_back(i);
		}
	}

	if (GetPropertyBool("shuffle") && (m_pPlaylist->GetListSize() > 1))
	{
		int randTrack = SallyAPI::System::CNumberGenerator::GetNumber(0, m_vImageListSmartShuffle.size() - 1);

		m_iCurrentNumber = m_vImageListSmartShuffle.at(randTrack);
	}
	else
	{
		if (m_iCurrentNumber + 1 >= m_pPlaylist->GetListSize())
			m_iCurrentNumber = 0;
		else
			m_iCurrentNumber++;
	}
	m_pPlaylist->SetStartItem(m_iCurrentNumber);
	OnCommandPlay(startAsThread);
}

void CAppMediaPlayer::OnCommandPrevious()
{
	if (m_pPlaylist->GetListSize() == 0)
	{
		OnCommandStop();
		return;
	}

	if (m_pMediaPlayer->GetState() == State_Paused)
		m_pMediaPlayer->Stop();

	if (m_vHistoryPlayList.size() > 0)
	{
		int i = m_vHistoryPlayList.size() - 1;
		m_iCurrentNumber = m_vHistoryPlayList.at(i);
		m_vHistoryPlayList.erase(m_vHistoryPlayList.begin() + i);

		if (m_iCurrentNumber >= m_pPlaylist->GetListSize())
			m_iCurrentNumber = m_pPlaylist->GetListSize() - 1;
	}
	else
	{
		if (m_iCurrentNumber - 1 <= -1)
			m_iCurrentNumber = m_pPlaylist->GetListSize() - 1;
		else
			m_iCurrentNumber--;
	}
	m_pPlaylist->SetStartItem(m_iCurrentNumber);
	OnCommandPlay();
}

void CAppMediaPlayer::OnCommandGoToFile(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	SallyAPI::GUI::SendMessage::CParameterInteger* parameterInteger = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterInteger*> (messageParameter);
	if (parameterInteger == NULL)
		return;
	if (m_pMediaPlayer->GetState() == State_Paused)
		m_pMediaPlayer->Stop();

	// add to history if not already in
	if (std::find(m_vHistoryPlayList.begin(), m_vHistoryPlayList.end(), m_iCurrentNumber) == m_vHistoryPlayList.end())
		m_vHistoryPlayList.push_back(m_iCurrentNumber);
	if (m_vHistoryPlayList.size() > 10)
		m_vHistoryPlayList.erase(m_vHistoryPlayList.begin());

	m_iCurrentNumber = parameterInteger->GetInteger();
	OnCommandPlay();
}

void CAppMediaPlayer::OnCommandRemoveFile(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	SallyAPI::GUI::SendMessage::CParameterInteger* parameterInteger = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterInteger*> (messageParameter);

	if (parameterInteger == NULL)
		return;

	m_pPlaylist->RemoveItem(parameterInteger->GetInteger() + 1);

	if (parameterInteger->GetInteger() < m_iCurrentNumber)
		m_iCurrentNumber--;
	else if (parameterInteger->GetInteger() == m_iCurrentNumber)
		m_iCurrentNumber = -1;

	// correct smart shuffle
	RemoveFromSmartShuffle(parameterInteger->GetInteger());

	// correct history
	CorrectHistory(parameterInteger->GetInteger());
}

void CAppMediaPlayer::CorrectHistory(int number)
{
	std::vector<int>::iterator iter = m_vHistoryPlayList.begin();
	std::vector<int>::iterator iterToDelete = m_vHistoryPlayList.end();
	while (iter != m_vHistoryPlayList.end())
	{
		int fileId = *iter;

		if (number == fileId)
		{
			m_vHistoryPlayList.erase(iter);

			// start from new... we maybe have more in the list
			iter = m_vHistoryPlayList.begin();
		}
		else if (number < fileId)
		{
			*iter = fileId - 1;
			++iter;
		}
		else
		{
			++iter;
		}
	}
}

void CAppMediaPlayer::RemoveFromSmartShuffle(int number)
{
	std::vector<int>::iterator iter = m_vImageListSmartShuffle.begin();
	std::vector<int>::iterator iterToDelete = m_vImageListSmartShuffle.end();
	while (iter != m_vImageListSmartShuffle.end())
	{
		int fileId = *iter;

		if (number == fileId)
		{
			iterToDelete = iter;
		}
		if (number < fileId)
		{
			*iter = fileId - 1;
		}
		++iter;
	}
	if (iterToDelete != m_vImageListSmartShuffle.end())
		m_vImageListSmartShuffle.erase(iterToDelete);
}

void CAppMediaPlayer::RemoveAsPlayedFromSmartShuffle(int number)
{
	std::vector<int>::iterator iter = m_vImageListSmartShuffle.begin();
	std::vector<int>::iterator iterToDelete = m_vImageListSmartShuffle.end();
	while ((iter != m_vImageListSmartShuffle.end()) && (iterToDelete == m_vImageListSmartShuffle.end()))
	{
		int fileId = *iter;

		if (number == fileId)
		{
			iterToDelete = iter;
		}
		++iter;
	}
	if (iterToDelete != m_vImageListSmartShuffle.end())
		m_vImageListSmartShuffle.erase(iterToDelete);
}

void CAppMediaPlayer::OnCommandPause()
{
	m_pMediaPlayer->Pause();

	m_pButtonPlay->SetImageId(GUI_THEME_SALLY_ICON_MEDIA_PLAY);
	m_pScreensaverButtonPlay->SetImageId(GUI_THEME_SALLY_ICON_MEDIA_PLAY);
}

void CAppMediaPlayer::OnCommandClearList()
{
	m_pPlaylist->Stop();

	// clears smart shuffle
	m_vImageListSmartShuffle.clear();
	m_iCurrentNumber = -1;

	m_pPlaylist->Clear();
	m_vHistoryPlayList.clear();
}

void CAppMediaPlayer::SendMessageToChilds(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case MS_SALLY_DEVICE_LOST:
		return;
	case MS_SALLY_DEVICE_RESTORE_START:
		OnCommandDeviceRestoreStart();
		return;
	case MS_SALLY_DEVICE_RESTORE_END:
		OnCommandDeviceRestoreEnd();
		return;
	}

	CApplicationWindow::SendMessageToChilds(reporter, reporterId, messageId, messageParameter);
}

void CAppMediaPlayer::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_APP_SHOW_ERROR_MESSAGE:
		ShowErrorMessage(messageParameter);
		return;
	case GUI_APP_PLAY_LAST_ADDED:
		OnCommandPlayLastFile();
		return;
	case GUI_APP_THREAD_ON_COMMAND_PLAY:
		OnCommandThreadPlay();
		return;
	case GUI_APP_MENU_DBUPDATE_STARTED:
		m_pDBUpdate->Visible(true);
		m_pDBUpdateLabel->Visible(true);
		return;
	case GUI_APP_MENU_DBUPDATE_FINISHED:
		m_pDBUpdate->Visible(false);
		m_pDBUpdateLabel->Visible(false);
		return;
	case MS_SALLY_COMMUNITY_NOTIFY:
		OnCommandCommunityNotify(messageParameter);
		return;
	case GUI_APP_PLAY_SNAP_BACK:
		SendMessageToParent(this, GUI_APP_MENU_NOW_PLAYING, GUI_BUTTON_CLICKED);
		m_pPlaylist->SavePlaylist(CPlaylistHelper::GetDefaultPlaylist(this), true);
		return;
	case GUI_CONTROL_BLENDED:
		if (((reporter == m_pDefaultForm) || (reporter == m_pSideMenu) || (reporter == m_pScreensaverForm))
			&& (reporter->GetAlphaBlending() == 255))
		{
			if (reporter == m_pDefaultForm)
				m_pDefaultForm->Enable(true);
			if (reporter == m_pSideMenu)
				m_pSideMenu->Enable(true);
			if (reporter == m_pScreensaverForm)
			{
				m_pScreensaverForm->Enable(true);
				m_eScreensaver = SCREENSAVER_STATE_ON;
			}
		}
		else if (((reporter == m_pDefaultForm) || (reporter == m_pSideMenu)  || (reporter == m_pScreensaverForm))
			&& (reporter->GetAlphaBlending() == 0))
		{
			if (reporter == m_pDefaultForm)
				m_pDefaultForm->Visible(false);
			if (reporter == m_pSideMenu)
				m_pSideMenu->Visible(false);
			if (reporter == m_pScreensaverForm)
			{
				m_pScreensaverForm->Visible(false);
				m_eScreensaver = SCREENSAVER_STATE_OFF;
			}
		}
		else if ((reporter == m_pScreensaverAlbumImageContainerBackground) && (reporter->GetAlphaBlending() == 0))
		{
			m_iAlbumLoadDone = m_iAlbumLoadDone | 1;
		}
		return;
	case GUI_CONTROL_MOVED:
		if (reporter->GetPositionX() == -WINDOW_WIDTH)
		{
			SallyAPI::GUI::CControl*  control = dynamic_cast<SallyAPI::GUI::CControl*> (reporter);
			control->Visible(false);
			control->Enable(false);
			return;
		}
		else if ((reporter->GetPositionX() == 0) && (reporter->GetPositionY() == 0))
		{
			SallyAPI::GUI::CControl*  control = dynamic_cast<SallyAPI::GUI::CControl*> (reporter);
			control->Enable(true);
			return;
		}
		else if ((reporter == m_pAlbumImageContainer) && (reporter->GetPositionX() == -m_iCoverSize))
		{
			m_iAlbumLoadDone = m_iAlbumLoadDone | 10;
		}
		break;
	case GUI_CONTROL_RESIZED:
		if (reporter == m_pVideoImageContainer)
			m_pVideoImageContainer->RotateAnimatedY(5, false);
		return;
	case MS_DIALOG_OK:
		switch (reporterId)
		{
		case GUI_APP_SAVE_PLAYLIST:
			OnCommandSavePlaylistDialog(messageParameter);
			return;
		}
		break;
	case MS_DIALOG_YES:
		switch (reporterId)
		{
		case GUI_APP_SAVE_PLAYLIST:
			OnCommandSavePlaylistDialog();
			return;
		}
		break;
	case MS_SALLY_CANCEL_OPEN_FILE:
		return;
	case MS_DIALOG_CANCEL:
		return;
	case GUI_LISTVIEW_ITEM_ADDED:
		switch (reporterId)
		{
		case GUI_APP_PLAYLIST:
			OnCommandPlaylistItemAdded();
			return;
		}
		return;
	case GUI_MOUSEMOVE_UP:
	case GUI_MOUSEMOVE_UP_FAST:
		switch (reporterId)
		{
		case GUI_APP_PICTURE_PRESSED:
			m_pParent->SendMessageToParent(this, m_iControlId, MS_SALLY_APP_START_SCREENSAVER);
			return;
		}	
		break;
	case GUI_RATING_CHANGED:
		switch (reporterId)
		{
		case GUI_APP_INFO_RATING_CHANGED:
			OnCommandRatingChanged(messageParameter);
			return;
		case GUI_APP_SCREENSAVER_RATING_CHANGED:
			m_pRating->SetRating(m_pScreensaverRating->GetRating());
			m_pInfoPopUp->UpdateRating(m_pScreensaverRating->GetRating());

			OnCommandUpdateRating();
			return;
		case GUI_APP_DEFAULT_RATING_CHANGED:
			m_pScreensaverRating->SetRating(m_pRating->GetRating());
			m_pInfoPopUp->UpdateRating(m_pRating->GetRating());

			OnCommandUpdateRating();
			return;
		}
		break;
	case GUI_BUTTON_CLICKED:
		switch (reporterId)
		{
		case GUI_APP_SHOW_INFO:
			m_pParent->SendMessageToParent(m_pInfoPopUp, 0, MS_SALLY_SHOW_POPUP_VIEW, 0);
			return;
		case GUI_APP_MENU_NOW_PLAYING:
			m_pSideMenuCurrentPlay->Visible(true);

			if (m_pSideMenuCurrentPlay->GetPositionY() != 0)
				m_pSideMenuCurrentPlay->Move(0, WINDOW_HEIGHT);

			m_pSideMenuCurrentPlay->MoveAnimated(0, 0, 4000);
			m_pAddMusicSearch->MoveAnimated(0, -WINDOW_HEIGHT, 4000);
			m_pAddMusicAlbum->MoveAnimated(0, -WINDOW_HEIGHT, 4000);
			m_pAddMusicExplorer->MoveAnimated(0, -WINDOW_HEIGHT, 4000);
			m_pAddMusicFavorites->MoveAnimated(0, -WINDOW_HEIGHT, 4000);

			m_pPlaylistLoad->MoveAnimated(0, -WINDOW_HEIGHT, 4000);
			m_pPlaylistDelete->MoveAnimated(0, -WINDOW_HEIGHT, 4000);

			m_pSideMenuNowPlaying->SetActive(true);
			m_pSideMenuSearch->SetActive(false);
			m_pSideMenuAlbum->SetActive(false);
			m_pSideMenuExplorer->SetActive(false);
			m_pSideMenuFavorites->SetActive(false);

			m_pSideMenuLoadPlaylist->SetActive(false);
			m_pSideMenuDeletePlaylist->SetActive(false);
			return;
		case GUI_APP_MENU_ADDMUSIC_SEARCH:
			m_pAddMusicSearch->Visible(true);
			m_pAddMusicSearch->Activate();

			if (m_pAddMusicSearch->GetPositionY() != 0)
				m_pAddMusicSearch->Move(0, WINDOW_HEIGHT);

			m_pSideMenuCurrentPlay->MoveAnimated(0, -WINDOW_HEIGHT, 4000);
			m_pAddMusicSearch->MoveAnimated(0, 0, 4000);
			m_pAddMusicAlbum->MoveAnimated(0, -WINDOW_HEIGHT, 4000);
			m_pAddMusicExplorer->MoveAnimated(0, -WINDOW_HEIGHT, 4000);
			m_pAddMusicFavorites->MoveAnimated(0, -WINDOW_HEIGHT, 4000);

			m_pPlaylistLoad->MoveAnimated(0, -WINDOW_HEIGHT, 4000);
			m_pPlaylistDelete->MoveAnimated(0, -WINDOW_HEIGHT, 4000);

			m_pSideMenuNowPlaying->SetActive(false);
			m_pSideMenuSearch->SetActive(true);
			m_pSideMenuAlbum->SetActive(false);
			m_pSideMenuExplorer->SetActive(false);
			m_pSideMenuFavorites->SetActive(false);

			m_pSideMenuLoadPlaylist->SetActive(false);
			m_pSideMenuDeletePlaylist->SetActive(false);

			m_pSnapBackTimer->Reset();
			return;
		case GUI_APP_MENU_ADDMUSIC_ALBUM:
			m_pAddMusicAlbum->Visible(true);

			if (m_pAddMusicAlbum->GetPositionY() != 0)
				m_pAddMusicAlbum->Move(0, WINDOW_HEIGHT);

			m_pSideMenuCurrentPlay->MoveAnimated(0, -WINDOW_HEIGHT, 4000);
			m_pAddMusicSearch->MoveAnimated(0, -WINDOW_HEIGHT, 4000);
			m_pAddMusicAlbum->MoveAnimated(0, 0, 4000);
			m_pAddMusicExplorer->MoveAnimated(0, -WINDOW_HEIGHT, 4000);
			m_pAddMusicFavorites->MoveAnimated(0, -WINDOW_HEIGHT, 4000);

			m_pPlaylistLoad->MoveAnimated(0, -WINDOW_HEIGHT, 4000);
			m_pPlaylistDelete->MoveAnimated(0, -WINDOW_HEIGHT, 4000);

			m_pSideMenuNowPlaying->SetActive(false);
			m_pSideMenuSearch->SetActive(false);
			m_pSideMenuAlbum->SetActive(true);
			m_pSideMenuExplorer->SetActive(false);
			m_pSideMenuFavorites->SetActive(false);

			m_pSideMenuLoadPlaylist->SetActive(false);
			m_pSideMenuDeletePlaylist->SetActive(false);

			m_pSnapBackTimer->Reset();
			return;
		case GUI_APP_MENU_ADDMUSIC_EXPLORER:
			m_pAddMusicExplorer->Visible(true);

			if (m_pAddMusicExplorer->GetPositionY() != 0)
				m_pAddMusicExplorer->Move(0, WINDOW_HEIGHT);

			m_pSideMenuCurrentPlay->MoveAnimated(0, -WINDOW_HEIGHT, 4000);
			m_pAddMusicSearch->MoveAnimated(0, -WINDOW_HEIGHT, 4000);
			m_pAddMusicAlbum->MoveAnimated(0, -WINDOW_HEIGHT, 4000);
			m_pAddMusicExplorer->MoveAnimated(0, 0, 4000);
			m_pAddMusicFavorites->MoveAnimated(0, -WINDOW_HEIGHT, 4000);

			m_pPlaylistLoad->MoveAnimated(0, -WINDOW_HEIGHT, 4000);
			m_pPlaylistDelete->MoveAnimated(0, -WINDOW_HEIGHT, 4000);

			m_pSideMenuNowPlaying->SetActive(false);
			m_pSideMenuSearch->SetActive(false);
			m_pSideMenuAlbum->SetActive(false);
			m_pSideMenuExplorer->SetActive(true);
			m_pSideMenuFavorites->SetActive(false);

			m_pSideMenuLoadPlaylist->SetActive(false);
			m_pSideMenuDeletePlaylist->SetActive(false);

			m_pSnapBackTimer->Reset();
			return;
		case GUI_APP_MENU_ADDMUSIC_FAVORITES:
			m_pAddMusicFavorites->Visible(true);

			if (m_pAddMusicFavorites->GetPositionY() != 0)
				m_pAddMusicFavorites->Move(0, WINDOW_HEIGHT);

			m_pSideMenuCurrentPlay->MoveAnimated(0, -WINDOW_HEIGHT, 4000);
			m_pAddMusicSearch->MoveAnimated(0, -WINDOW_HEIGHT, 4000);
			m_pAddMusicAlbum->MoveAnimated(0, -WINDOW_HEIGHT, 4000);
			m_pAddMusicExplorer->MoveAnimated(0, -WINDOW_HEIGHT, 4000);
			m_pAddMusicFavorites->MoveAnimated(0, 0, 4000);

			m_pPlaylistLoad->MoveAnimated(0, -WINDOW_HEIGHT, 4000);
			m_pPlaylistDelete->MoveAnimated(0, -WINDOW_HEIGHT, 4000);

			m_pSideMenuNowPlaying->SetActive(false);
			m_pSideMenuSearch->SetActive(false);
			m_pSideMenuAlbum->SetActive(false);
			m_pSideMenuExplorer->SetActive(false);
			m_pSideMenuFavorites->SetActive(true);

			m_pSideMenuLoadPlaylist->SetActive(false);
			m_pSideMenuDeletePlaylist->SetActive(false);

			m_pSnapBackTimer->Reset();
			return;
		case GUI_APP_MENU_LOAD_PLAYLIST:
			m_pPlaylistLoad->Visible(true);

			if (m_pPlaylistLoad->GetPositionY() != 0)
				m_pPlaylistLoad->Move(0, WINDOW_HEIGHT);

			m_pSideMenuCurrentPlay->MoveAnimated(0, -WINDOW_HEIGHT, 4000);
			m_pAddMusicSearch->MoveAnimated(0, -WINDOW_HEIGHT, 4000);
			m_pAddMusicAlbum->MoveAnimated(0, -WINDOW_HEIGHT, 4000);
			m_pAddMusicExplorer->MoveAnimated(0, -WINDOW_HEIGHT, 4000);
			m_pAddMusicFavorites->MoveAnimated(0, -WINDOW_HEIGHT, 4000);

			m_pPlaylistLoad->MoveAnimated(0, 0, 4000);
			m_pPlaylistDelete->MoveAnimated(0, -WINDOW_HEIGHT, 4000);

			m_pPlaylistLoad->Reload();

			m_pSideMenuNowPlaying->SetActive(false);
			m_pSideMenuSearch->SetActive(false);
			m_pSideMenuAlbum->SetActive(false);
			m_pSideMenuFavorites->SetActive(false);
			m_pSideMenuExplorer->SetActive(false);

			m_pSideMenuLoadPlaylist->SetActive(true);
			m_pSideMenuDeletePlaylist->SetActive(false);

			m_pSnapBackTimer->Reset();
			return;
		case GUI_APP_MENU_DELETE_PLAYLIST:
			m_pPlaylistDelete->Visible(true);

			if (m_pPlaylistDelete->GetPositionY() != 0)
				m_pPlaylistDelete->Move(0, WINDOW_HEIGHT);

			m_pSideMenuCurrentPlay->MoveAnimated(0, -WINDOW_HEIGHT, 4000);
			m_pAddMusicSearch->MoveAnimated(0, -WINDOW_HEIGHT, 4000);
			m_pAddMusicAlbum->MoveAnimated(0, -WINDOW_HEIGHT, 4000);
			m_pAddMusicExplorer->MoveAnimated(0, -WINDOW_HEIGHT, 4000);
			m_pAddMusicFavorites->MoveAnimated(0, -WINDOW_HEIGHT, 4000);

			m_pPlaylistLoad->MoveAnimated(0, -WINDOW_HEIGHT, 4000);
			m_pPlaylistDelete->MoveAnimated(0, 0, 4000);

			m_pPlaylistDelete->Reload();

			m_pSideMenuNowPlaying->SetActive(false);
			m_pSideMenuSearch->SetActive(false);
			m_pSideMenuAlbum->SetActive(false);
			m_pSideMenuFavorites->SetActive(false);
			m_pSideMenuExplorer->SetActive(false);

			m_pSideMenuLoadPlaylist->SetActive(false);
			m_pSideMenuDeletePlaylist->SetActive(true);

			m_pSnapBackTimer->Reset();
			return;
		case GUI_APP_MENU_SAVE_PLAYLIST:
			OnCommandMenuSavePlaylist();
			return;
		case GUI_APP_START_FULLSCREEN:
			m_pParent->SendMessageToParent(this, m_iControlId, MS_SALLY_APP_START_SCREENSAVER);
			return;
		case GUI_APP_STOP_FULLSCREEN:
			m_pParent->SendMessageToParent(this, m_iControlId, MS_SALLY_APP_STOP_SCREENSAVER);
			OnCommandHideBottomMenu();
			return;
		case GUI_APP_PLAY:
			if (m_pMediaPlayer->GetState() != State_Running)
			{
				if (m_pMediaPlayer->GetState() != State_Paused)
					OnCommandNext();
				else
					OnCommandPlay();
			}
			else
				OnCommandPause();
			return;
		case GUI_APP_STOP:
			OnCommandStop();
			return;
		case GUI_APP_NEXT:
			OnCommandNext();
			return;
		case GUI_APP_PREVIOUS:
			OnCommandPrevious();
			return;
		case GUI_APP_MENU_CLEAR:
			OnCommandClearList();
			return;
		case GUI_APP_SCREENSAVER_NEXT:
			OnCommandScreensaverNext();
			return;
		case GUI_APP_SCREENSAVER_PREVIOUS:
			OnCommandScreensaverPrevious();
			return;
		case GUI_APP_SCREENSAVER_PLAY:
			if ((m_pMediaPlayer->GetState() == State_Paused) || (m_pMediaPlayer->GetState() == State_Stopped))
				OnCommandScreensaverPlay();
			else
				OnCommandScreensaverPause();
			return;
		case GUI_APP_MENU_SHUFFLE_BOTTOM:
			OnCommandSwitchShuffle();
			return;
		case GUI_APP_MENU_SHUFFLE:
			if (m_pMenuShuffle->GetCheckStatus())
			{
				m_pMenuShuffle->SetCheckStatus(false);
				m_pShuffle->SetCheckStatus(false);
				SetPropertyBool("shuffle", false);
			}
			else
			{
				m_pMenuShuffle->SetCheckStatus(true);
				m_pShuffle->SetCheckStatus(true);
				SetPropertyBool("shuffle", true);
			}
			return;
		}
		break;
	case GUI_SLIDER_MOVED:
	case GUI_SLIDER_CLICKED:
		OnCommandProcessbarMoved(reporter, messageParameter);
		return;
	case GUI_LISTVIEW_ITEM_ACTION_CLICKED:
		OnCommandRemoveFile(messageParameter);
		return;
	case GUI_LISTVIEW_ITEM_CLICKED:
		OnCommandGoToFile(messageParameter);
		return;
	case GUI_APP_UPDATE_VIDEO_INFO:
		UpdateVideoScreensaver();
		return;
	case GUI_APP_UPDATE_MP3_INFO:
		UpdateMp3Screensaver();
		return;
	case GUI_APP_ALBUM_COVER_MP3HELPER_LOADED:
		UpdateAlbumCover(messageParameter);
		return;
	}
	if (m_eScreensaver == SCREENSAVER_STATE_ON)
	{
		if (reporter == m_pScreensaverForm)
		{
			SallyAPI::GUI::SendMessage::CParameterPoint* parameter = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterPoint*> (messageParameter);

			switch(messageId)
			{
			case GUI_MOUSEMOVE_UP_FAST:
			case GUI_MOUSEMOVE_UP:
				if ((parameter != NULL) && (parameter->GetY() > MENU_HEIGHT) && (parameter->GetY() < m_pBottomMenu->GetPositionY()))
					OnCommandSwitchShuffle();
				return;
			case GUI_MOUSEMOVE_DOWN_FAST:
			case GUI_MOUSEMOVE_DOWN:
				if ((parameter != NULL) && (parameter->GetY() > MENU_HEIGHT) && (parameter->GetY() < m_pBottomMenu->GetPositionY()))
				{
					m_pParent->SendMessageToParent(this, m_iControlId, MS_SALLY_APP_STOP_SCREENSAVER);
					OnCommandHideBottomMenu();
				}
				return;
			case GUI_MOUSEMOVE_LEFT_FAST:
			case GUI_MOUSEMOVE_LEFT:
				if ((parameter != NULL) && (parameter->GetY() > MENU_HEIGHT) && (parameter->GetY() < m_pBottomMenu->GetPositionY()))
					OnCommandScreensaverLeft();
				return;
			case GUI_MOUSEMOVE_RIGHT_FAST:
			case GUI_MOUSEMOVE_RIGHT:
				if ((parameter != NULL) && (parameter->GetY() > MENU_HEIGHT) && (parameter->GetY() < m_pBottomMenu->GetPositionY()))
					OnCommandScreensaverRight();
				return;
			case GUI_FORM_CLICKED:
				if (m_pBottomMenu->GetPositionY() == WINDOW_HEIGHT)
					OnCommandShowBottomMenu();
				else
					OnCommandHideBottomMenu();
				return;
			case GUI_FORM_DOUBLECLICKED:
				if (m_pMediaPlayer->GetState() == State_Paused)
					OnCommandScreensaverPlay();
				else
					OnCommandScreensaverPause();
				return;
			}
		}
	}

	CApplicationWindow::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

void CAppMediaPlayer::OnCommandPlayLastFile()
{
	if (m_pPlaylist->GetListSize() == 0)
		return;

	SallyAPI::GUI::SendMessage::CParameterInteger parameterInteger(m_pPlaylist->GetListSize() - 1);

	m_pPlaylist->SetStartItem(m_pPlaylist->GetListSize());

	OnCommandGoToFile(&parameterInteger);

	SallyAPI::GUI::SendMessage::CParameterOnScreenMenu messageOnScreenMenu(GUI_THEME_SALLY_OSM_PLAY, "Play");
	m_pParent->SendMessageToParent(this, 0, MS_SALLY_ON_SCREEN_MENU, &messageOnScreenMenu);
}

void CAppMediaPlayer::OnCommandPlaylistItemAdded()
{
	// add to smart shuffle list
	m_vImageListSmartShuffle.push_back(m_pPlaylist->GetListSize() - 1);
}

void CAppMediaPlayer::OnCommandRatingChanged(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	SallyAPI::GUI::SendMessage::CParameterInteger* parameterInterger = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterInteger*> (messageParameter);

	m_pScreensaverRating->SetRating(parameterInterger->GetInteger());
	m_pRating->SetRating(parameterInterger->GetInteger());

	OnCommandUpdateRating();
}

void CAppMediaPlayer::OnCommandUpdateRating()
{
	std::string messageTemp;
	std::string message;
	std::string action;

	GetStatusMessageText(action, messageTemp);

	message.append("... rates '");
	message.append(messageTemp);
	message.append("' with ");
	message.append(SallyAPI::String::StringHelper::ConvertToString(m_pScreensaverRating->GetRating()));
	message.append(" stars.");

	m_tUpdateRating.SetValues(m_pCurrentFile->GetFilename(), this, m_pScreensaverRating->GetRating(), action, message);
	m_tUpdateRating.Start();
}

void CAppMediaPlayer::OnCommandShowBottomMenu()
{
	m_pBottomMenu->MoveAnimated(0, WINDOW_HEIGHT - (MENU_HEIGHT + 25), 400);
	m_pTopMenu->MoveAnimated((WINDOW_WIDTH - 440) / 2, -20, 400);

	m_pTimerHideMenu->Reset();
	m_pTimerHideMenu->Start();

	m_pVolumeControl->UpdateView();
}

void CAppMediaPlayer::OnCommandHideBottomMenu()
{
	m_pBottomMenu->MoveAnimated(0, WINDOW_HEIGHT, 400);
	m_pTopMenu->MoveAnimated((WINDOW_WIDTH - 440) / 2, -90, 400);

	m_pTimerHideMenu->Stop();
}

void CAppMediaPlayer::OnCommandSwitchShuffle()
{
	SendMessageToParent(this, GUI_APP_MENU_SHUFFLE, GUI_BUTTON_CLICKED);
	if (m_pMenuShuffle->GetCheckStatus())
	{
		SallyAPI::GUI::SendMessage::CParameterOnScreenMenu messageOnScreenMenu(GUI_THEME_SALLY_OSM_SHUFFLEON, "Shuffle On");
		m_pParent->SendMessageToParent(this, 0, MS_SALLY_ON_SCREEN_MENU, &messageOnScreenMenu);
	}
	else
	{
		SallyAPI::GUI::SendMessage::CParameterOnScreenMenu messageOnScreenMenu(GUI_THEME_SALLY_OSM_SHUFFLEOFF, "Shuffle Off");
		m_pParent->SendMessageToParent(this, 0, MS_SALLY_ON_SCREEN_MENU, &messageOnScreenMenu);
	}
}

void CAppMediaPlayer::UpdateAlbumCover(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	CParameterPicture* parameter = dynamic_cast<CParameterPicture*>(messageParameter);
	if (parameter == NULL)
		return;
	if (m_pCurrentFile == NULL)
		return;
	
	SallyAPI::GUI::CPicture* newPicture = parameter->GetPicture();
	SallyAPI::GUI::CPicture* oldPicture = m_pAlbumImageContainer->GetPicture();

	EnterRenderLock();
	// remove from cover loader list
	m_mCoverLoaders.erase(parameter->GetFilename());

	if (m_pCurrentFile->GetFilename().compare(parameter->GetFilename()) != 0)
	{
		//not the current file...
		LeaveRenderLock();

		SafeDelete(newPicture);
		return;
	}

	m_pAlbumCover = newPicture;
	if (m_pAlbumCover != NULL)
	{
		m_pAlbumImageContainer->SetPicture(m_pAlbumCover);
		m_pScreensaverAlbumImageContainerBackground->SetPicture(m_pAlbumCover);
	}
	else
	{
		m_pAlbumImageContainer->SetImageId(GUI_APP_DEFAULT_CD + GetGraphicId());
		m_pScreensaverAlbumImageContainerBackground->SetImageId(GUI_APP_DEFAULT_CD + GetGraphicId());
	}
	LeaveRenderLock();

	m_iAlbumLoadDone = m_iAlbumLoadDone | 100;

	SafeDelete(oldPicture);

	/************************************************************************/
	/* If the application is not active than show a popup                   */
	/************************************************************************/
	if (!this->IsVisible())
	{
		SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
		SallyAPI::Config::CLanguageManager* languageManager = config->GetLanguageLocalization();

		if (m_pCurrentFile->GetType() == MEDIAFILE_AUDIO)
		{
			CAudioFile* mp3File = (CAudioFile*) m_pCurrentFile;
			MP3FileInfo* fileInfo = mp3File->GetMp3Tag();

			std::string infoMessage;
			if ((fileInfo != NULL) && ((fileInfo->GetSzArtist().length() > 0 || fileInfo->GetSzTitle().length() > 0)))
				infoMessage = languageManager->GetString("Now Playing: '%s' - '%s'\nFrom: '%s'", fileInfo->GetSzArtist().c_str(), fileInfo->GetSzTitle().c_str(), fileInfo->GetSzAlbum().c_str(), NULL);
			else
				infoMessage = languageManager->GetString("Now Playing: '%s'", mp3File->GetFilename().c_str(), NULL);

			if (m_pAlbumCover != NULL)
			{
				SallyAPI::GUI::SendMessage::CParameterInfoPopup sendMessageParameterInfoPopup(m_pAlbumCover, GetAppName(), infoMessage);
				m_pParent->SendMessageToParent(this, m_iControlId, MS_SALLY_SHOW_INFO_POPUP, &sendMessageParameterInfoPopup);

				m_iPopUpId = sendMessageParameterInfoPopup.GetId();
			}
			else
			{
				SallyAPI::GUI::SendMessage::CParameterInfoPopup sendMessageParameterInfoPopup(GUI_APP_DEFAULT_CD + GetGraphicId(), GetAppName(), infoMessage);
				m_pParent->SendMessageToParent(this, m_iControlId, MS_SALLY_SHOW_INFO_POPUP, &sendMessageParameterInfoPopup);

				m_iPopUpId = sendMessageParameterInfoPopup.GetId();
			}
		}
	}
}

/************************************************************************/
/* System Messages                                                      */
/************************************************************************/

bool CAppMediaPlayer::ProcessMouseUp(int x, int y)
{
	m_pSnapBackTimer->Reset();

	return SallyAPI::GUI::CApplicationWindow::ProcessMouseUp(x, y);
}

void CAppMediaPlayer::MuteSound()
{
	m_lVolumeMax = -6000;
	m_pMediaPlayer->SetVolume(m_lVolumeMax);
}

void CAppMediaPlayer::UnMuteSound()
{
	m_lVolumeMax = 0;
	m_pMediaPlayer->SetVolume(m_lVolumeMax);
}

/************************************************************************/
/* Screensaver                                                          */
/************************************************************************/
bool CAppMediaPlayer::ActivateScreensaver()
{
	if (m_pMediaPlayer->GetState() == State_Stopped)
		return false;

	m_pSideMenuCurrentPlay->Move(0, 0);
	m_pAddMusicSearch->Move(0, -WINDOW_HEIGHT);
	m_pAddMusicAlbum->Move(0, -WINDOW_HEIGHT);
	m_pAddMusicExplorer->Move(0, -WINDOW_HEIGHT);
	m_pAddMusicFavorites->Move(0, -WINDOW_HEIGHT);
	m_pPlaylistLoad->Move(0, -WINDOW_HEIGHT);
	m_pPlaylistDelete->Move(0, -WINDOW_HEIGHT);

	m_pSideMenuCurrentPlay->Visible(true);
	m_pAddMusicSearch->Visible(false);
	m_pAddMusicAlbum->Visible(false);
	m_pAddMusicExplorer->Visible(false);
	m_pAddMusicFavorites->Visible(false);
	m_pPlaylistLoad->Visible(false);
	m_pPlaylistDelete->Visible(false);

	m_pSideMenuNowPlaying->SetActive(true);
	m_pSideMenuSearch->SetActive(false);
	m_pSideMenuAlbum->SetActive(false);
	m_pSideMenuFavorites->SetActive(false);
	m_pSideMenuExplorer->SetActive(false);
	m_pSideMenuLoadPlaylist->SetActive(false);
	m_pSideMenuDeletePlaylist->SetActive(false);

	m_pAlbumImageContainer->RotateAnimatedY(1, true);

	m_pVideoImageContainer->MoveAnimated(0, 0, 2000);
	m_pVideoImageContainer->ResizeAnimated(WINDOW_WIDTH, WINDOW_HEIGHT, 2000);
	m_pVideoImageContainer->RotateAnimatedY(5, true);

	// blend forms out
	m_pDefaultForm->Enable(false);
	m_pSideMenu->Enable(false);

	m_pDefaultForm->BlendAnimated(0, 800);
	m_pSideMenu->BlendAnimated(0, 800);

	// blend screensaver form in
	m_pScreensaverForm->Visible(true);
	m_pScreensaverForm->BlendAnimated(255, 800);

	if (m_pAlbumImageContainer->GetPositionX() == SMALL_PICTURE_X)
	{
		m_pAlbumImageContainer->MoveAnimated(BIG_PICTURE_X, BIG_PICTURE_Y, 400);
		m_pScreensaverAlbumImageContainerBackground->BlendAnimated(255, 800);
	}
	if (m_pCurrentFile->GetType() == MEDIAFILE_VIDEO)
	{
		m_pScreensaverAlbumImageContainerBackground->BlendAnimated(255, 800);
	}

	m_eScreensaver = SCREENSAVER_STATE_PROCESSING_ON;

	return true;
}

bool CAppMediaPlayer::DeactivateScreensaver()
{
	m_pAlbumImageContainer->RotateAnimatedY(1, false);

	m_pVideoImageContainer->MoveAnimated(SMALL_VIDEO_X, SMALL_VIDEO_Y, 2000);
	m_pVideoImageContainer->ResizeAnimated(SMALL_VIDEO_WIDTH, SMALL_VIDEO_HEIGHT, 2000);
	m_pVideoImageContainer->SetRotationAngleY(0.0f);
	m_pVideoImageContainer->RotateAnimatedY(5, false);

	// blend screensaver form out
	m_pScreensaverForm->Enable(false);
	m_pScreensaverForm->BlendAnimated(0, 800);

	// blend forms out
	m_pDefaultForm->Visible(true);
	m_pSideMenu->Visible(true);

	m_pDefaultForm->BlendAnimated(255, 800);
	m_pSideMenu->BlendAnimated(255, 800);

	m_pScreensaverAlbumImageContainerBackground->BlendAnimated(0, 800);

	if (m_pAlbumImageContainer->GetPositionX() == BIG_PICTURE_X)
	{
		m_pAlbumImageContainer->MoveAnimated(SMALL_PICTURE_X, SMALL_PICTURE_Y, 400);
	}

	m_eScreensaver = SCREENSAVER_STATE_PROCESSING_OFF;

	return true;
}

void CAppMediaPlayer::OnCommandScreensaverRight()
{
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
}

void CAppMediaPlayer::OnCommandScreensaverLeft()
{
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
}

void CAppMediaPlayer::OnCommandScreensaverPrevious()
{
	OnCommandPrevious();

	SallyAPI::GUI::SendMessage::CParameterOnScreenMenu messageOnScreenMenu(GUI_THEME_SALLY_OSM_PREVIOUS, "Previous");
	m_pParent->SendMessageToParent(this, 0, MS_SALLY_ON_SCREEN_MENU, &messageOnScreenMenu);
}

void CAppMediaPlayer::OnCommandScreensaverNext()
{
	OnCommandNext();

	SallyAPI::GUI::SendMessage::CParameterOnScreenMenu messageOnScreenMenu(GUI_THEME_SALLY_OSM_NEXT, "Next");
	m_pParent->SendMessageToParent(this, 0, MS_SALLY_ON_SCREEN_MENU, &messageOnScreenMenu);
}

void CAppMediaPlayer::OnCommandScreensaverPause()
{
	OnCommandPause();

	SallyAPI::GUI::SendMessage::CParameterOnScreenMenu messageOnScreenMenu(GUI_THEME_SALLY_OSM_PAUSE, "Pause");
	m_pParent->SendMessageToParent(this, 0, MS_SALLY_ON_SCREEN_MENU, &messageOnScreenMenu);
}

void CAppMediaPlayer::OnCommandScreensaverPlay()
{
	OnCommandPlay();

	SallyAPI::GUI::SendMessage::CParameterOnScreenMenu messageOnScreenMenu(GUI_THEME_SALLY_OSM_PLAY, "Play");
	m_pParent->SendMessageToParent(this, 0, MS_SALLY_ON_SCREEN_MENU, &messageOnScreenMenu);
}

void CAppMediaPlayer::UpdateVideoScreensaver()
{
	CVideoFile* videoFile = (CVideoFile*) m_pCurrentFile;

	int rating = CMediaDatabase::GetRating(videoFile->GetFilename(), this);
	if (rating != -1)
	{
		m_pRating->Visible(true);
		m_pRatingDescription->Visible(true);
		m_pRating->SetRating(rating);
	}
	else
	{
		m_pRating->Visible(false);
		m_pRatingDescription->Visible(false);
	}


	/************************************************************************/
	/* Update InfoPopUp                                                     */
	/************************************************************************/
	int playTime = CMediaDatabase::GetPlaytime(videoFile->GetFilename(), this);
	std::string timeplayed = "-";
	if (playTime != -1)
	{
		timeplayed = SallyAPI::String::StringHelper::ConvertToString(playTime);
	}
	m_pInfoPopUp->UpdateVideo(videoFile, timeplayed, rating);

	/************************************************************************/
	/* If the application is not active than show a popup                   */
	/************************************************************************/
	if (!this->IsVisible())
	{
		SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
		SallyAPI::Config::CLanguageManager* languageManager = config->GetLanguageLocalization();

		std::string infoMessage = languageManager->GetString("Now Playing: '%s'", m_pCurrentFile->GetFilename().c_str(), NULL);

		SallyAPI::GUI::SendMessage::CParameterInfoPopup sendMessageParameterInfoPopup(m_pVideoPicture, GetAppName(), infoMessage);
		m_pParent->SendMessageToParent(this, m_iControlId, MS_SALLY_SHOW_INFO_POPUP, &sendMessageParameterInfoPopup);

		m_iPopUpId = sendMessageParameterInfoPopup.GetId();
	}

	// send status message
	SendStatusMessage();
}

void CAppMediaPlayer::UpdateMp3Screensaver()
{
	CAudioFile* mp3File = (CAudioFile*) m_pCurrentFile;

	MP3FileInfo* fileInfo = mp3File->GetMp3Tag();

	if (fileInfo != NULL)
	{
		if (fileInfo->GetSzArtist().length() > 0 || fileInfo->GetSzTitle().length() > 0)
		{
			m_pScreensaverStatusLabel[0]->SetText(fileInfo->GetSzArtist());
			m_pScreensaverStatusLabel[1]->SetText(fileInfo->GetSzTitle());
			m_pScreensaverStatusLabel[0]->Visible(true);
			m_pScreensaverStatusLabel[1]->Visible(true);
		}
		else
		{
			m_pScreensaverStatusLabel[0]->SetText(m_pCurrentFile->GetFormatedText());
			m_pScreensaverStatusLabel[1]->SetText("");
			m_pScreensaverStatusLabel[0]->Visible(true);
			m_pScreensaverStatusLabel[1]->Visible(false);
		}
		m_pScreensaverStatusLabel[2]->SetText(fileInfo->GetSzAlbum());
		if (fileInfo->GetSzAlbum().length() == 0)
			m_pScreensaverStatusLabel[2]->Visible(false);
		else
			m_pScreensaverStatusLabel[2]->Visible(true);
	}
	else
	{
		m_pScreensaverStatusLabel[0]->SetText(m_pCurrentFile->GetFormatedText());
		m_pScreensaverStatusLabel[1]->SetText("");
		m_pScreensaverStatusLabel[2]->SetText("");

		m_pScreensaverStatusLabel[0]->Visible(true);
		m_pScreensaverStatusLabel[1]->Visible(false);
		m_pScreensaverStatusLabel[2]->Visible(false);
	}

	int rating = CMediaDatabase::GetRating(mp3File->GetFilename(), this);
	if (rating != -1)
	{
		m_pRating->Visible(true);
		m_pRatingDescription->Visible(true);
		m_pRating->SetRating(rating);
		m_pScreensaverRating->Visible(true);
		m_pScreensaverRating->SetRating(rating);
	}
	else
	{
		m_pRating->Visible(false);
		m_pRatingDescription->Visible(false);
		m_pScreensaverRating->Visible(false);
	}

	/************************************************************************/
	/* Update InfoPopUp                                                     */
	/************************************************************************/
	int playTime = CMediaDatabase::GetPlaytime(mp3File->GetFilename(), this);
	std::string timeplayed = "-";
	if (playTime != -1)
	{
		timeplayed = SallyAPI::String::StringHelper::ConvertToString(playTime);
	}
	m_pInfoPopUp->UpdateMp3(mp3File, timeplayed, rating);

	// send status message
	SendStatusMessage();
}

void CAppMediaPlayer::SendStatusMessage()
{
	SallyAPI::Community::CCommunityManager* communityManager = SallyAPI::Community::CCommunityManager::GetInstance();

	std::string messageTemp;
	std::string message;
	std::string action;

	GetStatusMessageText(action, messageTemp);

	message.append("... is playing: ");
	message.append(messageTemp);

	communityManager->SendStatusMessage(this->GetExplicitAppName(), this->GetAppName(), message, action, "Search for this");
}

void CAppMediaPlayer::GetStatusMessageText(std::string& action, std::string& message)
{
	if (m_pCurrentFile->GetType() == MEDIAFILE_AUDIO)
	{
		// Start the Mp3Helper
		CAudioFile* mp3 = (CAudioFile*) m_pCurrentFile;

		MP3FileInfo* fileInfo = mp3->GetMp3Tag();

		if ((fileInfo != NULL) && ((fileInfo->GetSzArtist().length() > 0 || fileInfo->GetSzTitle().length() > 0)))
		{
			message.append(fileInfo->GetSzArtist());
			message.append(" - ");
			message.append(fileInfo->GetSzTitle());

			action.append(fileInfo->GetSzArtist());
			action.append("%");
			action.append(fileInfo->GetSzTitle());
		}
		else
		{
			action = SallyAPI::String::PathHelper::GetFileFromPath(m_pCurrentFile->GetFilename());
			message.append(action);
		}
	}
	else
	{
		action = SallyAPI::String::PathHelper::GetFileFromPath(m_pCurrentFile->GetFilename());
		message.append(action);
	}
}

void CAppMediaPlayer::OnCommandCommunityNotify(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	m_pAddMusicSearch->OnCommandPopUpInfoNotify(messageParameter);

	SendMessageToParent(this, GUI_APP_MENU_ADDMUSIC_SEARCH, GUI_BUTTON_CLICKED);
}

bool CAppMediaPlayer::SpecialKeyPressed(int key)
{
	if (m_eScreensaver == SCREENSAVER_STATE_ON)
	{
		switch (key)
		{
		case SPECIAL_KEY_PLAY:
			SendMessageToParent(this, GUI_APP_SCREENSAVER_PLAY, GUI_BUTTON_CLICKED);
			return true;
		case SPECIAL_KEY_PREVIOUS:
			OnCommandScreensaverPrevious();
			return true;
		case SPECIAL_KEY_NEXT:
			OnCommandScreensaverNext();
			return true;
		}
	}
	else if (m_eScreensaver == SCREENSAVER_STATE_OFF)
	{
		switch (key)
		{
		case SPECIAL_KEY_PLAY:
			SendMessageToParent(this, GUI_APP_SCREENSAVER_PLAY, GUI_BUTTON_CLICKED);
			return true;
		case SPECIAL_KEY_PREVIOUS:
			OnCommandScreensaverPrevious();
			return true;
		case SPECIAL_KEY_NEXT:
			OnCommandScreensaverNext();
			return true;
		case SPECIAL_KEY_STOP:
			SendMessageToParent(this, GUI_APP_STOP, GUI_BUTTON_CLICKED);
			return true;
		}
	}
	switch (key)
	{
	case SPECIAL_KEY_SEEK_FORWARD:
		if (m_pMediaPlayer != NULL)
		{
			REFTIME time;
			REFTIME duration;
			time = m_pMediaPlayer->GetCurrentPosition();
			duration = m_pMediaPlayer->GetDuration();
			time += 15;
			if (time > duration)
				time = duration;
			m_pMediaPlayer->SetCurrentPosition(time);
		}
		return true;
	case SPECIAL_KEY_SEEK_BACKWARD:
		if (m_pMediaPlayer != NULL)
		{
			REFTIME time;
			time = m_pMediaPlayer->GetCurrentPosition();
			time -= 15;
			if (time < 0)
				time = 0;
			m_pMediaPlayer->SetCurrentPosition(time);
		}
		return true;
	}
	return false;
}

void CAppMediaPlayer::OnCommandDeviceRestoreStart()
{
	if (m_pCurrentFile == NULL)
		return;
	if (m_pCurrentFile->GetType() != MEDIAFILE_VIDEO)
		return;

	m_pMediaPlayer->OnDeviceLost();
	OnCommandStop();
}

void CAppMediaPlayer::OnCommandDeviceRestoreEnd()
{
	if (m_pMediaPlayer->ShouldResume())
		OnCommandPlay(false);
}

bool CAppMediaPlayer::HasScreensaver()
{
	return true;
}