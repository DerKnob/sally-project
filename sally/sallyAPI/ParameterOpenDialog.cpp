////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ParameterOpenDialog.cpp
///
/// \brief	Implements the parameter open Dialog. 
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

#include "ParameterOpenDialog.h"

using namespace SallyAPI::GUI::SendMessage;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CParameterOpenDialog::CParameterOpenDialog(int controlId,
/// SallyAPI::GUI::CGUIBaseObject* notificationWindow, const std::string& dialogText)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	controlId					Identifier for the control. 
/// \param [in,out]	notificationWindow	If non-null, the notification window. 
/// \param	dialogText					The dialog text. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CParameterOpenDialog::CParameterOpenDialog(int controlId,
																 SallyAPI::GUI::CGUIBaseObject* notificationWindow,
																 const std::string& dialogText)
	:m_pNotificationWindow(notificationWindow), m_strDialogText(dialogText), m_iControlId(controlId),
	m_bShowRemovableDisk(true), m_bShowSubfolders(true), m_iActionCommand(0)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CParameterOpenDialog::~CParameterOpenDialog()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CParameterOpenDialog::~CParameterOpenDialog()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::GUI::CGUIBaseObject* CParameterOpenDialog::GetNotificationWindow()
///
/// \brief	Gets the notification window. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	null if it fails, else the notification window. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::GUI::CGUIBaseObject* CParameterOpenDialog::GetNotificationWindow()
{
	return m_pNotificationWindow;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CParameterOpenDialog::GetDialogText()
///
/// \brief	Gets the dialog text. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The dialog text. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CParameterOpenDialog::GetDialogText()
{
	return m_strDialogText;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CParameterOpenDialog::GetControlID()
///
/// \brief	Gets the control identifier. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The control identifier. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CParameterOpenDialog::GetControlID()
{
	return m_iControlId;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CParameterOpenDialog::SetStartFolders(std::vector<std::string>& startFolders)
///
/// \brief	Sets a start folders. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	startFolders	The start folders. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CParameterOpenDialog::SetStartFolders(std::vector<std::string>& startFolders)
{
	m_vStartFolders = startFolders;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CParameterOpenDialog::SetShowRemovableDisk(bool showRemovableDisk)
///
/// \brief	Sets a show removable disk. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	showRemovableDisk	true to show, false to hide the removable disk. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CParameterOpenDialog::SetShowRemovableDisk(bool showRemovableDisk)
{
	m_bShowRemovableDisk = showRemovableDisk;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CParameterOpenDialog::SetMimetypeList(std::map<std::string,
/// int>& mimetypeList)
///
/// \brief	Sets a mimetype list. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	mimetypeList	List of mimetypes. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CParameterOpenDialog::SetMimetypeList(std::map<std::string, int>& mimetypeList)
{
	m_mMimetypeList = mimetypeList;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CParameterOpenDialog::SetShowSubfolders(bool showSubfolders)
///
/// \brief	Sets a show subfolders. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	showSubfolders	true to show, false to hide the subfolders. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CParameterOpenDialog::SetShowSubfolders(bool showSubfolders)
{
	m_bShowSubfolders = showSubfolders;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::vector<std::string> CParameterOpenDialog::GetStartFolders()
///
/// \brief	Gets the start folders. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The start folders. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<std::string> CParameterOpenDialog::GetStartFolders()
{
	return m_vStartFolders;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CParameterOpenDialog::GetShowRemovableDisk()
///
/// \brief	Gets the show removable disk. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CParameterOpenDialog::GetShowRemovableDisk()
{
	return m_bShowRemovableDisk;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::map<std::string, int> CParameterOpenDialog::GetMimetypeList()
///
/// \brief	Gets the mimetype list. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The mimetype list. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::map<std::string, int> CParameterOpenDialog::GetMimetypeList()
{
	return m_mMimetypeList;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CParameterOpenDialog::GetShowSubfolders()
///
/// \brief	Gets the show subfolders. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CParameterOpenDialog::GetShowSubfolders()
{
	return m_bShowSubfolders;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CParameterOpenDialog::SetAction(int command, const std::string& text,
/// int image)
///
/// \brief	Sets an action. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	command	The command. 
/// \param	text	The text. 
/// \param	image	The image. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CParameterOpenDialog::SetAction(int command, const std::string& text, int image)
{
	m_iActionCommand = command;
	m_strActionText = text;
	m_iActionImage = image;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CParameterOpenDialog::GetActionCommand()
///
/// \brief	Gets the action command. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The action command. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CParameterOpenDialog::GetActionCommand()
{
	return m_iActionCommand;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CParameterOpenDialog::GetActionText()
///
/// \brief	Gets the action text. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The action text. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CParameterOpenDialog::GetActionText()
{
	return m_strActionText;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CParameterOpenDialog::GetActionImage()
///
/// \brief	Gets the action image. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The action image. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CParameterOpenDialog::GetActionImage()
{
	return m_iActionImage;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CParameterOpenDialog::SetGoToFolder(const std::string& goToFolder)
///
/// \brief	Sets a go to folder. 
///
/// \author	Christian Knobloch
/// \date	09.04.2011
///
/// \param	goToFolder	Pathname of the go to folder. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CParameterOpenDialog::SetGoToFolder(const std::string& goToFolder)
{
	m_strGoToFolder = goToFolder;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CParameterOpenDialog::GetGoToFolder()
///
/// \brief	Gets the go to folder. 
///
/// \author	Christian Knobloch
/// \date	09.04.2011
///
/// \return	The go to folder. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CParameterOpenDialog::GetGoToFolder()
{
	return m_strGoToFolder;
}