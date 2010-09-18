////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ParameterMessageBox.cpp
///
/// \brief	Implements the parameter message box class. 
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
/// it under the terms of the GNU Lesser General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
/// 
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
/// GNU Lesser General Public License for more details.
/// 
/// You should have received a copy of the GNU Lesser General Public License
/// along with this program. If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "ParameterMessageBox.h"

using namespace SallyAPI::GUI::SendMessage;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CParameterMessageBox::CParameterMessageBox(int controlId,
/// SallyAPI::GUI::CGUIBaseObject* notificationWindow, const std::string& dialogText,
/// SallyAPI::GUI::MessageBoxIcon icon)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	controlId					Identifier for the control. 
/// \param [in,out]	notificationWindow	If non-null, the notification window. 
/// \param	dialogText					The dialog text. 
/// \param	icon						The icon. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CParameterMessageBox::CParameterMessageBox(int controlId, SallyAPI::GUI::CGUIBaseObject* notificationWindow,
																 const std::string& dialogText, SallyAPI::GUI::MessageBoxIcon icon)
	:m_pNotificationWindow(notificationWindow), m_strDialogText(dialogText), m_iControlId(controlId), m_iIcon(icon)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CParameterMessageBox::~CParameterMessageBox()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CParameterMessageBox::~CParameterMessageBox()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::GUI::CGUIBaseObject* CParameterMessageBox::GetNotificationWindow()
///
/// \brief	Gets the notification window. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	null if it fails, else the notification window. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::GUI::CGUIBaseObject* CParameterMessageBox::GetNotificationWindow()
{
	return m_pNotificationWindow;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CParameterMessageBox::GetDialogText()
///
/// \brief	Gets the dialog text. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The dialog text. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CParameterMessageBox::GetDialogText()
{
	return m_strDialogText;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CParameterMessageBox::GetControlID()
///
/// \brief	Gets the control identifier. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The control identifier. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CParameterMessageBox::GetControlID()
{
	return m_iControlId;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::GUI::MessageBoxIcon CParameterMessageBox::GetIcon()
///
/// \brief	Gets the icon. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The icon. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::GUI::MessageBoxIcon CParameterMessageBox::GetIcon()
{
	return m_iIcon;
}