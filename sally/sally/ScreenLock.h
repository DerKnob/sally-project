////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\ScreenLock.h
///
/// \brief	Declares the screen lock class. 
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

class CScreenLock :
	public SallyAPI::GUI::CPopUpWindow
{
private:
	SallyAPI::GUI::CTimer*		m_pHideTimer;
	SallyAPI::GUI::CEdit*		m_pInput;

	virtual bool ProcessMouseUp(int x, int y);
public:
	CScreenLock(SallyAPI::GUI::CGUIBaseObject* parent);
	~CScreenLock();

	virtual void	SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
};
