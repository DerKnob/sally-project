////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\InfoPopUp.h
///
/// \brief	Declares the information pop up class. 
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
#include "AudioFile.h"
#include "VideoFile.h"

class CInfoPopUp
	: public SallyAPI::GUI::CPopUpWindow
{
private:
	SallyAPI::GUI::CApplicationWindow* m_pApplicationWindow;

	SallyAPI::GUI::CButton*		m_pRemoveRating;
	SallyAPI::GUI::CLabel*		m_pRatingDescription;

	SallyAPI::GUI::CGroupBox*	m_pBackground;

	SallyAPI::GUI::CImageBox*	m_pIcon;
	SallyAPI::GUI::CLabel*		m_pArtist;
	SallyAPI::GUI::CLabel*		m_pTitle;
	SallyAPI::GUI::CLabel*		m_pAlbum;
	SallyAPI::GUI::CLabel*		m_pDescriptionLabel[7];
	SallyAPI::GUI::CLabel*		m_pStatusLabel[7];
	SallyAPI::GUI::CRating*		m_pRating;

	void	OnCommandRatingChanged();
	void	OnCommandRemoveRating();
public:
	CInfoPopUp(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, const std::string& explicidAppName);
	virtual ~CInfoPopUp();

	virtual void SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);

	void UpdateMp3(CAudioFile* mp3File, const std::string& timeplayed, int rating);
	void UpdateVideo(CVideoFile* currentFile, const std::string& timeplayed, int rating);
	void UpdateRating(int rating);
};
