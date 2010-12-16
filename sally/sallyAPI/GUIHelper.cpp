////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\GUIHelper.cpp
///
/// \brief	Implements the graphical user interface helper class. 
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

#include "GUIHelper.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	GUIHelper::GUIHelper()
///
/// \brief	Default constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

GUIHelper::GUIHelper()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	GUIHelper::~GUIHelper()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

GUIHelper::~GUIHelper()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void GUIHelper::SelectFolder(int controlId, int commandID, int actionCommandID,
/// SallyAPI::GUI::CGUIBaseObject* notificationWindow, const std::string& text)
///
/// \brief	Select folder. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	controlId					Identifier for the control. 
/// \param	commandID					Identifier for the command. 
/// \param	actionCommandID				Identifier for the action command. 
/// \param [in,out]	notificationWindow	If non-null, the notification window. 
/// \param	text						The text. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void GUIHelper::SelectFolder(int controlId, int commandID, int actionCommandID,
							 SallyAPI::GUI::CGUIBaseObject* notificationWindow, const std::string& text)
{
	SallyAPI::GUI::SendMessage::CParameterOpenDialog openDialog(commandID, notificationWindow, text);

	std::map<std::string, int> mimetypeList;
	std::vector<std::string> folders;

	// My Documents
	char szPath[MAX_PATH];
	LPITEMIDLIST lpStartFolder = NULL;
	SHGetSpecialFolderLocation(0, CSIDL_MYDOCUMENTS, &lpStartFolder);
	SHGetPathFromIDList(lpStartFolder, szPath);
	folders.push_back(szPath);

	// CleanUp
	CoTaskMemFree(lpStartFolder);
	lpStartFolder = NULL;

	// My Music
	SHGetSpecialFolderLocation(0, CSIDL_MYMUSIC, &lpStartFolder);
	SHGetPathFromIDList(lpStartFolder, szPath);
	folders.push_back(szPath);

	// CleanUp
	CoTaskMemFree(lpStartFolder);
	lpStartFolder = NULL;

	// My Video
	SHGetSpecialFolderLocation(0, CSIDL_MYVIDEO, &lpStartFolder);
	SHGetPathFromIDList(lpStartFolder, szPath);
	folders.push_back(szPath);

	// CleanUp
	CoTaskMemFree(lpStartFolder);
	lpStartFolder = NULL;

	// My Pictures
	SHGetSpecialFolderLocation(0, CSIDL_MYPICTURES, &lpStartFolder);
	SHGetPathFromIDList(lpStartFolder, szPath);
	folders.push_back(szPath);

	// CleanUp
	CoTaskMemFree(lpStartFolder);
	lpStartFolder = NULL;

	std::map<std::string, SallyAPI::File::DRIVE_TYPE> driveList = SallyAPI::File::FileHelper::GetDriveList();
	std::map<std::string, SallyAPI::File::DRIVE_TYPE>::iterator iter = driveList.begin();

	while (iter != driveList.end())
	{
		SallyAPI::File::DRIVE_TYPE driveType = iter->second;
		std::string driveLetter = iter->first;

		switch (driveType)
		{
		case SallyAPI::File::DRIVE_TYPE_FIXED:
			folders.push_back(driveLetter);
			break;
		}
		++iter;
	}

	openDialog.SetMimetypeList(mimetypeList);
	openDialog.SetShowRemovableDisk(true);
	openDialog.SetStartFolders(folders);
	openDialog.SetShowSubfolders(true);
	openDialog.SetAction(actionCommandID, "select complete folder", GUI_THEME_SALLY_ICON_ACCEPT);

	notificationWindow->SendMessageToParent(notificationWindow, controlId, MS_SALLY_SHOW_OPEN_FILE, &openDialog);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void GUIHelper::CalculateImageSize(int sourceWidth, int sourceHeight,
/// int destinationWidth, int destinationHeight, int& x, int& y, int& width, int& height)
///
/// \brief	Calculates the image size. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	sourceWidth			Width of the source. 
/// \param	sourceHeight		Height of the source. 
/// \param	destinationWidth	Width of the destination. 
/// \param	destinationHeight	Height of the destination. 
/// \param [in,out]	x			The x coordinate. 
/// \param [in,out]	y			The y coordinate. 
/// \param [in,out]	width		The width. 
/// \param [in,out]	height		The height. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void GUIHelper::CalculateImageSize(int sourceWidth, int sourceHeight, int destinationWidth, int destinationHeight, 
								 int& x, int& y, int& width, int& height)
{
	if ((sourceWidth == 0) || (sourceHeight == 0))
	{
		x = 0;
		y = 0;
		width = destinationWidth;
		height = destinationHeight;
		return;
	}

	float iWidthRelation = (float) sourceWidth / destinationWidth;
	float iHeightRelation = (float) sourceHeight / destinationHeight;
	if (iWidthRelation > iHeightRelation)
	{
		long l = (long) (sourceHeight / iWidthRelation);
		long k = (long) ((destinationHeight - l) / 2);
		width = destinationWidth;
		height = l;
		y = k;
		x = 0;
	}
	else
	{
		long l = (long) (sourceWidth / iHeightRelation);
		long k = (long) ((destinationWidth - l) / 2);
		height = destinationHeight;
		width = l;
		y = 0;
		x = k;
	}
}