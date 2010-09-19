////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\LanguageChanger.h
///
/// \brief	Declares the language changer class. 
///
/// \author	Christian Knobloch
/// \date	13.09.2010
///
/// This file is part of the Sally Project
/// 
/// Copyright(c) 2008-2010 Sally Project
/// http://www.sally-project.de/
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

#define LANGUAGE_CHANGED		8000

class CLanguageChanger :
	public SallyAPI::System::CThread

{
private:
	std::string						m_strLanguage;
	SallyAPI::GUI::CWizardPanel*	m_pWizard;

	virtual void RunEx();
public:
	CLanguageChanger();
	~CLanguageChanger();

	void SetValues(SallyAPI::GUI::CWizardPanel* wizard, const std::string& language);
};
