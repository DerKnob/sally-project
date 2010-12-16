////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\StatusMessage.cpp
///
/// \brief	Implements the status message class. 
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

#include "StatusMessage.h"

using namespace SallyAPI::Community;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CStatusMessage::CStatusMessage(const std::string& createDate, const std::string& nickName,
/// const std::string& avatar, const std::string& explicidAppName, const std::string& appName,
/// const std::string& action, const std::string& actionName, const std::string& messageString)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	createDate		Date of the create. 
/// \param	nickName		Name of the nick. 
/// \param	avatar			The avatar. 
/// \param	explicidAppName	Name of the explicid application. 
/// \param	appName			Name of the application. 
/// \param	action			The action. 
/// \param	actionName		Name of the action. 
/// \param	messageString	The message string. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CStatusMessage::CStatusMessage(const std::string& createDate, const std::string& nickName,
							   const std::string& avatar, const std::string& explicidAppName,
							   const std::string& appName, const std::string& action,
							   const std::string& actionName, const std::string& messageString)
{
	m_strCreateDate = createDate;
	m_strNickName = nickName;
	m_strAvatar = avatar;
	m_strExplicidAppName = explicidAppName;
	m_strAppName = appName;
	m_strAction = action;
	m_strActionName = actionName;
	m_strMessageString = messageString;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CStatusMessage::~CStatusMessage()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CStatusMessage::~CStatusMessage()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CStatusMessage::GetNickName()
///
/// \brief	Gets the nick name. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The nick name. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CStatusMessage::GetNickName()
{
	return m_strNickName;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CStatusMessage::GetExplicidAppName()
///
/// \brief	Gets the explicid application name. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The explicid application name. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CStatusMessage::GetExplicidAppName()
{
	return m_strExplicidAppName;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CStatusMessage::GetAvatar()
///
/// \brief	Gets the avatar. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The avatar. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CStatusMessage::GetAvatar()
{
	return m_strAvatar;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CStatusMessage::GetAppName()
///
/// \brief	Gets the application name. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The application name. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CStatusMessage::GetAppName()
{
	return m_strAppName;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CStatusMessage::GetAction()
///
/// \brief	Gets the action. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The action. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CStatusMessage::GetAction()
{
	return m_strAction;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CStatusMessage::GetActionName()
///
/// \brief	Gets the action name. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The action name. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CStatusMessage::GetActionName()
{
	return m_strActionName;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CStatusMessage::GetMessageString()
///
/// \brief	Gets the message string. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The message string. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CStatusMessage::GetMessageString()
{
	return m_strMessageString;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CStatusMessage::GetCreateDate()
///
/// \brief	Gets the create date. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The create date. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CStatusMessage::GetCreateDate()
{
	return m_strCreateDate;
}