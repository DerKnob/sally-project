////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\LanguageChanger.cpp
///
/// \brief	Implements the language changer class. 
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

#include "LanguageChanger.h"

CLanguageChanger::CLanguageChanger()
{
}

CLanguageChanger::~CLanguageChanger()
{
}

void CLanguageChanger::SetValues(SallyAPI::GUI::CWizardPanel* wizard, const std::string& language)
{
	m_strLanguage = language;
	m_pWizard = wizard;
}

void CLanguageChanger::RunEx()
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	if (option->GetPropertyString("sally", "language").compare(m_strLanguage) != 0)
	{
		option->SetPropertyString("sally", "language", m_strLanguage);
		config->ReloadLanguage();
	}

	m_pWizard->SendMessageToParent(m_pWizard, 0, LANGUAGE_CHANGED);
}