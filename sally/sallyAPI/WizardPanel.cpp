////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\WizardPanel.cpp
///
/// \brief	Implements the wizard panel class. 
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

#include "WizardPanel.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CWizardPanel::CWizardPanel(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId,
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

CWizardPanel::CWizardPanel(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId,
						   const std::string& pluginPath)
	:SallyAPI::GUI::CAppBase(parent, graphicId, pluginPath)
{
	Move((WINDOW_WIDTH  - WIZARD_PANEL_WIDTH) / 2, (WINDOW_HEIGHT - WIZARD_PANEL_HEIGHT) / 2 - 20);
	Resize(WIZARD_PANEL_WIDTH, WIZARD_PANEL_HEIGHT);

	m_pBackgroundGroup = new SallyAPI::GUI::CGroupBox(this, 0, 0, WIZARD_PANEL_WIDTH, WIZARD_PANEL_HEIGHT);
	this->AddChild(m_pBackgroundGroup);

	m_pImageLogo = new SallyAPI::GUI::CImageBox(this, -20, -20, 64, 64);
	m_pImageLogo->SetImageId(graphicId);
	this->AddChild(m_pImageLogo);

	m_pLabelHeader = new SallyAPI::GUI::CLabel(this, 55, 20, WIZARD_PANEL_WIDTH - 55 - 20);
	m_pLabelHeader->SetFont("big3.font");
	this->AddChild(m_pLabelHeader);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CWizardPanel::~CWizardPanel()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CWizardPanel::~CWizardPanel()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CWizardPanel::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter,
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

void CWizardPanel::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	SallyAPI::GUI::CAppBase::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CWizardPanel::SaveConfig()
///
/// \brief	Saves the configuration. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CWizardPanel::SaveConfig()
{
	SetPropertyBool("firstStart", false);
	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CWizardPanel::LoadConfig()
///
/// \brief	Loads the configuration. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CWizardPanel::LoadConfig()
{
	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CWizardPanel::SetParent(SallyAPI::GUI::CGUIBaseObject* parent)
///
/// \brief	Sets a parent. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	parent	If non-null, the parent. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CWizardPanel::SetParent(SallyAPI::GUI::CGUIBaseObject* parent)
{
	m_pParent = parent;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CWizardPanel::IsFirstStart()
///
/// \brief	Query if this object is first start. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	true if first start, false if not. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CWizardPanel::IsFirstStart()
{
	return GetPropertyBool("firstStart", true);
}