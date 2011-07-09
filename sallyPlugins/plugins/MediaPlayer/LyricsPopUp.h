////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\LyricsPopUp.h
///
/// \brief	Declares the lyrics pop up class. 
///
/// \author	Christian Knobloch
/// \date	08.07.2011
///
/// This file is part of the Sally Project
/// 
/// Copyright(c) 2008-2011 Sally Project
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
#include "LyricGetter.h"

class CLyricsPopUp
	: public SallyAPI::GUI::CPopUpWindow
{
private:
	SallyAPI::System::CSmartThreadPool	m_ThreadPool;
	SallyAPI::System::CCritSection		m_Lock;
	std::string							m_pLastThreadId;
	SallyAPI::GUI::CGroupBox*			m_pBackground;

	SallyAPI::GUI::CImageBox*			m_pIcon;
	SallyAPI::GUI::CLabel*				m_pArtist;
	SallyAPI::GUI::CLabel*				m_pTitle;
	SallyAPI::GUI::CLabelBox*			m_pText;

	void	LyricsLoaded(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);
public:
	CLyricsPopUp(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, const std::string& explicidAppName);
	virtual ~CLyricsPopUp();

	virtual void SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);

	void GetLyric(const std::string& artist, const std::string& title);
};
