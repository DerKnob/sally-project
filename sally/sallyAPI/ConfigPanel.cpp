////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ConfigPanel.cpp
///
/// \brief	Implements the configuration panel class. 
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

#include "ConfigPanel.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CConfigPanel::CConfigPanel(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId,
/// const std::string& pluginPath) :SallyAPI::GUI::CAppBase(parent, graphicId, pluginPath)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	parent	If non-null, the parent. 
/// \param	graphicId		Identifier for the graphic. 
/// \param	pluginPath		Full pathname of the plugin file. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CConfigPanel::CConfigPanel(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId,
							 const std::string& pluginPath)
	:SallyAPI::GUI::CAppBase(parent, graphicId, pluginPath)
{
	Move(0, MENU_HEIGHT);
	Resize(WINDOW_WIDTH, WINDOW_HEIGHT - (MENU_HEIGHT * 2));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CConfigPanel::~CConfigPanel()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CConfigPanel::~CConfigPanel()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CConfigPanel::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter,
/// int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
///
/// \brief	Send message to parent. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	reporter			If non-null, the reporter. 
/// \param	reporterId					Identifier for the reporter. 
/// \param	messageId					Identifier for the message. 
/// \param [in,out]	messageParameter	If non-null, the message parameter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CConfigPanel::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case MS_SALLY_APP_CONFIG_SAVED:
		m_pParent->SendMessageToParent(this, m_iGraphicId, MS_SALLY_APP_CONFIG_CHANGED, messageParameter);
		return;
	}
	SallyAPI::GUI::CAppBase::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CConfigPanel::SetParent(SallyAPI::GUI::CGUIBaseObject* parent)
///
/// \brief	Sets a parent. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	parent	If non-null, the parent. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CConfigPanel::SetParent(SallyAPI::GUI::CGUIBaseObject* parent)
{
	m_pParent = parent;
}