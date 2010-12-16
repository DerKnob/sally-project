////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\LoadingMainWindow.cpp
///
/// \brief	Implements the loading main Windows Form. 
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

#include "LoadingMainWindow.h"

CLoadingMainWindowThread::CLoadingMainWindowThread()
{
}

CLoadingMainWindowThread::~CLoadingMainWindowThread()
{
}

void CLoadingMainWindowThread::RunEx()
{
	(*m_pGUIMainWindow) = new CMainWindow(m_pGUILoading);
	m_pGUILoading->SendMessageToParent(m_pGUILoading, 0, GUIM_LOADING_DONE);
}

void CLoadingMainWindowThread::SetValues(CMainWindow** mainWindow, CWindowLoading* loading)
{
	m_pGUIMainWindow = mainWindow;
	m_pGUILoading = loading;
}