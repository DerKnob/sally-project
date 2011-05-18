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
/// SallyAPI::GUI::CGUIBaseObject* notificationWindow, const std::string& text,
/// const std::string& goToFolder)
///
/// \brief	Select folder. 
///
/// \author	Christian Knobloch
/// \date	09.04.2011
///
/// \param	controlId					Identifier for the control. 
/// \param	commandID					Identifier for the command. 
/// \param	actionCommandID				Identifier for the action command. 
/// \param [in,out]	notificationWindow	If non-null, the notification window. 
/// \param	text						The text. 
/// \param	goToFolder					Pathname of the go to folder. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void GUIHelper::SelectFolder(int controlId, int commandID, int actionCommandID,
							 SallyAPI::GUI::CGUIBaseObject* notificationWindow, const std::string& text,
							 const std::string& goToFolder)
{
	SallyAPI::GUI::SendMessage::CParameterOpenDialog openDialog(commandID, notificationWindow, text);

	std::map<std::string, int> mimetypeList;
	std::vector<std::string> folders;

	openDialog.SetMimetypeList(mimetypeList);
	openDialog.SetShowRemovableDisk(true);
	openDialog.SetShowSpecialFolders(true);
	openDialog.SetStartFolders(folders);
	openDialog.SetShowSubfolders(true);
	openDialog.SetAction(actionCommandID, "select complete folder", GUI_THEME_SALLY_ICON_ACCEPT);
	openDialog.SetGoToFolder(goToFolder);

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