////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\AddMusicAlbum.h
///
/// \brief	Declares the add music album class. 
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
#include <sallyAPI\sallyAPI.h>
#include "Define.h"
#include "LoadImage.h"
#include "AlbumGetter.h"
#include "Box2DObject.h"
#include "MyConfigPanel.h"
#include "Playlist.h"

class CAddMusicAlbum :
	public SallyAPI::GUI::CForm
{
private:
	b2World*			m_pb2World;
	CBox2DObject*		m_pb2Object;
	int					m_iOldPosition;

	CPlaylist*	m_pPlaylist;

	SallyAPI::GUI::CForm*				m_pSmoothMoveForm;
	SallyAPI::GUI::CImageBox*			m_pZoomBackground;
	SallyAPI::GUI::CImageBox*			m_pImageBoxBig;
	SallyAPI::GUI::CButton*				m_pCharSelector[28];

	int			m_iCols;
	int			m_iRows;
	int			m_iBorderTop;
	int			m_iBorderLeft;
	int			m_iGraphicId;

 	std::map<int, std::vector<SallyAPI::GUI::CImageBox*>>	m_mImage;
 	std::map<int, std::vector<SallyAPI::GUI::CLabelBox*>>	m_mImageName;
	std::map<int, std::vector<SallyAPI::GUI::CButton*>>		m_mImageAddAlbum;
	std::map<std::string, SallyAPI::GUI::CPicture*>			m_mPictureMap;
	std::vector<CAlbum*>									m_vAlbumList;
	CLoadImage				m_tImageLoader;
	int						m_iStartPicture;

	SallyAPI::GUI::CLabelBox*				m_pAlbumName;
	CAlbumGetter							m_tAlbumGetter;
	SallyAPI::GUI::CListViewExt*			m_pAlbumTitles;
	SallyAPI::GUI::CGroupBox*				m_pAlbumBackground;
	SallyAPI::GUI::CButton*					m_pAlbumBackButton;
	SallyAPI::GUI::CButton*					m_pAlbumAddAll;

	SallyAPI::GUI::CForm*					m_pFilterBackground;
	SallyAPI::GUI::CEdit*					m_pArtistFilter;
	SallyAPI::GUI::CEdit*					m_pAlbumFilter;
	SallyAPI::GUI::CEdit*					m_pGenreFilter;
	SallyAPI::GUI::CButton*					m_pButtonClear;
	bool									m_bDisableAutoSearch;

	void	OnCommandDoubleClicked(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);
	void	OnCommandAddAlbum(int reporterId);
	void	OnCommandAlbumPicLoaded();
	void	OnCommandUpdateFilter();
	void	OnCommandProcessClicked(int reporterId);
	void	OnCommandBackClicked();

	void	OnCommandCharSelector(SallyAPI::GUI::CGUIBaseObject* reporter);
	
	void	DeleteLoadedCovers();
	bool	ResetBox2Object();

	void	ResetImages();
	void	UpdateImages();
	void	UpdateImages(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);

	void	AddAllToPlaylistFromListView();
	void	AddToPlaylistFromListView(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);

	virtual void OnCommandMouseMove(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);

	virtual bool CheckProcessMouseDown(int x, int y);
public:
	CAddMusicAlbum(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, CPlaylist* playlist);
	virtual ~CAddMusicAlbum();

	virtual void	SendMessageToParent(SallyAPI::GUI::CGUIBaseObject *reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
	virtual void	Timer(float timeDelta);
	
	void	LoadConfig();
};
