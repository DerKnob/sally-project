////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	ImageBrowser\AppImageBrowser.h
///
/// \brief	Declares the application image browser class. 
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

#pragma once
#include "Define.h"
#include <sallyAPI\sallyAPI.h>
#include "MyConfigPanel.h"
#include "MyWizardPanel.h"
#include "MainForm.h"
#include "ScreensaverForm.h"

class CAppImageBrowser :
	public SallyAPI::GUI::CApplicationWindow
{
private:
	SallyAPI::GUI::CConfigPanel*	m_pConfigPanel;
	SallyAPI::GUI::CWizardPanel*	m_pWizardPanel;

	SallyAPI::System::CCritSection	m_ConfigLock;

	CMainForm*			m_pMainForm;
	CScreensaverForm*	m_pScreensaverForm;

	virtual void SallyConfigChanged();
	virtual void LoadConfig(SallyAPI::GUI::SendMessage::CParameterIntegerVector* messageParameter = NULL);
public:
	CAppImageBrowser(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, const std::string& pluginPath);
	virtual ~CAppImageBrowser();

	virtual bool ActivateScreensaver();
	virtual bool DeactivateScreensaver();

	virtual bool HasScreensaver();

	virtual bool SpecialKeyPressed(int key);
};
