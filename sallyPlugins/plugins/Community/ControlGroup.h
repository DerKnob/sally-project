////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	Community\ControlGroup.h
///
/// \brief	Declares the control group class. 
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
#include <sallyAPI\sallyAPI.h>
#include "Define.h"

class CControlGroup
	: public SallyAPI::GUI::CGroupBox
{
private:
	SallyAPI::GUI::CImageBox*			m_pAvatar;
	SallyAPI::GUI::CLabel*				m_pName;
	SallyAPI::GUI::CLabelBox*			m_pMessage;
	SallyAPI::GUI::CButton*				m_pActionButton;
	std::string							m_strAction;
	SallyAPI::GUI::CApplicationWindow*	m_pWindow;

	void	OnCommandSendNotify();
public:
	CControlGroup(SallyAPI::GUI::CGUIBaseObject *parent, int x, int y, int width);
	virtual ~CControlGroup();

	virtual void SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);

	void SetValue(const std::string& name, const std::string& message, const std::string& action,
		const std::string& actionName, SallyAPI::GUI::CApplicationWindow* window);

	void ResetImage();
	void SetPicture(SallyAPI::GUI::CPicture* avatar);
	void SetImageId(int avatar);
};
