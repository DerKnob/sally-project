////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\LyricGetter.h
///
/// \brief	Declares the lyric getter class. 
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

class CLyricGetter
	: public SallyAPI::System::CThread
{
private:
	std::string						m_strArtist;
	std::string						m_strTitle;
	SallyAPI::GUI::CGUIBaseObject*	m_pParent;
	SallyAPI::GUI::CAppBase*		m_pAppBase;
	std::string						m_strErrorText;

	SallyAPI::Network::NETWORK_RETURN GetXML(std::string* response);
	
	bool		ProcessFile(const std::string& tempFile);
	std::string	GetRequestURL();

	virtual void RunEx();
public:
	CLyricGetter();
	virtual ~CLyricGetter();

	void SetValues(const std::string& artist, const std::string& title);
	void SetStaticValues(SallyAPI::GUI::CAppBase* appBase, SallyAPI::GUI::CGUIBaseObject* parent);
};
