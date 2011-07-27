////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	ImageBrowser\AppImageBrowser.cpp
///
/// \brief	Implements the application image browser class. 
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

#include "AppImageBrowser.h"

using namespace SallyAPI::GUI;

CAppImageBrowser::CAppImageBrowser(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, const std::string& pluginPath)
	:SallyAPI::GUI::CApplicationWindow(parent, graphicId, pluginPath)
{
	// Config Panel
	m_pConfigPanel = new CMyConfigPanel(this, GetGraphicId(), m_strPluginPath);
	SallyAPI::GUI::SendMessage::CParameterControl parameterConfig(m_pConfigPanel);
	m_pParent->SendMessageToParent(this, GetGraphicId(), MS_SALLY_ADD_CONFIG_PANEL, &parameterConfig);

	m_pWizardPanel = new CMyWizardPanel(this, GetGraphicId(), m_strExplicitAppName, (CMyConfigPanel*) m_pConfigPanel, this);
	SallyAPI::GUI::SendMessage::CParameterControl parameterWizard(m_pWizardPanel);
	m_pParent->SendMessageToParent(this, GetGraphicId(), MS_SALLY_ADD_WIZARD_PANEL, &parameterWizard);

	// Forms
	m_pScreensaverForm = new CScreensaverForm(this, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	m_pScreensaverForm->Visible(false);
	this->AddChild(m_pScreensaverForm);

	m_pMainForm = new CMainForm(this, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, m_pScreensaverForm);
	this->AddChild(m_pMainForm);
}

CAppImageBrowser::~CAppImageBrowser()
{
}

void CAppImageBrowser::LoadConfig()
{
	SallyAPI::System::CAutoLock lock(&m_ConfigLock);

	m_pScreensaverForm->LoadConfig();
	m_pMainForm->LoadConfig();
}

void CAppImageBrowser::SallyConfigChanged()
{
	LoadConfig();
}

bool CAppImageBrowser::HasScreensaver()
{
	return true;
}

bool CAppImageBrowser::ActivateScreensaver()
{
	if (!m_pMainForm->GetZoom())
	{
		if (!m_pScreensaverForm->ActivateScreensaver())
			return false;

		m_pScreensaverForm->Visible(true);
		m_pMainForm->Visible(false);
	}
	return true;
}

bool CAppImageBrowser::DeactivateScreensaver()
{
	if (!m_pMainForm->GetZoom())
	{
		m_pMainForm->Visible(true);
		m_pScreensaverForm->Visible(false);

		m_pScreensaverForm->DeactivateScreensaver();
	}
	return true;
}

bool CAppImageBrowser::SpecialKeyPressed(int key)
{
	if (m_pScreensaverForm->IsVisible())
		return m_pScreensaverForm->SpecialKeyPressed(key);
	else
		return m_pMainForm->SpecialKeyPressed(key);

	return false;
}