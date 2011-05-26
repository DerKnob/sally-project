////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\AppMediaPlayer.h
///
/// \brief	Declares the application media player class. 
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
#include "PlaylistHelper.h"
#include "MediaPlayerHelper.h"
#include "MyConfigPanel.h"
#include "MyWizardPanel.h"
#include "AddMusicSearch.h"
#include "AddMusicAlbum.h"
#include "AddMusicExplorer.h"
#include "AddMusicFavorites.h"
#include "PlaylistManager.h"
#include "UpdateRating.h"
#include "InfoPopUp.h"
#include "MediaPlayer.h"
#include "Playlist.h"
#include "ParameterPicture.h"
#include "CoverLoader.h"

#pragma comment(lib, "id3lib.lib")

enum SCREENSAVER_STATE { SCREENSAVER_STATE_ON, SCREENSAVER_STATE_OFF, SCREENSAVER_STATE_PROCESSING_ON, SCREENSAVER_STATE_PROCESSING_OFF };

class CAppMediaPlayer :
	public SallyAPI::GUI::CApplicationWindow
{
private:
	SallyAPI::GUI::CConfigPanel*	m_pConfigPanel;
	SallyAPI::GUI::CWizardPanel*	m_pWizardPanel;

	SallyAPI::GUI::CPicture*		m_pAlbumCover;
	SallyAPI::GUI::CPicture*		m_pAlbumCoverNew;
	SallyAPI::GUI::CPicture*		m_pVideoPicture;
	SallyAPI::GUI::CImageBox*		m_pScreensaverAlbumImageContainerBackground;
	SallyAPI::GUI::CImageBox*		m_pAlbumImageContainer;
	SallyAPI::GUI::CImageBox*		m_pVideoImageContainer;
	int								m_iCurrentNumber;
	std::string						m_strPlaylistName;
	CMediaPlayerHelper				m_tMediaPlayerHelper;
	CUpdateRating					m_tUpdateRating;
	SallyAPI::GUI::CSideMenu*		m_pSideMenu;
	std::vector<int>				m_vHistoryPlayList;
	CInfoPopUp*						m_pInfoPopUp;
	int								m_iPopUpId;
	std::vector<int>				m_vImageListSmartShuffle;
	CPlaylist*						m_pPlaylist;

	SallyAPI::GUI::CTimer*			m_pTimerSendFacebook;

	// Default Form
	SallyAPI::GUI::CForm*				m_pSideMenuCurrentPlay;
	SallyAPI::GUI::CForm*				m_pDefaultForm;
	SallyAPI::GUI::CForm*				m_pCoverForm;
	SallyAPI::GUI::CListViewExt*		m_pListViewPlaylist;
	SallyAPI::GUI::CSlider*				m_pSliderTime;
	SallyAPI::GUI::CRoundButton*		m_pButtonPlay;
	SallyAPI::GUI::CRoundButton*		m_pButtonStop;
	SallyAPI::GUI::CRoundButton*		m_pButtonNext;
	SallyAPI::GUI::CRoundButton*		m_pButtonPrevious;
	SallyAPI::GUI::CForm*				m_pPicture;
	SallyAPI::GUI::CLabel*				m_pTime;
	SallyAPI::GUI::CLabel*				m_pTrack;
	SallyAPI::GUI::CLabel*				m_pAlbum;
	SallyAPI::GUI::CLabel*				m_pRatingDescription;
	SallyAPI::GUI::CLabel*				m_pScreensaverStatusLabel[3];
	SallyAPI::GUI::CRating*				m_pRating;

	SallyAPI::GUI::CButtonBar*			m_pMenu;
	
	SallyAPI::GUI::CButtonBarButton*	m_pMenuShuffle;
	SallyAPI::GUI::CButtonBarButton*	m_pMenuClear;
	SallyAPI::GUI::CButtonBarButton*	m_pMenuRemoveBefore;
	SallyAPI::GUI::CButtonBarButton*	m_pMenuRemoveAfter;

	SallyAPI::GUI::CSideMenuButton*		m_pSideMenuNowPlaying;

	SallyAPI::GUI::CSideMenuButton*		m_pSideMenuSeperator1;
	SallyAPI::GUI::CSideMenuButton*		m_pSideMenuAddMusic;
	SallyAPI::GUI::CSideMenuButton*		m_pSideMenuSearch;
	SallyAPI::GUI::CSideMenuButton*		m_pSideMenuAlbum;
	SallyAPI::GUI::CSideMenuButton*		m_pSideMenuFavorites;
	SallyAPI::GUI::CSideMenuButton*		m_pSideMenuExplorer;

	SallyAPI::GUI::CSideMenuButton*		m_pSideMenuSeperator2;
	SallyAPI::GUI::CSideMenuButton*		m_pSideMenuPlaylist;
	SallyAPI::GUI::CSideMenuButton*		m_pSideMenuSavePlaylist;
	SallyAPI::GUI::CSideMenuButton*		m_pSideMenuManagePlaylist;

	SallyAPI::GUI::CSideMenuButton*		m_pSideMenuSeperator3;
	SallyAPI::GUI::CSideMenuButton*		m_pSideMenuOptions;
	SallyAPI::GUI::CSideMenuButton*		m_pStartFullscreen;
	SallyAPI::GUI::CSideMenuButton*		m_pMenuInfo;

	SallyAPI::GUI::CWorking*			m_pDBUpdate;
	SallyAPI::GUI::CLabel*				m_pDBUpdateLabel;

	// Screensaver Form
	SallyAPI::GUI::CGroupBox*			m_pTopMenu;
	SallyAPI::GUI::CVolumeControl*		m_pVolumeControl;

	SallyAPI::GUI::CButtonBar*			m_pMenuBar;
	SallyAPI::GUI::CButtonBarButton*	m_pExitFullscreen;
	SallyAPI::GUI::CButtonBarButton*	m_pShuffle;
	SallyAPI::GUI::CButtonBarButton*	m_pLikeIt;
	SallyAPI::GUI::CRoundButton*		m_pScreensaverButtonPlay;
	SallyAPI::GUI::CRoundButton*		m_pScreensaverButtonNext;
	SallyAPI::GUI::CRoundButton*		m_pScreensaverButtonPrevious;
	SallyAPI::GUI::CSlider*				m_pFullscreenSliderTime;
	SallyAPI::GUI::CLabel*				m_pFullscreenTime;

	SallyAPI::GUI::CBottonMenu*			m_pBottomMenu;
	SallyAPI::GUI::CTimer*				m_pTimerHideMenu;

	SallyAPI::GUI::CForm*		m_pScreensaverForm;
	SallyAPI::GUI::CForm*		m_pScreensaverMp3Form;
	SallyAPI::GUI::CForm*		m_pScreensaverLabelForm;
	SallyAPI::GUI::CRating*		m_pScreensaverRating;

	// Suche
	CAddMusicSearch*			m_pAddMusicSearch;
	CAddMusicAlbum*				m_pAddMusicAlbum;
	CAddMusicExplorer*			m_pAddMusicExplorer;
	CAddMusicFavorites*			m_pAddMusicFavorites;

	// Playlist
	CPlaylistManager*			m_pPlaylistManager;

	SallyAPI::GUI::CTimer*		m_pSnapBackTimer;

	CMediaPlayer*				m_pMediaPlayer;

	int					m_iCoverSize;
	SCREENSAVER_STATE	m_eScreensaver;
	int					m_iAlbumLoadDone;
	int					m_iGoNextTries;
	float				m_fDeltaStartPlay;
	long				m_lVolumeMax;

	SallyAPI::GUI::CThreadStarter*			m_pThreadPlay;
	std::map<std::string, CCoverLoader*>	m_mCoverLoaders;

	void			OnCommandListViewItemClicked(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);

	void			OnCommandRemoveBefore();
	void			OnCommandRemoveAfter();

	void			OnCommandLikeIt();
	void			ReloadMp3Stop();
	void			ReloadMp3Start();

	void			RemovePopUpInfo();
	void			CorrectHistory(int number);
	void			RemoveFromSmartShuffle(int number);
	void			RemoveAsPlayedFromSmartShuffle(int number);

	void			ShowErrorMessage(const std::string& showMessage);
	void			ShowErrorMessage(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);

	void			OnCommandDeviceRestoreStart();
	void			OnCommandDeviceRestoreEnd();

	void			OnCommandSavePlaylistDialog(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);
	void			OnCommandSavePlaylistDialog();
	void			OnCommandMenuSavePlaylist();

	bool			OnCommandPlayControled();
	void			OnCommandThreadPlay();
	void			OnCommandPlay(bool startAsThread = true);
	void			OnCommandStop();
	void			OnCommandNext(bool startAsThread = true);
	void			OnCommandPrevious();
	void			OnCommandPause();

	void			OnCommandShowBottomMenu();
	void			OnCommandHideBottomMenu();

	void			OnCommandPlayLastFile(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);
	void			OnCommandGoToFile(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);
	void			OnCommandRemoveFile(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);
	void			OnCommandClearList();
	void			OnCommandProcessbarMoved(SallyAPI::GUI::CGUIBaseObject* reporter, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);
	void			OnCommandPlaylistItemAdded();

	void			UpdateVideoScreensaver();
	void			UpdateMp3Screensaver();
	void			UpdateAlbumCover(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);
	void			OnCommandRatingChanged(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);

	void			OnCommandFacebookNotify(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);

	// Helper Functions
	void			OnCommandUpdateRating();
	void			CreateStatusMessageText(std::string& action, std::string& message);
	void			SendStatusMessage();

	void			OnCommandScreensaverNext();
	void			OnCommandScreensaverPrevious();

	void			OnCommandScreensaverRight();
	void			OnCommandScreensaverLeft();
	void			OnCommandScreensaverPause();
	void			OnCommandScreensaverPlay();

	std::string		CalculateTime(int seconds);

	void			CleanUpMedia();
	void			OnCommandSwitchShuffle();

	virtual void	LoadConfig();
	virtual void	SaveConfig();
	virtual bool	ActivateScreensaver();
	virtual bool	DeactivateScreensaver();
	virtual void	MuteSound();
	virtual void	UnMuteSound();
	virtual bool	ProcessMouseUp(int x, int y);
public:
	CAppMediaPlayer(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, const std::string& pluginPath);
	virtual ~CAppMediaPlayer();

	virtual void Timer(float fDelta);
	virtual void SendMessageToChilds(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
	virtual void SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
	virtual bool SpecialKeyPressed(int key);

	virtual bool HasScreensaver();
};
