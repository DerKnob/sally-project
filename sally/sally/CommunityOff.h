////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\CommunityOff.h
///
/// \brief	Declares the community off class. 
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

#pragma once
#include "Define.h"

class CCommunityOff :
	public SallyAPI::GUI::CForm
{
private:
	SallyAPI::GUI::CLabelBox*	m_pLabelBoxWelcome;
	SallyAPI::GUI::CButton*		m_pShowCommunityConfig;
public:
	CCommunityOff(SallyAPI::GUI::CGUIBaseObject* parent);
	virtual ~CCommunityOff();

	virtual void SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
};
