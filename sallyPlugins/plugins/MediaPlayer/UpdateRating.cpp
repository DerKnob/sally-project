////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\UpdateRating.cpp
///
/// \brief	Implements the update rating class. 
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

#include "UpdateRating.h"

CUpdateRating::CUpdateRating()
{
}

CUpdateRating::~CUpdateRating()
{
}

void CUpdateRating::SetValues(const std::string& filename, SallyAPI::GUI::CApplicationWindow* appWindow, int rating, const std::string& action, const std::string& message)
{
	m_strFilename = filename;
	m_pAppWindow = appWindow;
	m_iRating = rating;
	m_strAction = action;
	m_strMessage = message;
}

void CUpdateRating::RunEx()
{
	CMediaDatabase::UpdateRating(m_strFilename, m_pAppWindow, m_iRating);

	SallyAPI::Facebook::CFacebookManager* facebookManager = SallyAPI::Facebook::CFacebookManager::GetInstance();

	facebookManager->SendStatusMessage(m_pAppWindow->GetExplicitAppName(), m_pAppWindow->GetAppName(), m_strMessage, m_strAction, "Search for this");
}