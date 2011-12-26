////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\NotificationBox.cpp
///
/// \brief	Implements the notification box class. 
///
/// \author	Christian Knobloch
/// \date	26.12.2011
///
/// This file is part of the Sally Project
/// 
/// Copyright(c) 2008-2011 Sally Project
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

#include "NotificationBox.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CNotificationBox::CNotificationBox(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y,
/// int width, int height, int controlId) :SallyAPI::GUI::CForm(parent, x, y, width, height,
/// controlId)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	26.12.2011
///
/// \param [in,out]	parent	If non-null, the parent. 
/// \param	x				The x coordinate. 
/// \param	y				The y coordinate. 
/// \param	width			The width. 
/// \param	height			The height. 
/// \param	controlId		Identifier for the control. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CNotificationBox::CNotificationBox(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int height, int controlId)
	:SallyAPI::GUI::CForm(parent, x, y, width, height, controlId)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CNotificationBox::~CNotificationBox()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	26.12.2011
////////////////////////////////////////////////////////////////////////////////////////////////////

CNotificationBox::~CNotificationBox()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CNotificationBox::RenderControl()
///
/// \brief	Renders a control. 
///
/// \author	Christian Knobloch
/// \date	26.12.2011
////////////////////////////////////////////////////////////////////////////////////////////////////

void CNotificationBox::RenderControl()
{
	DrawButtonBackground(GUI_THEME_NOTIFICATIONBOX_LEFT_TOP, GUI_THEME_NOTIFICATIONBOX_TOP, GUI_THEME_NOTIFICATIONBOX_RIGHT_TOP, 
		GUI_THEME_NOTIFICATIONBOX_LEFT, GUI_THEME_NOTIFICATIONBOX_CENTER, GUI_THEME_NOTIFICATIONBOX_RIGHT, 
		GUI_THEME_NOTIFICATIONBOX_LEFT_BOTTOM, GUI_THEME_NOTIFICATIONBOX_BOTTOM, GUI_THEME_NOTIFICATIONBOX_RIGHT_BOTTOM);

	// Render Childs
	SallyAPI::GUI::CForm::RenderControl();
}
